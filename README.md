# Ziply

Ziply is a tool designed for efficient compression and encryption of files, allowing users to create and restore zipled files seamlessly. This README provides an overview of the functionality and usage of the Ziply application.

## Features

- **Create Zipled Files**: Compress and encrypt files into a zipled format.
- **Restore Files**: Decompress and decrypt zipled files back to their original format.
- **Multi-threaded Processing**: Utilizes a thread pool for efficient processing of frames during conversion.
- **Customizable Parameters**: Users can specify various parameters such as frame dimensions and password for encryption.

## Installation

To install Ziply, clone the repository and build the project using CMake:

```bash
git clone https://github.com/your-username/ziply.git
cd ziply
mkdir build
cd build
cmake ..
make
```

## Usage

To use Ziply, run the executable with the desired parameters. For example, to create a zipled file: