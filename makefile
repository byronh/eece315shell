all: minishell

minishell: minishell.c
    gcc -o minishell -include minishell.h minishell.c

clean:
    rm -rf minishell
