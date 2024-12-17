# Ziply

Ziply is a powerful tool designed to convert any file into a video format, allowing for easy restoration later. It supports various resolutions, ensuring that your output meets your desired quality. Additionally, Ziply offers encryption options to secure your files during the conversion process, providing peace of mind for sensitive data.


-------------------------------------------------------------------

## ⚙️ Installation

To use `ziply`, download the executable or compile it from the source code.

### Compiling the Source Code

Ensure you have a C++ compiler installed (e.g., `g++`):

```bash
g++ -std=c++11 -o ziply main.cpp parser.cpp error.cpp
```

-------------------------------------------------------------------

## 🚀 Usage

The general syntax for the tool is as follows:

```bash
ziply <command> [options]
```

-------------------------------------------------------------------

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

-------------------------------------------------------------------

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

-------------------------------------------------------------------

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

-------------------------------------------------------------------

## ❗ Error Handling

Ziply provides detailed error messages to help troubleshoot issues. Examples include:
- Missing mandatory arguments.
- Invalid commands or options.
- Unsupported resolutions.

For example:
```bash
ziply create
# Error: "Invalid argument set to create a zipled file..."
```

To resolve such errors, refer to the help text using `ziply --help`.

-------------------------------------------------------------------

## 🌟 Examples of Usage

1. **Create a video with default resolution:**
```bash
ziply create -f input.mp4
```

2. **Create a video with 720p resolution and a password:**
```bash
ziply create -f input.mp4 -o my_video -r 720p -p Secret123
```

3. **Restore a file with a password:**
```bash
ziply restore -f my_video.ziply -o restored_file -p Secret123
```

4. **View the help text:**
```bash
ziply --help
```

-------------------------------------------------------------------

## 🛠️ Support

If you encounter any issues, feel free to open a support ticket or 
consult the `--help` documentation for more guidance.

-------------------------------------------------------------------

## 📄 License

This project is licensed under the MIT License. Below are the details of the license.

---

### License Summary

The MIT License is a permissive free software license that allows you to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of this software, as long as you include the original copyright and permission notice in all copies or substantial portions of the software.

---

### MIT License

**Copyright (c) 2024 Arpan Bhandari**

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and its associated documentation (the "Software"), to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit others to whom the Software is provided to do so, subject to the following conditions:

- The above copyright notice and this permission notice must be included in all copies or substantial portions of the Software.

**Disclaimer:**

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE, ARISING FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
