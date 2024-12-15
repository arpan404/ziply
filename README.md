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

formatting:

```bash
find . -name '*.cpp' -o -name '*.hpp' -o -name '*.h' | xargs clang-format -i
```

| Command Options                                                   | Description                                                                                                                                                    |
| ----------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Creating a Ziply Video**                                        |                                                                                                                                                                |
| Command:                                                          | `ziply create [options]`                                                                                                                                       |
| **Mandatory Option:**                                             |                                                                                                                                                                |
| `-f <file_path>`                                                  | Path or name of the file to convert.                                                                                                                           |
| **Optional Options:**                                             |                                                                                                                                                                |
| `-o <output_path>`                                                | Path or name for the output file. (File extension is ignored if provided.)                                                                                     |
| `-r <resolution>`                                                 | Output video resolution. Options: 360p, 480p, 720p, 1080p [default], 1440p, 4k.                                                                                |
| `-p <secret_key>`                                                 | Secret key for encryption. [default: 'ziplySecret']                                                                                                            |
| `-c <ratio>`                                                      | Compression prevention (pixel-to-bit ratio). Options: 1[default], 2, 4, 8, 16. Higher values increase the file size, suitable for compression-based platforms. |
| **Examples:**                                                     |                                                                                                                                                                |
| `ziply create -f example.zip`                                     | Converts 'example.zip' into 'example.mp4' with default settings.                                                                                               |
| `ziply create -f example.png -o zipled -r 1080p -p mySecret -c 8` | Converts 'example.png' into 'zipled.mp4' at 1080p resolution, encrypted with 'mySecret', with a pixel-to-bit ratio of 8.                                       |
| **Restoring Original Data**                                       |                                                                                                                                                                |
| Command:                                                          | `ziply restore [options]`                                                                                                                                      |
| **Mandatory Option:**                                             |                                                                                                                                                                |
| `-f <file_path>`                                                  | Path or name of the video file to restore.                                                                                                                     |
| **Optional Options:**                                             |                                                                                                                                                                |
| `-o <output_path>`                                                | Path or name for the restored file. [default: current directory]                                                                                               |
| `-p <secret_key>`                                                 | Secret key for decryption. [default: 'ziplySecret']                                                                                                            |
| **Examples:**                                                     |                                                                                                                                                                |
| `ziply restore -f example.mp4`                                    | Restores the original data from 'example.mp4' using default settings.                                                                                          |
| `ziply restore -f example.mp4 -o zipled -p mySecret`              | Restores original data from 'example.mp4' to 'zipled', using the secret key 'mySecret'.                                                                        |

Requirements:
ffmpeg
