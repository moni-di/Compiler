/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

stack* initializeStack(){
    stack* stack_ptr = (stack*) malloc(sizeof(stack));
    if(stack_ptr == NULL){
        perror("stack init error.. memory not available\n");
    }
    stack_ptr->head = NULL;
    stack_ptr->tail = NULL;
    return stack_ptr;
}

void push(stack* stack_ptr, void* node){
    struct stackNode* sn = (struct stackNode*) malloc(sizeof(struct stackNode));
    if(stack_ptr->head == NULL){
        stack_ptr->head = sn;
        sn->prev = NULL;
        sn->next = NULL;
        sn->node = node;
        stack_ptr->tail = sn;
    }
    else{
        stack_ptr->tail->next = sn;
        sn->prev = stack_ptr->tail;
        sn->next = NULL;
        sn->node = node;
        stack_ptr->tail = sn;
    }
}

void* pop(stack* stack_ptr){
    if(stack_ptr->head == NULL){
        return NULL;
    }
    if(stack_ptr->head->next == NULL){
        struct stackNode* sn = stack_ptr->head;
        stack_ptr->head = NULL;
        stack_ptr->tail = NULL;
        void* top = sn->node;
        free(sn);
        return top;
    }
    else{
        struct stackNode* sn = stack_ptr->tail;
        sn->prev->next = NULL;
        stack_ptr->tail = sn->prev;
        void* top = sn->node;
        free(sn);
        return top;
    }
}

void* top(stack* stack_ptr){
    if(stack_ptr->head == NULL){
        return NULL;
    }
    else{
        return stack_ptr->tail->node;
    }
}
