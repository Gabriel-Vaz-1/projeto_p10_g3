#include <stdio.h>

int main(void) {
    int x;

    while (1) {
        // 1. Rejeita texto (ex: "abc") e fica a aguardar um inteiro válido
        while (scanf("%d", &x) != 1) {
            scanf("%*s"); // Descarta o texto inválido
        }

        // 2. Sentinela: se for -1, termina o programa sem imprimir nada
        if (x == -1) {
            break;
        }

        // 3. Cálculo da temperatura (fórmula oficial do Sprint 1)
        double temp = (260.0 * x) / 1023.0 - 20.0;

        // 4. Apresentação direta do resultado
        if (temp >= -10.0 && temp <= 190.0) {
            printf("%.2f\n", temp);
        } else {
            printf("Valor fora da gama\n");
        }
    }

    return 0;
}
