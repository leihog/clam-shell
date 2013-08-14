#include <stdio.h>
#include <stdarg.h>
#include <unistd.h> /* req by chdir,fork,execv */
#include <readline/readline.h>
#include <readline/history.h>
#include "strfunc.h"

#define COMMAND_DIR "cmds"

void die(const char *format, ...)
	__attribute__((noreturn))
	__attribute__((format (printf, 1, 2)));

void die(const char *format, ...)
{
	va_list params;
	va_start(params, format);
	vfprintf(stderr, format, params);
	fprintf (stderr, ".\n");
	va_end(params);
	exit(1);
}

static void cd_to_home()
{
	const char *home = getenv("HOME");
	if (!home)
		die("The env variable HOME must be set");
	if (chdir(home) == -1)
		die("Could not chdir to HOME");
}

bool is_valid_cmd(const char *cmd)
{
	// check if cmd contains . or /
	return cmd[strcspn(cmd, "./")] == '\0';
}

void prepend_path(char ***args)
{
	// prepend COMMAND_PATH to command name
	char *cmd = malloc(( strlen(*args[0]) + strlen(COMMAND_DIR) + 2 ));

	strcpy(cmd, COMMAND_DIR);
	strcat(cmd, "/");
	strcat(cmd, *args[0]);
	*args[0] = cmd;
}

void run_shell()
{
	int running = 1;
	char *input;

	char **argv;
	int count;

	cd_to_home();
	
	// disable auto completion for now
	rl_bind_key('\t', rl_insert);

	while(running) {

		input = (char *)NULL;
		input = readline("$ ");
		if (empty(input)) {
			fprintf(stderr, "Command not found\n");
			free(input);
			continue;
		}

		input = trim(input);
		add_history(input);

		if (!strcmp(input, "quit") || !strcmp(input, "exit")) {
			running = 0;
		} else {
			argv = tokenize_string(input, &count);

			if (!is_valid_cmd(argv[0])) {
				fprintf(stderr, "Invalid command\n");
			} else {
				prepend_path(&argv);

				if (!fork()) {
					execv(argv[0], argv);

					fprintf(stderr, "What?\n");
					exit(1);
				} else {
					wait(NULL);
				}
			}
			free(argv);
		}

		free(input);
	}
}

int main(int argc, char **argv)
{
	if (argc == 1) {
		run_shell();
		exit(0);
	} else if (argc != 3 || strcmp(argv[1], "-c")) {
		die("Run with no arguments or with -c cmd");
	}

	cd_to_home();

	int count = 0;
	char **cmd = tokenize_string(argv[2], &count);

	if (!is_valid_cmd(cmd[0]))
		die("Invalid command");

	prepend_path(&cmd);
	execv(cmd[0], cmd); // Will exit here unless execv failes

	free(cmd);
	die("Unrecognized command '%s'", argv[2]);
}

