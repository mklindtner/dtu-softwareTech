# Manual:

## compiler:
Clang: Recommended compiler the program.
GCC: Can be used but there may be unexpected errors. 

## Program Environment: <br />
This program has been tested on: Windows10 running wsl2:ubuntu 18.04
The program is an imitation of a shell, once running a user should be able to do any command implemented in their basic shell together with a range of predefined command. Some of these are included in the "list of commands".


## Usage (Assumes Clang is used): <br />
This program assumes that a default shell is installed on the system and that the location for this shell is used when calling execvp. The user of the program is responsible
for ensuring "execvp" works as expected. (for linux, the default location for shell commands is located in"/bin")

1) Ensure you're standing in the path you opened the zip file
2) Compile the file "mklab.c"; clang -o <name> mklab.c 
3) execute the <name> file ("./<name>" in linux)
4) write a command (see list of commands)
5) write "exit" to stop


## list of commands  <br />
ls //lists files in current directory <br />
ls | grep <name> //pipes ls output as grep's input, the pipe can be used for many different things. NB! do not use "" or '' for <name>
  
cat my_file.extension //prints the content of file <br />
touch my_file //outputs myfile.txt <br />
cd myfolder //goes to my folder
exit //stops program <br />
  
## Program Definitions <br />
### I/O redirection: <br />
There exists three files, in any system. stdin(keyboard), stdout(screen) and stderr(error messages for the screen). I/O redirection means to use the output from one file as the input to another. C allows access throug these under certain conventions, to write directly to one of these files, one could use the i/o call "write" to stdin, stdout or stderr. These files are shared accross any process, and thus they can be used to send a process output as anothers input. 

### System Calls: <br />
System calls is functionality that a OS handles, thus if a programmer wishes to write a program that make use of such call, the programming language must implement these functions for the OS, usually under the guidance of a standard (such as POSIX).
System calls work as follows: the original user program yields control to the trap handler.
Which creates a OS interrupt through the trap handler, the mode within the OS is changed to kernel mode, allowing for unrestricted access to the kernel. 
The trap handler is responsible for making the system call specified by the User, once finished, the mode is then changed back to user mode and the trap handler returns control to the user program.

A kernel mode, is the mode in which an OS allows for complete manipulation of any program.
The user mode, is the mode in which an OS restricts certain actions for the user, these modes are motivated due to security and easy-of-access concerns.

### Background Program Execution: <br />
A process is a Job(task) that looks to perform a series of actions, called instructions, with a specific goal in mind. 
A background process may look to run indefinitely or take a longer time than what is considered acceptable by the user.
It usually serves a general purpose, which exists aslong as the user is active. 
Some examples include; running a webservice, logging users acitvity or a scheduler.

### Inspiration: <br />
The material in the biblography represents represents different sources that have inspired / helped create the code. <br />
The methods "trimwhitespace" and "hash" is copied from the internet and appropriate sources is shown before the implementation of said functions. <br />
