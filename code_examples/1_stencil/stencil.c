#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

void printMatrix(int rows, int cols, double matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%f\t", matrix[i][j]);
        }
        printf("\n");
    }
}

void fillRandomMatrix(int n, int m, double matrix[n][m]) {
    // Seed the random number generator with current time
    srand(42);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            // Generate random value between 0 and 10

            // The halo
            if(i == 0 || j == 0 || i == n-1 || j == m-1) {
                matrix[i][j] = 0;
                continue;
            }

            matrix[i][j] = (rand() / (double)RAND_MAX) * 10.0;
        }
    }
}


int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: stencil n m");
        exit(1);
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    double (*a)[n+2];
    double (*b)[n+2];
    double (*c)[n+2];
    double (*aa)[n+2];
    double (*bb)[n+2];

    a = (double(*)[n+2]) malloc((m+2)*(n+2) * sizeof(double)); aa = a;
    a = (double(*)[n+2])((char*)a+(n+2+1) * sizeof(double));

    b = (double(*)[n+2]) malloc((m+2)*(n+2) * sizeof(double)); bb = b;
    b = (double(*)[n+2])((char*)b+(n+2+1) * sizeof(double));


    printf("--- BEFORE ---\n");
    fillRandomMatrix(n+2, m+2, a);
    printMatrix(n+2, m+2, a);
    printf("\n");

    int done = 0;
    while(!done) {
        for (int i=0; i<m; i++) {
            for (int j=0; j<n; j++) {
                b[i][j] = (a[i][j-1] + a[i+1][j-1] + a[i+1][j] + a[i+1][j+1] + a[i][j+1] + a[i-1][j+1] + a[i-1][j] + a[i-1][j-1] + a[i][j])/9;
            }
        }
        
        c = a; a = b; b = c; // swap matrices a and b

        done = 1; //or some other condition
    }

    printf("--- AFTER ---\n");
    printMatrix(n+2, m+2, a);

    free(aa); free(bb);

}