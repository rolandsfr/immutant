#include <stdio.h>
#include "runners.h"

int main(int argc, char** argv) {
    if(argc > 2) {
        printf("Immutant must receive either script name as an argument or no arguments at all to run in interactive mode.\n");
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        run_interactive();
    }

    return 0;
}
