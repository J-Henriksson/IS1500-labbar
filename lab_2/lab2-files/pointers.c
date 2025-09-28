/*
 pointers.c
 By David Broman.
 Edited by Viktor Eriksson
 Last modified: 2025-09-17
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>

char* text1 = "This is a string.";
char* text2 = "Yet another thing.";

int counter = 0;
int  list1[20];
int  list2[20];


void printlist(const int* lst){
  printf("ASCII codes and corresponding characters.\n");
  while(*lst != 0){
    printf("0x%03X '%c' ", *lst, (char)*lst);
    lst++;
  }
  printf("\n");
}

void endian_proof(const char* c){
  printf("\nEndian experiment: 0x%02x,0x%02x,0x%02x,0x%02x\n", 
         (int)*c,(int)*(c+1), (int)*(c+2), (int)*(c+3));
  
}

void copycodes(char* text, int* list, int* counter) {
  while ((*text) != 0) {
    (*list) = (int) (*text);  
    
    text++;   
    list++;
    (*counter)++;
  }
}

void work() {
  copycodes(text1, list1, &counter);
  copycodes(text2, list2, &counter);
}

int main(void){
 
    work();
    printf("\nlist1: ");
    printlist(list1);
    printf("\nlist2: ");
    printlist(list2);
    printf("\nCount = %d\n", counter);

    endian_proof((char*) &counter);
}
