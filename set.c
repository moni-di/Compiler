/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#include "set.h"

void initializeSet(set s){
    for(int i=0;i<SET_SIZE;i++){
        s[i] = 0;
    }
}

bool findElement(set s, int num){
    return s[num] == 1;
}

void addElement(set s, int num){
    s[num] = 1;
}

void removeElement(set s, int num){
    s[num] = 0;
}

void takeUnion(set dest, set first, set second){
    for(int i=0;i<SET_SIZE;i++){
        dest[i] = first[i] | second[i];
    }
}
