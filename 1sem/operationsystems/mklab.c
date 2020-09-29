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


int main(int argc, char **argv)
{
    receive_commands(argc, argv);
    return EXIT_SUCCESS; //0
}

//takes a buffer and inserts input from stdin, fails if input is to large
static int prompt_and_read(char *prompt)
{
    // receives a buffer(arg0) limited to size(arg1) and file(arg3), writes file(stdin) to arg1(buffer)
    //if input is greater than BUFSZIE, stop program
    if (fgets(prompt, BUFSIZE, stdin) == NULL)
        return -1;
    return 0;
}

//all commands the terminal considers
void receive_commands(int argc, char **argv)
{
    int initSize = BUFSIZE;
    //allocates space on the heap, the bufsize is the allowed input size and should have a 2^n value for good measure
    char *buf = malloc(sizeof(char) * initSize);
    printf("Terminal made by s205421\n");    
    int words = 0;
    //infinte loop
    while (1)
    {
        //stops if prompt fails  
        if (prompt_and_read(buf) == -1)
        {
            break;
        }
        //remove whitespace from user input
        char *trimBuf = trimwhitespace(buf);
        //slices input into words (ex. input=["word", "word2"] etc.)
        char **input = slice_words(trimBuf, &words);
        //expects run_shell to return !0 if fails
        if (run_shell(input, words) != 0)
        {
            break;
        }
        words = 0;
        //resets(0's out) the chunk of memory originally allocate, 
        //the program keeps the memory and doesn't release it to the OS until it terminates
        free_input(input);
    }
}

//check if input contains a '|' symbol, runs in O(n)
int is_pipe(char **input, int inputL)
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
void free_input(char **input)
{
    int row = sizeof(input) / sizeof(input[0]);
    for (int i = 0; i < row; i++)
    {
        free(input[i]);
    }
    free(input);
}

//check if POSIX implemented UNIX commands failed at their operation
void checkerr(int actionCode)
{
    if (actionCode < 0)
    {
        printf("action failed, stderr: %p\n", stderr);
    }
}

//takes a user input and its length, returns an array of words(**char)
char **slice_words(char *userInput, int *words)
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

/*
THIS IS COPIED CODE FOUND IN:
https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way 
TAKEN FROM ACCEPTED ANSWER
*/
//cleans up whitespace in input from user
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

//parameters made to fit signature for shellCommadns, returns -1 for run_shell
int stop_f(char **input, int inputL)
{
    return -1;
}

//takes a user input and its length, takes the second word and makes a .txt extension for said file
int touch_f(char **input, int inputL)
{
    //users second word
    char *filename = input[1];
    int fd, extSpace = 4;

    //allows creation, read/write, truncating and "0666" is octo for permission to read and write to file(rw-rw-rw)
    int fullAccess = O_CREAT | O_RDWR | O_TRUNC, rw = 0666;
    //makes a 1D array on the stack, size is filename+hardcoded extension
    char extension[strlen(filename) + extSpace];

    //copy filename, then concatinate string w. extension
    strcpy(extension, filename);
    strcat(extension, ".txt");
    //opens and closes a file, because of mode O_CREAT, this creates the file
    checkerr(fd = open(extension, fullAccess, rw));
    checkerr(close(fd));
    return 0;
}


