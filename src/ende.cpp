#include "ende.hpp"

// Derives a key and IV from the provided password and salt using PBKDF2.
// Parameters:
// - password: The password used for key derivation.
// - salt: The salt used in the key derivation process.
// Returns: An EncryptionParams object containing the derived key and IV.
Ende::EncryptionParams Ende::deriveKey(const std::string &password, const std::array<uint8_t, SALT_SIZE> &salt) {
  Ende::EncryptionParams params; // Structure to hold the derived key and IV.
  std::array<uint8_t, 32> keyAndIv; // Buffer to hold the key and IV.

  // Derive the key and IV using PBKDF2 with HMAC-SHA256.
  if (!PKCS5_PBKDF2_HMAC(password.c_str(), password.size(), salt.data(), salt.size(), 10000, EVP_sha256(),
                         keyAndIv.size(), keyAndIv.data())) {
    throw Error("Failed to derive key and IV", "error-ende-key");
  }

  // Split the derived key and IV into the respective fields.
  std::copy(keyAndIv.begin(), keyAndIv.begin() + 16, params.key.begin());
  std::copy(keyAndIv.begin() + 16, keyAndIv.end(), params.iv.begin());
  return params; // Return the derived parameters.
}

// Encrypts the provided data using the specified encryption parameters.
// Parameters:
// - data: The data to be encrypted.
// - params: The encryption parameters containing the key and IV.
// Returns: A vector containing the encrypted data.
std::vector<uint8_t> Ende::encrypt(const std::vector<uint8_t> &data, const EncryptionParams &params) {
  std::vector<uint8_t> encryptedData; // Vector to hold the encrypted data.
  encryptedData.resize(data.size() + EVP_MAX_BLOCK_LENGTH); // Resize to accommodate potential padding.

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new(); // Create a new encryption context.
  if (!ctx)
    throw Error("Failed to create encryption context", "error-ende-enc1");

  // Initialize the encryption operation.
  if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, params.key.data(), params.iv.data())) {
    EVP_CIPHER_CTX_free(ctx);
    throw Error("Failed to initialize encryption", "error-ende-enc2");
  }

  int len = 0; // Length of the encrypted data.
  // Provide the data to be encrypted.
  if (!EVP_EncryptUpdate(ctx, encryptedData.data(), &len, data.data(), data.size())) {
    EVP_CIPHER_CTX_free(ctx);
    throw Error("Failed during encryption", "error-ende-enc3");
  }

  int finalLen = 0; // Length of the final block of encrypted data.
  // Finalize the encryption operation.
  if (!EVP_EncryptFinal_ex(ctx, encryptedData.data() + len, &finalLen)) {
    EVP_CIPHER_CTX_free(ctx);
    throw Error("Failed to finalize encryption", "error-ende-enc4");
  }

  encryptedData.resize(len + finalLen); // Resize to the actual size of the encrypted data.
  EVP_CIPHER_CTX_free(ctx); // Free the encryption context.
  return encryptedData; // Return the encrypted data.
}

// Decrypts the provided data using the specified decryption parameters.
// Parameters:
// - data: The data to be decrypted.
// - params: The decryption parameters containing the key and IV.
// Returns: A vector containing the decrypted data.
std::vector<uint8_t> Ende::decrypt(const std::vector<uint8_t> &data, const EncryptionParams &params) {
  std::vector<uint8_t> decryptedData(data.size()); // Vector to hold the decrypted data.

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new(); // Create a new decryption context.
  if (!ctx)
    throw Error("Failed to create decryption context", "error-ende-dec1");

  // Initialize the decryption operation.
  if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, params.key.data(), params.iv.data())) {
    EVP_CIPHER_CTX_free(ctx);
    throw Error("Failed to initialize decryption", "error-ende-dec2");
  }

  int len = 0; // Length of the decrypted data.
  // Provide the data to be decrypted.
  if (!EVP_DecryptUpdate(ctx, decryptedData.data(), &len, data.data(), data.size())) {
    EVP_CIPHER_CTX_free(ctx);
    throw Error("Failed during decryption", "error-ende-dec3");
  }

  int finalLen = 0; // Length of the final block of decrypted data.
  // Finalize the decryption operation.
  if (!EVP_DecryptFinal_ex(ctx, decryptedData.data() + len, &finalLen)) {
    EVP_CIPHER_CTX_free(ctx);
    throw Error("Failed to finalize decryption", "error-ende-dec4");
  }

  decryptedData.resize(len + finalLen); // Resize to the actual size of the decrypted data.
  EVP_CIPHER_CTX_free(ctx); // Free the decryption context.
  return decryptedData; // Return the decrypted data.
}

