#include <stdio.h>
#include <stdlib.h>

#include <unistd.h> 
#include "builtins.h"

char *colors[] = {
    RED,
    GRN,
    YEL,
    BLU,
    MAG,
    CYN,
    WHT,
    RESET
};

char *random_color()
{
    int randomIndex = rand() % 8;
    char *color = colors[randomIndex];
    return color;
}

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