//takes userinput, a start and end idx, 
//finds the next pipe and returns the words to the LEFT of the pipe sign
//this function should only be called when there's a guarantee of a '|' char
static char **next_pipe(char **input, int *from, int to)
{
    int nullTerm = 1;
    //allocates a size of the word in from and a null terminatro
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

//returns the amount of pipes in a word
static int pipes_count(char **input, int inputL)
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

//takes a user input and its length
//creates a process, a pipe and create a unidirectional pipe, copying the value of STDOUT_FILENO into pipe.
int pipe_f(char **input, int inputL)
{
    pid_t pid;
    //pipe has 2 ends, 0 is read, 1 is write
    int fd[2];
    int READ_END = 0;
    int WRITE_END = 1;
    //make a pipe from fd
    pipe(fd);
    int pipeidx = 0;
    //find words of left and right pipe
    char **leftside = next_pipe(input, &pipeidx, inputL);
    char **rightside = next_pipe(input, &pipeidx, inputL);

    //start a proces
    pid = fork();

    if (pid == 0) //child 
    {
        printf("here");
        //copy value of fd[WRITE_END] into stdout
        dup2(fd[WRITE_END], STDOUT_FILENO);
        //close ends
        close(fd[WRITE_END]);
        close(fd[READ_END]);
        //call a new process that replaces the current one
        //leftside[0] will be cmd to be executed to leftside of "|"
        execvp(leftside[0], leftside);
        //exit the current progress, made from execvp
        exit(1);
    }
    else //parent
    {   
        //make a new progress
        pid = fork();
        if (pid == 0) //second child of fork
        {
            //copy value of fd[READ_END] into stdin
            dup2(fd[READ_END], STDIN_FILENO);
            //rightside[0] is the cmd of left side of pipe
            execvp(rightside[0], rightside);
            //exit the current progress, made from execvp
            exit(1);
        }
    }
    //reset left and rightside
    free(leftside);
    free(rightside);
    return 0; 
}

//takes a user input and its length
//print out the values of the first argument, fails if less than 1 argument is provided
int cat_f(char **input, int inputL)
{
    //printout to stdout if no argument
    if (inputL < 2)
    {
        printf("please provide a file to read from");
        return 0;
    }

    char buf[1];

    //open argument as file, returns a file descriptor to tell the program where how far it has read in the file 
    int fd = open(input[1], O_RDONLY);
    //reads the filedesriptor, write 1 char into buf[],
    while (read(fd, buf, 1) > 0)
    {
        //write 1 char out to terminal(stdout) from buf
        write(STDOUT_FILENO, &buf, 1);
    }
    //close filedescriptor
    close(fd);
    return 0;
}

//executes a generic command from an already existing shell.
int shell_f(char **input, int inputL)
{
    pid_t pid;
    //printout an error if fork fails
    checkerr(pid = fork());
    char **flags;
    if (pid == 0) //child
    {
        //userinput has flags
        if (inputL > 1)
        {
            flags = cutflags(input, 1, inputL);
            //make a process and executes the command given in the terminal
            execvp(input[0], flags);
        }
        else
        {
            input[1] = NULL;
            execvp(input[0], input);
        }
    }
    else //parent
    {
        //wait for child
        wait(NULL);
    }
    return 0;
}

//changes directory to first argument
int cd_f(char **userinput, int inputL)
{
    chdir(userinput[1]);
    return 0;
}

//takes userinput and substring length
char **cutflags(char **userinput, int from, int to)
{
    //find length for userinput
    int strL = sizeof(userinput) / sizeof(userinput[0]);
    //allocates space for the first row, representing the pointers to each word
    char **flags = malloc(sizeof(char) * strL);
    int base = 0;
    for (int i = from; i < from + 1; i++)
    {
        //allocate space for each flag
        flags[base] = malloc(sizeof(char) * sizeof(userinput[i][0])); 
        //copy value from userinput[i] into flags[base] and then increment the idx to flags
        flags[base++] = userinput[i];
    }
    return flags;
}

//I recommend reading about hashing functions if this is new
//hashing function, the value unsigned long hash is arbitary, but start as a prime for a lesser chance of collition
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;
    //takes the ascii value for the given char.
    //Uses "Ands" 33 w. the current ascii value and mulitiplies it by the current hash
    while ((c = *str++))
        hash = hash * 33 ^ c;
    return hash;
}

//contains a char* and a function to callback later
//a callback is a function which can be called back at a later time (in rumtime, in this example)
typedef struct
{
    char *cmd;
    int (*function)(char **userinput, int inputL);
} cmmds;

//an array of cmmds objects, this is essentially a hashmap running in lookup time O(n)
static cmmds shellCommands[] =
    {
        {"exit", stop_f},
        {"touch", touch_f},
        {"cd", cd_f},
        {"cat", cat_f},
        {"_call_pipe", pipe_f},       //_call_pipe must be len(shellCommands) - 2
        {"_default_shell", shell_f}}; //_default_shell must be len(shellCommands) - 1

//checks if a hashed word is equal to the hashed user command
int run_shell(char **input, int inputL)
{
    int shellL = (sizeof(shellCommands) / sizeof(shellCommands[0]));
    char *ucmd = input[0];    
    if (is_pipe(input, inputL))
    {
        //call pipe_f function if pipe exists in command
        return shellCommands[shellL - 2].function(input, inputL);
    }
    for (int i = 0; i < shellL; i++)
    {
        //check if user wrote a command that exists in shellCommands
        if (hash((unsigned char *)shellCommands[i].cmd) == hash((unsigned char *)ucmd))
        {
            //call selfmade function in shellCommand
            if (shellCommands[i].function(input, inputL) < 0)
            {
                return -1;
            }
            else 
            {
                return 0;
            }
        }
    }
    //call default shell, if no selfmade function was found
    return shellCommands[shellL - 1].function(input, inputL); //go to default shell
}