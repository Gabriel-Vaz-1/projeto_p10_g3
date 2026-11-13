#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_LEITURAS 100
#define MAX_LINHA 128
#define MAX_NOME_SENSOR 16
#define TEMP_MIN -10.0
#define TEMP_MAX 190.0

// ============================================================================
// ESTRUTURAS DE DADOS
// ============================================================================

// Define a calibração de cada tipo de sensor conhecido
typedef struct {
    char nome[MAX_NOME_SENSOR];
    double ganho;
    double desvio;
} CalibracaoSensor;

// Define uma leitura válida registada
typedef struct {
    char sensor[MAX_NOME_SENSOR];
    int instante;
    int valor_bruto;
    double temperatura;
} Leitura;

// Tabela única de calibração (Requisito Base 3)
const CalibracaoSensor SENSORES_CONHECIDOS[] = {
    {"TEMP1", 260.0, -20.0},
    {"TEMP2", 250.0, -15.0},
    {"TEMP3", 240.0, -10.0}
};
const int NUM_SENSORES_CONHECIDOS = 3;

// ============================================================================
// PROTÓTIPOS DAS FUNÇÕES
// ============================================================================
int procurar_sensor(const char *nome);
double converter_sensor_para_temp(int valor_sensor, double ganho, double desvio);
int temperatura_eh_valida(double temp);
int leitura_duplicada(const Leitura leituras[], int n, const char *sensor, int instante);
int ler_sequencia_valores(Leitura leituras[], int max_elementos);
double calcular_minimo(const double valores[], int n);
double calcular_maximo(const double valores[], int n);
double calcular_media(const double valores[], int n);
double calcular_desvio_padrao(const double valores[], int n);
void apresentar_resultados(const Leitura leituras[], int n);

