#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mklab.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFSIZE 256
#define maxWords 12

//dup2, exit(done), pipe(done), execvp(done)

int main(int argc, char **argv)
{
    receive_commands(argc, argv);
    return EXIT_SUCCESS; //0
}

//this function takes an allocated array and reads the value of stdin (the users input in this case)
static int prompt_and_read(char *prompt)
{
    // receives a buffer(arg0) limited to size(arg1) and file(arg3), writes file(stdin) to arg1(buffer)
    //if input is greater than BUFSZIE, stop program
    if (fgets(prompt, BUFSIZE, stdin) == NULL)
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
        if (runShell(input, words) != 0)
        {
            break;
        }
        // printf("cleaning up");
        words = 0;
        freeInput(input);
    }
}

int isPipe(char **input, int inputL)
{
    for (int i = 0; i < inputL; i++)
    {
        if (strstr(input[i], "|") != NULL)
        {
            return 1;
        }
    }
    return 0;
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

//returns char** representing an arrary of inputs sliced by empty space or - and sets int *words to the length of userinput
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
        cmd = strtok(NULL, " -");
    }
    inputs[idx++] = "\0";
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

//]from,to]
char **nextPipe(char **input, int *from, int to)
{
    int nullTerm = 1;
    char **lineOfPipes = malloc((sizeof(input) / sizeof(input[*from])) + nullTerm);
    int j = 0;
    for (int i = *from; i < to; i++)
    {
        *from += 1;
        if (!strcmp(input[i], "|"))
        {
            // printf("from is:%d\n", *from);
            lineOfPipes[j++] = NULL;

            return lineOfPipes;
        }
        lineOfPipes[j++] = input[i];
    }
    lineOfPipes[j++] = NULL;
    return lineOfPipes;
}

int pipesCount(char **input, int inputL)
{
    int pipeWord = 0;
    for (int i = 1; i < inputL; i++)
    {
        if (!strcmp("|", input[i]))
        {
            ++pipeWord;
        }
    }
    return pipeWord;
}


int pipe_f(char **input, int inputL)
{
    pid_t pid;
    int fd[2];
    int READ_END = 0;
    int WRITE_END = 1;
    pipe(fd);

    int pipeidx = 0;
    char **leftside = nextPipe(input, &pipeidx, inputL);
    char **rightside = nextPipe(input, &pipeidx, inputL);

    pid = fork();

    if (pid == 0) //ls -la | grep 'test'
    {
        printf("here");
        dup2(fd[WRITE_END], STDOUT_FILENO);
        close(fd[WRITE_END]);
        close(fd[READ_END]);
        
        execvp(leftside[0], leftside);
        exit(1);
    }
    else //parent
    {
        pid = fork();
        if (pid == 0) //second child of fork
        {
            dup2(fd[READ_END], STDIN_FILENO);            
            execvp(rightside[0], rightside);
            exit(1);
        }
    }
    free(leftside);
    free(rightside);

    return 0; //change to 0
}

// //creates a pipe in which the child reads and the parent writes
// int pipe_f(char **input, int inputL)
// {
//     int pipefd[2];
//     int read_end = 0;
//     int write_end = 1;
//     pipe(pipefd);
//     pid_t pid;
//     char buf;
//     write(STDOUT_FILENO, "-pipe started-\n", 15);

//     pid = fork();
//     if (pid == -1)
//     {
//         perror("fork failed");
//         return -1;
//     }
//     if (pid == 0) //child
//     {
//         close(pipefd[1]); //unused write end
//         while (read(pipefd[0], &buf, 1) > 0)
//         {
//             write(STDOUT_FILENO, &buf, 1);
//         }
//         write(STDOUT_FILENO, "\n", 1);
//         close(pipefd[0]); //finished reading from pipe
//         // exit(1);
//     }
//     else //parent
//     {
//         close(pipefd[0]); //unused read end
//         for (int i = 1; i <= inputL; i++)
//         {
//             //we could also write this to a file ..
//             write(pipefd[1], input[i], strlen(input[i]));
//         }
//         //finished writing
//         close(pipefd[1]);
//         printf("returning from parent in pipe");
//         // exit(1);
//     }
//     return 0;
// }

//inputL is the amount of arguments, to get the first input you have to add +1
int shell_f(char **input, int inputL)
{
    pid_t pid;
    pid = fork();
    checkErr(pid);
    char **flags;
    if (pid == 0) //child
    {

        if (inputL > 1) //has flags
        {
            flags = cutflags(input, 1, inputL);
            execvp(input[0], flags);
        }
        else
        {
            input[1] = NULL;
            execvp(input[0], input);
        }
    }
    else
    {
        wait(NULL); //wait for child, same as 0
    }
    return 0;
}

int cd_f(char **userinput, int inputL)
{
    chdir(userinput[1]);
    return 0;
}

char **cutflags(char **str, int from, int to)
{
    //this allocates a bit to much space and should instead just allocate the space in str[from - to], but meh
    int strL = sizeof(str) / sizeof(str[0]);
    char **res = malloc(sizeof(char) * strL);
    int base = 0;
    for (int i = from; i < from + 1; i++)
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

//an array of cmmds objects, this is static as we don't wish for other files to use it
static cmmds shellCommands[] =
    {
        {"exit", stop_f},
        {"touch", touch_f},
        // {"pipe", pipe_f},
        {"cd", cd_f},
        {"_call_pipe", pipe_f}, //_call_pipe must be len(shellCommands) - 2
        {"_default_shell", shell_f}}; //_default_shell must be len(shellCommands) - 1

//checks if a hashed word is equal to the hashed user command
//this has runtime O(n) and so it's much slower than an actual hashmap
int runShell(char **input, int inputL)
{
    int shellL = (sizeof(shellCommands) / sizeof(shellCommands[0]));
    char *ucmd = input[0];
    if (isPipe(input, inputL))
    {
        return shellCommands[shellL - 2].function(input, inputL);
    }
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
    return shellCommands[shellL - 1].function(input, inputL); //go to default shell
}

/*
    implement dup2 (DONE)
    find out why exit has to be called multiple times ??
    figure out why it hangs https://www.geeksforgeeks.org/pipe-system-call/
    make checkerr return result
    rewrite methods to use snake_case
*/

/*
// char **ck = malloc(sizeof(input) / sizeof(input[0]));
            // ck = input;
            // ck[0] = malloc(sizeof("grep"));
            // ck[0] = "grep";
            // ck[1] = malloc(sizeof("test"));
            // ck[1] = "test";
            // ck[2] = malloc(sizeof(NULL));
            // ck[2] = NULL;


*/