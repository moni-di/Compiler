/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#ifndef SEMANTIC_ANALYZER_DEF_H
#define SEMANTIC_ANALYZER_DEF_H

#include "driver.h"
#include "lexerDef.h"
#include "tree.h"
#include "hashtable.h"
#include <limits.h>

#define RED "\033[0;31m"
#define RESET "\033[0;37m"
#define YELLOW "\033[0;33m"
#define GREEN "\033[0;32m"

#define WIDTH_INTEGER 2
#define WIDTH_REAL 4
#define DONT_CARE -1

extern char keyToNT[NUM_OF_NONTERMINALS][MAX_SYMBOL_LENGTH];
extern char keyToToken[NUM_OF_TERMINALS][MAX_SYMBOL_LENGTH];

hash_entry ruidToNode[HASHTABLE_SIZE];

SymbolTable *currentSymbolTablePtr;
SymbolTable *rootSymbolTablePtr;

bool isLoopVariable();
void constructSemanticsVerification(nAryTreeNode* root);
void thirdASTPass();
void genConstrTypetoNodeMap();
void insertAlias();
void addUnionFields();
void addRecordFields();
bool isAlias();
bool doTypesMatch();
void markLoopVar();
bool isIdInWhileLoop();
void insertIdInLoopArgs();
void whileLoopSemanticsVerification();
type* getType();
void insertRecordOrUnion();
void insertParameterInList();
type* checkEnclFunParams();
void* findEntryInSymTab();
void assignmentSemanticsVerification();
void ioSemanticsVerification();
void markOutputParameter();
bool isRelationalOp();
void boolSemanticsVerification();
void conditionSemanticsVerification();
void constructSymbolTable();
void insertFunctionDefinition();
void funDefSemanticsVerification();
type* getTypeSingleOrRecId();
type* getArithmeticExpressionType();
void funCallSemanticsVerification();
void parameterListVerification();
bool scalarWithRecord(type * t1,type * t2,bool is_divide);
void printSymbolTable(SymbolTable * sym_table);
void printActivationRecord(SymbolTable * sym_table);
void printTypeExpression(type *t);
void printAlias(type* t);
void printGLobalVariables(SymbolTable * sym_table);
void printRecordTypesAndSizes(SymbolTable * sym_table);
// bool fnReDefn;0
// void freeParser(nAryTreeNode * root);
#endif