// ============================================================================
// FUNÇÃO PRINCIPAL
// ============================================================================
int main(void) {
    Leitura leituras[MAX_LEITURAS];

    int total_lidos = ler_sequencia_valores(leituras, MAX_LEITURAS);

    if (total_lidos > 0) {
        apresentar_resultados(leituras, total_lidos);
    }

    return 0;
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ============================================================================

/**
 * Procura o identificador do sensor na tabela de calibração.
 * Devolve: índice do sensor na tabela, ou -1 se for desconhecido.
 */
int procurar_sensor(const char *nome) {
    for (int i = 0; i < NUM_SENSORES_CONHECIDOS; i++) {
        if (strcmp(SENSORES_CONHECIDOS[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * Converte o valor lido em temperatura usando o ganho e desvio do sensor.
 */
double converter_sensor_para_temp(int valor_sensor, double ganho, double desvio) {
    return (ganho * valor_sensor) / 1023.0 + desvio;
}

/**
 * Verifica se a temperatura está dentro da gama permitida [-10.0, 190.0].
 */
int temperatura_eh_valida(double temp) {
    return (temp >= TEMP_MIN && temp <= TEMP_MAX);
}

/**
 * Verifica se já existe uma leitura registada com o mesmo sensor e instante.
 */
int leitura_duplicada(const Leitura leituras[], int n, const char *sensor, int instante) {
    for (int i = 0; i < n; i++) {
        if (leituras[i].instante == instante && strcmp(leituras[i].sensor, sensor) == 0) {
            return 1;
        }
    }
    return 0;
}

/**
 * Lê linhas da entrada padrão até "FIM" ou até atingir o limite máximo.
 * Valida formato, sensor conhecido, duplicados e gama de temperatura.
 */
int ler_sequencia_valores(Leitura leituras[], int max_elementos) {
    char linha[MAX_LINHA];
    int total = 0;

    while (total < max_elementos && fgets(linha, sizeof(linha), stdin) != NULL) {
        char nome_sensor[MAX_NOME_SENSOR];
        int instante, valor;
        char extra; // Para apanhar lixo no fim da linha (ex: "TEMP1 1 234 abc")

        // Verifica se a linha está em branco (só espaços/tabs/enters)
        int campos_lidos = sscanf(linha, " %15s %d %d %c", nome_sensor, &instante, &valor, &extra);
        
        if (campos_lidos <= 0) {
            continue; // Ignora linhas em branco
        }

        // Condição de paragem: linha contém apenas "FIM"
        if (campos_lidos == 1 && strcmp(nome_sensor, "FIM") == 0) {
            break;
        }

        // 1. Verificar se o sensor existe
        int idx_sensor = procurar_sensor(nome_sensor);
        if (idx_sensor == -1) {
            printf("Sensor desconhecido\n");
            continue;
        }

        // 2. Verificar se a linha tem exatamente os 3 campos válidos (SENSOR INT INT)
        if (campos_lidos != 3) {
            printf("Linha inválida\n");
            continue;
        }

        // 3. Verificar duplicados (mesmo sensor e mesmo instante)
        if (leitura_duplicada(leituras, total, nome_sensor, instante)) {
            continue; // Rejeita silenciosamente ou ignora duplicado
        }

        // 4. Calcular temperatura com os dados específicos do sensor
        double temp = converter_sensor_para_temp(
            valor,
            SENSORES_CONHECIDOS[idx_sensor].ganho,
            SENSORES_CONHECIDOS[idx_sensor].desvio
        );

        // 5. Validar gama da temperatura
        if (!temperatura_eh_valida(temp)) {
            printf("Valor fora da gama\n");
            continue;
        }

        // Leitura válida: imprime temperatura e guarda no array
        printf("%.2f\n", temp);

        strcpy(leituras[total].sensor, nome_sensor);
        leituras[total].instante = instante;
        leituras[total].valor_bruto = valor;
        leituras[total].temperatura = temp;
        total++;
    }

    return total;
}

double calcular_minimo(const double valores[], int n) {
    double min = valores[0];
    for (int i = 1; i < n; i++) {
        if (valores[i] < min) min = valores[i];
    }
    return min;
}

double calcular_maximo(const double valores[], int n) {
    double max = valores[0];
    for (int i = 1; i < n; i++) {
        if (valores[i] > max) max = valores[i];
    }
    return max;
}

double calcular_media(const double valores[], int n) {
    double soma = 0.0;
    for (int i = 0; i < n; i++) {
        soma += valores[i];
    }
    return soma / n;
}

double calcular_desvio_padrao(const double valores[], int n) {
    double media = calcular_media(valores, n);
    double soma_sq = 0.0;
    for (int i = 0; i < n; i++) {
        soma_sq += (valores[i] - media) * (valores[i] - media);
    }
    return sqrt(soma_sq / n);
}

/**
 * Apresenta a listagem geral de leituras e as estatísticas individuais por sensor.
 */
void apresentar_resultados(const Leitura leituras[], int n) {
    printf("\nLeituras registadas (sensor instante: valor -> temperatura):\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d: %3d -> %6.2f\n",
               leituras[i].sensor,
               leituras[i].instante,
               leituras[i].valor_bruto,
               leituras[i].temperatura);
    }

    // Estatísticas agrupadas para cada sensor conhecido que tenha leituras
    for (int s = 0; s < NUM_SENSORES_CONHECIDOS; s++) {
        double temps_sensor[MAX_LEITURAS];
        int count = 0;

        // Filtra apenas as temperaturas deste sensor específico
        for (int i = 0; i < n; i++) {
            if (strcmp(leituras[i].sensor, SENSORES_CONHECIDOS[s].nome) == 0) {
                temps_sensor[count++] = leituras[i].temperatura;
            }
        }

        // Só imprime se este sensor tiver pelo menos 1 leitura registada
        if (count > 0) {
            printf("\nSensor %s\n", SENSORES_CONHECIDOS[s].nome);
            printf("Número de valores lidos: %d\n", count);
            printf("%-20s %8.2f\n", "Temperatura mínima:", calcular_minimo(temps_sensor, count));
            printf("%-20s %8.2f\n", "Temperatura máxima:", calcular_maximo(temps_sensor, count));
            printf("%-20s %8.2f\n", "Temperatura média:", calcular_media(temps_sensor, count));
            printf("%-20s %8.2f\n", "Desvio-padrão:", calcular_desvio_padrao(temps_sensor, count));
        }
    }
}
