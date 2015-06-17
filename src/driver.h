#ifndef _DRIVER_H_
#define _DRIVER_H_

#include "globalDefs.h"
#include "constantDefs.h"

// terminal control codes

#define COLOR(s) if (codedOutput) {s}
#define COLOREXP(e) (codedOutput ? (e) : "")

#define SET_TERM(mode) ("\x1b[" mode "m")
#define AND ";"

#define RESET_CODE "0"
#define BRIGHT_CODE "1"

#define BLACK_CODE "30"
#define RED_CODE "31"
#define GREEN_CODE "32"
#define YELLOW_CODE "33"
#define BLUE_CODE "34"
#define MAGENTA_CODE "35"
#define CYAN_CODE "36"
#define WHITE_CODE "37"

// standard program strings

#define PROGRAM_STRING COLOREXP(SET_TERM(BRIGHT_CODE AND GREEN_CODE) )<<"anic"<<COLOREXP(SET_TERM(RESET_CODE))
#define LANGUAGE_STRING "ANI"
#define HOME_PAGE COLOREXP(SET_TERM(BRIGHT_CODE AND BLUE_CODE))<<"http://anic.googlecode.com/"<<COLOREXP(SET_TERM(RESET_CODE))
#define ERROR_STRING COLOREXP(SET_TERM(BRIGHT_CODE AND RED_CODE))<<"ERROR"<<COLOREXP(SET_TERM(RESET_CODE))
#define WARNING_STRING COLOREXP(SET_TERM(BRIGHT_CODE AND YELLOW_CODE))<<"WARNING"<<COLOREXP(SET_TERM(RESET_CODE))

#define HEADER_LITERAL PROGRAM_STRING<<" -- "<<LANGUAGE_STRING<<" Compiler v.["<<VERSION_STRING<<"."<<VERSION_STAMP<<"] (c) "<<VERSION_YEAR<<" Kajetan Adrian Biedrzycki\n" /* compiler header */
#define USAGE_LITERAL "\n\tusage:\t"<<PROGRAM_STRING<<" sourceFile... [-] [-o outputFile] [-p optimizationLevel]\n\t\t[-v] [-s] [-c] [-t tabWidth] [-e] [-h]\n" /* info literal */
#define SEE_ALSO_LITERAL "\n\tFor more information, type '"<<PROGRAM_STRING<<" -h'.\n" /* see also literal */
#define LINK_LITERAL "\thome page: "<<HOME_PAGE<<"\n" /* link literal */

// standard printing functions

#define print(s) if (!silentMode) { cout << s << "\n"; }
#define printLabel(s) if (!silentMode) { cout << "\n" << s << "\n"; }
#define printNotice(s) if (!silentMode) { cout << PROGRAM_STRING << ": " << s << ".\n"; }
#define printError(s) if (!silentMode) { cerr << ERROR_STRING << ": " << s << ".\n"; }; driverErrorCode++;
#define printWarning(s) if (!silentMode) { cerr << WARNING_STRING << ": " << s << ".\n"; }

#define GET_FILE_NAME(fi) ((fi != STANDARD_LIBRARY_FILE_INDEX) ? inFileNames[fi] : STANDARD_LIBRARY_FILE_NAME)

#define lexerError(fi,r,c,str) if (!silentMode) { silentMode = true; \
		cerr << ERROR_STRING << ": " << \
		COLOREXP(SET_TERM(CYAN_CODE)) << "LEXER" << COLOREXP(SET_TERM(RESET_CODE)) << \
		": " << COLOREXP(SET_TERM(BRIGHT_CODE AND BLACK_CODE)) << GET_FILE_NAME(fi) << ":" << r << ":" << c << COLOREXP(SET_TERM(RESET_CODE)) << ": " << str << ".\n"; \
		silentMode = false; } \
	lexerErrorCode++; \
	if (eventuallyGiveUp && lexerErrorCode >= TOLERABLE_ERROR_LIMIT) { printError("too many errors, giving up"); return NULL; }

#define parserError(fi,r,c,str) if (!silentMode) { silentMode = true; \
		cerr << ERROR_STRING << ": "<< \
		COLOREXP(SET_TERM(CYAN_CODE)) << "PARSER" << COLOREXP(SET_TERM(RESET_CODE)) << \
		": " << COLOREXP(SET_TERM(BRIGHT_CODE AND BLACK_CODE)) << GET_FILE_NAME(fi) << ":" << r << ":" << c << COLOREXP(SET_TERM(RESET_CODE)) << ": " << str << ".\n"; \
		silentMode = false; } \
	parserErrorCode++;

#define semmerError(fi,r,c,str) if (!silentMode) { silentMode = true; \
		cerr << ERROR_STRING << ": "<< \
		COLOREXP(SET_TERM(CYAN_CODE)) << "SEMMER" << COLOREXP(SET_TERM(RESET_CODE)) << \
		": " << COLOREXP(SET_TERM(BRIGHT_CODE AND BLACK_CODE)) << GET_FILE_NAME(fi) << ":" << r << ":" << c << COLOREXP(SET_TERM(RESET_CODE)) << ": " << str << ".\n"; \
		silentMode = false; } \
	semmerErrorCode++;

void printHeader(void);
void printUsage(void);
void printSeeAlso(void);
void printHelp(void);
void printLink(void);
const string &getFileName(unsigned int fileIndex);
void die(int errorCode);
void die(void);

#endif
