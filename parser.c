/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#include "parser.h"
#include "hashtable.h"
#include "stack.h"
#include "set.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void initializeParser(){
    initTable(terminalTable);
    initTable(nonTerminalTable);
    for(int i=0;i<NUM_OF_NONTERMINALS;i++){
        initializeSet(firstSet[i]);
    }
    for(int i=0;i<NUM_OF_TERMINALS;i++){
        insertTable(terminalTable, keyToToken[i], i);
    }
    for(int i=0;i<NUM_OF_NONTERMINALS;i++){
      insertTable(nonTerminalTable, keyToNT[i], i);
    }
    for(int i=0;i<NUM_OF_NONTERMINALS;i++){
        for(int j=0;j<NUM_OF_TERMINALS;j++){
            parse_table[i][j] = NO_MATCHING_RULE;
        }
    }

}

void populateGrammar(FILE *fp)
{
    int rule_num = 0;
    char buffer[RHS_MAX_LENGTH];
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        char *sym_read;
        sym_read = strtok(buffer, " \n");
        grammar[rule_num].lhs = getSymbol(sym_read).nt;
        grammar[rule_num].head = NULL;
        grammar[rule_num].tail = NULL;
        sym_read = strtok(NULL, " \n");

        while (sym_read != NULL)
        {
            symbol sym = getSymbol(sym_read);
            insertSymbol(&(grammar[rule_num].tail), sym);
            if (grammar[rule_num].head == NULL)
            {
                grammar[rule_num].head = grammar[rule_num].tail;
            }
            sym_read = strtok(NULL, " \n");
        }


        rule_num++;
      }
}

symbol getSymbol(char* str){
    symbol sym;
    char lexeme[strlen(str)];
    strcpy(lexeme, str);
    if(str[0] >= 'A' && str[0] <= 'Z'){
        sym.isTerminal = true;
        sym.t = searchHashTable(terminalTable, lexeme);
    }
    else{
        sym.isTerminal = false;
        sym.nt = searchHashTable(nonTerminalTable, lexeme);
    }
    return sym;
}

void insertSymbol(rhsnode** ptr_tail, symbol sym){
    rhsnode* node = (rhsnode*) malloc(sizeof(rhsnode));
    if(node == NULL){
        perror("insertion at end failed\n");
        exit(1);
    }
    node->sym = sym;
    node->next = NULL;
    if(*ptr_tail != NULL){
        (*ptr_tail)->next = node;
    }
    *ptr_tail = node;
}





void computeFirstSets(){


    for(int i=NUM_OF_RULES-1;i>=0;i--){

        int curr_non_terminal=grammar[i].lhs;
        rhsnode* head=grammar[i].head;
        rhsnode* temp=head;


        while(temp!=NULL){

            if((temp->sym).isTerminal==true){
                addElement(firstSet[curr_non_terminal],(temp->sym).t);
                break;
            }else{
                set copyFirstSet=(set)malloc(sizeof(int)*SET_SIZE);

                for(int j=0;j<SET_SIZE;j++){
                    copyFirstSet[j]=firstSet[(temp->sym).nt][j];
                }

                if(findElement(copyFirstSet,EPSILON)==true){

                    if(temp->next!=NULL){
                        removeElement(copyFirstSet,EPSILON);
                    }

                    takeUnion(firstSet[curr_non_terminal],firstSet[curr_non_terminal],copyFirstSet);

                    temp=temp->next;
                }else{
                  takeUnion(firstSet[curr_non_terminal],firstSet[curr_non_terminal],firstSet[(temp->sym).nt]);
                  break;
                }


            }



        }


    }

}

