#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void run(const char* script_name) {
    int fd = open(script_name, O_RDONLY);

    if(fd == -1) {
        printf("Error: Could not open file %s\n", script_name);
    } 

    close(fd);
}

int main(int argc, char** argv) {
    printf("argc: %d\n", argc);
    if(argc > 2) {
        printf("Immutant must receive either script name as an argument or no arguments at all to run in interactive mode.\n");
    } else if (argc == 2) {
        run(argv[1]);
    } else {
        // TODO: implement interactive mode
        printf("No arguments provided to the interpreter.\n");
    }
    return 0;
}
