#include <stdio.h>

#include "repl/repl.h"
#include "run_file/run_file.h"

int main(int argc, char** argv)
{
	if (argc > 2) {
		printf("Immutant must receive either script name as an argument or no "
			   "arguments at all to run in interactive mode.\n");
	} else if (argc == 2) {
		run_file(argv[1]);
	} else {
		repl();
	}

	return 0;
}
