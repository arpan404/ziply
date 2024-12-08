#ifndef ENDE_HPP
#define ENDE_HPP

#include <array>
#include <fstream>
#include <lzma.h>         // LZMA for compression
#include <openssl/aes.h>  // OpenSSL AES for AES encryption
#include <openssl/evp.h>  // OpenSSL EVP for encryption and decryption
#include <openssl/rand.h> // OpenSSL RAND for random number generation
#include <openssl/sha.h>
#include <stdexcept>
#include <string>
#include <vector>
#include "error.hpp"

constexpr size_t PBKDF2_ITERATIONS = 10000;  // Recommended minimum iterations for PBKDF2
constexpr size_t IV_OFFSET = 32;             // Assuming this is the offset where IV starts in the key
constexpr size_t IV_SIZE = 16;               // Standard IV size for AES

// Class Ende handles encryption and decryption of data using AES and LZMA compression
class Ende
{
public:
    // Constants for key and IV sizes
    static constexpr int KEY_SIZE = 32;         // Size of the encryption key (in bytes)
    static constexpr int IV_SIZE = 16;          // Size of the initialization vector (in bytes)
    static constexpr size_t SALT_SIZE = 32;     // Size of the salt used in key derivation (in bytes)
    static constexpr size_t BUFFER_SIZE = 4096; // Buffer size for reading/writing data

    // Structure to hold encryption parameters
    struct EncryptionParams
    {
        std::array<uint8_t, KEY_SIZE> key;   // Encryption key
        std::array<uint8_t, IV_SIZE> iv;     // Initialization vector
        std::array<uint8_t, SALT_SIZE> salt; // Salt for key derivation
    };

    // Compresses and encrypts the input file, saving to the output file
    // inputFilePath: path to the input file
    // outputFilePath: path to the output file
    // password: password used for encryption
    // compressionLevel: level of compression (default is 9)
    static bool compressAndEncrypt(const std::string &inputFilePath, const std::string &outputFilePath, const std::string &password, uint32_t compressionLevel = 9);

    // Decompresses and decrypts the input file, saving to the output file
    // inputFilePath: path to the input file
    // outputFilePath: path to the output file
    // password: password used for decryption
    static bool decompressAndDecrypt(const std::string &inputFilePath, const std::string &outputFilePath, const std::string &password);

private:
    // Derives the encryption key and IV from the password and salt
    // password: the password used for key derivation
    // salt: the salt used in the key derivation process
    static EncryptionParams deriveKey(const std::string &password, const std::array<uint8_t, SALT_SIZE> &salt);

    // Encrypts the data using the provided encryption parameters
    // data: the data to be encrypted
    // params: the encryption parameters including key and IV
    static std::vector<uint8_t> encrypt(const std::vector<uint8_t> &data, const EncryptionParams &params);

    // Decrypts the data using the provided encryption parameters
    // data: the data to be decrypted
    // params: the encryption parameters including key and IV
    static std::vector<uint8_t> decrypt(const std::vector<uint8_t> &data, const EncryptionParams &params);
};

#endif