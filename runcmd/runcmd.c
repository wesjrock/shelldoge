/* runcmd.c - Execute a command as a subprocess. 

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

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

#include <runcmd.h>

/* limit for arguments */
#define MAX 50

/* Executes 'command' in a subprocess. Information on the subprocess execution
   is stored in 'result' after its completion, and can be inspected with the
   aid of macros made available for this purpose. Argument 'io' is a pointer
   to an integer vector where the first, second and third positions store
   file descriptors to where standard input, output and error, respective, 
   shall be redirected; if NULL, no redirection is performed. On
   success, returns subprocess' pid; on error, returns 0. */

int runcmd(const char* command, int* result, int* io) {
	char* args[MAX], *cmd = NULL, *current = NULL;
	int nargs = 0, cstatus, fd;
	pid_t cpid;

	cmd = (char*) malloc(sizeof(char) *(strlen(command)+1));
	if(cmd == NULL) {
		return -1;
	}

	strcpy(cmd, command);
	args[nargs] = strtok(cmd, " ");
	nargs++;

	while(nargs < MAX && (current = strtok(NULL, " ")) != NULL) {
		args[nargs++] = current;
	}

	args[nargs] = NULL;

	cpid = fork();

	if(cpid < 0) {
		return -1;
	}

	/* child */
	if(cpid == 0) {
		execvp(args[0], args);

		/* execvp fail */
		close(open(".tmpExcFail~", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR));
		free(cmd);					
		exit(EXECFAILSTATUS);
	}

	waitpid(cpid, &cstatus, 0);
	if(result != NULL) {
		*(result) = 0;

		if(WIFEXITED(cstatus)) {
			*(result) = WEXITSTATUS(cstatus);
			*(result) |= NORMTERM;
			if((fd = open(".tmpExcFail~", O_RDONLY)) != -1) {
				close(fd);
				unlink(".tmpExcFail~");
			} else {
				*(result) |= EXECOK;
			}
		}
	}

	free(cmd);
	return cpid;
}

void (*runcmd_onexit)(void) = NULL;

