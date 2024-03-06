//Page 48

#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include "util.h"


int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: stencil m n\n");
        exit(1);
    }

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);

    double (*a)[n+2];
    double (*b)[n+2];
    double (*c)[n+2];
    double (*aa)[n+2];
    double (*bb)[n+2];

    a = (double(*)[n+2]) malloc((m+2)*(n+2) * sizeof(double)); aa = a;
    a = (double(*)[n+2])((char*)a+(n+2+1) * sizeof(double));

    b = (double(*)[n+2]) malloc((m+2)*(n+2) * sizeof(double)); bb = b;
    b = (double(*)[n+2])((char*)b+(n+2+1) * sizeof(double));


    printf("Matrix before Average\n");
    
    zeroMatrix(m+2, n+2, aa);
    fillRandomMatrix(m, n, a);
    printMatrix(m+2, n+2, aa);
    printf("\n");
    
    zeroMatrix(m+2, n+2, bb);
    


    int done = 0;
    int i,j;
    while(!done) {
        for (i=0; i<m; i++) {
            for (j=0; j<n; j++) {
                b[i][j] = (a[i][j]+a[i-1][j]+a[i+1][j]+a[i][j-1]+a[i][j+1])/5;
            }
        }
        
        c = a; a = b; b = c; // swap matrices a and b

        done = 1; //or some other condition
    }

    printf("--- AFTER ---\n");
    printMatrix(m+2, n+2, bb);

    free(aa); free(bb);

}