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

#include "processHandler.h"

Processes processList;

void newProcessList() {
    processList.end = processList.start = processList.current = NULL;
}

void destroyProcessList() {
	node *n;
	n = processList.end;

	while(n != NULL) {
		kill(n->process.pid, SIGKILL);
		processList.end = processList.end->previous;

		free(n);
		n = processList.end;
	}
}

void insertProcess(int pid, int status, char *path) {
	node *n = (node*) malloc(sizeof(node));

	if(n == NULL) {
		fprintf(stderr, "Can't insert new process\n");
		exit(EXIT_FAILURE);
	}

	/* if it's the first process inserted */
	if(processList.start == NULL) {
		processList.start = processList.end = n;
		n->previous = NULL;
		n->process.id = 1;
	} else {
		processList.end->next = n;
		n->previous = processList.end;
		processList.end = n;
		n->process.id = n->previous->process.id + 1;
	}

	n->next = NULL;

	n->process.pid = pid;
	n->process.status = status;
	strcpy(n->process.path, path);
	processList.current = n;	/* make current be the last inserted */
}

int changeProcessStatus(int id, int status) {
	node* current = processList.current;

	while(processList.current != NULL &&
		processList.current->process.id < id) {

		processList.current = processList.current->next;
	}

	while(processList.current != NULL &&
		processList.current->process.id > id) {

		processList.current = processList.current->previous;
	}

	if(processList.current == NULL) {
		processList.current = current;

		return -1;
	}

	else if(processList.current->process.status == status) {
		processList.current = current;
		return -2;
	}

	processList.current->process.status = status;

	return 0;
}

void changeCurrentProcessStatus(int status) {
	processList.current->process.status = status;
}

void removeProcess(pid_t pid) {
	node* n = processList.current;

	while(n->process.pid < pid) {
		n = n->next;
	}

	while (n->process.pid > pid)
		n = n->previous;

	if(n->previous == NULL &&
		n->next == NULL) {		/* one process available */

		processList.current = processList.start = processList.end = NULL;
	} else if(n->previous == NULL) {	/* remove first process */

		n->next->previous = NULL;
		processList.start = n->next;

		if(n == processList.current) {
			processList.current = n->next;
		}
	} else if(n->next == NULL) {		/* remove last process */
		n->previous->next = NULL;
		processList.end = n->previous;

		if(n == processList.current) {
			processList.current = n->previous;
		}
	} else {		/* remove process somewhere else in the list */
		n->previous->next = n->next;
		n->next->previous = n->previous;

		if(n == processList.current) {
		    processList.current = n->next;
		}
	}

	free(n);

}

void removeCurrentProcess() {
	node* current = processList.current;

	if(processList.current->previous == NULL &&
		processList.current->next == NULL) { /* one process available */

		processList.current = processList.start = processList.end = NULL;

	} else if(processList.current->previous == NULL) { /* remove first process */

		processList.current = processList.start = processList.current->next;
		processList.current->previous = NULL;

	} else if(processList.current->next == NULL) { /* remove last process */

		processList.current = processList.end = processList.current->previous;
		processList.current->next = NULL;

	} else {		/* remove process somewhere else in the list */

		processList.current->previous->next = processList.current->next;
		processList.current->next->previous = processList.current->previous;
		processList.current = processList.current->next;
	}

	free(current);
}

void showProcesses() {
	node *n;
	n = processList.start;

	while(n != NULL) {
		printf("pid: %d\tstatus: ", n->process.pid);

		switch(n->process.status) {
			case 1:
				printf("BACKGROUND");
				break;
			case 2:
				printf("STOPPED");
				break;
		}

		printf("\tpath: %s\n", n->process.path);
		n = n->next;
	}
}
