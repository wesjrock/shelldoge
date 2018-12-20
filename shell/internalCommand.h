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

#ifndef INTERNAL_COMMAND_H
#define INTERNAL_COMMAND_H

#include "parser.h"
#include "processHandler.h"
#include "signalHandler.h"

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef UNISTD_H
#define UNISTD_H
#include <unistd.h>
#endif

#ifndef SYSTYPES_H
#define SYSTYPES_H
#include <sys/types.h>
#endif

#ifndef SYSWAIT_H
#define SYSWAIT_H
#include <sys/wait.h>
#endif

#ifndef SIGNAL_H
#define SIGNAL_H
#include <signal.h>
#endif

#ifndef PATH_LENGTH
#define PATH_LENGTH 1000
#endif

#ifndef NO_JOBS_CONTROL
#define NO_JOBS_CONTROL 0
#endif

#ifndef JOBS_CONTROL
#define JOBS_CONTROL 1
#endif

/* return true or false if found exit or quit command */
int checkExit(char *line);

/* return true or false if executed a internal command */
int tryInternalCommand(char *cmd, int jobsControl);

/* skip characters for the start of command (normally c = cmd[0], d = cmd[1]) */
void cd(char *cmd, int skip);
void fg(char *cmd, int jobsControl);
void bg(char *cmd, int jobsControl);
void jobs(char *cmd);

/* print jobs functions */
void printStatus(int status);
void printJob(int id, char *stringID, char status, char *path);
void printJobWithPID(int id, pid_t pid, char *stringID, char status, char *path);

#endif
