#include <iostream>
#include <cstdlib>   // for system()
#include <unistd.h>  // for getuid() and getlogin()

int main(int argc, char* argv[]) {
    // Check if at least one argument is provided
    if (argc < 2) {
        std::cerr << "Usage: grab [filename] [commands (optional)] or grab -ls\n";
        return 1;
    }

    // Check if the user wants to list files in /mnt
    if (std::string(argv[1]) == "-ls") {
        std::cout << "Listing files in /mnt directory:\n";
        if (system("ls /mnt") != 0) {
            std::cerr << "Error: Failed to list files in /mnt.\n";
            return 1;
        }
        return 0;
    }

    // Get the filename from the first argument
    std::string filename = argv[1];

    // Get any future commands passed as arguments (for now, just print them)
    if (argc > 2) {
        std::cout << "Additional commands provided (not yet implemented): ";
        for (int i = 2; i < argc; ++i) {
            std::cout << argv[i] << " ";
        }
        std::cout << std::endl;
    }

    // Construct full path for the file in /mnt
    std::string source_path = "/mnt/" + filename;

    // Get the current user's username and UID
    uid_t uid = getuid();
    const char* user = getlogin();

    if (user == nullptr) {
        std::cerr << "Error: Unable to get the username.\n";
        return 1;
    }

    // Step 1: Copy the file from /mnt to the current directory
    std::string copy_command = "sudo cp " + source_path + " .";
    std::cout << "Copying file: " << source_path << " to current directory...\n";
    if (system(copy_command.c_str()) != 0) {
        std::cerr << "Error: Failed to copy the file.\n";
        return 1;
    }

    // Step 2: Change ownership of the copied file to the current user
    std::string chown_command = "sudo chown " + std::string(user) + ":" + std::string(user) + " " + filename;
    std::cout << "Changing ownership of the file to: " << user << "\n";
    if (system(chown_command.c_str()) != 0) {
        std::cerr << "Error: Failed to change file ownership.\n";
        return 1;
    }

    std::cout << "File successfully grabbed and ownership updated.\n";

    return 0;
}
