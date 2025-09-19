/*
 prime.c
 By Joel Henriksson.
 Last modified: 2025-09-16
 This file is in the public domain.
*/


#include <stdio.h>

int is_prime(int n){
  //base cases
  if (n <= 1) return 0;        
  if (n <= 3) return 1;        
  if (n % 2 == 0) return 0;

  //loop through all values of i up until sqrt(n)
  int i = 3;
  while (i <= n / i) {         
    if (n % i == 0) return 0;
    i += 2;                 
    }
    return 1;
}
int main(void){
  //Tests
  printf("%d\n", is_prime(11));  
  printf("%d\n", is_prime(383)); 
  printf("%d\n", is_prime(987)); 
}
