# grab V 1.0.0

grab is a command-line tool that helps you easily manage files and folders in the /mnt directory. If you're working with mounted drives and need to upload, delete, or organize files, this tool makes it much easier with simple commands.



# Install 

to isntall grab all commands must be run as sudo: 

```git clone https://github.com/n4whp/grab.git ```

```cd grab```

```sudo chmod +x build.sh```

```sudo ./build.sh```

Now you can run grab!!! Need to run as sudo!! 



# Available Commands:
```
    grab -ls: Lists all files and directories in /mnt, displaying detailed file information (permissions, owner, size, etc.).
    grab -u [filename]: Uploads the specified file to /mnt.
    grab -rmf [filename]: Removes the specified file from /mnt.
    grab -rmd [directory]: Removes an empty directory from /mnt.
    grab -rmfd [directory]: Recursively removes a directory and all of its contents from /mnt.
    grab -mkdir [directory]: Creates a new directory in /mnt.
    grab -help: Displays the help guide with all available commands and their usage.
```

# How to Use grab

Here’s how you can use the tool for different tasks:

## List Files in /mnt:
    
    sudo grab -ls

 This will show all files and directories inside /mnt with extra details like file permissions, size, and last modified date.

## Upload a File to /mnt:

Run:

    sudo grab -u yourfile.txt

This will copy the file yourfile.txt into /mnt.

## Remove a File:

Run:

    sudo grab -rmf yourfile.txt

This removes the file yourfile.txt from /mnt.

## Remove an Empty Directory:

Run:

    sudo grab -rmd yourdir

This will delete the empty directory yourdir from /mnt.

## Remove a Directory and Its Contents:

Run:

    sudo grab -rmfd yourdir

This deletes yourdir along with everything inside it.

## Create a Directory in /mnt:

Run:
    
    sudo grab -mkdir newdir

This creates a new directory called newdir inside /mnt.

## Help Command:

If you’re not sure what to do, you can always run:

    grab -help

This will list all the available commands and explain how to use them.
