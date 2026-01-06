#include "shell.h"

/**
 * spawn_and_wait - Forks a child process, executes command, and waits.
 * @command_path: Full path to the executable to run.
 * @argv: NULL-terminated arguments array for execve.
 *
 * Return: Exit status of the child on success, 1 on system error.
 */

static int spawn_and_wait(char *command_path, char *argv[])
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror(SHELL_NAME);
		return (1);
	}
	if (pid == 0)
	{
		if (execve(command_path, argv, environ) == -1)
		{
			perror(SHELL_NAME);
			exit(126);
		}
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			perror(SHELL_NAME);
			return (1);
		}
		if (!WIFEXITED(status))
			return (1);
		return (WEXITSTATUS(status));
	}
	return (0);
}

/**
 * run_command - Execute a command with built-in and PATH handling.
 * @argv: NULL-terminated array of arguments (argv[0] is the command name).
 *
 * Return: 0 on success, 1 on system error, 127 if command is not found
 * EXIT_SHELL if the exit built-in is invoked.
 */

static int run_command(char *argv[])
{
	char *command_path;
	int ret;

	if (argv[0] == NULL)
		return (0);
	if (is_builtin(argv[0]))
	{
		ret = execute_builtin(argv[0]);
		return (ret);
	}
	command_path = find_command(argv[0]);
	if (command_path == NULL)
	{
		if (strchr(argv[0], '/') != NULL)
		{
			fprintf(stderr, "%s: %s: No such file or directory\n", SHELL_NAME, argv[0]);
		} else
		{
			fprintf(stderr, "%s: %s: not found\n", SHELL_NAME, argv[0]);
		}
		return (127);
	}
	return (spawn_and_wait(command_path, argv));
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
	char *argv[MAX_ARGS], *token;
	int i, status;
	int last_status = 0;

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
		if (argv[0] == NULL)
			continue;
		status = run_command(argv);
		if (status == EXIT_SHELL)
			break;
		last_status = status;
	}
	free(line);
	return (last_status);
}
