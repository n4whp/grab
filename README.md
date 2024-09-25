# grab

grab will automate the chown and cp funtions as noted in this class... With only one short command ```grab``` then be prompted to enter the file name. Keep in mind for this script you need to have a folder or device in ```/mnt```


to isntall grab all commands must be run as sudo: 

```git clone https://github.com/n4whp/grab.git ```

```cd grab```

```sudo chmod +x build.sh```

```sudo ./build.sh```

Now you can run grab!!! Need to run as sudo!! 



Available Commands:
```
    grab -ls: Lists all files and directories in /mnt, displaying detailed file information (permissions, owner, size, etc.).
    grab -u [filename]: Uploads the specified file to /mnt.
    grab -rmf [filename]: Removes the specified file from /mnt.
    grab -rmd [directory]: Removes an empty directory from /mnt.
    grab -rmfd [directory]: Recursively removes a directory and all of its contents from /mnt.
    grab -mkdir [directory]: Creates a new directory in /mnt.
    grab -help: Displays the help guide with all available commands and their usage.
```