// Compresses and encrypts the input file, saving the result to the output file.
// Parameters:
// - inputFilePath: The path to the input file to be compressed and encrypted.
// - outputFilePath: The path to the output file where the result will be saved.
// - password: The password used for encryption.
// - compressionLevel: The level of compression to apply.
// Returns: true if the operation was successful.
bool Ende::compressAndEncrypt(const std::string &inputFilePath, const std::string &outputFilePath,
                              const std::string &password, uint32_t compressionLevel) {

  std::ifstream inputFile(inputFilePath, std::ios::binary | std::ios::ate); // Open the input file.
  if (!inputFile)
    throw Error("Failed to open input file", "error-ende-input");

  std::cout << "Compressing the file..." << std::endl;
  std::vector<uint8_t> inputData(static_cast<size_t>(inputFile.tellg())); // Read the entire file into memory.
  inputFile.seekg(0);
  inputFile.read(reinterpret_cast<char *>(inputData.data()), inputData.size());
  inputFile.close(); // Close the input file.

  lzma_stream strm = LZMA_STREAM_INIT; // Initialize the LZMA stream for compression.
  lzma_ret ret = lzma_easy_encoder(&strm, compressionLevel, LZMA_CHECK_CRC64); // Set up the encoder.
  if (ret != LZMA_OK)
    throw Error("Failed to initialize LZMA encoder", "error-ende-compress");

  std::vector<uint8_t> compressedData; // Vector to hold the compressed data.
  compressedData.resize(inputData.size() + BUFFER_SIZE); // Resize to accommodate the compressed data.

  // Set up the input and output buffers for the LZMA encoder.
  strm.next_in = inputData.data();
  strm.avail_in = inputData.size();
  strm.next_out = compressedData.data();
  strm.avail_out = compressedData.size();

  // Perform the compression.
  ret = lzma_code(&strm, LZMA_FINISH);
  if (ret != LZMA_STREAM_END) {
    lzma_end(&strm);
    throw Error("Compression failed", "error-ende-compress-end");
  }

  compressedData.resize(compressedData.size() - strm.avail_out); // Resize to the actual size of the compressed data.
  lzma_end(&strm); // Clean up the LZMA stream.

  std::cout << "Completed compressing the file ✅" << std::endl;
  std::array<uint8_t, SALT_SIZE> salt; // Buffer to hold the salt for encryption.
  if (!RAND_bytes(salt.data(), salt.size())) { // Generate a random salt.
    throw Error("Failed to generate salt", "error-ende-salt");
  }

  // Derive the encryption parameters using the password and generated salt.
  Ende::EncryptionParams params = deriveKey(password, salt);
  std::cout << "Encrypting the file..." << std::endl;
  std::vector<uint8_t> encryptedData = encrypt(compressedData, params); // Encrypt the compressed data.
  std::cout << "Completed encrypting the file ✅" << std::endl;

  std::ofstream outputFile(outputFilePath, std::ios::binary); // Open the output file for writing.
  if (!outputFile)
    throw Error("Failed to open output file", "error-ende-output");

  // Write the salt and encrypted data to the output file.
  outputFile.write(reinterpret_cast<char *>(salt.data()), salt.size());
  outputFile.write(reinterpret_cast<char *>(encryptedData.data()), encryptedData.size());
  outputFile.close(); // Close the output file.

  return true; // Indicate success.
}

// Decompresses and decrypts the input file, saving the result to the output file.
// Parameters:
// - inputFilePath: The path to the input file to be decrypted and decompressed.
// - outputFilePath: The path to the output file where the result will be saved.
// - password: The password used for decryption.
// Returns: true if the operation was successful.
bool Ende::decompressAndDecrypt(const std::string &inputFilePath, const std::string &outputFilePath,
                                const std::string &password) {

  std::ifstream inputFile(inputFilePath, std::ios::binary | std::ios::ate); // Open the input file.
  if (!inputFile)
    throw Error("Failed to open input file", "error-ende-input");

  std::cout << "Decrypting the file..." << std::endl;
  size_t fileSize = static_cast<size_t>(inputFile.tellg()); // Get the size of the input file.
  inputFile.seekg(0);

  if (fileSize < SALT_SIZE) { // Ensure the file is large enough to contain the salt.
    throw Error("Invalid file format", "error-ende-file-size");
  }

  std::array<uint8_t, SALT_SIZE> salt; // Buffer to hold the salt.
  inputFile.read(reinterpret_cast<char *>(salt.data()), SALT_SIZE); // Read the salt from the input file.

  std::vector<uint8_t> encryptedData(fileSize - SALT_SIZE); // Vector to hold the encrypted data.
  inputFile.read(reinterpret_cast<char *>(encryptedData.data()), encryptedData.size()); // Read the encrypted data.
  inputFile.close(); // Close the input file.

  // Derive the decryption parameters using the password and the read salt.
  Ende::EncryptionParams params = deriveKey(password, salt);
  std::vector<uint8_t> compressedData = decrypt(encryptedData, params); // Decrypt the data.
  std::cout << "Completed decrypting the file ✅" << std::endl;

  std::cout << "Decompressing the file..." << std::endl;
  lzma_stream strm = LZMA_STREAM_INIT; // Initialize the LZMA stream for decompression.
  lzma_ret ret = lzma_auto_decoder(&strm, UINT64_MAX, LZMA_CONCATENATED); // Set up the decoder.
  if (ret != LZMA_OK)
    throw Error("Failed to initialize LZMA decoder", "error-ende-decompress");

  std::vector<uint8_t> outputData(BUFFER_SIZE); // Vector to hold the decompressed data.
  strm.next_in = compressedData.data();
  strm.avail_in = compressedData.size();
  strm.next_out = outputData.data();
  strm.avail_out = outputData.size();

  std::ofstream outputFile(outputFilePath, std::ios::binary); // Open the output file for writing.
  if (!outputFile)
    throw Error("Failed to open output file", "error-ende-output");

  // Decompress the data in a loop until all data is processed.
  do {
    ret = lzma_code(&strm, LZMA_FINISH);
    if (strm.avail_out == 0 || ret == LZMA_STREAM_END) {
      size_t written = outputData.size() - strm.avail_out; // Calculate the amount of data written.
      outputFile.write(reinterpret_cast<char *>(outputData.data()), written); // Write the decompressed data to the output file.
      strm.next_out = outputData.data(); // Reset the output buffer.
      strm.avail_out = outputData.size();
    }
  } while (ret == LZMA_OK);

  if (ret != LZMA_STREAM_END) { // Check for errors during decompression.
    lzma_end(&strm);
    throw Error("Decompression failed", "error-ende-decompress-end");
  }

  lzma_end(&strm); // Clean up the LZMA stream.
  std::cout << "Completed decompressing the file ✅" << std::endl;
  outputFile.close(); // Close the output file.

  return true; // Indicate success.
}