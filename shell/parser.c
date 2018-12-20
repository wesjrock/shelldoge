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

#include "parser.h"

char *readline(FILE *stream) {
	char c;
	char *str = NULL;
	int i = 0;

	do {
		c = fgetc(stream);
		str = (char *) realloc(str, sizeof(char) * (i+1));
		str[i++] = c;
	} while(c != ENTER && !feof(stream));

	str[i-1] = '\0';

	return str;
}

int setExpressionDelimiters(char *line, char *pattern, int *start, int *end) {
	regex_t regex;
	regmatch_t matchptr[1];

	if(strlen(line) == 0) {
		return 0;
	}

	/* compile using regular expression */
	regcomp(&regex, pattern, REG_EXTENDED);

	/* execute the regular expression */
	if(!regexec(&regex, line, 1, matchptr, 0)) {
		*start = matchptr[0].rm_so;	/* set start */
		*end = matchptr[0].rm_eo;	/* set end (last char + 1) */

		return 1;
	}

	return 0;
}

char *getExpression(char* line, int start, int end, char skip) {
	int size = end - start;
	char *expression;

	expression = malloc(size + 1 - skip);		/* +'/0' */
	strncpy(expression, line + start + skip, size - skip);
	expression[size - skip] = '\0';
	trimLeadingWhitespace(expression);

	return expression;
}

int countExpression(char *line, char *pattern) {
	regex_t regex;
	regmatch_t matchptr[1];
	int offset;
	int i;

	regcomp(&regex, pattern, REG_EXTENDED);

	offset = 0;

	/* i = number of expressions */
	for(i = 0; !regexec(&regex, line + offset, 1, matchptr, 0); i++) {
		offset += matchptr[0].rm_eo;	/* last char position */
	}

	return i;
}

void trimLeadingWhitespace(char *line) {
	int i = 0;

	if (line[i] != ' ') {
		return;
	}

	while(line[i] == ' ') {
		i++;
	}

	int size = strlen(line) - i + 1;

	strncpy(line, line + i, size - 1);
	line[size - 1] = '\0';
}

void tokenize(char *cmd, char *cmd_tok[]) {
	char *expression = NULL;
	int i;

	/*	startLine = begin of next expression search
		startExp = begin of expression found
		endExp = end of expression found */
	int startLine = 0, startExp, endExp;

	for(i = 0; setExpressionDelimiters(cmd + startLine,
		"[^\" ]+|([^ \"]*\"[^\"]*\"[^ \"]*)+",
		&startExp, &endExp); i++) {

		expression = getExpression(cmd + startLine, startExp, endExp, 0);

		/* argument with no quotes */
		char argument[strlen(expression) + 1];
		removeQuotes(expression, argument);

		cmd_tok[i] = malloc(strlen(expression) + 1);
		strcpy(cmd_tok[i], argument);

		/* set next search */
		startLine += endExp;

		free(expression);
	}

	cmd_tok[i] = NULL;
}

void removeQuotes(char *exp, char *quoteExp) {
	int size = strlen(exp);
	int iExp = 0, iQuoteExp = 0;

	while(iExp < size) {
		if(exp[iExp] != '"') {
			quoteExp[iQuoteExp] = exp[iExp];
			iQuoteExp++;
		}

		iExp++;
	}

	quoteExp[iQuoteExp] = '\0';
}

int existExpression(char *line, char *pattern) {
	regex_t regex;
	regcomp(&regex, pattern, REG_EXTENDED);

	return !regexec(&regex, line, 0, NULL, 0);
}
