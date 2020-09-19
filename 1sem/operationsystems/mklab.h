void receive_commands();
// unsigned long hash(unsigned char *str);
// char** commands();
char* addWord(char*);
char *trimwhitespace(char *);
char** sliceWords(char *, int *);
// int shellCommands(char **, int);
void checkErr(int);
int wordSize(char **);
char *buildstring(char **foo, int words);
void freeInput(char **input);



//new thaang
int runShell(char **input, int inputL);
// int isMatch(cmmds , char*);



