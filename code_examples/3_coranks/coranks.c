#include <stdlib.h>
#include <stdio.h>
#include "util.h"

void corank(int n, double A[n], int m, double B[m], int i)
{
    int j, d, k;
    int jlow, klow, done;

    j = min(i, m);
    k = i - j;
    jlow = max(0, i - n);
    klow = 0;
    done = 0;
    do
    {
        if (j > 0 && k < n && A[j - 1] > B[k])
        {
            d = (1 + j - jlow) / 2;
            klow = k;
            j -= d;
            k += d;
        }
        else if (k > 0 && j < m && B[k - 1] >= A[j])
        {
            d = (1 + k - klow) / 2;
            jlow = j;
            k -= d;
            j += d;
        }
        else
            done = 1;
    } while (!done);

    printf("%i %i\n", j, k);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: corank n\n");
        exit(1);
    }

    int n = atoi(argv[1]);

    double a[n];
    double b[n];

    for(int i = 0; i<n; i++) a[i] = i*4;
    for(int i = 0; i<n; i++) b[i] = i*2+1;

    printArray(n, a);
    printArray(n, b);

    corank(n, a, n, b, 5);
}