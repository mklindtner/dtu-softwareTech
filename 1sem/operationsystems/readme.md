
# Manual:

## compiler:
Clang: Recommended compiler the program.
GCC: Can be used but there may be unexpected errors. 

## Program Environment: <br />
This program has been tested on: Windows10 running wsl2:ubuntu 18.04



## Usage (Assumes Clang is used): <br />
This program assumes that a default shell is installed on the system and that the location for this shell is used when calling execvp. The user of the program is responsible
for ensuring "execvp" works as expected. (for linux, the default location for shell commands is located in"/bin")

1) Ensure you're standing in the path you opened the zip file
2) Compile the file "mklab.c"; clang -o selfmade_term mklab.c 
3) You're now presented with a range of possibilities. See Examples for more


## Examples <br />
ls //lists files in current directory <br />
ls | grep <regular expression> //outputs whatever was caught in the Regex <br />
cat my_file.extension //prints the content of file <br />
touch my_file //outputs myfile.txt <br />
exit //stops program <br />
  
## Program Definitions <br />
### I/O redirection: <br />
This is a way for any process to read and write from a predefined input and output. In C, it's called "STDIN_FILENO" and "STDOUT_FILENO", C also provides a "stdin" and "stdout" both of which are considered FILE* structs.
It's common that all process shares the same input and output. Thus they are able to communicate with each other through these registers.
Commonly a shell sends the input of a user to the stdin, and the output it generates/receives to the shell viewed by the user.

### System Calls: <br />
When the user program (C file in this case) wishes to provide an action, reserved for the OS, it can perform a system call.
This means the mode within the OS is changed to kernel mode, allowing for unrestricted access to the kernel. 
It also creates an OS interrupt through the trap handler. The original user program yields control to the trap handler.
The trap handler is responsible for making the system call, once finished, the mode is changed back to user mode and the trap handler returns control to the user program.

### Background Program Execution: <br />
A process is a Job(task) that looks to perform a series of actions, called instructions, with a specific goal in mind. 
A background process may look to run indefinitely or take a longer time than what is considered acceptable by the user.
It usually serves a general purpose, which exists aslong as the user is active. 
Some examples include; running a webservice, logging users acitvity or a scheduler.

### Inspiration: <br />
The material in the biblography represents represents different sources that have inspired / helped create the code. <br />
The methods "trimwhitespace" and "hash" is copied from the internet and appropriate sources is shown before the implementation of said function. <br />
