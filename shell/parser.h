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

#ifndef PARSER_H
#define PARSER_H

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef REGEX_H
#define REGEX_H
#include <regex.h>
#endif

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#ifndef ENTER
#define ENTER 10
#endif

/* read a string from a given file (stdin or file stream) */
char *readline(FILE *stream);

/* 'start' and 'end' define the expression delimiters */

/* set the first expression delimiters with a given pattern
   return true or false if found any */
int setExpressionDelimiters(char *line, char *pattern, int *start, int *end);

/* get the expression/command in a given line */
char *getExpression(char* line, int start, int end, char skip);

/* calculate the amount of expression found */
int countExpression(char *line, char *pattern);

/* remove whitespace */
void trimLeadingWhitespace(char *line);

/* separete cmd arguments to cmd_tok */
void tokenize(char *cmd, char *cmd_tok[]);

/* remove double quotes of expression */
void removeQuotes(char *exp, char *quoteExp);

/* return true or false if there is the command of pattern in line */
int existExpression(char *line, char *pattern);

#endif
