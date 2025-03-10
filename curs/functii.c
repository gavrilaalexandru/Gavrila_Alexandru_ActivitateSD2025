#include <stdio.h>

double sumaValoare(float x, float y) {
    double s;
    s = x + y;
    return s;
}

void sumaAdrese(float x, float y, double* z) {
    *z = sumaValoare(x, y);
}

int main() {

    float a = 1.2, b = 4.7;
    double c;
    
    sumaAdrese(a, b, &c);
    printf("\nSuma apel functie parametri transfer prin valoare si adresa: %.1f\n", c);
    return 0;
}