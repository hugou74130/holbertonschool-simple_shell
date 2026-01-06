#include "shell.h"

/**
 * spawn_and_wait - Forks a child process, executes command, and waits for it.
 * @command_path: Full path to the executable to run.
 * @args: NULL-terminated array of arguments to pass to execve.
 * @shell_name: Name of the shell program (used for error messages).
 *
 * Return: Exit status of the child on success, 1 on system error.
 */


static int spawn_and_wait(char *command_path, char *args[], char *shell_name)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror(shell_name);
		return (1);
	}
	if (pid == 0)
	{
		if (execve(command_path, args, environ) == -1)
		{
			perror(shell_name);
			exit(126);
		}
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			perror(shell_name);
			return (1);
		}
		if (!WIFEXITED(status))
			return (1);
		return (WEXITSTATUS(status));
	}
	return (0);
}

/**
 * run_command - Executes a command with built-in and PATH handling.
 * @args: NULL-terminated array of arguments (args[0] is the command name).
 * @shell_name: Name of the shell program (used for error messages).
 *
 * Return: 0 on success, 1 on system error, 127 if command is not found,
 * EXIT_SHELL if the exit built-in is invoked.
 */


static int run_command(char *args[], char *shell_name)
{
	char *command_path;
	int ret;

	if (args[0] == NULL)
		return (0);
	if (is_builtin(args[0]))
	{
		ret = execute_builtin(args[0]);
		return (ret);
	}
	command_path = find_command(args[0]);
	if (command_path == NULL)
	{
		if (strchr(args[0], '/') != NULL)
		{
			fprintf(stderr, "%s: %s: No such file or directory\n", shell_name, args[0]);
		} else
		{
			fprintf(stderr, "%s: %s: not found\n", shell_name, args[0]);
		}
		return (127);
	}
	return (spawn_and_wait(command_path, args, shell_name));
}

/**
 * main - Entry point for the simple shell.
 * @argc: Number of command-line arguments.
 * @argv: Array of command-line arguments (argv[0] is the program name).
 *
 * Return: Exit status of the last executed command.
 */


int main(int argc __attribute__((unused)), char **argv)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char *args[MAX_ARGS], *token;
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
			args[i] = token;
			i++;
			token = strtok(NULL, " \t");
		}
		args[i] = NULL;
		if (args[0] == NULL)
			continue;
		status = run_command(args, argv[0]);
		if (status == EXIT_SHELL)
			break;
		last_status = status;
	}
	free(line);
	return (last_status);
}
