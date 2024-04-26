#include <stdio.h>
typedef struct complex {
    float real;
    float imag;
} complex;

complex add(complex n1, complex n2);

int main() {
    complex n1, n2, result;

    n1.real = 44.4444f;
    n1.imag = 5.0f;
    n2.real = 6.66666f;
    n2.imag = 8.0f;

    result = add(n1, n2);

    printf("Sum = %.1f + %.1fi", result.real, result.imag);
    return 0;
}

complex add(complex n1, complex n2) {
    complex temp;
    temp.real = n1.real + n2.real;
    temp.imag = n1.imag + n2.imag;
    return (temp);
}
