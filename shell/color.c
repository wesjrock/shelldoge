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

#include "color.h"

void showcwd(char *prompt) {
	char cwd[PATH_LENGTH];		/* current path */
	
	printf(BOLD_ON FG_YELLOW "%s" RESET, getcwd(cwd, sizeof(cwd)));
	printf(FG_WHITE "%s " RESET, prompt);
}

void dogeWhite() {
	printf(RESET);
	printf("                Y.                      _          \n");	/* 1 */
	printf("                YiL                   .```.        \n");	/* 2 */
	printf("                Yii;                .; .;;`.       \n");	/* 3 */
	printf("                YY;ii._           .;`.;;;; :       \n");	/* 4 */
	printf("                iiYYYYYYiiiii;;;;i` ;;::;;;;       \n");	/* 5 */
	printf("            _.;YYYYYYiiiiiiYYYii  .;;.   ;;;       \n");	/* 6 */
	printf("         .YYYYYYYYYYiiYYYYYYYYYYYYii;`  ;;;;       \n");	/* 7 */
	printf("       .YYYYYYY$$YYiiYY$$$$iiiYYYYYY;.ii;`..       \n");	/* 8 */
	printf("      :YYY$!.  TYiiYY$$$$$YYYYYYYiiYYYYiYYii.      \n");	/* 9 */
	printf("      Y$MM$:   :YYYYYY$!\"``\"4YYYYYiiiYYYYiiYY.   \n");	/* 10 */
	printf("   `. :MM$$b.,dYY$$Yii\" :'   :YYYYllYiiYYYiYY     \n");	/* 11 */
	printf("_.._ :`4MM$!YYYYYYYYYii,.__.diii$$YYYYYYYYYYY      \n");	/* 12 */
	printf(".,._ $b`P`     \"4$$$$$iiiiiiii$$$$YY$$$$$$YiY;    \n");	/* 13 */
	printf("   `,.`$:       :$$$$$$$$$YYYYY$$$$$$$$$YYiiYYL    \n");	/* 14 */
	printf("    \"`;$$.    .;PPb$`.,.``T$$YY$$$$YYYYYYiiiYYU:  \n");	/* 15 */
	printf("    ;$P$;;: ;;;;i$y$\"!Y$$$b;$$$Y$YY$$YYYiiiYYiYY  \n");	/* 16 */
	printf("    $Fi$$ .. ``:iii.`-\":YYYYY$$YY$$$$$YYYiiYiYYY  \n");	/* 17 */
	printf("    :Y$$rb ````  `_..;;i;YYY$YY$$$$$$$YYYYYYYiYY:  \n");	/* 18 */
	printf("     :$$$$$i;;iiiiidYYYYYYYYYY$$$$$$YYYYYYYiiYYYY. \n");	/* 19 */
	printf("      `$$$$$$$YYYYYYYYYYYYY$$$$$$YYYYYYYYiiiYYYYYY \n");	/* 20 */
	printf("      .i!$$$$$$YYYYYYYYY$$$$$$YYY$$YYiiiiiiYYYYYYY \n");	/* 21 */
	printf("     :YYiii$$$$$$$YYYYYYY$$$$YY$$$$YYiiiiiYYYYYYi' \n");	/* 22 */

	fflush(stdout);
}

void dogeColor() {
	printf(FG_YELLOW);
	printf(BOLD_OFF);
	printf(BOLD_OFF"                Y.                      _          \n");	/* 1 */
	printf(BOLD_OFF"                YiL                   .```.        \n");	/* 2 */
	printf(BOLD_OFF"                Yii;                .; .;;`.       \n");	/* 3 */
	printf(BOLD_OFF"                YY;ii._           .;`.;;;; :       \n");	/* 4 */
	printf(BOLD_OFF"                iiYYYYYYiiiii;;;;i` ;;::;;;;       \n");	/* 5 */
	printf(BOLD_OFF"            _.;YYYYYYiiiiiiYYYii  .;;.   ;;;       \n");	/* 6 */
	printf(BOLD_OFF"         .YYYYYYYYYYiiYYYYYYYYYYYYii;`  ;;;;       \n");	/* 7 */
	printf(BOLD_OFF"       .YYYYYYY$$YYiiYY$$$$iiiYYYYYY;.ii;`..       \n");	/* 8 */
	printf(BOLD_OFF"      :YYY$!"FG_WHITE"."FG_YELLOW"  TYiiYY$$$$$YYYYYYYiiYYYYiYYii.      \n");	/* 9 */
	printf(BOLD_OFF"      Y$MM$"FG_WHITE":"FG_YELLOW"   :YYYYYY$!\"``\"4YYYYYiiiYYYYiiYY.   \n");	/* 10 */
	printf(BOLD_ON "   `. "BOLD_OFF":MM$$b.,dYY$$Yii\" "FG_WHITE":'"FG_YELLOW"   :YYYYllYiiYYYiYY     \n");	/* 11 */
	printf(BOLD_ON "_.._ :`"BOLD_OFF"4MM$!YYYYYYYYYii,.__.dii"BOLD_ON"i$$YYYYYYY"BOLD_OFF"YYYY      \n");	/* 12 */
	printf(BOLD_ON ".,._ $b`P"BOLD_OFF"`     \"4$$$$$iiiii"BOLD_ON"iii$$$$YY$$$$$$"BOLD_OFF"YiY;    \n");	/* 13 */
	printf(BOLD_ON "   `,.`$:       :$$$$$$$$$YYYYY$$$$$$$$$YYii"BOLD_OFF"YYL    \n");	/* 14 */
	printf(BOLD_ON "    \"`;$$.    .;PPb$`.,.``T$$YY$$$$YYYYYYiii"BOLD_OFF"YYU:  \n");	/* 15 */
	printf(BOLD_ON "    ;$P$;;: ;;;;i$y$\"!Y$$$b;$$$Y$YY$$YYYiiiY"BOLD_OFF"YiYY  \n");	/* 16 */
	printf(BOLD_ON "    $Fi$$ .. ``:iii.`-\":YYYYY$$YY$$$$$YYYiiY"BOLD_OFF"iYYY  \n");	/* 17 */
	printf(BOLD_ON "    :Y$$rb ````  `_..;;i;YYY$YY$$$$$$$YYYYYY"BOLD_OFF"YiYY:  \n");	/* 18 */
	printf(BOLD_ON "     :$$$$$i;;iiiiidYYYYYYYYYY$$$$$$YYYYYYY"BOLD_OFF"iiYYY"BOLD_OFF"Y. \n");	/* 19 */
	printf(BOLD_ON "      `$$$$$$$YYYYYYYYYYYYY$$$$$$YYYYYYYY"BOLD_OFF"iiiYYYYYY \n");	/* 20 */
	printf(BOLD_ON "      .i!$$$$$$YYYYYYYYY$$$$$$YYY$$YY"BOLD_OFF"iiiiii"BOLD_ON"YYYYYY"BOLD_OFF"Y \n");	/* 21 */
	printf(BOLD_ON "     :YYiii$$$$$$$YYYYYYY$$$$YY$$$$YYiiiiiYYYYYY"BOLD_OFF"i' \n");	/* 22 */
	printf(RESET);

	fflush(stdout);
}

