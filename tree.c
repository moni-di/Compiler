/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#include "tree.h"
#include <stdlib.h>
nAryTreeNode* createTreeNode(){
  nAryTreeNode* newNode = (nAryTreeNode*)malloc(sizeof(nAryTreeNode));
  if(newNode == NULL){
    perror("Error creating new node..\n");
    exit(0);
  }
  newNode->parent = NULL;
  newNode->brother = NULL;
  newNode->leftMostChild = NULL;
  newNode->rightMostChild = NULL;
  newNode->numOfChildren = 0;
  newNode->extraArguments = NULL;
  newNode->visited = false;
  newNode->scopeSymTab = NULL;
  newNode->enclFunTypePtr = NULL;
  newNode->token.line_no = 0;
  // newNode->numASTChildren = 0;
  return newNode;
}

void addChild(nAryTreeNode* parent, nAryTreeNode* child){
  if(parent->numOfChildren==0){
    parent->leftMostChild = child;
    parent->rightMostChild = child;
  }
  else{
    parent->rightMostChild->brother = child;
    parent->rightMostChild = child;
  }
  parent->numOfChildren = parent->numOfChildren + 1;
  child->parent = parent;
}

nAryTreeNode* getNthChild(nAryTreeNode* node, int n){
  nAryTreeNode* temp = node->leftMostChild;
  for(int i=0;i<n-1;i++){
    if(temp)
      temp=temp->brother;
    else
      return NULL;
  }
  return temp;
}
