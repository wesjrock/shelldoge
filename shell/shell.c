/* 
	Shelldoge, Copyright (c) 2016 Luan G. O. <luan.orlandi@usp.br>,
	Renato J. A.<renato.jose93@gmail.com> and Wesley T., <wes@usp.br>
	Shelldoge is derived from POSIX-eg, Copyright (c) Monaco F. J. <2015>

	This file is part of Shelldoge.

	Shelldoge is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Shelldoge is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Shelldoge.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "shell.h"

void execute(char *line) {
	char *cmd;			/* command */
	int start, end;		/* expression delimiters */

	if(setExpressionDelimiters(line, "[^| ]([^|]*[^| ])?", &start, &end)) {
		cmd = getExpression(line, start, end, 0);
		
		ignoreSignals();
		
		if(hasPipe(line)) {
			executePipe(line + end, cmd);
		} else if(!tryInternalCommand(cmd, JOBS_CONTROL)) {
			executeExternalCommand(cmd);
		}
		
		free(cmd);
	}
}

void executeExternalCommand(char *cmd) {
	int bgFg = getFgBg(cmd);	/* fg(0) or bg(1) */
	int status;
	
	pid_t pid = fork();

    if(pid < 0) {
        perror("Fork error, pid < 0");
	/* parent */
	} else if(pid > 0) {
		/* set process group ID of child to self pid */
		/* will suspend when receive SIGTTIN and SIGTTOU signals */
		setpgid(pid, pid);

		/* set terminal control to new process group with child */
		tcsetpgrp(STDIN_FILENO, pid);

		/* insert process in the list */
		insertProcess(pid, bgFg, cmd);

		if(processList.current->process.status == FOREGROUND) {
			/* wait child finish */
			waitpid(pid, &status, WUNTRACED);
		}

		/* parent regains terminal control */
		tcsetpgrp(STDIN_FILENO, getpgid(0));
	/* child */
    } else {
		/* signals SIGTSTP and SIGINT will show default behavior in child */
		defaultSignals();

		/* set group id of child to self pid */
		/* suspend when receive signals SIGTTIN and SIGTTOUT */
		/* the child could go through exec before the parent set the group */
		setpgid(0, 0);

		/* try to execute command */
		analyseCommand(cmd);
	}
}

int getFgBg(char *line) {
	int start, end;
	char *expression = NULL; /* '&' or '&[^>]', it's not an error redirection */
	
	if(setExpressionDelimiters(line, "&($|[^>])", &start, &end)) {
		expression = getExpression(line, start, end, 0);	/* find start */
		line[start] = ' ';
		
		free(expression);
		
		return BACKGROUND;
	}
	
	return FOREGROUND;
}

void analyseCommand(char *cmd) {
	char *program = NULL;
	int start, end;
	int canExecute = 0;

	if(setExpressionDelimiters(cmd, "^( *[^&<> ]+)+", &start, &end)) {
		/* command begins from the start */
		
		program = getExpression(cmd, start, end, 0);
		canExecute = 1;
	} else if (setExpressionDelimiters(cmd, "[^<> ] ( *[^<> ]+)+", &start, &end)) {
		/* command begins from somewhere else */
		
		program = getExpression(cmd, start, end, 1);
		canExecute = 1;
	}

	if(!canExecute) {
		exit(EXIT_FAILURE);
	}

	if(redirectInput(cmd)) {
		exit(EXIT_FAILURE);
	}

	redirectOutput(cmd);

	free(cmd);
	executeProgram(program);
}

int redirectInput(char *line) {
	char *input = NULL;
	int start, end;
	int file;

	if(setExpressionDelimiters(line, "< *[^<>| ]+", &start, &end)) {
		/* get one input redirection */
		input = getExpression(line, start, end, 1);

		file = open(input, O_RDONLY, 0);

		if(file < 0) {
			perror("Failed to open input file");

			return 1;
		}

		/* try to redirect input */
		if(dup2(file, STDIN_FILENO) < 0) {
			perror("Failed to duplicate input file descriptor");

			return 2;
		}

		close(file);
		free(input);
	}

	return 0;
}

void redirectOutput(char *line) {
	char *output = NULL;
	int start, end;
	int file;

	int mode = 0;
	/*	0 = overwrite
		1 = concatenate
		2 = overwrite and error redirection */

	if(setExpressionDelimiters(line, "> *[^<>| ]+", &start, &end)) {
		if(start == 0) {
			mode = 0;
		} else if(line[start - 1] == '>') {
			mode = 1;
		} else {
			/* line[start - 1] == '&' */
			mode = 2;
		}
		
		/* get one output redirection */
		output = getExpression(line, start, end, 1);

		if(mode == 1) {
			file = open(output, O_WRONLY|O_APPEND|O_CREAT, 0644);
		} else {
			remove(output);
			file = open(output, O_WRONLY|O_CREAT, 0644);

			if (mode == 2) {
				/* try to redirect output */
				if(dup2(file, STDERR_FILENO) < 0) {
					perror("Failed to duplicate output file descriptor");
				}
			}
		}

		if (file < 0) {
			perror("Failed to create output file");
		}

		if (dup2(file, STDOUT_FILENO) < 0) {
			perror("Failed to duplicate output file descriptor");
		}

		close(file);
		free(output);
	}
}

