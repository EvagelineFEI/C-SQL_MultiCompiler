#include <stdio.h>
#include <math.h>

int main() {
    int i, a, b, c, sum;
    printf("三位数的水仙花数有：\n");
    for (i = 100; i < 1000; i++) {
        a = i / 100;
        b = (i % 100) / 10;
        c = i % 10;
        sum = pow(a, 3) + pow(b, 3) + pow(c, 3);
        if (sum == i) {
            printf("%d\n", i);
        }
    }
    return 0;
}