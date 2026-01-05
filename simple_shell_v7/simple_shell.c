#include "shell.h"

/**
 * run_command - Execute a command with built-in and PATH handling.
 * @argv: NULL-terminated array of arguments (argv[0] is the command name).
 *
 * Return: 0 on success, 1 on system error, 127 if command is not found.
 */

static int run_command(char *argv[])
{
	char *command_path;
	pid_t pid;
	int status;

	if (argv[0] == NULL)
		return (0);
	if (is_builtin(argv[0]))
	{
		execute_builtin(argv[0]);
		return (0);
	}
	command_path = find_command(argv[0]);
	if (command_path == NULL)
	{
		fprintf(stderr, "./shell: %s: No such file or directory\n", argv[0]);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("./shell");
		return (1);
	}
	if (pid == 0)
	{
		if (execve(command_path, argv, environ) == -1)
		{
			perror("./shell");
			exit(127);
		}
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("./shell");
			return (1);
		}
	}
	return (0);
}

/**
 * main - Simple shell with PATH support and built-ins
 *
 * Return: Always 0
 */

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char *argv[MAX_ARGS];
	char *token;
	int i;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}
		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';
		if (strlen(line) == 0)
			continue;

		i = 0;
		token = strtok(line, " \t");
		while (token != NULL && i < MAX_ARGS - 1)
		{
			argv[i] = token;
			i++;
			token = strtok(NULL, " \t");
		}
		argv[i] = NULL;

		run_command(argv);
	}
	free(line);
	return (0);
}
