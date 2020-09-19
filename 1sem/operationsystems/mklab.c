#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mklab.h"
#include "mklab2.h"
#include <unistd.h>   //rmdir
#include <sys/stat.h> //mkdir
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFSIZE 1024
#define maxWords 12;

//https://linux.die.net/man/3/mkfifo
int main(int argc, char **argv)
{
    receive_commands();
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

void receive_commands()
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
        int shellL = (sizeof(shellCommands) / sizeof(shellCommands[0]));
        for(int i = 0; i < shellL; i++)
        {

        }
        // if (shellCommands(input, words) != 0)
        // {
        //     break;
        // }
        //free the space to be used for next input
        freeInput(input);
        

        //check if input is larger than 1024mb and double if that's the case
    }
}


// int shellCommands(char **input, int words)
// {
//     //cmd
//     char *cmd = input[0];
//     char **cases = commands();


//     // printf("words size:%d\n", wordSize(input));
//     if (strcmp(cmd, cases[0]) == 0)
//     {
//         printf("called stop");
//         return -1;
//     }
//     //ls
//     if (strcmp(cmd, cases[1]) == 0)
//     {
//         printf("you said: %s", cmd);
//         return 0;
//     }
//     //mkdir
//     if (strcmp(cmd, cases[2]) == 0)
//     {
//         char *dirName = input[1];
//         int mode = 0;
//         checkErr(mkdir(dirName, mode));
//         return 0;
//     }
//     //rmdir
//     if (strcmp(cmd, cases[3]) == 0)
//     {
//         checkErr(rmdir(input[1]));
//     }
//     //rm file/empty directory
//     if (strcmp(cmd, cases[4]) == 0)
//     {
//         checkErr(remove(input[1]));
//     }
//     //touch
//     if (strcmp(cmd, cases[5]) == 0)
//     {
//         int fd, extSpace = 4;
//         //allows creation, read/write, truncating and "0666" is octo for permission to read and write to file(rw-rw-rw)
//         int fullAccess = O_CREAT | O_RDWR | O_TRUNC, rw = 0666;
//         char *filename = input[1];
//         char extension[strlen(filename) + extSpace];
//         //copy filename, then concatinate string w. extension
//         strcpy(extension, filename);
//         strcat(extension, ".txt");
//         //opens and closes a file, because of mode O_CREAT, this creates the file
//         checkErr(fd = open(extension, fullAccess, rw));
//         checkErr(close(fd));
//     }
//     //pipe
//     if (strcmp(cmd, cases[6]) == 0)
//     {
//         int pipefd[2];
//         pipe(pipefd);
//         pid_t pid;
//         char buf;
//         write(STDOUT_FILENO, "-pipe started-\n", 15);

//         pid = fork();
//         if (pid == -1)
//         {
//             perror("fork failed");
//             return -1;
//         }
//         if (pid == 0) //child
//         {
//             close(pipefd[1]); //unused write end
//             while (read(pipefd[0], &buf, 1) > 0)
//             {
//                 write(STDOUT_FILENO, &buf, 1);
//             }
//             write(STDOUT_FILENO, "\n", 1);
//             close(pipefd[0]); //finished reading from pipe
//             return 0;
//         }
//         else //parent
//         {
//             close(pipefd[0]); //unused read end
//             for(int i = 1; i <= words; i++)
//             {
//                 //we could also write this to a file ..
//                 write(pipefd[1], "\n", 1);
//                 write(pipefd[1], input[i], strlen(input[i]));
//             }
//             //finished writing
//             close(pipefd[1]);

//             return 0;
//         }
//     }
//     return 0;
// }

void freeInput(char **input)
{
    int row = sizeof(input) / sizeof(input[0]);
    for(int i = 0; i < row; i++)
    {
        free(input[i]);
    }
    free(input);
}

void checkErr(int actionCode)
{
    if (actionCode < 0)
    {
        printf("action failed, stderr: %p\n", stderr);
    }
}

//this will not work w. "smart" flags, so "rm -rf" won't work
char **sliceWords(char *userInput, int *words)
{
    size_t length = (sizeof(userInput) / sizeof(userInput[0]));
    char **inputs = (char **)malloc(length * sizeof(char*));

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

//clean up whitespace in input from user, a result of using fgets
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

char *addWord(char *word)
{
    char *allocatedWord = malloc(sizeof(char) * strlen(word));
    allocatedWord = word;
    return allocatedWord;
}

char **commands()
{
    int wordCount = maxWords;
    char **words = malloc(sizeof(char) * wordCount);
    words[0] = addWord("stop");
    words[1] = addWord("ls");
    words[2] = addWord("mkdir");
    words[3] = addWord("rmdir");
    words[4] = addWord("rm");
    words[5] = addWord("touch");
    words[6] = addWord("pipe");
    return words;
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

//if we run out of memory (from malloc), we will realloc(ate) more memory into the heap.
//in this
// char* shouldRealloc(int* curSize, int position, char* buf)
// {
//     if (position >= BUFSIZE) {
//       *curSize += BUFSIZE;
//       char *buffer = realloc(buf, curSize);
//       if (!buffer) {
//         fprintf(stderr, "lsh: allocation error\n");
//         exit(EXIT_FAILURE);
//       }
//     }
// }

// unsigned long hash(unsigned char *str)
// {
//     unsigned long hash = 5381;
//     int c;

//     while (c = *str++)
//         hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

//     return hash;
// }
