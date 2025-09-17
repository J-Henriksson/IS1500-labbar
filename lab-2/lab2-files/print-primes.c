/*
 print-primes.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6

int is_prime(int n) {
  if (n <= 1) return 0;        
  if (n <= 3) return 1;        
  if (n % 2 == 0) return 0;

  int i = 3;
  while (i <= n / i) {         
    if (n % i == 0) return 0;
    i += 2;                 
    }
    return 1;
}


void print_number(int value) {
    static int count = 0;  // keeps track across calls

    printf("%d ", value);
    count++;

    if (count % COLUMNS == 0) {
        printf("\n");
    }
}

void print_primes(int n){
    for (int i = 2; i <= n; i++) {
        if (is_prime(i)) {
            print_number(i);
        }
    }
    printf("\n");
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

 
