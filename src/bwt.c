#include "bwt.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct String {
    char* string;
    size_t n;
    int alphabetSize;
    int* sightings;
};

struct String* prepareString(const char* string, size_t n) {
    struct String* x = malloc(sizeof *x);
    x->n = n;
    x->alphabetSize = 5; //Cheating for now, assume dna

    x->sightings = malloc(x->alphabetSize*sizeof (*(x->sightings)));
    x->sightings[0] = 1;

    x->string = malloc(n*sizeof(*x->string));
    strcpy(x->string, string);

    for(int i=0; i<x->alphabetSize; i++) {
        x->sightings[i] = 0;
    }
    char currChar;
    for(int i=0; i<=n; i++) {
        currChar = x->string[i];
        x->string[i] =
                currChar == '\0' ? 0 :
                currChar == 'a' ? 1 :
                currChar == 'c' ? 2 :
                currChar == 'g' ? 3 :
                currChar == 't' ? 4 :
                currChar;
        (x->sightings[x->string[i]])++;
    }
    return x;
}

int *constructSARadix(struct String string) {
    char* x = string.string;
    int n = string.n;
    int alphabetSize = string.alphabetSize;
    int* sightings = string.sightings;

    int *sa = malloc(n * sizeof *sa);
    int *saCopy = malloc(n * sizeof *sa);

    for (int i = 0; i<n; i++) {
        sa[i] = i;
    }
    int *bucketsIndices = sightings;
    int accumSum = 0;
    for(int i=0; i<alphabetSize; i++) {
        int sighting = bucketsIndices[i];
        bucketsIndices[i] = accumSum;
        accumSum += sighting;
    }
    int *buckets = malloc(alphabetSize*sizeof *buckets);

    for(int i=n-1; i>=0; i--) {
        memset(buckets, 0, alphabetSize * sizeof *buckets);
        for(int j=0; j<n; j++) {
            int charIndex = (sa[j] + i) % n;
            char c = x[charIndex];
            int elemInBucket = buckets[c]++;
            int saIndex = bucketsIndices[c] + elemInBucket;
            saCopy[saIndex] = sa[j];
        }
        int *temp = sa;
        sa = saCopy;
        saCopy = temp;
    }

    free(saCopy);
    free(buckets);

    return sa;
}


void bwt(size_t n, const char x[n], char y[n])
{
    struct String* preparedX = prepareString(x, n);
    int* sa = constructSARadix(*preparedX);
    for(int i=0; i<(preparedX->n)+1; i++) {
        y[i] = sa[i] ? sa[i]-1: 0;
    }
    free(preparedX->string);
    free(preparedX->sightings);
    free(preparedX);
    free(sa);
}

int** makeO(struct String* string) {
    char currChar;
    int aSize = string->alphabetSize;
    int** O = malloc(aSize*sizeof(int*)); //TODO what is wrong with this line? SIGTRAP
    for(int i=1; i<aSize; i++) {
        int* column = calloc(1+string->n, sizeof(int));
        for(int j=1; j<=string->n; j++) {
            currChar = string->string[j-1];
            column[j] = column[j-1] + (currChar==i);
        }
        O[i] = column;
    }
    return O;
}

void rbwt(size_t n, const char y[n], char x[n])
{
    struct String* preparedY = prepareString(y, n);
    int** O = makeO(preparedY);
    int* count = calloc(preparedY->alphabetSize, sizeof(int));

    x[n] = '\0';
    int currentIndex = 0;
    char currChar;
    for(int i=n-1; i>=0; i--) {
        currChar = preparedY->string[currentIndex];
        x[i] = currChar;
        currentIndex = O[currChar][currentIndex] + count[currChar]++;
    }
}
