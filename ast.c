/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#include "ast.h"
# include <stdlib.h>
# include <stdio.h>
#include <string.h>
int usefulTokens(int tokenClass) {
    switch(tokenClass) {
        case TK_CALL:
        case TK_WITH:
        case TK_PARAMETERS:
        case TK_PARAMETER:
        case TK_SEM:
        case TK_OP:
        case TK_CL:
        case TK_SQL:
        case TK_SQR:
        case TK_COMMA:
        case TK_ENDIF:
        case TK_ENDUNION:
        case TK_ENDRECORD:
        case TK_TYPE:
        case TK_COLON:
        case TK_LIST:
        case TK_END:
        case TK_DEFINETYPE:
        case TK_AS:
        case TK_ENDWHILE:
        case TK_IF:
        case TK_ASSIGNOP:
        case TK_WHILE:
        case TK_THEN:
        case TK_DOT:
        case EPSILON:
            return 0;
            break;

        default:
            return 1;
    }
}

void copyTreeNode(nAryTreeNode* dst,nAryTreeNode* src)
{
    dst->numOfChildren = src->numOfChildren;
    dst->leftMostChild = src->leftMostChild;
    dst->rightMostChild = src->rightMostChild;
    // dst->brother = src->brother;
    dst->sym.isTerminal = src->sym.isTerminal;
    if(dst->sym.isTerminal){
      dst->sym.t = src->sym.t;
      dst->token.line_no = src->token.line_no;
      dst->token.name = src->token.name;
      switch (dst->token.name) {
        case TK_NUM: dst->token.num = src->token.num;break;
        case TK_RNUM: dst->token.rnum = src->token.rnum;break;
        default:strncpy(dst->token.id, src->token.id, MAX_LEX_LEN);
      }
    }
    else
      dst->sym.nt = src->sym.nt;

}

void constructASTChildren(nAryTreeNode* parent, nAryTreeNode* children){
  // printf("%s-",parent->sym.isTerminal?keyToToken[parent->sym.t]:keyToNT[parent->sym.nt]);
	nAryTreeNode* temp = children;
	while(temp!=NULL){
		nAryTreeNode* curChild = constructAST(temp);
		if(curChild != NULL){
      // printf("%s\n",curChild->sym.isTerminal?keyToToken[curChild->sym.t]:keyToNT[curChild->sym.nt]);
			addChild(parent, curChild);
		}
		temp = temp->brother;
	}
}

nAryTreeNode* constructAST(nAryTreeNode* root)
{
	if(!root)
		return NULL;

	nAryTreeNode* ast;

	if(root->numOfChildren==0){

      if(!root->sym.isTerminal){
          return NULL;
      }else if(!usefulTokens(root->sym.t)){
				return NULL;
      }
			else{
				ast = createTreeNode();
				copyTreeNode(ast, root);
				return ast;
			}
	}
	ast = createTreeNode();
  ast->sym.isTerminal = root->sym.isTerminal;
    if(ast->sym.isTerminal){
      ast->sym.t = root->sym.t;
      ast->token.name = root->token.name;
      switch (ast->token.name) {
        case TK_NUM: ast->token.num = root->token.num;break;
        case TK_RNUM: ast->token.rnum = root->token.rnum;break;
        default:strncpy(ast->token.id, root->token.id, MAX_LEX_LEN);
      }
    }
    else ast->sym.nt = root->sym.nt;



  //ast me root ko copy karna hai

	constructASTChildren(ast, root->leftMostChild);

	if(ast->numOfChildren==0){
    free(ast);
		return NULL;
	}
	else if(ast->numOfChildren == 1 ){
    if(ast->sym.nt != program){
      nAryTreeNode * temp=ast->leftMostChild;
      copyTreeNode(ast, ast->leftMostChild);
      free(temp);
    }
    ast->parent = NULL;
	}
  // if(ast){
  //     printf("%s*\n",ast->sym.isTerminal?keyToToken[ast->sym.t]:keyToNT[ast->sym.nt]);
  //     nAryTreeNode* temp = ast->leftMostChild;
  //     while(temp){
  //         printf("%s\n",temp->sym.isTerminal?keyToToken[temp->sym.t]:keyToNT[temp->sym.nt]);
  //         temp = temp->brother;
  //     }
  // }
	return ast;
}
