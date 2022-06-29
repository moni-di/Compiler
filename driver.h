/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#ifndef DRIVER_H
#define DRIVER_H
#include <time.h>
#include <string.h>
#include<stdbool.h>
#define NUM_OF_TERMINALS 64
#define MAX_SYMBOL_LENGTH 50
#define NUM_OF_NONTERMINALS 53

char keyToToken[NUM_OF_TERMINALS][MAX_SYMBOL_LENGTH];
char keyToNT[NUM_OF_NONTERMINALS][MAX_SYMBOL_LENGTH];
bool errorsDetected;

#endif
