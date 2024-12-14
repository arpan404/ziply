#include "ende.hpp"
#include <iostream>

Ende::EncryptionParams Ende::deriveKey(const std::string &password, const std::array<uint8_t, SALT_SIZE> &salt)
{
    Ende::EncryptionParams params;
    std::array<uint8_t, 32> keyAndIv;

    if (!PKCS5_PBKDF2_HMAC(password.c_str(), password.size(),
                           salt.data(), salt.size(),
                           10000, EVP_sha256(),
                           keyAndIv.size(), keyAndIv.data()))
    {
        throw Error("Failed to derive key and IV", "error-ende-key");
    }

    std::copy(keyAndIv.begin(), keyAndIv.begin() + 16, params.key.begin());
    std::copy(keyAndIv.begin() + 16, keyAndIv.end(), params.iv.begin());
    return params;
}

std::vector<uint8_t> Ende::encrypt(const std::vector<uint8_t> &data, const EncryptionParams &params)
{
#ifdef __AES__

#endif

    std::vector<uint8_t> encryptedData;
    encryptedData.resize(data.size());

    static const size_t AES_ALIGN = 16;
    if (reinterpret_cast<uintptr_t>(encryptedData.data()) % AES_ALIGN != 0)
    {
        encryptedData.reserve(encryptedData.size() + AES_ALIGN);
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
        throw Error("Failed to create encryption context", "error-ende-enc1");

    EVP_CIPHER *cipher = EVP_CIPHER_fetch(nullptr, "AES-128-CTR", nullptr);
    if (!cipher)
    {
        cipher = EVP_aes_128_ctr();
    }

    if (!EVP_EncryptInit_ex2(ctx, cipher, params.key.data(), params.iv.data(), nullptr))
    {
        EVP_CIPHER_free(cipher);
        EVP_CIPHER_CTX_free(ctx);
        throw Error("Failed to initialize encryption", "error-ende-enc2");
    }

    EVP_CIPHER_CTX_set_num_threads(ctx, 4);

    int len = 0;
    if (!EVP_EncryptUpdate(ctx, encryptedData.data(), &len, data.data(), data.size()))
    {
        EVP_CIPHER_free(cipher);
        EVP_CIPHER_CTX_free(ctx);
        throw Error("Failed during encryption", "error-ende-enc3");
    }

    int finalLen = 0;
    if (!EVP_EncryptFinal_ex(ctx, encryptedData.data() + len, &finalLen))
    {
        EVP_CIPHER_free(cipher);
        EVP_CIPHER_CTX_free(ctx);
        throw Error("Failed to finalize encryption", "error-ende-enc4");
    }

    encryptedData.resize(len + finalLen);
    EVP_CIPHER_free(cipher);
    EVP_CIPHER_CTX_free(ctx);
    return encryptedData;
}

bool Ende::compressAndEncrypt(const std::string &inputFilePath, const std::string &outputFilePath,
                              const std::string &password, uint32_t compressionLevel)
{
    std::ifstream inputFile(inputFilePath, std::ios::binary | std::ios::ate);
    if (!inputFile)
        throw Error("Failed to open input file", "error-ende-input");

    std::vector<uint8_t> inputData(static_cast<size_t>(inputFile.tellg()));
    inputFile.seekg(0);
    inputFile.read(reinterpret_cast<char *>(inputData.data()), inputData.size());
    inputFile.close();

    // Create thread pool with hardware concurrency
    ThreadPool pool(std::thread::hardware_concurrency());

    // Split data into chunks
    const size_t chunkSize = inputData.size() / std::thread::hardware_concurrency();
    std::vector<std::future<std::vector<uint8_t>>> futures;
    std::vector<std::vector<uint8_t>> compressedChunks;

    for (size_t offset = 0; offset < inputData.size(); offset += chunkSize)
    {
        size_t currentChunkSize = std::min(chunkSize, inputData.size() - offset);

        futures.push_back(pool.enqueue([=, &inputData, compressionLevel]()
                                       {
            std::vector<uint8_t> chunk(inputData.begin() + offset, 
                                     inputData.begin() + offset + currentChunkSize);
            
            lzma_stream strm = LZMA_STREAM_INIT;
            lzma_ret ret = lzma_easy_encoder(&strm, compressionLevel, LZMA_CHECK_CRC64);
            if (ret != LZMA_OK)
                throw Error("Failed to initialize LZMA encoder", "error-ende-compress");

            std::vector<uint8_t> compressedChunk;
            compressedChunk.resize(chunk.size() + BUFFER_SIZE);

            strm.next_in = chunk.data();
            strm.avail_in = chunk.size();
            strm.next_out = compressedChunk.data();
            strm.avail_out = compressedChunk.size();

            while (strm.avail_in > 0) {
                ret = lzma_code(&strm, LZMA_RUN);
                if (ret != LZMA_OK) {
                    lzma_end(&strm);
                    throw Error("Compression failed", "error-ende-compress");
                }
            }

            ret = lzma_code(&strm, LZMA_FINISH);
            if (ret != LZMA_STREAM_END) {
                lzma_end(&strm);
                throw Error("Compression failed", "error-ende-compress-end");
            }

            compressedChunk.resize(compressedChunk.size() - strm.avail_out);
            lzma_end(&strm);
            return compressedChunk; }));
    }

    // Collect results
    std::vector<uint8_t> compressedData;
    size_t totalCompressedSize = 0;

    for (auto &future : futures)
    {
        auto chunk = future.get();
        totalCompressedSize += chunk.size();
        compressedChunks.push_back(std::move(chunk));
    }

    // Combine all compressed chunks
    compressedData.reserve(totalCompressedSize);
    for (const auto &chunk : compressedChunks)
    {
        compressedData.insert(compressedData.end(), chunk.begin(), chunk.end());
    }

    std::array<uint8_t, SALT_SIZE> salt;
    if (!RAND_bytes(salt.data(), salt.size()))
    {
        throw Error("Failed to generate salt", "error-ende-salt");
    }

    Ende::EncryptionParams params = deriveKey(password, salt);
    std::vector<uint8_t> encryptedData = encrypt(compressedData, params);

    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile)
        throw Error("Failed to open output file", "error-ende-output");

    outputFile.write(reinterpret_cast<char *>(salt.data()), salt.size());
    outputFile.write(reinterpret_cast<char *>(encryptedData.data()), encryptedData.size());
    outputFile.close();

    return true;
}