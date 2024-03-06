//Page 88

//REMARK Why do all the matrices contain a??? 

#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include "util.h"


int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Usage: matrix_matrix m l n\n");
        exit(1);
    }

    int m = atoi(argv[1]);
    int l = atoi(argv[2]);
    int n = atoi(argv[3]);

    double (*a)[l] = (double(*)[l]) malloc(m * l * sizeof(double)); // for indexing in mxl matrix
    double (*b)[n] = (double(*)[n]) malloc(l * n * sizeof(double)); // for indexing in lxn matrix
    double (*c)[n] = (double(*)[n]) malloc(m * n * sizeof(double)); // for indexing in mxn matrix


    printf("Matrix a\n");
    
    zeroMatrix(m, l, a);
    fillMatrixRandom(m, l,  a);
    printMatrix(m, l,  a);
    printf("\n");
    
    printf("Matrix b\n");
    zeroMatrix(l, n, b);
    fillMatrixRandom(l, n, b);
    printMatrix(l, n, b);
    printf("\n");


    int i,j, k;
    for (i=0; i<m; i++) {
        for (j=0; j<n; j++) {
            c[i][j] = 0;
            for(k=0; k<l; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
            //printf("i: %i j: %i v: %f\n", i, j, c[i][j]);
        }
    }
    


    printf("Matrix c\n");
    printMatrix(m, n, c);
    printf("\n");

    free(a); free(b); free(c);
}