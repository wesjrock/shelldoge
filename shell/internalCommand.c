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

#include "internalCommand.h"

int checkExit(char *line) {
	if(existExpression(line, "^ *exit *$") ||
		existExpression(line, "^ *quit *$")) {
		
		return 1;
	}
	
	return 0;
}

int tryInternalCommand(char *cmd, int jobsControl) {
	int start, end;
	
	if(setExpressionDelimiters(cmd, "^ *cd($| )", &start, &end)) {
		cd(cmd, end);
		return 1;
	}
	
	if(setExpressionDelimiters(cmd, "^ *fg($| )", &start, &end)) {
		fg(cmd + end, jobsControl);
		return 1;
	}
	
	if(setExpressionDelimiters(cmd, "^ *bg($| )", &start, &end)) {
		bg(cmd + end, jobsControl);
		return 1;
	}
	
	if(setExpressionDelimiters(cmd, "^ *jobs($| )", &start, &end)) {
		jobs(cmd + end);
		return 1;
	}

	return 0;
}

void cd(char *cmd, int skip) {
	char cwd[PATH_LENGTH];	/* current working directory */
	char *arg;				/* '..' or '~' */
	
	/* to keep alternating between current and previous directory */
	int isOLDWDset = 0;
	
	/* expression delimiters */
	int start, end;

	if(getcwd(cwd, sizeof(cwd)) == NULL) {
		fprintf(stderr, "Can't get current directory\n");
	}

	/* cd (home) */
	if(existExpression(cmd + skip, "^ *$")) {
		char *HOME = getenv("HOME");

		if(chdir(HOME)) {
			perror("Can't go to HOME");
		} else {
			setenv("OLDPWD", cwd, 1);
			setenv("PWD", HOME, 1);
		}
	/* cd - (alternate) */
	} else if(existExpression(cmd + skip, "^ *-")) {
		char *oldwd = getenv("OLDPWD");

		/* change to previous directory */
		if(oldwd != NULL) {
			if(chdir(oldwd)) {
				perror("Can't go to previous directory");
			} else {
				setenv("OLDPWD", cwd, 1);
				setenv("PWD", oldwd, 1);
			}
		} else {
			fprintf(stderr, "Environment variable OLDPWD not set\n");
		}
	/* cd arg (argument) */
	} else {
		char oldwd[PATH_LENGTH];
		strcpy(oldwd, cwd);

		if(setExpressionDelimiters(cmd + skip, "^ */", &start, &end)) {
			skip += end;	/* go to next search */

			if(chdir("/")) {
				perror("Can't go to root");
			} else {
				setenv("OLDPWD", oldwd, 1);
				setenv("PWD", "/", 1);
				strcpy(cwd, "/");
			}
		}
		
		/* while find directories, or '..' or '~' */
		while(setExpressionDelimiters(cmd + skip, "[^/ ]([^/]*[^/ ])?",
			&start, &end)) {
			
			arg = getExpression(cmd+skip, start, end, 0);
			skip += end;	/* go to next search */

			/* go home */
			if(strcmp(arg, "~") == 0) {
				char *HOME = getenv("HOME");
				strcpy(cwd, HOME);
				
				if(chdir(HOME)) {
					perror("Can't go to HOME");
				} else {
					if(!isOLDWDset) {
						setenv("OLDPWD", oldwd, 1);
						isOLDWDset = 1;
					}
					
					setenv("PWD", cwd, 1);
				}
			} else {
				strcat(cwd, "/");
				strcat(cwd, arg);
				
				if(chdir(cwd) != 0){
					perror("Can't advance directory");
				} else {
					if(!isOLDWDset) {
						setenv("OLDPWD", oldwd, 1);
						isOLDWDset = 1;
					}

					setenv("PWD", cwd, 1);
				}
			}

			free(arg);
		}
	}
}

