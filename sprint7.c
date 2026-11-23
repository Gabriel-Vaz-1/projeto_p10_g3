#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_LEITURAS 100
#define MAX_LINHA 128
#define MAX_NOME_SENSOR 16

// ============================================================================
// ESTRUTURAS DE DADOS
// ============================================================================

// Ficha técnica completa de cada sensor (Requisito Base 1 + Requisito Adicional 1)
typedef struct {
    char nome[MAX_NOME_SENSOR];
    double ganho;
    double desvio;
    double temp_min;
    double temp_max;
} CalibracaoSensor;

// Define uma leitura válida registada
typedef struct {
    char sensor[MAX_NOME_SENSOR];
    int instante;
    int valor_bruto;
    double temperatura;
} Leitura;

// ============================================================================
// PROTÓTIPOS DAS FUNÇÕES
// ============================================================================
int procurar_sensor(const char *nome, const CalibracaoSensor sensores[], int num_sensores);
double converter_sensor_para_temp(CalibracaoSensor sensor, int valor_sensor);
int temperatura_eh_valida(CalibracaoSensor sensor, double temp);
int leitura_duplicada(const Leitura leituras[], int n, const char *sensor, int instante);
int ler_sequencia_valores(Leitura leituras[], int max_elementos, const CalibracaoSensor sensores[], int num_sensores);
double calcular_minimo(const double valores[], int n);
double calcular_maximo(const double valores[], int n);
double calcular_media(const double valores[], int n);
double calcular_desvio_padrao(const double valores[], int n);
void apresentar_resultados(const Leitura leituras[], int n, const CalibracaoSensor sensores[], int num_sensores);

// ============================================================================
// FUNÇÃO PRINCIPAL
// ============================================================================
int main(void) {
    // Tabela única de calibração e gama de funcionamento (Requisito Base 2 + Adicional 1)
    const CalibracaoSensor SENSORES_CONHECIDOS[] = {
        {"TEMP1", 260.0, -20.0, -10.0, 190.0},
        {"TEMP2", 250.0, -15.0,   0.0, 150.0},
        {"TEMP3", 240.0, -10.0, -10.0, 100.0}
    };
    const int NUM_SENSORES_CONHECIDOS = (int)(sizeof(SENSORES_CONHECIDOS) / sizeof(SENSORES_CONHECIDOS[0]));

    Leitura leituras[MAX_LEITURAS];

    int total_lidos = ler_sequencia_valores(leituras, MAX_LEITURAS, SENSORES_CONHECIDOS, NUM_SENSORES_CONHECIDOS);

    if (total_lidos > 0) {
        apresentar_resultados(leituras, total_lidos, SENSORES_CONHECIDOS, NUM_SENSORES_CONHECIDOS);
    }

    return 0;
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ============================================================================

/**
 * Procura o identificador do sensor na coleção de sensores conhecidos (Requisito Base 3).
 * Devolve: índice do sensor na tabela (>= 0), ou -1 se for desconhecido.
 */
int procurar_sensor(const char *nome, const CalibracaoSensor sensores[], int num_sensores) {
    for (int i = 0; i < num_sensores; i++) {
        if (strcmp(sensores[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * Converte o valor lido em temperatura recebendo a ficha do sensor (Requisito Base 4).
 */
double converter_sensor_para_temp(CalibracaoSensor sensor, int valor_sensor) {
    return (sensor.ganho * valor_sensor) / 1023.0 + sensor.desvio;
}

/**
 * Verifica se a temperatura está dentro da gama própria do sensor (Requisito Adicional 2).
 */
int temperatura_eh_valida(CalibracaoSensor sensor, double temp) {
    return (temp >= sensor.temp_min && temp <= sensor.temp_max);
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
 * Valida formato, sensor conhecido, duplicados e gama própria de temperatura.
 */
int ler_sequencia_valores(Leitura leituras[], int max_elementos, const CalibracaoSensor sensores[], int num_sensores) {
    char linha[MAX_LINHA];
    int total = 0;

    while (total < max_elementos && fgets(linha, sizeof(linha), stdin) != NULL) {
        char nome_sensor[MAX_NOME_SENSOR];
        int instante, valor;
        char extra; // Para apanhar lixo no fim da linha (ex: "TEMP1 1 234 abc")

        int campos_lidos = sscanf(linha, " %15s %d %d %c", nome_sensor, &instante, &valor, &extra);

        if (campos_lidos <= 0) {
            continue; // Ignora linhas em branco
        }

        // Condição de paragem: linha contém apenas "FIM"
        if (campos_lidos == 1 && strcmp(nome_sensor, "FIM") == 0) {
            break;
        }

        // 1. Verificar se o sensor existe
        int idx_sensor = procurar_sensor(nome_sensor, sensores, num_sensores);
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
            continue; // Ignora duplicado
        }

        // 4. Calcular temperatura passando a ficha completa do sensor (Requisito Base 4)
        CalibracaoSensor sensor_atual = sensores[idx_sensor];
        double temp = converter_sensor_para_temp(sensor_atual, valor);

        // 5. Validar gama própria do sensor (Requisito Adicional 2)
        if (!temperatura_eh_valida(sensor_atual, temp)) {
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
void apresentar_resultados(const Leitura leituras[], int n, const CalibracaoSensor sensores[], int num_sensores) {
    printf("\nLeituras registadas (sensor instante: valor -> temperatura):\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d: %3d -> %6.2f\n",
               leituras[i].sensor,
               leituras[i].instante,
               leituras[i].valor_bruto,
               leituras[i].temperatura);
    }

    // Estatísticas agrupadas para cada sensor conhecido que tenha leituras
    for (int s = 0; s < num_sensores; s++) {
        double temps_sensor[MAX_LEITURAS];
        int count = 0;

        for (int i = 0; i < n; i++) {
            if (strcmp(leituras[i].sensor, sensores[s].nome) == 0) {
                temps_sensor[count++] = leituras[i].temperatura;
            }
        }

        if (count > 0) {
            printf("\nSensor %s\n", sensores[s].nome);
            printf("Número de valores lidos: %d\n", count);
            printf("%-20s %8.2f\n", "Temperatura mínima:", calcular_minimo(temps_sensor, count));
            printf("%-20s %8.2f\n", "Temperatura máxima:", calcular_maximo(temps_sensor, count));
            printf("%-20s %8.2f\n", "Temperatura média:", calcular_media(temps_sensor, count));
            printf("%-20s %8.2f\n", "Desvio-padrão:", calcular_desvio_padrao(temps_sensor, count));
        }
    }
}
