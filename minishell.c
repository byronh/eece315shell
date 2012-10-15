#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"



void printPrompt() {
	/* Build the prompt string to have the machine name, current directory, or other desired information */
	char promptString[] = "$" ; 
	printf("%s", promptString);
}

void readCommand(char* buffer) {
	/* This code uses any set of I/O functions, such as those in the stdio library to read the entire command line into the buffer. This implementation is greatly simplified, but it does the job. */
	gets (buffer);
}

int parsePath(char *dirs) {
	/* This function reads the PATH variable for this environment, then builds an array, dirs[], of the directories in PATH */
	char *pathEnvVar; 
	char *thePath;
	int i;
	
	for(i=0; i<MAX_ARGS; i++) 
	{
		dirs[i] = NULL; /* set to null */
	}
	pathEnvVar = (char *) getenv ("PATH");
	thePath = (char *) malloc(strlen(pathEnvVar) + 1); 
	strcpy(thePath, pathEnvVar);
	/* Loop to parse thePath. Look for a ':' delimiter between each path name. */
	
	printf("%s",thePath);
    
    //TODO: parse the environment variable PATH which is in thePath
    
	return 1;
}

int main (int argc, char *argv[])
{
	//initialize
	buffered = malloc(MAX_ARGS*MAX_ARG_LEN*sizeof(char));
	dirs = malloc(MAX_PATHS*MAX_PATH_LEN*sizeof(char));

	printf ("Enter a command\n");
	printPrompt();
	
	readCommand(buffered);
	parsePath(dirs);
	
	return 0;
}

