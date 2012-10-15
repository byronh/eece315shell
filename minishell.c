#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"

void printPrompt() {
	// Build the prompt string to have the machine name, current directory, or
	// other desired information
	char promptString[] = "$" ; 
	printf("%s", promptString);
}

void readCommand(char* buffer) {
	// This code uses any set of I/O functions, such as those in the stdio
	// library to read the entire command line into the buffer.
	// This implementation is greatly simplified, but it does the job.
	gets (buffer);
}

int parsePath(char *dirs) {
	// This function reads the PATH variable for this environment, then builds
	// an array, dirs[], of the directories in PATH
	char *pathEnvVar; 
	char *thePath;
	int i;
	
	for(i=0; i<MAX_ARGS; i++) {
		dirs[i] = NULL; // set to null
	}
	pathEnvVar = (char *) getenv ("PATH");
	thePath = (char *) malloc(strlen(pathEnvVar) + 1); 
	strcpy(thePath, pathEnvVar);
	// Loop to parse thePath. Look for a ':' delimiter between each path name.
	
	printf("%s",thePath);
	
	//TODO: parse the environment variable PATH which is in thePath
	return 1;
}

int parseCommand(char *cLine, struct command_t *cmd) 
{ 
	int argc; 
	char **clPtr; 

	/* Initialization */ 
	clPtr = &cLine; /* cLine is the command line */ 
	argc = 0; 
	cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN); 

	/* Fill argv[] */ 
	while((cmd->argv[argc] = strsep(clPtr, WHITESPACE))!= NULL) 
	{ 
		cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN); 
	}
 
	/* Set the command name and argc */ 
	cmd->argc = argc - 1; 
	cmd->name = (char *) malloc(sizeof(cmd->argv[0])); 
	strcpy(cmd->name, cmd->argv[0]); 

	return 1; 
}

// Runs an internal command and prints the output
int runInternalCommand(char *command) {
	// Initialize variables
	FILE *fileptr;
	char *buffer = "%s >> %s";
	char *filename = ".temp_internal_command_output";
	char *internal_command;

	// Format the command to be sent to `system()` function
	size_t size = snprintf(NULL, 0, buffer, command, filename);
	internal_command = malloc(size + 1);
	snprintf(internal_command, size + 1, buffer, command, filename);

	// Run the command. The `>>` redirects output a file
	int retval = system(internal_command);
	if (retval != 0) {
		printf("Error: Internal command `%s` failed.", command);
		return -1;
	}

	// Read the output from the temporary file and print it
	fileptr = fopen(filename, "r");
	char output[1024];
	while (fgets(output, sizeof(output) - 1, fileptr) != NULL) {
		printf("%s", output);
	}

	// Close the temporary file and delete it
	fclose(fileptr);
	remove(filename);
	return 0;
}

int main (int argc, char *argv[]) {
	//initialize
	runInternalCommand("clear");
	buffered = malloc(MAX_ARGS*MAX_ARG_LEN*sizeof(char));
	dirs = malloc(MAX_PATHS*MAX_PATH_LEN*sizeof(char));

	printf ("Enter a command\n");
	printPrompt();
	
	readCommand(buffered);
	parsePath(dirs);
	
	return 0;
}

