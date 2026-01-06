#include "shell.h"

/**
 * handle_sigint - Handles SIGINT signal (Ctrl+C).
 * @sign: Signal number.
 *
 * Return: Nothing
 */

static void handle_sigint(int sign)
{
	(void) sign;
	write(STDOUT_FILENO, "\n", 1);
}

/**
 * setup_signals - Installs signal handlers for the shell.
 *
 * Return: Nothing
 */

void setup_signals(void)
{
	struct sigaction sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);
}
