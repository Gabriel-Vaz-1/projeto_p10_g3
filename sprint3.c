#include <stdio.h>

int main(void) {
    int x;

    printf("Entrada:\n");

    while (1) {
        // 1. Rejeita texto (ex: "abc") e pede o valor outra vez até ser um número inteiro
        while (scanf("%d", &x) != 1) {
            scanf("%*s"); // Descarta o texto inválido
        }

        // 2. Sentinela: se o valor for -1, termina a execução sem imprimir saída
        if (x == -1) {
            break;
        }

        // 3. Cálculo da temperatura
        double temp = (260.0 * x) / 1023.0 - 20.0;

        // 4. Apresentação do resultado com o rótulo Saída:
        printf("\nSaída:\n");
        if (temp >= -10.0 && temp <= 190.0) {
            printf("%.2f\n", temp);
        } else {
            printf("Valor fora da gama\n");
        }
        printf("\n"); // Prepara espaço para a próxima leitura
    }

    return 0;
}
