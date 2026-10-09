#include <stdio.h>

int main(void) {
    int x;

    printf("Entrada:\n");

    // Rejeita texto (ex: "abc") e pede o valor outra vez até ser um número inteiro
    while (scanf("%d", &x) != 1) {
        scanf("%*s"); // Descarta o texto inválido
    }

    // Cálculo da temperatura
    double temp = (260.0 * x) / 1023.0 - 20.0;

    // Apresentação do resultado
    printf("\nSaída:\n");
    if (temp >= -10.0 && temp <= 190.0) {
        printf("%.2f\n", temp);
    } else {
        printf("Valor fora da gama\n");
    }

    return 0;
}
