#!/bin/bash

# Variables
SOURCE_FILE="grab.cpp"
SHELL_SCRIPT="grab.sh"
OUTPUT_FILE="grab"
INSTALL_PATH="/usr/local/bin"

# Function to compile the C++ program
compile_cpp() {
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
}

# Function to install and run the shell script
install_shell_script() {
    # Check if the shell script exists
    if [ ! -f "$SHELL_SCRIPT" ]; then
        echo "Error: $SHELL_SCRIPT not found!"
        exit 1
    fi

    # Make the shell script executable
    echo "Making $SHELL_SCRIPT executable..."
    chmod +x "$SHELL_SCRIPT"
    if [ $? -ne 0 ]; then
        echo "Error: Failed to make $SHELL_SCRIPT executable!"
        exit 1
    fi

    # Move the shell script to /usr/local/bin (requires sudo)
    echo "Moving the shell script to $INSTALL_PATH..."
    sudo mv "$SHELL_SCRIPT" "$INSTALL_PATH"
    if [ $? -ne 0 ]; then
        echo "Error: Failed to move the shell script!"
        exit 1
    fi

    echo "The $SHELL_SCRIPT script has been successfully installed to $INSTALL_PATH."
    echo "You can now run the program with: $SHELL_SCRIPT"
}

# Ask the user if they want to compile the source or use the shell script
echo "What would you like to do?"
echo "1. Compile C++ source and install the program"
echo "2. Install and use the shell script instead"
read -p "Please choose an option (1 or 2): " choice

case $choice in
    1)
        compile_cpp
        ;;
    2)
        install_shell_script
        ;;
    *)
        echo "Invalid choice. Exiting."
        exit 1
        ;;
esac