void fg(char *cmd, int jobsControl) {
	int status;
	int start, end;
	
	/* verify if has job control */
	if(jobsControl == NO_JOBS_CONTROL) {
		fprintf(stderr, "No jobs control\n");
		return;
	}

	/* verify if there are any jobs */
	if(processList.end == NULL) {
		fprintf(stderr, "No jobs available\n");
		return;
	}

	/* fg */
	if(existExpression(cmd, "^ *$")) {
		defaultSignals();
		
		/* set terminal control to new process group, with the child process */
		tcsetpgrp(STDIN_FILENO, processList.current->process.pid);
		processList.current->process.status = FOREGROUND;
		
		/* process was suspended, continue in foreground */
		kill(processList.current->process.pid, SIGCONT);
		printf("Current job now in foreground\n");
		
		/* SIGCHLD can be capture when a suspedend job come back (generate EINTR) */
		while(waitpid(processList.current->process.pid, &status, WUNTRACED) == -1);
		
		/* parent recover terminal control */
		tcsetpgrp(STDIN_FILENO, getpgid(0));
	/* fg pid */
	} else if(setExpressionDelimiters(cmd, "^ *[0-9]+", &start, &end)) {
		char* stringid = NULL;	/* string id */
		int id;					/* integer id */
		
		/* get id as integer */
		stringid = getExpression(cmd, start, end, 0);
		id = atoi(stringid);
		free(stringid);
		
		if(changeProcessStatus(id, FOREGROUND) == -1) {
			fprintf(stderr, "No such job\n");
		} else {
			defaultSignals();
			
			/* set terminal control to new process group, with the child process */
			tcsetpgrp(STDIN_FILENO, processList.current->process.pid);
			
			/* process was suspended, continue in foreground */
			kill(processList.current->process.pid, SIGCONT);
			printf("Job %d now in foreground\n", id);
			
			/* SIGCHLD can be capture when a suspedend job come back (generate EINTR) */
			while(waitpid(processList.current->process.pid, &status, WUNTRACED) == -1);
			
			/* parent recover terminal control */
			tcsetpgrp(STDIN_FILENO, getpgid(0));
		}
	} else {
		fprintf(stderr, "Invalid command for fg\n");
	}
}

void bg(char *cmd, int jobsControl) {
	int start, end;
	
	/* verify if has job control */
	if(jobsControl == NO_JOBS_CONTROL) {
		fprintf(stderr, "No jobs control\n");
		return;
	}

	/* verify if there are any jobs */
	if(processList.end == NULL) {
		fprintf(stderr, "No jobs available\n");
		return;
	}

	/* bg */
	if(existExpression(cmd, "^ *$")) {
		if(processList.current->process.status == BACKGROUND) {
			fprintf(stderr, "Current job already in background\n");
		} else {
			processList.current->process.status = BACKGROUND;
			
			/* process was suspended, continue in background */
			kill(processList.current->process.pid, SIGCONT);
			printf("Current job now in background\n");
		}
	/* bg pid */
	} else if(setExpressionDelimiters(cmd, "^ *[0-9]+", &start, &end)) {
		char* stringid = NULL;	/* string id */
		int id;					/* integer id */
		
		/* get id as integer */
		stringid = getExpression(cmd, start, end, 0);
		id = atoi(stringid);
		free(stringid);
		
		switch(changeProcessStatus(id, BACKGROUND)) {
			case -1:
				fprintf(stderr, "No such job\n");
				break;
			case -2:
				fprintf(stderr, "Job %d already in background\n", id);
				break;
			default:
				/* process was suspended, continue in background */
				kill(processList.current->process.pid, SIGCONT);
				printf("Job %d now in background\n", id);
		}
	} else {
		fprintf(stderr, "Invalid command for bg\n");
	}
}

void jobs(char *cmd) {
	node *n = processList.start;
	
	/* verify if there are any jobs */
	if(processList.end == NULL) {
		fprintf(stderr, "No jobs to show\n");
		return;
	}
	
	/* jobs (no pid) */
	if(existExpression(cmd, "^ *$")) {
		while(n != NULL) {
			if(n == processList.current) {
				printJob(n->process.id, "[%d]+   ",
					n->process.status, n->process.path);
			} else {
				printJob(n->process.id, "[%d]    ",
					n->process.status, n->process.path);
			}

			n = n->next;
		}
	/* jobs -p (only pid) */
	} else if(existExpression(cmd, "^ *-p")) {
		while(n != NULL) {
			printf("%d\n", (int)n->process.pid);
			n = n->next;
		}
	/* jobs -l (everything) */
	} else if(existExpression(cmd, "^ *-l")) {
		while(n != NULL) {
			if(n == processList.current) {
				printJobWithPID(n->process.id, n->process.pid,
					"[%d]+   ", n->process.status, n->process.path);
			} else {
				printJobWithPID(n->process.id, n->process.pid,
					"[%d]    ", n->process.status, n->process.path);
			}

			n = n->next;
		}
	} else {
		fprintf(stderr, "Unavailable option, use -l or -p\n");
	}
}

void printStatus(int status) {
	switch(status) {
		case FOREGROUND: printf("Running"); break;
		case BACKGROUND: printf("Running"); break;
		case STOPPED: printf("Stopped"); break;
		default: printf("Unknow");
	}
}

void printJob(int id, char *stringID, char status, char *path) {
	printf(stringID, id);		/* process id */
	printStatus(status);		/* process status */
	printf("    %s\n", path);	/* process path */
}

void printJobWithPID(int id, pid_t pid, char *stringID, char status, char *path) {
	printf(stringID, id);		/* process id */
	printf("%d  ", (int) pid);	/* process pid */
	printStatus(status);		/* process status */
	printf("    %s\n", path);	/* process path */
}
