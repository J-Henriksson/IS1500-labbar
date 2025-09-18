/*
 prime.c
 By David Broman.
 edited by Viktor Eriksson 
 Last modified: 2025-09-16
 This file is in the public domain.
*/


#include <stdio.h>

int is_prime(int n){
  if (n < 2) return 0;        //no prime < 2
  if ( n == 2) return 1;      //special case 
  if (n % 2 == 0) return 0;   //check if even

  for (int i = 3; i*i <= n; i += 2) {  //check all odd numbers up to sqrt(n)
    if (n%i == 0) { //not prime if divisible with i
      return 0;
    }
  }
  return 1;
}
int main(void){
  printf("%d\n", is_prime(11));  // 11 is a prime.      Should print 1.
  printf("%d\n", is_prime(383)); // 383 is a prime.     Should print 1.
  printf("%d\n", is_prime(987)); // 987 is not a prime. Should print 0.
}
