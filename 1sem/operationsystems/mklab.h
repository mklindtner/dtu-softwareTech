void receive_commands();
unsigned long hash(unsigned char *str);
char* addWord(char*);
char *trimwhitespace(char *);
char** sliceWords(char *, int *);
void checkErr(int);
int wordSize(char **);
char *buildstring(char **foo, int words);
void freeInput(char **input);

int runShell(char **input, int inputL);
char** cutstring(char**str, int from, int to);


