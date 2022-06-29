/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#ifndef AST_H
#define AST_H
    
    # include "parser.h"

    int usefulTokens(int tokenClass);
    void copyTreeNode(nAryTreeNode* dst, nAryTreeNode* src);
    void constructASTChildren(nAryTreeNode* parent, nAryTreeNode* children);
    nAryTreeNode* constructAST(nAryTreeNode* root);


#endif