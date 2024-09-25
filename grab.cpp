#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <ctime>
#include <iomanip>

// Function to get file permissions in string format
std::string get_permissions(mode_t mode) {
    std::string perms = "----------";
    perms[0] = S_ISDIR(mode) ? 'd' : '-';
    perms[1] = (mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (mode & S_IXUSR) ? 'x' : '-';
    perms[4] = (mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (mode & S_IXGRP) ? 'x' : '-';
    perms[7] = (mode & S_IROTH) ? 'r' : '-';
    perms[8] = (mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (mode & S_IXOTH) ? 'x' : '-';
    return perms;
}

// Function to print file details (similar to ls -la)
void print_file_info(const std::string& dir, const std::string& filename) {
    struct stat file_stat;
    std::string full_path = dir + "/" + filename;
    
    if (stat(full_path.c_str(), &file_stat) == 0) {
        // File permissions
        std::string perms = get_permissions(file_stat.st_mode);
        
        // Number of links
        int links = file_stat.st_nlink;
        
        // File owner and group
        struct passwd *pw = getpwuid(file_stat.st_uid);
        struct group  *gr = getgrgid(file_stat.st_gid);
        std::string owner = pw ? pw->pw_name : "unknown";
        std::string group = gr ? gr->gr_name : "unknown";
        
        // File size
        off_t size = file_stat.st_size;
        
        // Modification time
        char time_str[100];
        struct tm *timeinfo = localtime(&file_stat.st_mtime);
        strftime(time_str, sizeof(time_str), "%b %d %H:%M", timeinfo);
        
        // Print in a formatted way
        std::cout << std::left << std::setw(11) << perms << " "
                  << std::right << std::setw(3) << links << " "
                  << std::left << std::setw(8) << owner << " "
                  << std::setw(8) << group << " "
                  << std::right << std::setw(8) << size << " "
                  << std::left << std::setw(12) << time_str << " "
                  << filename << std::endl;
    }
}

// Function to list files in /mnt (like ls -la)
void list_directory(const std::string& dir) {
    DIR *dp;
    struct dirent *entry;
    
    dp = opendir(dir.c_str());
    if (dp == nullptr) {
        std::cerr << "Unable to open directory: " << dir << std::endl;
        return;
    }
    
    while ((entry = readdir(dp)) != nullptr) {
        // Skip "." and ".." directories
        if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..")
            continue;
        
        // Print file information
        print_file_info(dir, entry->d_name);
    }
    
    closedir(dp);
}

// Function to copy a file to /mnt
bool copy_file_to_mnt(const std::string& source_file) {
    std::string destination_file = "/mnt/" + source_file;
    
    // Open source file
    std::ifstream source(source_file, std::ios::binary);
    if (!source.is_open()) {
        std::cerr << "Error: Could not open source file: " << source_file << std::endl;
        return false;
    }
    
    // Open destination file
    std::ofstream destination(destination_file, std::ios::binary);
    if (!destination.is_open()) {
        std::cerr << "Error: Could not open destination file in /mnt." << std::endl;
        return false;
    }
    
    // Copy file contents
    destination << source.rdbuf();
    
    std::cout << "File '" << source_file << "' successfully uploaded to /mnt." << std::endl;
    
    return true;
}

int main(int argc, char* argv[]) {
    // Check for arguments
    if (argc < 2) {
        std::cerr << "Usage: grab [filename] [command]" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "-ls") {
        // List files in /mnt
        list_directory("/mnt");
    } else if (command == "-u") {
        if (argc < 3) {
            std::cerr << "Usage: grab -u [filename]" << std::endl;
            return 1;
        }
        
        std::string filename = argv[2];
        // Upload file to /mnt
        copy_file_to_mnt(filename);
    } else {
        // Handle other commands in the future
        std::cout << "Command not supported: " << command << std::endl;
    }

    return 0;
}
