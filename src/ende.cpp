#include "ende.hpp"

bool Ende::compressAndEncrypt(const std::string &inputFilePath, const std::string &outputFilePath, const std::string &password, uint32_t compressionLevel)
{

    std::array<uint8_t, SALT_SIZE> salt;

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
            throw Error("Failed to open input file", "error-ende-x4");
        }

        std::vector<uint8_t> inBuf(BUFFER_SIZE);
        std::vector<uint8_t> outBuf(BUFFER_SIZE);

        while (inFile)
        {
            inFile.read(reinterpret_cast<char *>(inBuf.data()), inBuf.size());
            sizet readSize = inFile.gcount();

            strm.next_in = inBuf.data();
            strm.avail_in = readSize;

            do
            {
                strm.next_out = outBuf.data();
                strm.avail_out = outBuf.size();

                ret = lzma_code(&strm, LZMA_FINISH);
                if (ret != LZMA_OK && ret != LZMA_STREAM_END)
                {
                    throw Error("Failed to compress data", "error-ende-x5");
                }
                size_t have = outBuf.size() - strm.avail_out;
                compressedData.insert(compressedData.end(), outBuf.data(), outBuf.data() + have);
            } while (strm.avail_out == 0);
        }
        lzma_end(&strm);
    }

    std::vector<uint8_t> encryptedData = encrypt(compressedData, params);

    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile)
    {
        throw std::runtime_error("Cannot create output file: " + outputPath);
    }

    outFile.write(reinterpret_cast<const char *>(salt.data()), salt.size());

    outFile.write(reinterpret_cast<const char *>(encryptedData.data()),
                  encryptedData.size());

    return true;
}

Ende::EncryptionParams Ende::deriveKey(const std::string &password, const std::array<uint8_t, SALT_SIZE> &salt)
{
    EncryptionParams params;

    if (!PKCS5_PBKDF2_HMAC(password.c_str(), password.size(), salt.data(), salt.size(), 10000, EVP_sha256(), KEY_SIZE, params.key.data()))
    {
        throw Error("Failed to derive key", "error-ende-x2");
    }

    std::copy(
        params.key.begin() + KEY_SIZE,
        params.key.data() + KEY_SIZE + IV_SIZE,
        params.iv.data());
    params.salt = salt;

    return params;
}

std::vector<uint8_t> Ende::encrypt(const std::vector<uint8_t> &data, const EncryptionParams &params)
{

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        throw Error("Failed to create cipher context", "error-ende-x6");
    }
    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, params.key.data(), params.iv.data()))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw Error("Failed to initialize encryption", "error-ende-x7");
    }
    std::vector<uint8_t> encrypted(data.size() + EVP_MAX_BLOCK_LENGTH);

    int outLen1 = 0;
    if (!EVP_EncryptUpdate(ctx, encrypted.data(), &outLen1, data.data(), data.size()))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Encryption failed");
    }

    int outLen2 = 0;
    if (!EVP_EncryptFinal_ex(ctx, encrypted.data() + outLen1, &outLen2))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Encryption finalization failed");
    }

    encrypted.resize(outLen1 + outLen2);
    EVP_CIPHER_CTX_free(ctx);
    return encrypted;
}

std::vector<uint8_t> Ende::decrypt(const std::vector<uint8_t> &data, const EncryptionParams &params)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        throw Error("Failed to create cipher context", "error-ende-x8");
    }
    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, params.key.data(), params.iv.data()))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw Error("Failed to initialize decryption", "error-ende-x9");
    }

    std::vector<uint8_t> decrypted(data.size());
    int outLen1 = 0;
    if (!EVP_DecryptUpdate(ctx, decrypted.data(), &outLen1, data.data(), data.size()))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw Error("Failed to decrypt data", "error-ende-x10");
    }

    int outLen2 = 0;
    if (!EVP_DecryptFinal_ex(ctx, decrypted.data() + outLen1, &outLen2))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw Error("Failed to finalize decryption", "error-ende-x11");
    }
    decrypted.resize(outLen1 + outLen2);
    EVP_CIPHER_CTX_free(ctx);
    return decrypted;
}