void dogeFullColor() {
	printf(FG_YELLOW);
	printf(BOLD_OFF);
	printf(BOLD_OFF"                Y.     "FG_RED"much improve"FG_YELLOW"     _          \n");	/* 1 */
	printf(BOLD_OFF"  "FG_WHITE"Shelldoge"FG_YELLOW"     YiL                   .```."FG_BLUE"so amaze"FG_YELLOW"\n");	/* 2 */
	printf(BOLD_OFF"                Yii;                .; .;;`.       \n");	/* 3 */
	printf(BOLD_OFF"                YY;ii._           .;`.;;;; :       \n");	/* 4 */
	printf(BOLD_OFF"                iiYYYYYYiiiii;;;;i` ;;::;;;;       \n");	/* 5 */
	printf(BOLD_OFF"            _.;YYYYYYiiiiiiYYYii  .;;.   ;;;       \n");	/* 6 */
	printf(BOLD_OFF"         .YYYYYYYYYYiiYYYYYYYYYYYYii;`  ;;;;       \n");	/* 7 */
	printf(BOLD_OFF"  "FG_GREEN"wow"FG_YELLOW"  .YYYYYYY$$YYiiYY$$$$iiiYYYYYY;.ii;`..       \n");	/* 8 */
	printf(BOLD_OFF"      :YYY$!"FG_WHITE"."FG_YELLOW"  TYiiYY$$$$$YYYYYYYiiYYYYiYYii.      \n");	/* 9 */
	printf(BOLD_OFF"      Y$MM$"FG_WHITE":"FG_YELLOW"   :YYYYYY$!\"``\"4YYYYYiiiYYYYiiYY.   \n");	/* 10 */
	printf(BOLD_ON "   `. "BOLD_OFF":MM$$b.,dYY$$Yii\" "FG_WHITE":'"FG_YELLOW"   :YYYYllYiiYYYiYY     \n");	/* 11 */
	printf(BOLD_ON "_.._ :`"BOLD_OFF"4MM$!YYYYYYYYYii,.__.dii"BOLD_ON"i$$YYYYYYY"BOLD_OFF"YYYY      \n");	/* 12 */
	printf(BOLD_ON ".,._ $b`P"BOLD_OFF"`     \"4$$$$$iiiii"BOLD_ON"iii$$$$YY$$$$$$"BOLD_OFF"YiY;    \n");	/* 13 */
	printf(BOLD_ON "   `,.`$:       :$$$$$$$$$YYYYY$$$$$$$$$YYii"BOLD_OFF"YYL    \n");	/* 14 */
	printf(BOLD_ON "    \"`;$$.    .;PPb$`.,.``T$$YY$$$$YYYYYYiii"BOLD_OFF"YYU:  \n");	/* 15 */
	printf(BOLD_ON "    ;$P$;;: ;;;;i$y$\"!Y$$$b;$$$Y$YY$$YYYiiiY"BOLD_OFF"YiYY  \n");	/* 16 */
	printf(BOLD_ON "    $Fi$$ .. ``:iii.`-\":YYYYY$$YY$$$$$YYYiiY"BOLD_OFF"iYYY  \n");	/* 17 */
	printf(BOLD_ON "    :Y$$rb ````  `_..;;i;YYY$YY$$$$$$$YYYYYY"BOLD_OFF"YiYY:  \n");	/* 18 */
	printf(BOLD_ON "     :$$$$$i;;iiiiidYYYYYYYYYY$$$$$$YYYYYYY"BOLD_OFF"iiYYY"BOLD_OFF"Y. \n");	/* 19 */
	printf(BOLD_ON "      `$$$$$$$YYYYYYYYYYYYY$$$$$$YYYYYYYY"BOLD_OFF"iiiYYYYYY \n");	/* 20 */
	printf(BOLD_ON "      .i!$$$$$$YYYYYYYYY$$$$$$YYY$$YY"BOLD_OFF"iiiiii"BOLD_ON"YYYYYY"BOLD_OFF"Y \n");	/* 21 */
	printf(BOLD_ON "     :YYiii$$$$$$$YYYYYYY$$$$YY$$$$YYiiiiiYYYYYY"BOLD_OFF"i' \n");	/* 22 */
	printf(RESET);

	fflush(stdout);
}
