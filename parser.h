/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#ifndef PARSER_H
#define PARSER_H

#include "parserDef.h"
#include "tree.h"
#include <stdio.h>

void populateGrammar(FILE *fp);

symbol getSymbol(char str[]);

void insertSymbol(rhsnode*  *tail, symbol sym);

void printGrammar();

void initializeParser();

void generateParseTable();

void computeFirstSets();

void computeFollowSets();

void printFirstSets();

void printFollowSets();

void printParseTable();

void printParseTreePreorder(char* prefix, nAryTreeNode *root, bool isLeft);
void printParseTree(nAryTreeNode* root, FILE* op);

void printRule(int rule_no);

nAryTreeNode *parseInputSourceCode(FILE *source);

void freeGrammar();

void printSymbol(symbol sym);
extern TOKEN getNextToken(FILE *fp);
#endif
