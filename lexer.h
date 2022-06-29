/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#ifndef LEXERDEF_H
#define LEXER_H

#include "lexerDef.h"
#include <stdio.h>
#include <stdlib.h>

void initializeLexer(FILE *source);
void generateLookupTable();
extern void generateTokenMap();

extern TOKEN getNextToken(FILE *fp);
extern TOKEN getToken();
char getChar(FILE *fp);

extern void fillBuffer(FILE *fp);
void fillCommentBuffer(FILE* fp);
extern void retract(int numOfChars);

extern tokenName searchKeyword(char *lexeme);

void removeComments(char * sourceFile);
void getStream(FILE *source);

#endif