void computeFollowSets(){
    bool has_change=true;
    addElement(followSet[program],TK_DOLLAR);
    while(has_change==true){

        has_change=false;
        for(int i=0;i<NUM_OF_RULES;i++){

            int lhs=grammar[i].lhs;
            rhsnode* head=grammar[i].head;
            rhsnode* temp=head;

            while(temp!=NULL){

                if((temp->sym).isTerminal==true){
                    temp=temp->next;
                }else{

                    set old_follow_set=(set)malloc(sizeof(int)*SET_SIZE);
                    set curr_follow_set=(set)malloc(sizeof(int)*SET_SIZE);

                    rhsnode* temp1=temp->next;

                    if(curr_follow_set==NULL){
                        perror("The new set can't be formed in first_set_of_remaining!!");
                        exit(1);
                    }

                    initializeSet(curr_follow_set);

                    while(temp1!=NULL){

                        if((temp1->sym).isTerminal==true){
                            addElement(curr_follow_set,(temp1->sym).t);
                            break;
                        }else{

                            takeUnion(curr_follow_set,curr_follow_set,firstSet[(temp1->sym).nt]);
                            if(temp1->next==NULL){
                                break;
                            }
                            else if(findElement(curr_follow_set,EPSILON)==false){
                                break;
                            }else{
                                removeElement(curr_follow_set,EPSILON);
                                temp1=temp1->next;
                            }

                        }

                    }

                    if(old_follow_set==NULL){
                        perror("old follow set is not formed!!");
                        exit(1);
                    }

                    for(int j=0;j<SET_SIZE;j++){
                        old_follow_set[j]=followSet[(temp->sym).nt][j];
                    }

                    if(findElement(curr_follow_set,EPSILON)==true){
                        removeElement(curr_follow_set,EPSILON);
                        takeUnion(curr_follow_set,curr_follow_set,followSet[lhs]);
                    }

                    if(temp->next==NULL){
                        takeUnion(curr_follow_set,curr_follow_set,followSet[lhs]);
                    }

                    takeUnion(followSet[(temp->sym).nt],followSet[(temp->sym).nt],curr_follow_set);

                    for(int j=0;j<SET_SIZE;j++){
                        if(old_follow_set[j]!=followSet[(temp->sym).nt][j]){
                            has_change=true;
                        }
                    }


                    free(old_follow_set);
                    temp=temp->next;
                }

            }

        }

    }

}

void generateParseTable() {
  for (int i = 0; i < NUM_OF_RULES; i++) {
	ntName nt = grammar[i].lhs;

	    rhsnode* temp=grammar[i].head;

    set curr_follow_set=(set)malloc(sizeof(int)*SET_SIZE);

    if(curr_follow_set==NULL){
        perror("The new set can't be formed in first_set_of_remaining!!");
        exit(1);
    }

    initializeSet(curr_follow_set);

    while(temp!=NULL){

        if((temp->sym).isTerminal==true){
            addElement(curr_follow_set,(temp->sym).t);
            break;
        }else{

            takeUnion(curr_follow_set,curr_follow_set,firstSet[(temp->sym).nt]);
            if(temp->next==NULL){
                break;
            }
            else if(findElement(curr_follow_set,EPSILON)==false){
                break;
            }else{
                removeElement(curr_follow_set,EPSILON);
                temp=temp->next;
            }

        }

    }

  for(int j=0;j<NUM_OF_TERMINALS;j++){
    if(findElement(curr_follow_set, j)==false)
      continue;
      parse_table[nt][j] = i;
  }
  }
}

