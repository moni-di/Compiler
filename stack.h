/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#ifndef STACKADTDEF_H
#define STACKADTDEF_H

struct stackNode{
    void* node;
    struct stackNode* next;
    struct stackNode* prev;
};

typedef struct{
    struct stackNode* head;
    struct stackNode* tail;
} stack;

stack* initializeStack();
void push(stack* stack_ptr, void* node);
void* top(stack* stack_ptr);
void* pop(stack* stack_ptr);

#endif