void executeProgram(char *program) {
    int n = countExpression(program, " [^ ]") + 1; /* count arguments */
    char *prg_tok[n + 1];		/* last will be null */
    int i;

    tokenize(program, prg_tok);		/* load arguments */
    
    if(execvp(prg_tok[0], prg_tok) < 0) {
        perror("Execvp error");
	}

	for(i = 0; prg_tok[i] != NULL; i++) {
		free(prg_tok[i]);
	}

	free(program);
	
	exit(EXIT_FAILURE);
}

void executePipe(char *line, char *cmd) {
	int bgFg = getFgBg(cmd);	/* fg(0) or bg(1) */
	int status;

	pid_t pid = fork();

	if(pid < 0) {
		perror("Fork error, pid < 0");
	/* parent */
	} else if(pid > 0) {
		/* set process group ID of child to self pid */
		/* will suspend when receive SIGTTIN and SIGTTOU signals */
		setpgid(pid, pid);

		/* set terminal control to new process group with child */
		tcsetpgrp(STDIN_FILENO, pid);

		/* insert process in the list */
		insertProcess(pid, bgFg, cmd);


		if(processList.current->process.status == FOREGROUND) {
			/* wait child finish */
			waitpid(pid, &status, WUNTRACED);
		}

		/* parent regains terminal control */
		tcsetpgrp(STDIN_FILENO, getpgid(0));
	/* child */
	} else {
		/* signals SIGTSTP and SIGINT will show default behavior in child */
		defaultSignals();
		
		/* set group id of child to self pid */
		/* suspend when receive signals SIGTTIN and SIGTTOUT */
		/* the child could go through exec before the parent set the group */
		setpgid(0, 0);

        int startLine = 0;	/* search begin of next expression */
        int start, end;		/* expression delimiters */
        char *previousCmd;
		
		/* while find commands between pipes */
        while(setExpressionDelimiters(line + startLine,
        	"[^| ]([^|]*[^| ])?", &start, &end)) {
        	
            previousCmd = cmd;
            
            /* get current command */
            cmd = getExpression(line + startLine, start, end, 0);
            
            /* go to next search */
            startLine += end;

			/* file input and output of pipe */
            int file[2];
            pid_t pid;

            newPipe(file, &pid);

			/* parent */
			if(pid > 0) {
				readPipe(file);
			/* child */
			} else {
				writePipe(file);
				internalExternalPipe(previousCmd, NO_JOBS_CONTROL);
			}
        }

		/* execute last command */
        internalExternalPipe(cmd, NO_JOBS_CONTROL);
    }
}

int hasPipe(char *line) {
	return strchr(line, '|') != NULL;
}

void internalExternalPipe(char* cmd, int jobsControl) {
	if(!tryInternalCommand(cmd, jobsControl)) {
		analyseCommand(cmd);
	} else {
		free(cmd);
		exit(EXIT_SUCCESS);
	}
}

void newPipe(int *file, pid_t *pid) {
	if(pipe(file) != 0) {
		perror("Can't create new pipe");
	}

	*pid = fork();
	
	if(*pid < 0) {
		perror("Fork error, pid < 0");
	}
}

void writePipe(int *file) {
	close(file[PIPE_INPUT]);
	dup2(file[PIPE_OUTPUT], STDOUT_FILENO);
	close(file[PIPE_OUTPUT]);
}

void readPipe(int *file) {
	close(file[PIPE_OUTPUT]);
	dup2(file[PIPE_INPUT], STDIN_FILENO);
	close(file[PIPE_INPUT]);
}

FILE *handleArguments(int argc, char *argv[]) {
	int showVersion = 0;		/* --version */
	int readArgCommand = 0;		/* --command or -c */
	int readScript = 0;			/* argc > 1 with no option */
	int i;
	
	/* no arguments, only program name */
	if(argc <= 1) {
		return stdin;
	}
	
	/* check for version */
	for(i = 1; i < argc; i++) {
		if(strcmp(argv[i], "--version") == 0) {
			showVersion = 1;
			break;
		}
	}
	
	/* check for command */
	for(i = 1; i < argc; i++) {
		if(strcmp(argv[i], "--command") == 0 ||
			strcmp(argv[i], "-c") == 0) {
			readArgCommand = 1;
			break;
		}
	}
	
	if(showVersion) {
		printf("Shelldoge version %s\n", VERSION);
	}
	
	/* check for script */
	if(!readArgCommand) {
		for(i = 1; i < argc; i++) {
			if(argv[i][0] != '-') {
				readScript = 1;
				break;
			}
		}
	}
	
	/* give a script file to be the input */
	if(readScript) {
		FILE *input = fopen(argv[i], "r");
		if(input != NULL) {
			return input;
		} else {
			fprintf(stderr,
			"Script ignored: Can't open file \"%s\", stdin is the input\n",
			argv[i]);
		}
	}
	
	/* execute arguments as commands */
	if(readArgCommand) {
		for(i = 1; i < argc; i++) {
			if(argv[i][0] != '-') {
				
				
				if(checkExit(argv[i])) {
					exit(EXIT_SUCCESS);
				} else {
					execute(argv[i]);
				}
			}
		}
	}
	
	return stdin;
}
