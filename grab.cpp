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
#include <string>
#include <cstdlib>

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
        std::string perms = get_permissions(file_stat.st_mode);
        int links = file_stat.st_nlink;
        struct passwd *pw = getpwuid(file_stat.st_uid);
        struct group  *gr = getgrgid(file_stat.st_gid);
        std::string owner = pw ? pw->pw_name : "unknown";
        std::string group = gr ? gr->gr_name : "unknown";
        off_t size = file_stat.st_size;
        
        char time_str[100];
        struct tm *timeinfo = localtime(&file_stat.st_mtime);
        strftime(time_str, sizeof(time_str), "%b %d %H:%M", timeinfo);
        
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
        if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..")
            continue;
        print_file_info(dir, entry->d_name);
    }
    closedir(dp);
}

// Function to copy a file to /mnt
bool copy_file_to_mnt(const std::string& source_file) {
    std::string destination_file = "/mnt/" + source_file;
    std::ifstream source(source_file, std::ios::binary);
    if (!source.is_open()) {
        std::cerr << "Error: Could not open source file: " << source_file << std::endl;
        return false;
    }
    std::ofstream destination(destination_file, std::ios::binary);
    if (!destination.is_open()) {
        std::cerr << "Error: Could not open destination file in /mnt." << std::endl;
        return false;
    }
    destination << source.rdbuf();
    std::cout << "File '" << source_file << "' successfully uploaded to /mnt." << std::endl;
    return true;
}

// Function to remove a file
bool remove_file(const std::string& filename) {
    std::string filepath = "/mnt/" + filename;
    if (remove(filepath.c_str()) == 0) {
        std::cout << "File '" << filename << "' removed successfully." << std::endl;
        return true;
    } else {
        std::cerr << "Error: Could not remove file '" << filename << "'." << std::endl;
        return false;
    }
}

// Function to remove an empty directory
bool remove_directory(const std::string& dir) {
    std::string dirpath = "/mnt/" + dir;
    if (rmdir(dirpath.c_str()) == 0) {
        std::cout << "Directory '" << dir << "' removed successfully." << std::endl;
        return true;
    } else {
        std::cerr << "Error: Could not remove directory '" << dir << "'." << std::endl;
        return false;
    }
}

// Function to remove a directory and its contents recursively
bool remove_full_directory(const std::string& dir) {
    std::string command = "rm -rf /mnt/" + dir;
    int result = system(command.c_str());
    if (result == 0) {
        std::cout << "Directory '" << dir << "' and its contents removed successfully." << std::endl;
        return true;
    } else {
        std::cerr << "Error: Could not remove directory '" << dir << "'." << std::endl;
        return false;
    }
}

// Function to create a directory
bool create_directory(const std::string& dir) {
    std::string dirpath = "/mnt/" + dir;
    if (mkdir(dirpath.c_str(), 0755) == 0) {
        std::cout << "Directory '" << dir << "' created successfully." << std::endl;
        return true;
    } else {
        std::cerr << "Error: Could not create directory '" << dir << "'." << std::endl;
        return false;
    }
}

// Function to print help
void print_help() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "  grab -ls               : List files in /mnt with detailed information." << std::endl;
    std::cout << "  grab -u [filename]     : Upload a file to /mnt." << std::endl;
    std::cout << "  grab -rmf [filename]   : Remove a file from /mnt." << std::endl;
    std::cout << "  grab -rmd [dir]        : Remove an empty directory from /mnt." << std::endl;
    std::cout << "  grab -rmfd [dir]       : Remove a directory and all its contents from /mnt." << std::endl;
    std::cout << "  grab -mkdir [dir]      : Create a directory in /mnt." << std::endl;
    std::cout << "  grab -help             : Show this help message." << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: grab [command] [args]" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "-help") {
        print_help();
    } else if (command == "-ls") {
        list_directory("/mnt");
    } else if (command == "-u") {
        if (argc < 3) {
            std::cerr << "Usage: grab -u [filename]" << std::endl;
            return 1;
        }
        std::string filename = argv[2];
        copy_file_to_mnt(filename);
    } else if (command == "-rmf") {
        if (argc < 3) {
            std::cerr << "Usage: grab -rmf [filename]" << std::endl;
            return 1;
        }
        std::string filename = argv[2];
        remove_file(filename);
    } else if (command == "-rmd") {
        if (argc < 3) {
            std::cerr << "Usage: grab -rmd [directory]" << std::endl;
            return 1;
        }
        std::string dir = argv[2];
        remove_directory(dir);
    } else if (command == "-rmfd") {
        if (argc < 3) {
            std::cerr << "Usage: grab -rmfd [directory]" << std::endl;
            return 1;
        }
        std::string dir = argv[2];
        remove_full_directory(dir);
    } else if (command == "-mkdir") {
        if (argc < 3) {
            std::cerr << "Usage: grab -mkdir [directory]" << std::endl;
            return 1;
        }
        std::string dir = argv[2];
        create_directory(dir);
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        print_help();
        return 1;
    }

    return 0;
}
