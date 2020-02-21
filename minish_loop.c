#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/wait.h>

#include "minish_loop.h"
#include "builtins.h"

void minish_loop()
{
    char *line;
    char **args;
    int status;

    do {
        printf("%s-> ", random_color());
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
