# Tree_Shell_Command

I make a copy of tree command in Linux, but i don't implement all the optional arguments, you can find all with --help command <br>
typing "./example.out tree --help" on your prompt after compiling main.c 

Tree is a recursive directory listing program that produces a depth indented listing of files. With no arguments, tree lists the files in the current directory. Upon completion of listing all files/directories found, tree returns the total number of files and/or directories listed.

List of the optional arguments implemented: <br>
        -a: Print files and directories wich start with dot <br>
        -d: Print and visit only directories <br>
        -f: Prints the full path prefix for each file <br>
        -p: Print the file type and permissions for each file <br>
        -s: Print the size of each file in bytes along with the name <br>
        -u: Print the username <br>
        -g: Print the group name <br>
        -D: Print the date of the last modification time for the file visited <br>
        -r: Sort the output in reverse alphabetic order <br>
        -t: Sort the output by last modification time instead of alphabetically <br>
        -L: Max display depth of the directory tree <br>
        --inodes: Prints the inode number of the file or directory <br>
        --dirsfirst: List directories before files <br>
   
   This is an example of the default command: <br>
   <img width="422" alt="Schermata 2023-04-25 alle 01 03 20" src="https://user-images.githubusercontent.com/99051485/234134941-0c7071a8-22f4-4e13-83fc-f9991a229810.png">
