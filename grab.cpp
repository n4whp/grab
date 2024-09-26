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
#include <vector>
#include <sstream>
#include <fstream>

// Global variable to store the current mount point
std::string current_mount_point = "/mnt"; // Default mount point

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

// Function to list files in the current mount point (like ls -la)
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

// Function to copy a file to the current directory where the user is running the program
bool copy_file_to_current_dir(const std::string& source_file) {
    std::string destination_file = "./" + source_file;  // Save to current working directory
    std::ifstream source(source_file, std::ios::binary);
    if (!source.is_open()) {
        std::cerr << "Error: Could not open source file: " << source_file << std::endl;
        return false;
    }
    std::ofstream destination(destination_file, std::ios::binary);
    if (!destination.is_open()) {
        std::cerr << "Error: Could not open destination file in the current directory." << std::endl;
        return false;
    }
    destination << source.rdbuf();
    std::cout << "File '" << source_file << "' successfully uploaded to the current directory." << std::endl;
    return true;
}

// Function to remove a file
bool remove_file(const std::string& filename) {
    std::string filepath = current_mount_point + "/" + filename;
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
    std::string dirpath = current_mount_point + "/" + dir;
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
    std::string command = "rm -rf " + current_mount_point + "/" + dir;
    int result = system(command.c_str());
    if (result == 0) {
        std::cout << "Directory '" << dir << "' and its contents removed successfully." << std::endl;
        return true;
    } else {
        std::cerr << "Error: Could not remove directory '" << dir << "'." << std::endl;
        return false;
    }
}

bool copy_file_to_current_dir(const std::string& filename) {
    std::string source = current_mount_point + "/" + filename;
    std::string destination = "./" + filename; // Copy to current working directory

    // Copy file and change ownership
    std::string command = "sudo cp " + source + " " + destination + " && sudo chown $(id -u):$(id -g) " + destination;
    if (system(command.c_str()) == 0) {
        std::cout << "Successfully copied " << filename << " to the current directory." << std::endl;
    } else {
        std::cerr << "Error copying " << filename << std::endl;
    }
}


// Function to create a directory
bool create_directory(const std::string& dir) {
    std::string dirpath = current_mount_point + "/" + dir;
    if (mkdir(dirpath.c_str(), 0755) == 0) {
        std::cout << "Directory '" << dir << "' created successfully." << std::endl;
        return true;
    } else {
        std::cerr << "Error: Could not create directory '" << dir << "'." << std::endl;
        return false;
    }
}

// Function to print the current mount point
void show_current_mount_point() {
    std::cout << "Current mount point: " << current_mount_point << std::endl;
}

// Function to change the mount point
bool change_mount_point(const std::string& new_mount_point) {
    current_mount_point = new_mount_point;
    std::cout << "Mount point changed to: " << current_mount_point << std::endl;
    return true;
}

// Function to list all mount points and details
void list_all_mount_points() {
    std::ifstream mounts("/proc/mounts");
    if (!mounts.is_open()) {
        std::cerr << "Error: Could not open /proc/mounts." << std::endl;
        return;
    }
    
    std::string line;
    std::cout << "Mount Points:" << std::endl;
    while (std::getline(mounts, line)) {
        std::istringstream iss(line);
        std::string mount_point, fs_type;
        iss >> mount_point >> fs_type;
        std::cout << "Mount Point: " << mount_point << ", Filesystem Type: " << fs_type << std::endl;
    }
}

// Function to set the default mount point in the config file
bool set_default_mount_point(const std::string& mount_point) {
    std::ofstream config_file("/usr/local/bin/grab_config");
    if (!config_file.is_open()) {
        std::cerr << "Error: Could not open config file for writing." << std::endl;
        return false;
    }
    config_file << "default_mount_point=" << mount_point << std::endl;
    config_file.close();
    std::cout << "Default mount point set to: " << mount_point << " in /usr/local/bin/grab_config" << std::endl;
    return true;
}

// Function to print help
void print_help() {
    std::cout << "Grab V 1.0.0" << std::endl;
    std::cout << "Available commands:" << std::endl;
    std::cout << "  grab -ls               : List files in current mount point with detailed information." << std::endl;
    std::cout << "  grab -g [filename]     : Copies file from mount point and moves it to working DIR" << std::endl;
    std::cout << "  grab -u [filename]     : Upload a file to the current directory." << std::endl;
    std::cout << "  grab -rmf [filename]   : Remove a file from current mount point." << std::endl;
    std::cout << "  grab -rmd [dir]        : Remove an empty directory from current mount point." << std::endl;
    std::cout << "  grab -rmd -rf [dir]    : Remove a directory and its contents from current mount point." << std::endl;
    std::cout << "  grab -md [dir]         : Create a new directory in current mount point." << std::endl;
    std::cout << "  grab -smp [mount]     : Show the current mount point." << std::endl;
    std::cout << "  grab -cmp [mount]   : Change the current mount point." << std::endl;
    std::cout << "  grab -lmp           : List all mount points." << std::endl;
    std::cout << "  grab -sdmp [mount]: Set a default mount point in config." << std::endl;
    std::cout << "  grab -h             : Show this help message." << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }

    std::string command = argv[1];

    if (command == "-ls") {
        list_directory(current_mount_point);
    } else if (command == "-u") {
        if (argc < 3) {
            std::cerr << "Error: No filename provided for upload." << std::endl;
            return 1;
        }
        std::string filename = argv[2];
        copy_file_to_current_dir(filename);
    } else if (command == "-rmf") {
        if (argc < 3) {
            std::cerr << "Error: No filename provided for removal." << std::endl;
            return 1;
        }
        std::string filename = argv[2];
        remove_file(filename);
    } else if (command == "-rmd") {
        if (argc < 3) {
            std::cerr << "Error: No directory name provided for removal." << std::endl;
            return 1;
        }
        std::string dirname = argv[2];
        remove_directory(dirname);
    } else if (command == "-rmd" && argc > 3 && std::string(argv[2]) == "-rf") {
        std::string dirname = argv[3];
        remove_full_directory(dirname);
    } else if (command == "-md") {
        if (argc < 3) {
            std::cerr << "Error: No directory name provided to create." << std::endl;
            return 1;
        }
        std::string dirname = argv[2];
        create_directory(dirname);
    } else if (command == "-smp") {
        show_current_mount_point();
    } else if (command == "-cmp") {
        if (argc < 3) {
            std::cerr << "Error: No new mount point provided." << std::endl;
            return 1;
        }
        std::string new_mount_point = argv[2];
        change_mount_point(new_mount_point);
    } else if (command == "-lmp") {
        list_all_mount_points();
    } else if (command == "-sdmp") {
        if (argc < 3) {
            std::cerr << "Error: No mount point provided to set as default." << std::endl;
            return 1;
        }
        std::string mount_point = argv[2];
        set_default_mount_point(mount_point);

    } else if (command == "-g") {
        if (argc < 3) {
            std::cerr << "Error: No filename provided for grabbing." << std::endl;
            return 1;
        }
        std::string filename = argv[2];
        copy_file_to_current_dir(filename); 
    } else if (command == "-h") {
        print_help();
    } else {
        std::cerr << "Error: Unknown command." << std::endl;
        print_help();
        return 1;
    }

    return 0;
}