nAryTreeNode* parseInputSourceCode(FILE* source){
  stack* main_stack = initializeStack();
  stack* aux_stack = initializeStack();
  TOKEN tkn;
  bool is_error = false;
  nAryTreeNode* root = createTreeNode();
  (root->sym).t = TK_DOLLAR;
  (root->sym).isTerminal = true;
  push(main_stack, root);
  nAryTreeNode* start = createTreeNode();
  (start->sym).nt = program;
  (start->sym).isTerminal = false;
  push(main_stack,start);

  set synSet=(set)malloc(sizeof(int)*SET_SIZE);
  initializeSet(synSet);
  addElement(synSet,TK_SEM);
  addElement(synSet,TK_END);
  addElement(synSet,TK_ENDRECORD);
  addElement(synSet,TK_ENDUNION);
  addElement(synSet,TK_ENDWHILE);
  addElement(synSet,TK_ENDIF);
  addElement(synSet,TK_ELSE);

  tkn = getNextToken(source);
  while(tkn.name == TK_COMMENT){
    tkn = getNextToken(source);
  }
  while(main_stack->head!=NULL){
      nAryTreeNode* stTop = (nAryTreeNode*)pop(main_stack);
      if(tkn.name == LEX_ERROR1){
        printf("\033[0;31m");
        printf("Line %d error: ", tkn.line_no);
        printf("\033[0m");
        printf("Unknown pattern <%s>\n",tkn.id);
        tkn = getNextToken(source);
        while(tkn.name == TK_COMMENT){
          tkn = getNextToken(source);
        }
        if (tkn.name == TK_DOLLAR) {
          return start;
        }
      }
      else if(tkn.name == LEX_ERROR2){
        printf("\033[0;31m");
        printf("Line %d Error: ", tkn.line_no);
        printf("\033[0m");
        printf("Variable Identifier is longer than the prescribed length of 20 characters.\n");
        tkn = getNextToken(source);
        while(tkn.name == TK_COMMENT){
          tkn = getNextToken(source);
        }
        if (tkn.name == TK_DOLLAR) {
          printf("\033[0;34m");
            printf("\nParsing Completed with errors\n");
            printf("\033[0m");
          return start;
        }
      }
      if((stTop->sym).isTerminal){
        if(tkn.name == TK_DOLLAR){
          if((stTop->sym).t == tkn.name){
            if(!is_error){
              printf("\033[0;32m");
              printf("\nInput source code is syntactically correct....\n");
              printf("\033[0m");
            }
            else{
              printf("\033[0;34m");
                printf("\nParsing Completed with errors\n");
                printf("\033[0m");
            }

            break;
          }
          else{
            is_error = true;
            printf("\033[0;31m");
            printf("Line %d Error:",tkn.line_no);
            printf("\033[0m");
            perror("Stack not empty but reached end of file\n");
            return start;
          }
        }
        if((stTop->sym).t == tkn.name){
          stTop->token.line_no = tkn.line_no;
          stTop->token.name = tkn.name;
          switch (tkn.name) {
            case TK_NUM: stTop->token.num = tkn.num;break;
            case TK_RNUM: stTop->token.rnum = tkn.rnum;break;
            default:strncpy(stTop->token.id, tkn.id, MAX_LEX_LEN);

          }
          tkn = getNextToken(source);
          while(tkn.name == TK_COMMENT){
            tkn = getNextToken(source);
          }
          if (tkn.name == TK_DOLLAR) {
            if(!is_error){
              printf("\033[0;32m");
              printf("\nInput source code is syntactically correct....\n");
              printf("\033[0m");
            }
            else{
              printf("\033[0;34m");
                printf("\nParsing Completed with errors\n");
                printf("\033[0m");
            }
            return start;
          }
          continue;
        }
        else{
          is_error = true;
          printf("\033[0;31m");
          printf("Line %d Error: ", tkn.line_no);
          printf("\033[0m");
          printf("The token %s for lexeme %s does not match with the expected token %s\n", keyToToken[tkn.name], tkn.id, keyToToken[stTop->sym.t]);
        }
      }
      else{
        int ruleNo = parse_table[(stTop->sym).nt][tkn.name];
        if(ruleNo == NO_MATCHING_RULE){
          if(findElement(firstSet[stTop->sym.nt],EPSILON)==false){
            is_error = true;
            printf("\033[0;31m");
            printf("Line %d Error: ", tkn.line_no);
            printf("\033[0m");
            printf("Invalid  token %s encountered with value %s stack top %s\n",keyToToken[tkn.name], tkn.id, keyToNT[stTop->sym.nt]);
          }


          while ((findElement(followSet[stTop->sym.nt], tkn.name) == false)&&(findElement(synSet,tkn.name)==false)) {
            is_error = true;
            printf("\033[0;31m");
            printf("Line %d Error: ", tkn.line_no);
            printf("\033[0m");
            if(tkn.name == LEX_ERROR1)
              printf("Unknown pattern <%s>\n",tkn.id);
            else if(tkn.name == LEX_ERROR2)
              printf("Variable Identifier is longer than the prescribed length of 20 characters.\n");
            else{
              printf("Invalid  token %s encountered with value ",keyToToken[tkn.name]);
              switch (tkn.name) {
                case TK_NUM: printf("%d ", tkn.num);break;
                case TK_RNUM: printf("%.2f ", tkn.rnum); break;
                default: printf("%s ", tkn.id);
              }
              printf("stack top %s\n", keyToNT[stTop->sym.nt]);
            }


            tkn = getNextToken(source);
            while(tkn.name == TK_COMMENT){
              tkn = getNextToken(source);
            }
        		if (tkn.name == TK_DOLLAR) {
              printf("\033[0;34m");
                printf("\nParsing Completed with errors\n");
                printf("\033[0m");
        		  return start;
        		}
      	  }
        }
        else{
          ruleRHS rule = grammar[ruleNo];
          rhsnode* temp = rule.head;

          while(temp!=NULL){
            nAryTreeNode* node = createTreeNode();
            (node->sym).isTerminal = (temp->sym).isTerminal;
            if((node->sym).isTerminal)
              (node->sym).t = (temp->sym).t;
            else
              (node->sym).nt = (temp->sym).nt;
            if((node->sym).isTerminal==false || (node->sym).t != EPSILON)
              push(aux_stack, node);
            addChild(stTop, node);
            temp=temp->next;
          }

          while(aux_stack->head!=NULL){
            nAryTreeNode* cur = pop(aux_stack);
            push(main_stack, cur);
          }

        }
      }
  }
  return start;
}

