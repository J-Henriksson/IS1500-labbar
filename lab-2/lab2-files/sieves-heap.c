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

static void print_number(int prime) {
    static int count = 0;
    if (count % COLUMNS == 0) printf("\n");
    printf("%10d ", prime);
    count++;
}

static void print_sieves(int n) {
    if (n < 2) { printf("\n"); return; }

    //memory: (n+1) bytes on the heap
    unsigned char *mark = (unsigned char*)malloc((size_t)(n + 1));
    if (!mark) { perror("malloc"); exit(1); }
    

    mark[0] = 0; mark[1] = 0;
    for (int i = 2; i <= n; i++) mark[i] = 1;

    int limit = (int)floor(sqrt((double)n));
    for (int p = 2; p <= limit; p++) {
        if (mark[p]) {
            long long start = (long long)p * (long long)p;
            for (long long m = start; m <= n; m += p) {
                mark[m] = 0;
            }
        }
    }

    for (int i = 2; i <= n; i++) {
        if (mark[i]) print_number(i);
    }
    printf("\n");

    free(mark);  // don’t forget
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        print_sieves(atoi(argv[1]));
    } else {
        printf("Please state an integer number.\n");
    }
    return 0;
}
