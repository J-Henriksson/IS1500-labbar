/*
 sieves.c
 By Joel Henriksson.
 Last modified: 2025-09-18
 This file is in the public domain.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define COLUMNS 6

void print_number(int prime) {
    static int count = 0;  // keeps track across calls of when to move down a row

    if (count % COLUMNS == 0) {
        printf("\n");
    }

    printf("%10d ", prime);
    count++;
}

// Sieve on stack: uses a char array of (n+1) bytes 0/1 either prime or not prime
static void print_sieves(int n) {
    if (n < 2) { printf("\n"); return; }

   
    unsigned char mark[n + 1];   
    // --------------------------------------------

    //Initialize starting values
    mark[0] = 0; mark[1] = 0;
    //Set all other values to 1
    for (int i = 2; i <= n; i++) mark[i] = 1;

    // sieve algorithm 
    int limit = (int)floor(sqrt((double)n));
    for (int p = 2; p <= limit; p++) {
        if (mark[p]) {
            // start at p*p to avoid re-marking same position
            long long start = (long long)p * (long long)p;
            for (long long m = start; m <= n; m += p) {
                mark[m] = 0;
            }
        }
    }

    // print primes
    for (int i = 2; i <= n; i++) {
        if (mark[i]) print_number(i);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        print_sieves(atoi(argv[1]));
    } else {
        printf("Please state an integer number.\n");
    }
    return 0;
}
