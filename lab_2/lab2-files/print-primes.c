/*
 print-primes.c
 By David Broman.
 Edited by Viktor Eriksson
 Last modified: 2025-09-16
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6
int primes_in_column = 0;

void print_number(int n) {
  if (primes_in_column == COLUMNS) {  //insert newline if at last column
    printf("\n");
    primes_in_column = 0; //reset column counter
  } 
  printf("%10d ", n);
  primes_in_column++;
}

int is_prime(int n){
  if (n < 2) return 0;        //no prime < 2
  if ( n == 2) return 1;      //special case 
  if (n % 2 == 0) return 0;   //check if even

  for (int i = 3; i*i <= n; i += 2) {  //check all odd numbers up to sqrt(n)
    if (n%i == 0) {
      return 0;
    }
  }
  return 1;
}

void print_primes(int n){
    for (int i = 2; i <= n; i++) {  //loop through all numbers up to n, and print if prime
      if (is_prime(i))
      {
        print_number(i);
      }
    } 
    printf("\n"); //new line for readability
}



// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
    if(argc == 2)
    {
        print_primes(atoi(argv[1]));
    }
  else
    printf("Please state an integer number.\n");
  return 0;
}

 
