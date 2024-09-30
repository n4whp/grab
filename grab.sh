#!/bin/bash

# Default mount point
current_mount_point="/mnt"

# Function to display help
print_help() {
    echo "Grab V 1.0.0  (Shell)"
    echo "Available commands:"
    echo "  grab -ls               : List files in current mount point with detailed information."
    echo "  grab -g [filename]     : Copies file from mount point to the current working directory."
    echo "  grab -u [filename]     : Upload a file to the current directory."
    echo "  grab -rmf [filename]   : Remove a file from current mount point."
    echo "  grab -rmd [dir]        : Remove an empty directory from current mount point."
    echo "  grab -rmd -rf [dir]    : Remove a directory and its contents from current mount point."
    echo "  grab -md [dir]         : Create a new directory in current mount point."
    echo "  grab -smp              : Show the current mount point."
    echo "  grab -cmp [mount]      : Change the current mount point."
    echo "  grab -lmp              : List all mount points."
    echo "  grab -sdmp [mount]     : Set a default mount point in config."
    echo "  grab -h                : Show this help message."
}

# Function to list files in the current mount point (similar to ls -la)
list_directory() {
    echo "Listing files in $current_mount_point:"
    ls -la "$current_mount_point"
}

# Function to copy a file from mount point to current directory
copy_file_to_current_dir() {
    if [ -f "$current_mount_point/$1" ]; then
        cp "$current_mount_point/$1" .
        echo "File '$1' copied to current directory."
    else
        echo "Error: File '$1' not found in $current_mount_point."
    fi
}

# Function to remove a file from the current mount point
remove_file() {
    if [ -f "$current_mount_point/$1" ]; then
        rm "$current_mount_point/$1"
        echo "File '$1' removed successfully."
    else
        echo "Error: File '$1' not found in $current_mount_point."
    fi
}

# Function to remove a directory
remove_directory() {
    if [ -d "$current_mount_point/$1" ]; then
        rmdir "$current_mount_point/$1"
        echo "Directory '$1' removed successfully."
    else
        echo "Error: Directory '$1' not found or is not empty."
    fi
}

# Function to recursively remove a directory and its contents
remove_full_directory() {
    if [ -d "$current_mount_point/$1" ]; then
        rm -rf "$current_mount_point/$1"
        echo "Directory '$1' and its contents removed successfully."
    else
        echo "Error: Directory '$1' not found."
    fi
}

# Function to create a directory
create_directory() {
    mkdir -p "$current_mount_point/$1"
    echo "Directory '$1' created in $current_mount_point."
}

# Function to show the current mount point
show_current_mount_point() {
    echo "Current mount point: $current_mount_point"
}

# Function to change the current mount point
change_mount_point() {
    if [ -d "$1" ]; then
        current_mount_point="$1"
        echo "Mount point changed to: $current_mount_point"
    else
        echo "Error: '$1' is not a valid directory."
    fi
}

# Function to list all mount points
list_all_mount_points() {
    echo "Listing all mount points:"
    cat /proc/mounts | awk '{print $2}' | column
}

# Function to set a default mount point in a config file
set_default_mount_point() {
    echo "default_mount_point=$1" > ~/.grab_config
    echo "Default mount point set to: $1"
}

# Main script
if [ $# -lt 1 ]; then
    print_help
    exit 1
fi

command="$1"

case $command in
    -ls)
        list_directory
        ;;
    -g)
        if [ $# -lt 2 ]; then
            echo "Error: No filename provided for grabbing."
            exit 1
        fi
        copy_file_to_current_dir "$2"
        ;;
    -rmf)
        if [ $# -lt 2 ]; then
            echo "Error: No filename provided for removal."
            exit 1
        fi
        remove_file "$2"
        ;;
    -rmd)
        if [ "$2" == "-rf" ]; then
            remove_full_directory "$3"
        else
            remove_directory "$2"
        fi
        ;;
    -md)
        if [ $# -lt 2 ]; then
            echo "Error: No directory name provided to create."
            exit 1
        fi
        create_directory "$2"
        ;;
    -smp)
        show_current_mount_point
        ;;
    -cmp)
        if [ $# -lt 2 ]; then
            echo "Error: No new mount point provided."
            exit 1
        fi
        change_mount_point "$2"
        ;;
    -lmp)
        list_all_mount_points
        ;;
    -sdmp)
        if [ $# -lt 2 ]; then
            echo "Error: No mount point provided to set as default."
            exit 1
        fi
        set_default_mount_point "$2"
        ;;
    -h)
        print_help
        ;;
    *)
        echo "Error: Invalid command."
        print_help
        exit 1
        ;;
esac
