/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#ifndef PARSERDEF_H
#define PARSERDEF_H

#include "driver.h"
#include "set.h"
#include "lexerDef.h"
#include "hashtable.h"
#include <stdio.h>
#include <stdbool.h>
#define RHS_MAX_LENGTH 100
#define NO_MATCHING_RULE -1
#define SYN_RULE -2
#define NUM_OF_RULES 95

typedef enum{
  #include "nonTerminals.txt"
}ntName;

extern char keyToNT[NUM_OF_NONTERMINALS][MAX_SYMBOL_LENGTH];
extern char keyToToken[NUM_OF_TERMINALS][MAX_SYMBOL_LENGTH];

typedef struct{
  union{
    tokenName t;
    ntName nt;
  };
  bool isTerminal;
}symbol;

typedef struct rhsnode{
  symbol sym;
  struct rhsnode *next;
} rhsnode;

typedef struct{
  ntName lhs;
  rhsnode* head;
  rhsnode* tail;
} ruleRHS;

int firstSet[NUM_OF_TERMINALS][SET_SIZE];
int followSet[NUM_OF_TERMINALS][SET_SIZE];

ruleRHS grammar[NUM_OF_RULES];

hash_table terminalTable;
hash_table nonTerminalTable;

int parse_table[NUM_OF_NONTERMINALS][NUM_OF_TERMINALS];
#endif
