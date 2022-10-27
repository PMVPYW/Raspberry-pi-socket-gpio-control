#include "misc.h"

void printBin(int n) {
    int size = sizeof(int) * 8;
    int aux = 1;
    int e;
    for (int i = size-1; i >= 0; i--) {
        e = n & (aux << i);
        if (e != 0) {
            e = 1;
        }
        printf("%d", e);
    }
}

