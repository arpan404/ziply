# Ziply

Ziply is a powerful tool designed to convert any file into a video format, allowing for easy restoration later. It supports various resolutions, ensuring that your output meets your desired quality. Additionally, Ziply offers encryption options to secure your files during the conversion process, providing peace of mind for sensitive data.

---

## ⚙️ Installation and Compilation

### Prerequisites

- CMake
- C++ Compiler (g++, Visual Studio, or MinGW)
- **Additional Libraries**:
  - OpenCV (4.5.0 or later)
  - OpenSSL (1.1.1 or later)
  - LibLZMA (5.2.0 or later)

### Library Installation

#### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install -y \
    cmake g++ \
    libopencv-dev \
    libssl-dev \
    liblzma-dev
```

#### macOS (via Homebrew)

```bash
brew install \
    cmake \
    opencv \
    openssl \
    xz
```

#### Windows

1. **Manual Library Installation**:

   - Download and install [OpenCV](https://opencv.org/releases/)
   - Download [OpenSSL](https://slproweb.com/products/Win32OpenSSL.html)
   - Download [LibLZMA](https://tukaani.org/xz/)

   **Recommended**: Use vcpkg for easier dependency management

   ```bash
   vcpkg install opencv
   vcpkg install openssl
   vcpkg install liblzma
   ```

### Compilation Steps

#### Linux (Ubuntu)

1. **Clone the Repository**:

```bash
git clone https://github.com/your-username/ziply.git
cd ziply
```

2. **Create a Build Directory**:

```bash
mkdir build
cd build
```

3. **Run CMake**:

```bash
cmake -DCMAKE_PREFIX_PATH="/usr/local/lib;/usr/lib" ..
```

4. **Compile the Code**:

```bash
make
```

5. **Run the Application**:

```bash
./ziply
```

#### macOS (via Homebrew)

1. **Clone the Repository**:

```bash
git clone https://github.com/your-username/ziply.git
cd ziply
```

2. **Create a Build Directory**:

```bash
mkdir build
cd build
```

3. **Run CMake**:

```bash
cmake -DCMAKE_PREFIX_PATH="/usr/local/opt/opencv;/usr/local/opt/openssl;/usr/local/opt/xz" ..
```

4. **Compile the Code**:

```bash
make
```

5. **Run the Application**:

```bash
./ziply
```

#### Windows

1. **Clone the Repository**:

```bash
git clone https://github.com/your-username/ziply.git
cd ziply
```

2. **Create a Build Directory**:

```bash
mkdir build
cd build
```

3. **Run CMake**:
   - For Visual Studio (with vcpkg):
     ```bash
     cmake -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake ..
     ```
   - For MinGW:
     ```bash
     cmake -G "MinGW Makefiles" ..
     ```
4. **Compile the Code**:
   - For Visual Studio: Open the generated `.sln` file and build the solution
   - For MinGW:
     ```bash
     mingw32-make
     ```
5. **Run the Application**:

```bash
ziply.exe
```

### Optional: System-wide Installation

#### Linux/macOS

```bash
sudo mv ziply /usr/local/bin/
```

#### Windows

Add the directory containing `ziply.exe` to the system PATH environment variable.

## Additional Notes

- Ensure you have the latest versions of CMake and required libraries
- Check system compatibility before installation
- Verify library dependencies are correctly linked
- Report any compilation or dependency issues on the project's GitHub repository

## Troubleshooting

- If CMake cannot find libraries, manually specify library paths using:
  ```bash
  cmake -DCMAKE_PREFIX_PATH="/path/to/libraries" ..
  ```
- Ensure all prerequisite libraries are installed with development headers

---

## 🚀 Usage

The general syntax for the tool is as follows:

```bash
ziply <command> [options]
```

---

## 📝 Commands

Ziply supports the following commands:

### 1. **`create`**

Create a Ziply video file from an input file.

#### Syntax:

```bash
ziply create -f <input_file> [-o <output_file>] [-r <resolution>] [-p <password>]
```

#### Options:

- `-f <input_file>` : Path or name of the input file to convert (mandatory).
- `-o <output_file>` : Path or name of the output file. The extension is ignored if provided (optional).
- `-r <resolution>` : Output video resolution.
  **Options**: `360p`, `480p`, `720p`, `1080p` (default), `1440p`, `4k`.
- `-p <password>` : Set a password for encryption or protection (optional).

#### Example:

```bash
ziply create -f input.mp4 -o output_video -r 1080p -p MySecretKey
```

### 2. **`restore`**

Restore data from a Ziply video file.

#### Syntax:

```bash
ziply restore -f <input_file> [-o <output_file>] [-p <password>]
```

#### Options:

- `-f <input_file>` : Path or name of the input Ziply file (mandatory).
- `-o <output_file>` : Path or name of the restored output file (optional).
- `-p <password>` : Password for restoring the file, if encryption was used (optional).

#### Example:

```bash
ziply restore -f output_video.ziply -o restored_file -p MySecretKey
```

### 3. **`--help`**

Display help text with all available commands and options.

#### Syntax:

```bash
ziply --help
```

#### Example:

```bash
ziply --help
```

---

## ❗ Error Handling

Ziply provides detailed error messages to help troubleshoot issues. Examples include:

- Missing mandatory arguments
- Invalid commands or options
- Unsupported resolutions

For example:

```bash
ziply create
# Error: "Invalid argument set to create a zipled file..."
```

To resolve such errors, refer to the help text using `ziply --help`.

---

## 🌟 Examples of Usage

1. **Create a video with default resolution:**

```bash
ziply create -f input.ext
```

2. **Create a video with 720p resolution and a password:**

```bash
ziply create -f input.ext -o my_video -r 720p -p Secret123
```

3. **Restore a file with a password:**

```bash
ziply restore -f my_video.mp4 -o restored_file -p Secret123
```

4. **View the help text:**

```bash
ziply --help
```

---

## 🛠️ Support

If you encounter any issues, feel free to open a support ticket or
consult the `--help` documentation for more guidance.

---

## 📄 License

This project is licensed under the MIT License.

### License Summary

The MIT License is a permissive free software license that allows you to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of this software, as long as you include the original copyright and permission notice in all copies or substantial portions of the software.

### MIT License

**Copyright (c) 2024 Arpan Bhandari**

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and its associated documentation (the "Software"), to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit others to whom the Software is provided to do so, subject to the following conditions:

- The above copyright notice and this permission notice must be included in all copies or substantial portions of the Software.

**Disclaimer:**

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE, ARISING FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# Ziply

Ziply is a powerful tool designed to convert any file into a video format, allowing for easy restoration later. It supports various resolutions, ensuring that your output meets your desired quality. Additionally, Ziply offers encryption options to secure your files during the conversion process, providing peace of mind for sensitive data.

---

## ⚙️ Installation and Compilation

[Previous Installation and Compilation section remains unchanged]

---

## 🤝 Contributing to Ziply

We welcome contributions from the community! Here's how you can help:

### Ways to Contribute

1. **Reporting Bugs**

   - Use GitHub Issues to report any bugs you find
   - Include detailed information about your environment
   - Provide steps to reproduce the issue

2. **Suggesting Enhancements**

   - Open an issue to discuss new features
   - Provide clear use cases and potential implementation approaches

3. **Code Contributions**
   - Fork the repository
   - Create a new branch for your feature or bugfix
   - Ensure your code follows the project's coding standards
   - Write tests for new functionality
   - Submit a pull request with a clear description of changes

### Contribution Guidelines

- Follow existing code style and formatting
- Write clear, concise commit messages
- Update documentation when making changes
- Ensure to test the program ans format it according to '.clang-format' before submitting a pull request
- Be respectful and constructive in all communications

### Development Setup

1. Clone the repository
2. Install all dependencies
3. Compile and test the setup before commiting changes

```bash
cmake . && make
```

### Code of Conduct

- Be inclusive and respectful
- Constructive feedback is encouraged
- No harassment or discriminatory language will be tolerated

### Reporting Security Issues

For security vulnerabilities, please contact @arpan404 instead of creating a public issue.

---
