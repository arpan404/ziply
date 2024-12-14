#include "generator.hpp"
#include "ende.hpp"

Generator::Generator(const std::string &fileName, const std::string &outputFileName, const std::string &password, const int frameWidth, const int frameHeight, const float bitPixelRatio)
{
    this->inputFileName = fileName;
    this->outputFileName = outputFileName;
    this->password = password;
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
    this->bitPixelRatio = bitPixelRatio;
}

std::vector<std::future<std::vector<char>>> Generator::readFileInChunks(const fs::path& filePath, size_t chunkSize) 
{
    std::vector<std::future<std::vector<char>>> futures;
    
    // Open file and get its size
    std::shared_ptr<std::ifstream> file = std::make_shared<std::ifstream>(filePath, std::ios::binary);
    if (!file->is_open()) {
        throw std::runtime_error("Could not open file: " + filePath.string());
    }

    file->seekg(0, std::ios::end);
    size_t fileSize = file->tellg();
    file->seekg(0, std::ios::beg);

    // Calculate number of chunks
    size_t numChunks = (fileSize + chunkSize - 1) / chunkSize;

    // Create async tasks for each chunk
    for (size_t i = 0; i < numChunks; ++i) {
        size_t offset = i * chunkSize;
        size_t currentChunkSize = std::min(chunkSize, fileSize - offset);

        // Launch async task
        futures.push_back(std::async(std::launch::async,
            [file, offset, currentChunkSize, this]() {
                return this->readFileChunk(*file, offset, currentChunkSize);
            }
        ));
    }

    return futures;
}

std::vector<char> Generator::readFileChunk(std::ifstream& file, size_t offset, size_t chunkSize)
{
    std::vector<char> buffer(chunkSize);
    
    // Set the file position
    file.seekg(offset);
    
    // Read the chunk
    file.read(buffer.data(), chunkSize);
    
    // Resize buffer to actual number of bytes read
    size_t bytesRead = file.gcount();
    buffer.resize(bytesRead);
    
    return buffer;
}

void Generator::generate()
{
    std::vector<fs::path> filePaths = file::getConvertFilePath(this->inputFileName, this->outputFileName);
    fs::path outputFilePath = filePaths[1].replace_extension(".ziply");
    Ende::compressAndEncrypt(filePaths[0], outputFilePath, this->password, 9);
    
    // Calculate chunk size based on frame dimensions and bit-pixel ratio
    size_t chunkSize = static_cast<size_t>((this->frameWidth * this->frameHeight) / this->bitPixelRatio);
    
    try {
        // Read file in chunks asynchronously
        auto futures = readFileInChunks(outputFilePath, chunkSize);
        
        // Process the chunks as they complete
        for (auto& future : futures) {
            std::vector<char> chunk = future.get();  // This will wait for the chunk to be ready
            std::cout << "Processed chunk of size: " << chunk.size() << std::endl;
            // Process your chunk here
        }
    } catch (const std::exception& e) {
        std::cerr << "Error processing file: " << e.what() << std::endl;
        throw;
    }
}

void Generator::restore()
{
    // Get absolute paths for both input and output files
    std::vector<fs::path> filePaths = file::getConvertFilePath(this->inputFileName, this->outputFileName);
    fs::path inputFilePath = filePaths[0];
    fs::path outputFilePath = filePaths[1].replace_extension(".png");

    // Add file existence check
    if (!fs::exists(inputFilePath))
    {
        std::cerr << "Error: Input file '" << inputFilePath << "' does not exist." << std::endl;
        throw std::runtime_error("Input file not found");
    }

    // Ensure we have read permissions for input file
    if (!fs::is_regular_file(inputFilePath))
    {
        std::cerr << "Error: '" << inputFilePath << "' is not a regular file." << std::endl;
        throw std::runtime_error("Invalid input file");
    }

    std::cout << inputFilePath << std::endl;

    try
    {
        // Add debug output before decompression
        std::cout << "Starting decompression and decryption..." << std::endl;
        Ende::decompressAndDecrypt(inputFilePath, outputFilePath, this->password);
        std::cout << "Decompression and decryption completed successfully" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Detailed error during decompression: " << e.what() << std::endl;
        throw;
    }
}