void printParseTree(nAryTreeNode* root, FILE* op){
  if(root==NULL) return;
  // printf("yes\n");
  // if(root->sym.isTerminal){
  //   printf("%s\n", keyToToken[root->sym.t]);
  // }
  // else
  //   printf("%s\n", keyToNT[root->sym.nt]);
  printParseTree(root->leftMostChild, op);
  // if(root->sym.is_terminal)

  char buf[MAX_LEX_LEN];
  if(root->sym.isTerminal){
    switch(root->token.name){
      case TK_NUM: fprintf(stdout, "%-30d ", root->token.num);break;
      case TK_RNUM: fprintf(stdout, "%-30.2f ", root->token.rnum);break;
      default: fprintf(stdout, "%-30s ",root->token.id);
    }
  }
  else{
    fprintf(stdout,"-----                          ");
  }
  if(root->sym.isTerminal)
    fprintf(stdout, "%-30d ", root->token.line_no);
  else
    fprintf(stdout, "-                             ");
  if(root->sym.isTerminal)
    fprintf(stdout, "%-30s ", keyToToken[root->sym.t]);
  else{
    fprintf(stdout, "-----                         ");
  }

  if(root->sym.isTerminal){
    switch(root->token.name){
      case TK_NUM: fprintf(stdout, "%-30d ", root->token.num);break;
      case TK_RNUM: fprintf(stdout, "%-30.2f ", root->token.rnum);break;
      default: fprintf(stdout, "-----                         ");
    }
  }
  else{
    fprintf(stdout, "-----                        ");
  }

  if(root->sym.isTerminal){
    fprintf(stdout, "%-30s ",keyToToken[root->sym.t]);
  }
  else{
    if(root->sym.nt == program)
      fprintf(stdout, "ROOT                         ");
    else
      fprintf(stdout, "%-30s ", keyToNT[root->parent->sym.nt]);
  }

  if(root->numOfChildren==0){
    fprintf(stdout, "yes                            ");
  }
  else
    fprintf(stdout, "no                             ");

  if(root->sym.isTerminal)
    fprintf(stdout, "%-30s\n", keyToToken[root->sym.t]);
  else{
    fprintf(stdout, "%-30s\n",keyToNT[root->sym.nt]);
  }
  nAryTreeNode* temp = root->leftMostChild;
  while(temp!=NULL){
    temp=temp->brother;
    printParseTree(temp, op);
  }
}

