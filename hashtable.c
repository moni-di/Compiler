/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include<stdlib.h>
#include "hashtable.h"

int calculateHash(char* str){
    long long hash_value = 0;
    int n = strlen(str);
    const long long p = 31;
    const long long PRIME = 1000000007;
    long long prime_pow = 1;
    for(int i=0;i<n;i++){
        hash_value = (hash_value + (str[i] * prime_pow) % PRIME) % PRIME;
        prime_pow = (prime_pow * p) % PRIME;
    }
    return hash_value % HASHTABLE_SIZE;
}

void initTable(hash_entry* table){
    for(int i=0;i<HASHTABLE_SIZE;i++){
        table[i].present = false;
        table[i].value = NULL;
    }
}

void insertTable(hash_entry* table, char* lexeme, int value){
    int hash_value = calculateHash(lexeme);
    int probe_no = 0;
    while(table[hash_value].present == true && probe_no < HASHTABLE_SIZE){
        hash_value = (hash_value + 1) % HASHTABLE_SIZE;
        probe_no++;
    }
    if(table[hash_value].present == true){
        printf("Hash Table full, cannot insert\n");
        return;
    }
    table[hash_value].present = true;
    strncpy(table[hash_value].lexeme, lexeme, MAX_LEX_LEN);
    table[hash_value].value = malloc(sizeof(int));
    *(int*)(table[hash_value].value) = value;
}

void insertTablePtr(hash_entry* table, char* lexeme, void* value){
    int hash_value = calculateHash(lexeme);
    int probe_no = 0;
    while(table[hash_value].present == true && probe_no < HASHTABLE_SIZE){
        hash_value = (hash_value + 1) % HASHTABLE_SIZE;
        probe_no++;
    }
    if(table[hash_value].present == true){
        printf("Hash Table full, cannot insert\n");
        return;
    }
    table[hash_value].present = true;
    strncpy(table[hash_value].lexeme, lexeme, MAX_LEX_LEN);
    table[hash_value].value = value;
}

bool searchTable(hash_entry* table, char* lexeme){
    int hash_value = calculateHash(lexeme);
    int probe_no = 0;
    while(table[hash_value].present == true && probe_no < HASHTABLE_SIZE){
        if(strcmp(table[hash_value].lexeme, lexeme) == 0){
            return true;
        }
        hash_value = (hash_value + 1) % HASHTABLE_SIZE;
        probe_no++;
    }
    return false;
}

int searchHashTable(hash_entry* table, char* lexeme){
    int hash_value = calculateHash(lexeme);
    int probe_no = 0;
    while(table[hash_value].present == true && probe_no < HASHTABLE_SIZE){
        if(strcmp(table[hash_value].lexeme, lexeme) == 0){
            return *(int*)(table[hash_value].value);
        }
        hash_value = (hash_value + 1) % HASHTABLE_SIZE;
        probe_no++;
    }
    return KEY_NOT_FOUND;
}

void* searchHashTablePtr(hash_entry* table, char* lexeme){
    int hash_value = calculateHash(lexeme);
    int probe_no = 0;
    while(table[hash_value].present == true && probe_no < HASHTABLE_SIZE){
        if(strcmp(table[hash_value].lexeme, lexeme) == 0){
            return table[hash_value].value;
        }
        hash_value = (hash_value + 1) % HASHTABLE_SIZE;
        probe_no++;
    }
    return NULL;
}

SymbolTable* initSymbolTable(){
  SymbolTable* symTab = (SymbolTable*) malloc(sizeof(SymbolTable));
  initTable(symTab->table);
  symTab->parentSymTab=NULL;
  symTab->leftmostChildSymTab=NULL;
  symTab->rightmostChildSymTab=NULL;
  symTab->brotherSymTab=NULL;
}
