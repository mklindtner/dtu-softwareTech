#include "mklab2.h"
#include <stdio.h>
#include <unistd.h>   //rmdir
#include <sys/stat.h> //mkdir
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

typedef struct
{
    char *cmd;
    int (*function)(char *userinput, int inputL);
} commands;

static commands shellCommands[] =
    {
        {"stop", stop_f},
        {"mkdir", mkdir_f},
        {"touch", touch_f}
    };

int stop_f(char *userInput)
{
    return -1;
}

int mkdir_f(char *userinput)
{
    char *dirName = userinput[1];
    int mode = 0;
    checkErr(mkdir(dirName, mode));
    return 0;
}

int touch_f(char *filename)
{
    int fd, extSpace = 4;
    //allows creation, read/write, truncating and "0666" is octo for permission to read and write to file(rw-rw-rw)
    int fullAccess = O_CREAT | O_RDWR | O_TRUNC, rw = 0666;
    // char *filename = userInput[1];
    char extension[strlen(filename) + extSpace];
    //copy filename, then concatinate string w. extension
    strcpy(extension, filename);
    strcat(extension, ".txt");
    //opens and closes a file, because of mode O_CREAT, this creates the file
    checkErr(fd = open(extension, fullAccess, rw));
    checkErr(close(fd));
}

/*
for(...) {
const Command cmd;
if (is_match(cmd, inputStr){
int result = call(cmd);
return result
}
}
return command_not_found();
}

*/