#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define LINE_LEN 80
#define MAX_ARGS 64
#define MAX_ARG_LEN 16
#define MAX_PATHS 64
#define MAX_PATH_LEN 96
#define WHITESPACE " .,\t\n"

#ifndef NULL 
#define NULL ... 
#endif

struct command_t {
	char *name;
	int argc;
	char *argv[MAX_ARGS];
};

void readCommand(char* buffer) {
	// This code uses any set of I/O functions, such as those in the stdio
	// library to read the entire command line into the buffer.
	// This implementation is greatly simplified, but it does the job.
	gets (buffer);
}


void parsePath(char* dirs[]) {
	char *pathEnvVar; 
	char *thePath;
	int i=0;
	for(i=0; i<MAX_ARGS; i++) dirs[i] = NULL; /* set to null */
	
	pathEnvVar = (char *) getenv ("PATH");
	thePath = (char *) malloc(strlen(pathEnvVar) + 1); 
	
	strcpy(thePath, pathEnvVar);
	
	int start = 0;
	int end = 0;
	int dirsCount = 0;
	
	for (i=0;i<strlen(thePath);i++)
	{
		char sampleChar[1];
  		strncpy(sampleChar, thePath+i, 1);
		if (sampleChar[0]==':')
		{
			char* eachPath = (char*)malloc(sizeof(char)*MAX_PATH_LEN);
			if (start == 0)
			{
				strncpy(eachPath,thePath,i-start);
			}
			else
			{
				strncpy(eachPath,thePath+start+1,i-start-1);
			}
			dirs[dirsCount] = eachPath;
			dirsCount++;
			start = i;
		}
	}
	//get the last path
	char* eachPath = (char*)malloc(sizeof(char)*MAX_PATH_LEN);
	strncpy(eachPath,thePath+start+1,strlen(thePath)-1-start);
	dirs[dirsCount] = eachPath;
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

// Runs an internal command and returns the output
char* runInternalCommand(char *command) {
	// Initialize variables
	FILE *fileptr;
	char *buffer = "%s >> %s";
	char *filename = ".temp_internal_command_output";
	char *internal_command;

	// Format the command to be sent to `system()` function
	size_t size = snprintf(NULL, 0, buffer, command, filename);
	internal_command = malloc(size + 1);
	snprintf(internal_command, size + 1, buffer, command, filename);

	// Run the command. The `>>` redirects output to a file
	int retval = system(internal_command);
	if (retval != 0) {
		printf("Error: Internal command `%s` failed.", command);
		return NULL;
	}

	// Read the output from the temporary file and store in result
	fileptr = fopen(filename, "r");
	char output[1024];
	fgets(output, sizeof(output) - 1, fileptr);
	size_t ln = strlen(output) - 1;
	if (output[ln] == '\n') output[ln] = '\0'; // Strip newline
	char *result = malloc(ln + 1);
	strcpy(result, output);
	
	// Close the temporary file and delete it
	fclose(fileptr);
	remove(filename);
	return result;
}

char *lookupPath(char **argv, char **dir)
{
	/* This function searches the directories identified by the dir argument to see 
	if argv[0] (the file name) appears there. 
	Allocate a new string, place the full path name in it, then return the string. */ 
	char* pName;
	int i;

	printf("%s", *argv);

	if (*argv[0] == '/')
	{
		// check to see if file name already absolute path
		if (access(*argv, F_OK) != -1)
		{
			return *argv;
		}
	}

	for(i = 0; i < MAX_PATHS; i++)
	{
		pName = dir[i];
		// look in PATH directories, use access() to see if file is in a dir
		if (access(pName, F_OK) != -1)
		{
			return pName;
		}
	}
	fprintf(stderr, "%s: Command not found!!!!!\n", argv[0]);
	return NULL;
}

void printPrompt() {
	// Builds the prompt string with user, machine name, and current directory
	char *user = runInternalCommand("whoami");
	char *machine = runInternalCommand("hostname");
	char *currentdir = runInternalCommand("pwd");
	printf("%s@%s %s $ ", user, machine, currentdir);
}

int main (int argc, char *argv[]) {

	static char* buffered;

	//initialize
	system("clear");
	buffered = malloc(MAX_ARGS*MAX_ARG_LEN*sizeof(char));
	char* envPath [MAX_PATHS];
	struct command_t cmd;

	parsePath(envPath);

	printf ("Enter a command\n");
	printPrompt();
	
	readCommand(buffered);
	parseCommand(buffered, &cmd);
	lookupPath(cmd.argv, envPath);

	return 0;
}

