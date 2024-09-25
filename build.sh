#!/bin/bash

# Variables
SOURCE_FILE="grab.cpp"
OUTPUT_FILE="grab"
INSTALL_PATH="/usr/local/bin"

# Check if the source file exists
if [ ! -f "$SOURCE_FILE" ]; then
    echo "Error: $SOURCE_FILE not found!"
    exit 1
fi

# Compile the C++ source code
echo "Compiling $SOURCE_FILE..."
g++ -o "$OUTPUT_FILE" "$SOURCE_FILE"
if [ $? -ne 0 ]; then
    echo "Error: Compilation failed!"
    exit 1
fi
echo "Compilation successful!"

# Check if the output file was created
if [ ! -f "$OUTPUT_FILE" ]; then
    echo "Error: $OUTPUT_FILE was not created!"
    exit 1
fi

# Move the executable to /usr/local/bin (requires sudo)
echo "Moving the executable to $INSTALL_PATH..."
sudo mv "$OUTPUT_FILE" "$INSTALL_PATH"
if [ $? -ne 0 ]; then
    echo "Error: Failed to move the file!"
    exit 1
fi

# Make sure the file is executable
sudo chmod +x "$INSTALL_PATH/$OUTPUT_FILE"
if [ $? -ne 0 ]; then
    echo "Error: Failed to make the file executable!"
    exit 1
fi

echo "The $OUTPUT_FILE program has been successfully installed to $INSTALL_PATH."
echo "You can now run the program with: $OUTPUT_FILE"

