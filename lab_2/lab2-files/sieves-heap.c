/*
sieves.c
By Viktor Eriksson.
Last modified: 2025-09-16
*/

#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6
int primes_in_column = 0;

void print_number(int n) {
  if (primes_in_column == COLUMNS) {    //insert newline if at last column
    printf("\n");
    primes_in_column = 0;   //reset column counter
  } 
  printf("%10d ", n);       //print number
  primes_in_column++;
}

void print_sieves(int n) {
    char *is_prime = (char *) malloc(n+1);

    is_prime[0] = 0;    // 0 not prime
    is_prime[1] = 0;    // 1 not prime
    for (int i = 2; i <= n; i++) // initilize all other elements as 1
    {
        is_prime[i] = 1;
    }
    
    int i = 2;
    for (int i = 2; i*i <= n; i++)  //loop from 2 to sqrt(n)
    {

        if (is_prime[i] == 1) {     //if prime, mark multiples as not prime
            int j = i*i;
            while (j <= n) {
                is_prime[j] = 0;
                j += i;
            }
        }
    }

    int last_prime = 0;
    int distance_4_count = 0;
    for (int i = 0; i <= n; i++) {  //print numbers stil marked as prime
        if (is_prime[i]) {
            print_number(i);
            
            if (i - last_prime == 4) distance_4_count++; 
            last_prime = i;
        }
    }
    printf("\nnumber of distance 4: %d\n", distance_4_count);
    free(is_prime);
}

int main(int argc, char *argv[]){
    if(argc == 2)
    {
        print_sieves(atoi(argv[1]));
    }
  else
    printf("Please state an integer number.\n");
  return 0;
}
