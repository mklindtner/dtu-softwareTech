void receive_commands();
unsigned long hash(unsigned char *str);
char *trimwhitespace(char *);
char** slice_words(char *, int *);
void checkerr(int);
char *buildstring(char **foo, int words);
void free_input(char **input);
int run_shell(char **input, int inputL);
char** cutflags(char**str, int from, int to);


