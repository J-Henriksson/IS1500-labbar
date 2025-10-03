/*
 pointers.c
 By Joel Henriksson
 Last modified: 2015-09-19
 This file is in the public domain.
*/

#include <stdio.h>
#include <stdlib.h>

//template
char* text1 = "This is a string.";
char* text2 = "Yet another thing.";


int list1[20] = {0};
int list2[20] = {0};
int counter = 0;


void copycodes(const char *text, int *list, int *counter) {
    const unsigned char *p = (const unsigned char *)text; // read bytes
    int *q = list;

    while (*p != 0) {          
        *q = (int)(*p);        
        q++;                   
        p++;                   
        (*counter)++;          
    }
    *q = 0;                    
}



void work(void) {
  copycodes(text1, list1, &counter);
  copycodes(text2, list2, &counter);
}


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

int main(void){
  work();
  printf("\nlist1: ");
  printlist(list1);
  printf("\nlist2: ");
  printlist(list2);
  printf("\nCount = %d\n", counter);
  endian_proof((char*) &counter);
}

