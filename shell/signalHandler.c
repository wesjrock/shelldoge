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

#include "signalHandler.h"

void ignoreSignals() {
	struct sigaction signal;

	signal.sa_handler = SIG_IGN;		/* ignore signals */
	sigaction(SIGINT, &signal, NULL);	/* ignore SIGINT */
	sigaction(SIGTSTP, &signal, NULL);	/* ignore SIGTSTP */
}

void defaultSignals() {
	struct sigaction signal;

	signal.sa_handler = SIG_DFL;	/* signals get default behavior */
	sigaction(SIGINT, &signal, NULL);
	sigaction(SIGTSTP, &signal, NULL);
}

void prepareSignals() {
	struct sigaction signal;

	signal.sa_sigaction = sa_sigactionn;/* deal with SIGCHLD */
	signal.sa_flags = SA_SIGINFO;		/* use sa_sigaction and not sa_handler */

	sigaction(SIGCHLD, &signal, NULL);	/* deal with SIGHLD */
	sigaction(SIGHUP, &signal, NULL);	/* deal with SIGHUP */

	signal.sa_handler = SIG_IGN;
	sigaction(SIGTTOU, &signal, NULL);
}

void sa_sigactionn(int signo, siginfo_t *siginfo, void *context) {
	/* if child is destroyed */
	/* (SA_NOCLDSTOP setted to ignore when suspended or running) */
	if (signo == SIGCHLD) {
	
		if (siginfo->si_code == CLD_STOPPED) {		/* if child suspended */
			changeCurrentProcessStatus(STOPPED);	/* parent get suspended */
		} else if (siginfo->si_code == CLD_EXITED ||
			siginfo->si_code == CLD_KILLED ||
			siginfo->si_code == CLD_DUMPED) {		/* child got destroyed */
			
			pid_t pid;
			int zombie = 0;

			/* remove zombies */
			while((pid = waitpid((pid_t)(-1), 0, WNOHANG)) > 0) {
				zombie = 1;
				removeProcess(pid);
			}

			if(!zombie) {
				removeCurrentProcess();
			}
		}
	} else if(signo == SIGHUP) {	/* terminal close signal */
		destroyProcessList();	/* destroy all processes */
		kill(getpid(), SIGKILL);	/* destroy shell */
	}
}
