#include "ende.hpp"
#include <memory>

bool Ende::compressAndEncrypt(const std::string &inputFilePath, const std::string &outputFilePath, const std::string &password, uint32_t compressionLevel)
{
    std::array<uint8_t, SALT_SIZE> salt;

    // Generate salt
    if (RAND_bytes(salt.data(), salt.size()) != 1)
    {
        throw Error("Failed to generate salt", "error-ende-x1");
    }

    EncryptionParams params = deriveKey(password, salt);

    std::vector<uint8_t> compressedData;
    {
        lzma_stream strm = LZMA_STREAM_INIT;
        lzma_ret ret = lzma_easy_encoder(&strm, compressionLevel, LZMA_CHECK_CRC64);
        if (ret != LZMA_OK)
        {
            throw Error("Failed to create lzma encoder", "error-ende-x3");
        }

        std::ifstream inFile(inputFilePath, std::ios::binary);
        if (!inFile)
        {
            lzma_end(&strm);
            throw Error("Failed to open input file", "error-ende-x4");
        }

        std::vector<uint8_t> inBuf(BUFFER_SIZE);
        std::vector<uint8_t> outBuf(BUFFER_SIZE);

        while (inFile)
        {
            inFile.read(reinterpret_cast<char *>(inBuf.data()), inBuf.size());
            size_t readSize = inFile.gcount();

            if (readSize == 0)
                break; // Check for end of file

            strm.next_in = inBuf.data();
            strm.avail_in = readSize;

            do
            {
                strm.next_out = outBuf.data();
                strm.avail_out = outBuf.size();

                ret = lzma_code(&strm, LZMA_RUN);
                if (ret != LZMA_OK && ret != LZMA_STREAM_END)
                {
                    lzma_end(&strm);
                    throw Error("Failed to compress data", "error-ende-x5");
                }
                size_t have = outBuf.size() - strm.avail_out;
                compressedData.insert(compressedData.end(), outBuf.data(), outBuf.data() + have);
            } while (strm.avail_out == 0);
        }

        lzma_end(&strm);
    }

    std::vector<uint8_t> encryptedData = encrypt(compressedData, params);

    std::ofstream outFile(outputFilePath, std::ios::binary);
    if (!outFile)
    {
        throw Error("Cannot create output file", "error-ende-x6");
    }

    outFile.write(reinterpret_cast<const char *>(salt.data()), salt.size());
    outFile.write(reinterpret_cast<const char *>(encryptedData.data()), encryptedData.size());

    return true;
}

bool Ende::decompressAndDecrypt(const std::string &inputFilePath, const std::string &outputFilePath, const std::string &password)
{
    std::ifstream inFile(inputFilePath, std::ios::binary);
    if (!inFile)
    {
        throw Error("Failed to open input file", "error-ende-x12");
    }

    std::array<uint8_t, SALT_SIZE> salt;
    inFile.read(reinterpret_cast<char *>(salt.data()), salt.size());

    EncryptionParams params = deriveKey(password, salt);

    std::vector<uint8_t> encryptedData(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>());

    std::vector<uint8_t> compressedData = decrypt(encryptedData, params);

    std::ofstream outFile(outputFilePath, std::ios::binary);
    if (!outFile)
    {
        throw Error("Failed to create output file", "error-ende-x13");
    }

    lzma_stream strm = LZMA_STREAM_INIT;
    lzma_ret ret = lzma_easy_decoder(&strm, UINT64_MAX, 0);
    if (ret != LZMA_OK)
    {
        throw Error("Failed to create lzma decoder", "error-ende-x14");
    }

    strm.next_in = compressedData.data();
    strm.avail_in = compressedData.size();

    std::vector<uint8_t> outBuf(BUFFER_SIZE);
    do
    {
        strm.next_out = outBuf.data();
        strm.avail_out = outBuf.size();
        ret = lzma_code(&strm, LZMA_RUN);
        if (ret != LZMA_OK && ret != LZMA_STREAM_END)
        {
            lzma_end(&strm);
            throw Error("Failed to decompress data", "error-ende-x15");
        }
        size_t have = outBuf.size() - strm.avail_out;
        outFile.write(reinterpret_cast<const char *>(outBuf.data()), have);
    } while (ret != LZMA_STREAM_END);

    lzma_end(&strm);

    return true;
}

Ende::EncryptionParams Ende::deriveKey(const std::string &password, const std::array<uint8_t, SALT_SIZE> &salt)
{
    EncryptionParams params;

    if (!PKCS5_PBKDF2_HMAC(password.c_str(), password.size(), salt.data(), salt.size(), PBKDF2_ITERATIONS, EVP_sha256(), KEY_SIZE, params.key.data()))
    {
        throw Error("Failed to derive key", "error-ende-x2");
    }

    std::copy(
        params.key.begin() + IV_OFFSET,
        params.key.begin() + IV_OFFSET + IV_SIZE,
        params.iv.data());
    params.salt = salt;

    return params;
}

std::vector<uint8_t> Ende::encrypt(const std::vector<uint8_t> &data, const EncryptionParams &params)
{
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free);
    if (!ctx)
    {
        throw Error("Failed to create cipher context", "error-ende-x6");
    }
    if (!EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, params.key.data(), params.iv.data()))
    {
        throw Error("Failed to initialize encryption", "error-ende-x7");
    }

    std::vector<uint8_t> encrypted(data.size() + EVP_MAX_BLOCK_LENGTH);

    int outLen1 = 0;
    if (!EVP_EncryptUpdate(ctx.get(), encrypted.data(), &outLen1, data.data(), data.size()))
    {
        throw Error("Encryption failed", "error-ende-x8");
    }

    int outLen2 = 0;
    if (!EVP_EncryptFinal_ex(ctx.get(), encrypted.data() + outLen1, &outLen2))
    {
        throw Error("Encryption finalization failed", "error-ende-x9");
    }

    encrypted.resize(outLen1 + outLen2);
    return encrypted;
}

std::vector<uint8_t> Ende::decrypt(const std::vector<uint8_t> &data, const EncryptionParams &params)
{
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free);
    if (!ctx)
    {
        throw Error("Failed to create cipher context", "error-ende-x10");
    }
    if (!EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, params.key.data(), params.iv.data()))
    {
        throw Error("Failed to initialize decryption", "error-ende-x11");
    }

    std::vector<uint8_t> decrypted(data.size());
    int outLen1 = 0;
    if (!EVP_DecryptUpdate(ctx.get(), decrypted.data(), &outLen1, data.data(), data.size()))
    {
        throw Error("Decryption failed", "error-ende-x12");
    }

    int outLen2 = 0;
    if (!EVP_DecryptFinal_ex(ctx.get(), decrypted.data() + outLen1, &outLen2))
    {
        throw Error("Decryption finalization failed", "error-ende-x13");
    }

    decrypted.resize(outLen1 + outLen2);
    return decrypted;
}