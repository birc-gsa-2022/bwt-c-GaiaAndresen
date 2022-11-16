#include "bwt.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>


int *constructSARadix(const char* string, int n) {
    int* saReader = malloc(n * sizeof *saReader);
    int* saWriter = malloc(n * sizeof *saWriter);

    int alphabetSize = 5; //TODO Remove assumption of dna
    int* buckets = malloc(alphabetSize*sizeof *buckets);
    int* bucketsIndices = calloc(alphabetSize, sizeof *bucketsIndices);
    int* stringConvertion = malloc(n*sizeof *stringConvertion);

    for (int i = 0; i<n; i++) {
        saReader[i] = i;

        char stringChar = string[i];
        int bucketIndex =
                //TODO Remove assumption of dna
                stringChar == 't' ? 4:
                stringChar == 'g' ? 3:
                stringChar == 'c' ? 2:
                stringChar == 'a'; //'a' => 1, '\0' => 0
        stringConvertion[i] = bucketIndex;
        bucketsIndices[bucketIndex]++;
    }

    int accumSum = 0;
    for(int i=0; i<alphabetSize; i++) {
        int sighting = bucketsIndices[i];
        bucketsIndices[i] = accumSum;
        accumSum += sighting;
    }

    for(int i=n-1; i>=0; i--) {
        memset(buckets, 0, alphabetSize * sizeof *buckets);
        for(int j=0; j<n; j++) {
            int charIndex = (saReader[j] + i) % n;
            int c = stringConvertion[charIndex];
            int elemInBucket = buckets[c]++;
            int saIndex = bucketsIndices[c] + elemInBucket;
            saWriter[saIndex] = saReader[j];
        }
        int* temp = saReader;
        saReader = saWriter;
        saWriter = temp;
    }

    free(saWriter);
    free(buckets);
    free(bucketsIndices);
    free(stringConvertion);

    return saReader;
}


void bwt(size_t n, const char x[n], char y[n])
{
    int* sa = constructSARadix(x, (int)n);
    for(int i=0; i<n; i++) {
        y[i] = sa[i] ? x[sa[i]-1] : '\0'; //Why is x mad about giving a char?
    }
    free(sa);
}

void makeOandC(const char* string, size_t n, int** O, int* C, int alphabetSize) {
    int* convertedString = malloc(n*sizeof *convertedString);

    for (int i = 0; i<n; i++) {
        char stringChar = string[i];
        int convertedChar =
                //TODO Remove assumption of dna
                stringChar == 't' ? 4:
                stringChar == 'g' ? 3:
                stringChar == 'c' ? 2:
                stringChar == 'a'; //'a' => 1, '\0' => 0
        convertedString[i] = convertedChar;
        C[convertedChar]++;
    }

    int accum = 0;
    for(int i=0; i<alphabetSize; i++) {
        int val = C[i];
        C[i] = accum;
        accum += val;
    }

    int currChar;
    int* firstColumn = calloc((1+n), sizeof *firstColumn);
    O[0] = firstColumn;
    for(int i=1; i<alphabetSize; i++) {
        int* column = malloc((1+n)* sizeof *column);
        column[0] = 0;
        for(int j=1; j<=n; j++) {
            currChar = convertedString[j-1];
            column[j] = column[j-1] + (currChar==i);
        }
        O[i] = column;
    }

    free(convertedString);
}

void rbwt(size_t n, const char y[n], char x[n])
{
    int alphabetSize = 5; //TODO Remove assumption of dna

    int* C = calloc(alphabetSize,sizeof(*C));
    int** O = malloc(alphabetSize*sizeof *O);

    makeOandC(y, n, O, C, alphabetSize);

    x[n-1] = '\0';
    int currentIndex = 0;
    char currChar;
    int OChar;
    for(int i=(int)n-2; i>=0; i--) {
        currChar = y[currentIndex];
        x[i] = currChar;
        //TODO Remove assumption of dna
        OChar = currChar == 't' ? 4:
                currChar == 'g' ? 3:
                currChar == 'c' ? 2:
                currChar == 'a';
        currentIndex = O[OChar][currentIndex] + C[OChar];
    }

    free(C);
    for(int i=0; i<alphabetSize; i++) {
        free(O[i]);
    }
    free(O);
}
