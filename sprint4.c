#include <stdio.h>
#include <math.h>

#define MAX_LEITURAS 20 // Limite definido num único ponto do código

int main(void) {
    int valores_sensor[MAX_LEITURAS];
    double temperaturas[MAX_LEITURAS];
    int total_lidos = 0;
    int x;

    // 1. Ciclo de leitura
    while (total_lidos < MAX_LEITURAS) {
        // Validação de entrada: rejeita texto (ex: "abc")
        while (scanf("%d", &x) != 1) {
            scanf("%*s");
        }

        // Condição de terminação (sentinela -1)
        if (x == -1) {
            break;
        }

        // Cálculo da temperatura (fórmula oficial do Sprint 1)
        double temp = (260.0 * x) / 1023.0 - 20.0;

        // Apresenta o resultado da conversão no momento
        if (temp >= -10.0 && temp <= 190.0) {
            printf("%.2f\n", temp);
        } else {
            printf("Valor fora da gama\n");
        }

        // Guarda nos arrays para o resumo final
        valores_sensor[total_lidos] = x;
        temperaturas[total_lidos] = temp;
        total_lidos++;
    }

    // 2. Apresentação do resumo final
    if (total_lidos > 0) {
        printf("\nValores lidos (sensor -> temperatura):\n");
        double soma = 0.0;
        double min_temp = temperaturas[0]; // Inicialização correta
        double max_temp = temperaturas[0];

        for (int i = 0; i < total_lidos; i++) {
            printf("%d -> %.2f\n", valores_sensor[i], temperaturas[i]);
            soma += temperaturas[i];

            if (temperaturas[i] < min_temp) {
                min_temp = temperaturas[i];
            }
            if (temperaturas[i] > max_temp) {
                max_temp = temperaturas[i];
            }
        }

        double media = soma / total_lidos;

        // Cálculo do desvio-padrão
        double soma_diferencas_sq = 0.0;
        for (int i = 0; i < total_lidos; i++) {
            soma_diferencas_sq += (temperaturas[i] - media) * (temperaturas[i] - media);
        }
        double desvio_padrao = sqrt(soma_diferencas_sq / total_lidos);

        // Apresentação das estatísticas com o alinhamento exato do exemplo
        printf("\nNúmero de valores lidos: %d\n", total_lidos);
        printf("%-20s %8.2f\n", "Temperatura mínima:", min_temp);
        printf("%-20s %8.2f\n", "Temperatura máxima:", max_temp);
        printf("%-20s %8.2f\n", "Temperatura média:", media);
        printf("%-20s %8.2f\n", "Desvio-padrão:", desvio_padrao);
    }

    return 0;
} 
