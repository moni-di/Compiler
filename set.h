/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#ifndef SETADTDEF_H
#define SETADTDEF_H

#include <stdbool.h>

#define SET_SIZE 100

typedef int* set;


void initializeSet(set s);
bool findElement(set s, int num);
void addElement(set s, int num);
void removeElement(set s, int num);
void takeUnion(set dest, set first, set second);

#endif
