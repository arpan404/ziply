#include "error.hpp"
#include <array>
#include <fstream>
#include <lzma.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <stdexcept>
#include <string>
#include <vector>

constexpr size_t PBKDF2_ITERATIONS = 10000;
constexpr size_t IV_OFFSET = 32;
constexpr size_t IV_SIZE = 16;

class Ende {
  public:
    static constexpr int KEY_SIZE = 32;
    static constexpr int IV_SIZE = 16;
    static constexpr size_t SALT_SIZE = 32;
    static constexpr size_t BUFFER_SIZE = 4096;

    struct EncryptionParams {
        std::array<uint8_t, KEY_SIZE> key;
        std::array<uint8_t, IV_SIZE> iv;
        std::array<uint8_t, SALT_SIZE> salt;
    };

    static bool compressAndEncrypt(const std::string &inputFilePath, const std::string &outputFilePath,
                                   const std::string &password, uint32_t compressionLevel = 9);
    static bool decompressAndDecrypt(const std::string &inputFilePath, const std::string &outputFilePath,
                                     const std::string &password);

  private:
    static EncryptionParams deriveKey(const std::string &password, const std::array<uint8_t, SALT_SIZE> &salt);
    static std::vector<uint8_t> encrypt(const std::vector<uint8_t> &data, const EncryptionParams &params);
    static std::vector<uint8_t> decrypt(const std::vector<uint8_t> &data, const EncryptionParams &params);
};
