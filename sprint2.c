#include <stdio.h>

int main(void) {
    int x;

    // 1. Rejeita texto (ex: "abc") e fica a aguardar um inteiro válido
    while (scanf("%d", &x) != 1) {
        scanf("%*s"); // Descarta a palavra inválida
    }

    // 2. Cálculo da temperatura (fórmula oficial do Sprint 1)
    double temp = (260.0 * x) / 1023.0 - 20.0;

    // 3. Verificação da gama válida [-10.0, 190.0]
    if (temp >= -10.0 && temp <= 190.0) {
        printf("%.2f\n", temp);
    } else {
        printf("Valor fora da gama\n");
    }

    return 0;
}
