#include "error.hpp" // Include custom error handling class
#include <array>     // Include for using fixed-size arrays
#include <fstream>   // Include for file input/output operations
#include <lzma.h>    // Include for LZMA compression and decompression
#include <openssl/aes.h> // Include for AES encryption
#include <openssl/evp.h> // Include for high-level encryption and decryption functions
#include <openssl/rand.h> // Include for random number generation
#include <openssl/sha.h>  // Include for SHA hashing functions
#include <stdexcept>      // Include for standard exceptions
#include <string>         // Include for using string data type
#include <vector>         // Include for using dynamic arrays

// Number of iterations for PBKDF2 key derivation function
constexpr size_t PBKDF2_ITERATIONS = 10000;
// Offset for the initialization vector (IV) in the encrypted data
constexpr size_t IV_OFFSET = 32;
// Size of the initialization vector (IV) used in encryption
constexpr size_t IV_SIZE = 16;

class Ende {
public:
  // Size of the encryption key in bytes
  static constexpr int KEY_SIZE = 32;
  // Size of the initialization vector (IV) in bytes
  static constexpr int IV_SIZE = 16;
  // Size of the salt used in key derivation
  static constexpr size_t SALT_SIZE = 32;
  // Size of the buffer used for reading/writing data
  static constexpr size_t BUFFER_SIZE = 4096;

  // Structure to hold encryption parameters including key, IV, and salt
  struct EncryptionParams {
    std::array<uint8_t, KEY_SIZE> key; // Encryption key
    std::array<uint8_t, IV_SIZE> iv;   // Initialization vector
    std::array<uint8_t, SALT_SIZE> salt; // Salt for key derivation
  };

  // Compresses the input file and encrypts it, saving to the output file
  // Parameters:
  // - inputFilePath: Path to the input file to be compressed and encrypted
  // - outputFilePath: Path to save the encrypted output file
  // - password: Password used for encryption
  // - compressionLevel: Level of compression (default is 9)
  static bool compressAndEncrypt(const std::string &inputFilePath, const std::string &outputFilePath,
                                 const std::string &password, uint32_t compressionLevel = 9);
  
  // Decompresses and decrypts the input file, saving to the output file
  // Parameters:
  // - inputFilePath: Path to the encrypted input file to be decrypted
  // - outputFilePath: Path to save the decrypted output file
  // - password: Password used for decryption
  static bool decompressAndDecrypt(const std::string &inputFilePath, const std::string &outputFilePath,
                                   const std::string &password);

private:
  // Derives the encryption key from the password and salt using PBKDF2
  // Parameters:
  // - password: The password used for key derivation
  // - salt: The salt used in the key derivation process
  static EncryptionParams deriveKey(const std::string &password, const std::array<uint8_t, SALT_SIZE> &salt);
  
  // Encrypts the given data using the specified encryption parameters
  // Parameters:
  // - data: The data to be encrypted
  // - params: The encryption parameters including key and IV
  static std::vector<uint8_t> encrypt(const std::vector<uint8_t> &data, const EncryptionParams &params);
  
  // Decrypts the given data using the specified encryption parameters
  // Parameters:
  // - data: The data to be decrypted
  // - params: The encryption parameters including key and IV
  static std::vector<uint8_t> decrypt(const std::vector<uint8_t> &data, const EncryptionParams &params);
};
