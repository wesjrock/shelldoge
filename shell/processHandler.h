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

#ifndef PROCESSHANDLER_H
#define PROCESSHANDLER_H

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef SYSTYPES_H
#define SYSTYPES_H
#include <sys/types.h>
#endif

#ifndef SIGNAL_H
#define SIGNAL_H
#include <signal.h>
#endif


#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef FOREGROUND
#define FOREGROUND 0
#endif

#ifndef BACKGROUND
#define BACKGROUND 1
#endif

#ifndef STOPPED
#define STOPPED 2
#endif

typedef struct {
    pid_t pid;
    int id;		/* used for fg and bg commands */
    int status;
    char path[1000];	/* command path */
} Process;

typedef struct block {
    Process process;
    struct block *previous, *next;
} node;

/* process list */
typedef struct {
	node *start;
	node *current;
	node *end;
} Processes;

/* global list for processes */
extern Processes processList;

void newProcessList();
void destroyProcessList();
void insertProcess(int pid, int status, char *path);

/* changes status of the process with id for status, pid also becomes current */
/* return 0 if found process and assigned state */
/* return -1 if the process id was not found */
/* return -2 if the process has already same status */
int changeProcessStatus(int id, int status);

void changeCurrentProcessStatus(int status);

/* clear zombies */
void removeProcess(pid_t pid);

void removeCurrentProcess();

/* print all process pid, status and path */
void showProcesses();

#endif
