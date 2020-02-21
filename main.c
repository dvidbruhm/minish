#include <stdlib.h>

#include "builtins.h"
#include "minish_loop.h"


int main(int argc, char **argv)
{
    // INIT: Load config files

    // Run main loop
    minish_loop();

    // SHUTDOWN: cleanup files, etc.
    
    return EXIT_SUCCESS;
}


