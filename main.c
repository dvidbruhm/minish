#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <sys/wait.h>

void minish_loop();
char *read_line();
char **split_line();
int launch();
int exec_command();

int minish_cd(char **args);
int minish_exit(char **args);
int minish_help(char **args);

int main(int argc, char **argv)
{
    // INIT: Load config files

    // Run main loop
    minish_loop();

    // SHUTDOWN: cleanup files, etc.
    
    return EXIT_SUCCESS;
}

void minish_loop()
{
    char *line;
    char **args;
    int status;

    do {
        printf("-> ");
        line = read_line();
        args = split_line(line);
        status = exec_command(args);
    } while (status);
}

char *read_line()
{
    char *line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

#define TOK_BUFSIZE 100
#define TOK_DELIMITERS " \t\r\n\a"
char **split_line(char *line)
{
    char **tokens = malloc(TOK_BUFSIZE * sizeof(char*));
    char *token;
    int counter = 0;

    token = strtok(line, TOK_DELIMITERS);
    while (token != NULL)
    {
        //printf("%d %s\n", counter, token);
        tokens[counter] = token;
        counter++;
        token = strtok(NULL, TOK_DELIMITERS);
    }

    tokens[counter] = NULL;
    return tokens;
}

int launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();

    if(pid == 0)
    {
        //Child process
        if (execvp(args[0], args) == -1)
        {
            perror("minish");
        }
        exit(EXIT_FAILURE);
    }
    else if(pid > 0)
    {
        //Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    else
    {
        perror("minish");
    }

    return 1;
}


// BUILTINS
char *builtin_names[] = {
    "cd",
    "exit",
    "help"
};

int (*builtin_funcs[]) (char**) = {
    &minish_cd,
    &minish_exit,
    &minish_help
};

int nb_builtins()
{
    return sizeof(builtin_names) / sizeof(char *);
}

int minish_cd(char **args)
{
    if(args[1] == NULL)
    {
        fprintf(stderr, "minish: expected 1 argument to \"cd\".\n");
    }
    else
    {
        if(chdir(args[1]) != 0)
        {
            perror("minish");
        }
    }
     return 1;
}

int minish_exit(char **args)
{
    return 0;
}

int minish_help(char **args)
{
    printf("Minish - Mini shell\n");
    printf("Here are the built-in functions:\n");

    for(int i = 0; i < nb_builtins(); i++)
    {
        printf("    - %s\n", builtin_names[i]);
    }
    return 1;
}

int exec_command(char **args)
{
    if(args[0] == NULL)
    {
        return 1;
    }

    for(int i = 0; i < nb_builtins(); i++)
    {
        if(strcmp(args[0], builtin_names[i]) == 0)
        {
            return (*builtin_funcs[i])(args);
        }
    }

    return launch(args);
}
