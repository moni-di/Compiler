/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#ifndef TREE_ADT_DEF_H
#define TREE_ADT_DEF_H

#include <stdbool.h>
#include "parserDef.h"
#define MAX_ERR_LEN 100
// #include "semantic_analyzer.h"

typedef struct LOOP_VAR{
  char varName[MAX_LEX_LEN];
  bool isAssigned;
  struct LOOP_VAR* nextVar;
}loopVar;

typedef struct{
  int count;
  loopVar* firstVar;
}loopVars;


typedef struct type type;

typedef struct parListNode{
    type *t;
    char paramName[MAX_LEX_LEN];
    struct parListNode *next;
}parListNode;

typedef struct paramsList{
    parListNode *first;
    parListNode *last;
    int length;
} paramsList;

typedef struct ALIASES{
  char aliasName[MAX_LEX_LEN];
  struct ALIASES* next;
}aliases;


typedef struct type{
    tokenName name;
    union{
        struct{
            bool isTaggedUnion;
            /* only used in case record is a tagged union*/
            type* tag;
            type* variantField;
            /*********************************************/
            aliases* alias;
            paramsList *members;
            char recName[MAX_LEX_LEN];
            int currentOffset;
        }Record;

        struct{
          aliases* alias;
          paramsList *members;
          char uniName[MAX_LEX_LEN];
          int currentOffset;
        }Union;

        struct{
          char aliasOf[MAX_LEX_LEN];
          char aliasName[MAX_LEX_LEN];
        }Alias;

        struct{
            paramsList *inputParList;
            paramsList *outputParList;
            char functionName[MAX_LEX_LEN];
            int currentOffset;
            int offsetParams;
        }function;

          char errorMsg[MAX_ERR_LEN];
        // }

    } typeInformation;
    bool isAssigned;
    int width;
    int offset;
    int offsetUsed;
    char *enclFunName;
    int line_no;
}type;

typedef struct NARYTREENODE{
  struct NARYTREENODE *parent;
  struct NARYTREENODE *brother;
  struct NARYTREENODE *leftMostChild;
  struct NARYTREENODE *rightMostChild;
  symbol sym;
  TOKEN token;
  int numOfChildren;
  void* extraArguments;
  bool visited;
  SymbolTable* scopeSymTab;
  type* enclFunTypePtr;
  // int numASTChildren;
}nAryTreeNode;


nAryTreeNode* createTreeNode();
void addChild(nAryTreeNode* parent, nAryTreeNode* child);
nAryTreeNode* getNthChild(nAryTreeNode* node, int n);

#endif
