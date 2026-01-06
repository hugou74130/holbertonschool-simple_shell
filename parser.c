#include "shell.h"

/**
 * parse_line - Tokenizes an input line into an argument array
 * @line: input line to tokenize
 * @args: array to fill with tokens
 *
 * Return: number of tokens found
 */

int parse_line(char *line, char *args[])
{
	int i = 0;
	char *token;

	token = strtok(line, " \t");
	while (token != NULL && i < MAX_ARGS - 1)
	{
		args[i] = token;
		i++;
		token = strtok(NULL, " \t");
	}
	args[i] = NULL;
	return (i);
}
