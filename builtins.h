int minish_cd(char **args);
int minish_exit(char **args);
int minish_help(char **args);

int nb_builtins();
char *builtin_names[3];
int (*builtin_funcs[3]) (char**);
char *colors[8];
char *random_color();

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