void printParseTreePreorder(char* prefix, nAryTreeNode *root, bool isLeft) {
	if(root==NULL) return;
  printf("%s",prefix);
  if(isLeft){
    printf("|---");
  }
  else{
    printf("'---");
  }
  char node[100];
  if((root->sym).isTerminal){
    strcpy(node, keyToToken[(root->sym).t]);
  }
  else{
    strcpy(node, keyToNT[(root->sym).nt]);
  }
  if((root->sym).isTerminal)
  printf("\033[0;33m");
  printf("%s\n",node);
  if((root->sym).isTerminal)
  printf("\033[0m");
  char suf1[] = "|   ";
  char suf2[] = "    ";
  nAryTreeNode* cur = root->leftMostChild;
  for(int i=0;i<root->numOfChildren;i++){
    char pref [100];
    strcpy(pref, prefix);
    if(isLeft)
      strcat(pref, suf1);
    else strcat(pref, suf2);
    if(i==0){
      printParseTreePreorder(pref, root->leftMostChild, true);
    }
    else{
      if(i==root->numOfChildren-1)
        printParseTreePreorder(pref, cur, false);
      else
        printParseTreePreorder(pref, cur, true);
    }
    cur = cur->brother;
  }
}
void printParseTable() {
  for (int i = 0; i < NUM_OF_NONTERMINALS; i++) {
	for (int j = 0; j < NUM_OF_TERMINALS; j++) {
	  if (parse_table[i][j] != NO_MATCHING_RULE) {
		printf("parse_table[%s][%s] : ", keyToNT[i], keyToToken[j]);
		printRule(parse_table[i][j]);
	  }
	}
  }
}

void freeGrammar()
{
    int i;
    for (i = 0; i < NUM_OF_RULES; i++)
    {
        rhsnode* temp = grammar[i].head;
        rhsnode* prev;
        while (temp != NULL)
        {
            prev = temp->next;
            free(temp);
            temp = prev;
        }
    }
}

void printGrammar()
{
    for (int i = 0; i < NUM_OF_RULES; i++)
    {
        int lhs = grammar[i].lhs;
        rhsnode* head = grammar[i].head;
        printf("%s -> ", keyToNT[lhs]);
        while (head != NULL)
        {
            printSymbol(head->sym);
            printf(" ");
            head = head->next;
        }
        printf("\n");
    }
}

void printRule(int i){
  int lhs = grammar[i].lhs;
  rhsnode* head = grammar[i].head;
  printf("%s -> ", keyToNT[lhs]);
  while (head != NULL)
  {
      printSymbol(head->sym);
      printf(" ");
      head = head->next;
  }
  printf("\n");
}

void printSymbol(symbol sym)
{
    if (sym.isTerminal != true)
    {
        printf("%s", keyToNT[sym.nt]);
    }
    else
    {
        printf("%s", keyToToken[sym.t]);
    }
}

void printFirstSets(){
  FILE * fp = fopen("first.txt", "w");
  if(fp==NULL){
    perror("fopen error !!");
    return;
  }

    for(int i=0;i<NUM_OF_NONTERMINALS;i++){

        fprintf(fp, "%s => {",keyToNT[i]);

        for(int j=0;j<SET_SIZE;j++){
            if(findElement(firstSet[i],j)==true){
                fprintf(fp, "%s ,  ",keyToToken[j]);
            }
        }

        fprintf(fp,"  }\n");

    }
    fclose(fp);
}

void printFollowSets(){

  FILE * fp = fopen("follow.txt", "w");
  if(fp==NULL){
    perror("fopen error !!");
    return;
  }
    for(int i=0;i<NUM_OF_NONTERMINALS;i++){

        fprintf(fp,"follow set of (%s)=>{",keyToNT[i]);

        for(int j=0;j<SET_SIZE;j++){
            if(findElement(followSet[i],j)==true){
                fprintf(fp,"%s,",keyToToken[j]);
            }
        }

         fprintf(fp,"}\n");

    }
    fclose(fp);
}
