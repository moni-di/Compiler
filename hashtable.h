/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#ifndef HASHTABLEDEF_H
#define HASHTABLEDEF_H

#define HASHTABLE_SIZE 517
#define MAX_LEX_LEN 100
#define KEY_NOT_FOUND -1
#include <stdbool.h>

typedef struct{
    char lexeme[MAX_LEX_LEN+1];
    bool present;
    void* value;
} hash_entry;

typedef hash_entry hash_table[HASHTABLE_SIZE];

typedef struct SymbolTable{
	hash_table table;
	char *enclFunName;
	int offset;
	struct SymbolTable *parentSymTab;
	struct SymbolTable *leftmostChildSymTab;
	struct SymbolTable *rightmostChildSymTab;
	struct SymbolTable *brotherSymTab;
}SymbolTable;

int calculateHash(char* str);
void initTable(hash_entry* table);
void insertTable(hash_entry* table, char* lexeme, int value);
void insertTablePtr(hash_entry* table, char* lexeme, void* value);
bool searchTable(hash_entry* table, char* lexeme);
int searchHashTable(hash_entry* table, char* lexeme);
void* searchHashTablePtr(hash_entry* table, char* lexeme);
SymbolTable* initSymbolTable();

#endif
