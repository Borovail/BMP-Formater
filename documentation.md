# BMP Image Transformation Tool

## Overview

This tool is designed for working with BMP (BitMaP) images, facilitating various transformations. It helps users learn about structured data types, standard streams, dynamic memory allocation, binary file operations, and more. The project also covers BMP format structure, endianness, and handling Base64 encoded data.

## Features

- Load and save BMP files.
- Apply transformations like rotation, flipping, cropping, and scaling.
- Handle input and output through standard streams or specified files.
- Logging functionalities to track and debug operations.

## Installation

To set up the BMP Image Transformation Tool, follow these steps:

1. Clone the project repository.
2. Navigate to the project directory.

    ```bash
    cd path/to/BMP-Formater
    ```

3. Compile the project using the Makefile.

    ```bash
    make
    ```

This will generate a `build` directory in your project where the compiled executable and other build files are located.

## Project Structure

The project directory, `BMP-Formater/`, includes the following files and directories:

- `transformation.c`, `transformation.h`: Source and header files for the image transformation logic.
- `bmp.c`, `bmp.h`: Source and header files for handling BMP file operations.
- `log.h`: Header file containing the logging functionalities, which helps in tracking operations and debugging.
- `main.c`: Contains the `main` function which processes command-line arguments and coordinates image transformations.
- `Makefile`: Defines rules for building the project. Key targets include:
  - Compiling individual modules (`bmp.o`, `transformation.o`).
  - Building the complete executable.
  - Cleaning up build artifacts with the `clean` target.
- `README`: A file providing an overview and basic instructions for the project.
- `build/`: This directory is created upon compilation and contains all executable and object files.
- `assets/`: A directory with sample BMP images used for testing and demonstration purposes.

## Usage

The tool is used via the command line interface with the following syntax:

    ```bash
    bmp [OPTION]... [FILE]...
    ```

### Options

- `-r`: Rotate the image to the right.
- `-l`: Rotate the image to the left.
- `-h`: Flip the image horizontally.
- `-v`: Flip the image vertically.
- `-c y,x,h,w`: Crop the image from the position `[y,x]` with the specified height `h` and width `w`.
- `-s factor`: Scale the image by a given factor.
- `-o file`: Write the output to a specified file.
- `-i file`: Read input from a specified file.

If no input file (`-i`) is specified, the program reads from standard input (stdin). Similarly, if no output file (`-o`) is specified, the program writes to standard output (stdout).

### Examples

Rotate the image `saboteur.bmp` to the right and save the result to `output.bmp`:

    ```bash
    bmp -i saboteur.bmp -o output.bmp -r
    ```

Chain multiple operations using pipes:

    ```bash
    bmp -i saboteur.bmp | bmp -r | bmp -l | bmp -o output.bmp
    ```

This approach allows flexible and powerful image processing in a sequence of transformations.
