#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 1024
#define MAX_PATH 4096
#define PROMPT "-_- \\_(ツ)_/ $ "
#define EXIT_SHELL -2

extern char **environ;

/* Structure pour les built-ins */

/**
 * struct builtin_s - Built-in command mapping structure.
 * @name: Name of the built-in command (e.g., "exit", "env", "cd").
 * @func: Pointer to the function that executes the built-in command.
 */

typedef struct builtin_s
{
	char *name;
	int (*func)(void);
} builtin_t;

/* Built-in functions */
int builtin_exit(void);
int builtin_env(void);
int builtin_cd(void);

/* Environment */
char *_getenv(const char *name);

/* Parsing / tokenization */
int parse_line(char *line, char **argv, int max_args);

/* Built-in handling */
int is_builtin(char *command);
int execute_builtin(char *command);

/* PATH / command lookup */
char *get_path(void);
char *find_command(char *command);

#endif
