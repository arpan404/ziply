#include "ende.hpp"

Ende::EncryptionParams Ende::deriveKey(const std::string &password, const std::array<uint8_t, SALT_SIZE> &salt) {
    Ende::EncryptionParams params;
    std::array<uint8_t, 32> keyAndIv;

    if (!PKCS5_PBKDF2_HMAC(password.c_str(), password.size(), salt.data(), salt.size(), 10000, EVP_sha256(),
                           keyAndIv.size(), keyAndIv.data())) {
        throw Error("Failed to derive key and IV", "error-ende-key");
    }

    std::copy(keyAndIv.begin(), keyAndIv.begin() + 16, params.key.begin());
    std::copy(keyAndIv.begin() + 16, keyAndIv.end(), params.iv.begin());
    return params;
}

std::vector<uint8_t> Ende::encrypt(const std::vector<uint8_t> &data, const EncryptionParams &params) {
    std::vector<uint8_t> encryptedData;
    encryptedData.resize(data.size() + EVP_MAX_BLOCK_LENGTH);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
        throw Error("Failed to create encryption context", "error-ende-enc1");

    if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, params.key.data(), params.iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        throw Error("Failed to initialize encryption", "error-ende-enc2");
    }

    int len = 0;
    if (!EVP_EncryptUpdate(ctx, encryptedData.data(), &len, data.data(), data.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw Error("Failed during encryption", "error-ende-enc3");
    }

    int finalLen = 0;
    if (!EVP_EncryptFinal_ex(ctx, encryptedData.data() + len, &finalLen)) {
        EVP_CIPHER_CTX_free(ctx);
        throw Error("Failed to finalize encryption", "error-ende-enc4");
    }

    encryptedData.resize(len + finalLen);
    EVP_CIPHER_CTX_free(ctx);
    return encryptedData;
}

std::vector<uint8_t> Ende::decrypt(const std::vector<uint8_t> &data, const EncryptionParams &params) {
    std::vector<uint8_t> decryptedData(data.size());

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
        throw Error("Failed to create decryption context", "error-ende-dec1");

    if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, params.key.data(), params.iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        throw Error("Failed to initialize decryption", "error-ende-dec2");
    }

    int len = 0;
    if (!EVP_DecryptUpdate(ctx, decryptedData.data(), &len, data.data(), data.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw Error("Failed during decryption", "error-ende-dec3");
    }

    int finalLen = 0;
    if (!EVP_DecryptFinal_ex(ctx, decryptedData.data() + len, &finalLen)) {
        EVP_CIPHER_CTX_free(ctx);
        throw Error("Failed to finalize decryption", "error-ende-dec4");
    }

    decryptedData.resize(len + finalLen);
    EVP_CIPHER_CTX_free(ctx);
    return decryptedData;
}

bool Ende::compressAndEncrypt(const std::string &inputFilePath, const std::string &outputFilePath,
                              const std::string &password, uint32_t compressionLevel) {

    std::ifstream inputFile(inputFilePath, std::ios::binary | std::ios::ate);
    if (!inputFile)
        throw Error("Failed to open input file", "error-ende-input");

    std::vector<uint8_t> inputData(static_cast<size_t>(inputFile.tellg()));
    inputFile.seekg(0);
    inputFile.read(reinterpret_cast<char *>(inputData.data()), inputData.size());
    inputFile.close();

    lzma_stream strm = LZMA_STREAM_INIT;
    lzma_ret ret = lzma_easy_encoder(&strm, compressionLevel, LZMA_CHECK_CRC64);
    if (ret != LZMA_OK)
        throw Error("Failed to initialize LZMA encoder", "error-ende-compress");

    std::vector<uint8_t> compressedData;
    compressedData.resize(inputData.size() + BUFFER_SIZE);

    strm.next_in = inputData.data();
    strm.avail_in = inputData.size();
    strm.next_out = compressedData.data();
    strm.avail_out = compressedData.size();

    ret = lzma_code(&strm, LZMA_FINISH);
    if (ret != LZMA_STREAM_END) {
        lzma_end(&strm);
        throw Error("Compression failed", "error-ende-compress-end");
    }

    compressedData.resize(compressedData.size() - strm.avail_out);
    lzma_end(&strm);

    std::array<uint8_t, SALT_SIZE> salt;
    if (!RAND_bytes(salt.data(), salt.size())) {
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

bool Ende::decompressAndDecrypt(const std::string &inputFilePath, const std::string &outputFilePath,
                                const std::string &password) {

    std::ifstream inputFile(inputFilePath, std::ios::binary | std::ios::ate);
    if (!inputFile)
        throw Error("Failed to open input file", "error-ende-input");

    size_t fileSize = static_cast<size_t>(inputFile.tellg());
    inputFile.seekg(0);

    if (fileSize < SALT_SIZE) {
        throw Error("Invalid file format", "error-ende-file-size");
    }

    std::array<uint8_t, SALT_SIZE> salt;
    inputFile.read(reinterpret_cast<char *>(salt.data()), SALT_SIZE);

    std::vector<uint8_t> encryptedData(fileSize - SALT_SIZE);
    inputFile.read(reinterpret_cast<char *>(encryptedData.data()), encryptedData.size());
    inputFile.close();

    Ende::EncryptionParams params = deriveKey(password, salt);
    std::vector<uint8_t> compressedData = decrypt(encryptedData, params);

    lzma_stream strm = LZMA_STREAM_INIT;
    lzma_ret ret = lzma_auto_decoder(&strm, UINT64_MAX, LZMA_CONCATENATED);
    if (ret != LZMA_OK)
        throw Error("Failed to initialize LZMA decoder", "error-ende-decompress");

    std::vector<uint8_t> outputData(BUFFER_SIZE);
    strm.next_in = compressedData.data();
    strm.avail_in = compressedData.size();
    strm.next_out = outputData.data();
    strm.avail_out = outputData.size();

    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile)
        throw Error("Failed to open output file", "error-ende-output");

    do {
        ret = lzma_code(&strm, LZMA_FINISH);
        if (strm.avail_out == 0 || ret == LZMA_STREAM_END) {
            size_t written = outputData.size() - strm.avail_out;
            outputFile.write(reinterpret_cast<char *>(outputData.data()), written);
            strm.next_out = outputData.data();
            strm.avail_out = outputData.size();
        }
    } while (ret == LZMA_OK);

    if (ret != LZMA_STREAM_END) {
        lzma_end(&strm);
        throw Error("Decompression failed", "error-ende-decompress-end");
    }

    lzma_end(&strm);
    outputFile.close();

    return true;
}