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

#ifndef SHELL_H
#define SHELL_H

#include "parser.h"
#include "signalHandler.h"
#include "processHandler.h"
#include "internalCommand.h"
#include "color.h"

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef SYSTYPES_H
#define SYSTYPES_H
#include <sys/types.h>
#endif

#ifndef SYSWAIT_H
#define SYSWAIT_H
#include <sys/wait.h>
#endif

#ifndef UNISTD_H
#define UNISTD_H
#include <unistd.h>
#endif

#ifndef FCNTL_H
#define FCNTL_H
#include <fcntl.h>
#endif

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#ifndef PIPE_INPUT
#define PIPE_INPUT 0
#endif

#ifndef PIPE_OUTPUT
#define PIPE_OUTPUT 1
#endif

#ifndef VERSION
#define VERSION "1.0.0"
#endif

/* try to execute a internal or external command found in line */
void execute(char *line);

void executeExternalCommand(char *cmd);

/* return if must execute in fg or bg (macro in processHandler.h) */
int getFgBg(char *line);

/* try to found a program to execute, also make I/O redirection if necessary */
void analyseCommand(char *cmd);

/* redirect I/O if found any */
int redirectInput(char *line);
void redirectOutput(char *line);

/* execute program with execvp */
void executeProgram(char *program);

/* execute a line with pipe */
void executePipe(char *line, char *cmd);

/* pipe functions */
int hasPipe(char *line);
void internalExternalPipe(char* cmd, int jobsControl);
void newPipe(int *file, pid_t *pid);
void writePipe(int *file);
void readPipe(int *file);

/* handle options, commands by arguments and script as input */
/* return file input that shell will read (stdin or a script file) */
FILE *handleArguments(int argc, char *argv[]);

#endif
