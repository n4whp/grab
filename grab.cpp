#include <iostream>
#include <cstdlib>   // for system()
#include <unistd.h>  // for getuid() and getlogin()

int main() {
    std::string filename;
    
    std::cout << "Enter the filename to grab (from /mnt): ";
    std::cin >> filename;

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
