#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mklab.h"
#include <unistd.h>   //rmdir
#include <sys/stat.h> //mkdir
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFSIZE 1024
#define maxWords 12;

int main(int argc, char **argv)
{
    receive_commands(argc, argv);
    return EXIT_SUCCESS; //0
}

//this function takes an allocated array and reads the value of stdin (probably the users input in this case),
//it returns -1 if the return value of fgets is NULL (unreadable, "\0" etc.)
//static makes this function only readable in its object file, meaning "mklab.o" that the gcc will convert
//this means we cannot link to function to other files (ex. "mklab2.o" could not contain this prompt_and_read)
static int prompt_and_read(char *prompt)
{
    // receives a buffer(arg0) limited to size(arg1) and file(arg3), writes file(stdin) to arg1(buffer)
    if (fgets(prompt, BUFSIZE, stdin) == NULL) //check if fgets succeds
        return -1;
    return 0;
}

void receive_commands(int argc, char **argv)
{
    int initSize = BUFSIZE;
    char *buf = malloc(sizeof(char) * initSize);
    printf("Terminal made by s205421\n");
    //all commands the terminal considers
    int words = 0;
    while (1)
    {
        //break if unable to read for some reason
        if (prompt_and_read(buf) == -1)
        {
            break;
        }
        char *trimBuf = trimwhitespace(buf);
        char **input = sliceWords(trimBuf, &words);
        // execvp(input[0], input);
        if (runShell(input, words) != 0)
        {
            break;
        }
        freeInput(input);
    }
}

//free memory after use
void freeInput(char **input)
{
    int row = sizeof(input) / sizeof(input[0]);
    for (int i = 0; i < row; i++)
    {
        free(input[i]);
    }
    free(input);
}

//check if POSIX implemented UNIX commands failed at their operation
void checkErr(int actionCode)
{
    if (actionCode < 0)
    {
        printf("action failed, stderr: %p\n", stderr);
    }
}

//split userinput = ["word", "word2"] etc. sets the address of words to the length of userinput
char **sliceWords(char *userInput, int *words)
{
    size_t length = (sizeof(userInput) / sizeof(userInput[0]));
    char **inputs = (char **)malloc(length * sizeof(char *));

    char *cmd = strtok(userInput, " ");
    int idx = 0;
    while (cmd != NULL)
    {
        inputs[idx] = (char *)malloc(strlen(cmd));
        inputs[idx++] = cmd;
        //get flags / arguments for command
        cmd = strtok(NULL, " -");
    }
    //cmd is omitted
    *words = idx - 1;
    return inputs;
}

//clean up whitespace in input from user, this must be used because of fgets (creates a whitespace)
char *trimwhitespace(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

//stop function
int stop_f(char **input, int inputL)
{
    return -1;
}

//create dir funtion
int mkdir_f(char **input, int inputL)
{
    char *dirName = input[0];
    int mode = 0;
    checkErr(mkdir(dirName, mode));
    return 0;
}

//makes a file w. default permissions
int touch_f(char **input, int inputL)
{
    char *filename = input[1];
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
    return 0;
}

//creates a pipe in which the child reads and the parent writes
int pipe_f(char **input, int inputL)
{

    int pipefd[2];
    pipe(pipefd);
    pid_t pid;
    char buf;
    write(STDOUT_FILENO, "-pipe started-\n", 15);

    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        return -1;
    }
    if (pid == 0) //child
    {
        close(pipefd[1]); //unused write end
        while (read(pipefd[0], &buf, 1) > 0)
        {
            write(STDOUT_FILENO, &buf, 1);
        }
        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]); //finished reading from pipe
        return 0;
    }
    else //parent
    {
        close(pipefd[0]); //unused read end
        for (int i = 1; i <= inputL; i++)
        {
            //we could also write this to a file ..
            write(pipefd[1], "\n", 1);
            write(pipefd[1], input[i], strlen(input[i]));
        }
        //finished writing
        close(pipefd[1]);

        return 0;
    }
}

//inputL is the amount of arguments, to get the first input you have to add +1 
int shell_f(char **input, int inputL)
{
    pid_t pid;
    pid = fork();
    checkErr(pid);        
    if (pid == 0) //child
    {
        if(inputL > 0) //has flags
        {
            char** flags = cutstring(input, 1, inputL);
            printf("\ninputL: %d\t flags[0]:%s", inputL, flags[0]);
            execvp(input[0], flags);            
            freeInput(flags);
        } 
        else 
        {
            execvp(input[0], input);
        }
        
    }
    else
    {
        wait(NULL); //wait for child?
        return 0;
    }
    return 0;
}

char** cutstring(char**str, int from, int to)
{
    printf("str[0]:%s", str[0]);
    //this allocates a bit to much space and should instead just allocate the space in str[from - to], but meh
    int strL = sizeof(str) / sizeof(str[0]); 
    char** res = malloc(sizeof(char) * strL);
    int base = 0;
    for(int i = from; i < from+1; i++)
    {
        res[base] = malloc(sizeof(char) * sizeof(str[i][0])); //set space
        res[base++] = str[i]; 
    }
    return res;
}

//hashing function
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = hash * 33 ^ c;
    return hash;
}

//contains a char* and a function to callback later
typedef struct
{
    char *cmd;
    int (*function)(char **userinput, int inputL);
} cmmds;

//a pseudo hashmap, returns a function, for a given char*
static cmmds shellCommands[] =
    {
        {"stop", stop_f},
        {"mkdir", mkdir_f},
        {"touch", touch_f},
        {"pipe", pipe_f},
        {"_default_shell", shell_f}};

//checks if a hashed word is equal to the hashed user command
//this has runtime O(n) and so it's much slower than an actual hashmap
int runShell(char **input, int inputL)
{
    int shellL = (sizeof(shellCommands) / sizeof(shellCommands[0]));
    char *ucmd = input[0];
    for (int i = 0; i < shellL; i++)
    {
        //check if user wrote a command that exists in shellCommands
        if (hash((unsigned char *)shellCommands[i].cmd) == hash((unsigned char *)ucmd))
        {
            if (shellCommands[i].function(input, inputL) < 0)
            {
                return -1;
            }
        }
    }
    return shellCommands[shellL-1].function(input, inputL); //go to default shell

    // return 0;
}

/*
    TODO: 
        Fix segfault for whitespace
        what does 0 for mode mean for mkdir?
        create a manual page
        create symlink(link)
        append to file
        pipeline
            stops after pipe input
            check for proper commands w. pipe
            rewrite pipe into "|"

        realloc or give error if input is to large
        wordsize is _really_ expensive for what it is, find a better way to determine size of array
*/
