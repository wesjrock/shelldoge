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

int main(int argc, char *argv[]) {
	int exit = 0;
	char *line;			/* string to be read from input */
	FILE *input;		/* file input stream */

	dogeFullColor();

	newProcessList();
	prepareSignals();
	
	input = handleArguments(argc, argv);

	while(!exit) {
		showcwd("$");
		line = readline(input);
		
		if(checkExit(line)) {
			exit = 1;
		} else {
			execute(line);
		}

		free(line);
	}

	destroyProcessList();

	return EXIT_SUCCESS;
}
