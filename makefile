all: minishell

minishell: minishell.c
	gcc -o minishell minishell.c

clean:
	rm -rf minishell
