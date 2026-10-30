#include <stdio.h>
#include <math.h> // Necessário para a função sqrt() do desvio-padrão

// Constantes Globais (Permitidas)
#define MAX_LEITURAS 5
#define TEMP_MIN 0.0
#define TEMP_MAX 150.0

// ============================================================================
// PROTÓTIPOS DAS FUNÇÕES
// ============================================================================
double converter_sensor_para_temp(int valor_sensor);
int temperatura_eh_valida(double temp);
int ler_sequencia_valores(int sensores[], double temperaturas[], int max_elementos);
double calcular_minimo(const double valores[], int n);
double calcular_maximo(const double valores[], int n);
double calcular_media(const double valores[], int n);
double calcular_desvio_padrao(const double valores[], int n);
void apresentar_resultados(const int sensores[], const double temperaturas[], int n);
void executar_testes_unitarios(void);

// ============================================================================
// FUNÇÃO PRINCIPAL (MAIN)
// ============================================================================
int main(void) {
    // Requisito Adicional 2: Executa os testes unitários automáticos
    executar_testes_unitarios();

    int sensores[MAX_LEITURAS];
    double temperaturas[MAX_LEITURAS];

    // 1. Leitura e processamento dos dados
    int total_lidos = ler_sequencia_valores(sensores, temperaturas, MAX_LEITURAS);

    // 2. Apresentação dos resultados (se existirem dados)
    if (total_lidos > 0) {
        apresentar_resultados(sensores, temperaturas, total_lidos);
    } else {
        printf("\nNenhum valor valido foi lido.\n");
    }

    return 0;
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ============================================================================

/**
 * Converte o valor bruto do sensor (0-1023) para temperatura em graus Celsius.
 * Recebe: valor_sensor (int)
 * Devolve: temperatura calculada (double)
 */
double converter_sensor_para_temp(int valor_sensor) {
    return (150.0 * valor_sensor) / 1023.0;
}

/**
 * Verifica se uma temperatura está dentro dos limites válidos de operação.
 * Recebe: temp (double)
 * Devolve: 1 se for válida, 0 se for inválida (int)
 */
int temperatura_eh_valida(double temp) {
    return (temp >= TEMP_MIN && temp <= TEMP_MAX);
}

/**
 * Lê os valores do sensor a partir do teclado, converte-os e guarda-os nos arrays.
 * Recebe: sensores[], temperaturas[], max_elementos
 * Devolve: número de elementos lidos com sucesso (int)
 */
int ler_sequencia_valores(int sensores[], double temperaturas[], int max_elementos) {
    int total = 0;
    int valor_lido;

    while (total < max_elementos) {
        // Validação da leitura do teclado (limpeza do buffer em caso de erro)
        if (scanf("%d", &valor_lido) != 1) {
            scanf("%*s"); // Descarta texto inválido
            continue;
        }

        // Condição de paragem (sentinela)
        if (valor_lido == -1) {
            break;
        }

        double temp_calculada = converter_sensor_para_temp(valor_lido);

        // Guarda a leitura independentemente de estar na gama para relatório posterior
        sensores[total] = valor_lido;
        temperaturas[total] = temp_calculada;
        total++;
    }

    return total;
}

/**
 * Determina o valor mínimo de um array de números.
 */
double calcular_minimo(const double valores[], int n) {
    double min = valores[0];
    for (int i = 1; i < n; i++) {
        if (valores[i] < min) {
            min = valores[i];
        }
    }
    return min;
}

/**
 * Determina o valor máximo de um array de números.
 */
double calcular_maximo(const double valores[], int n) {
    double max = valores[0];
    for (int i = 1; i < n; i++) {
        if (valores[i] > max) {
            max = valores[i];
        }
    }
    return max;
}

/**
 * Calcula a média aritmética de um array de números.
 */
double calcular_media(const double valores[], int n) {
    double soma = 0.0;
    for (int i = 0; i < n; i++) {
        soma += valores[i];
    }
    return soma / n;
}

/**
 * Calcula o desvio-padrão populacional de um array de números.
 */
double calcular_desvio_padrao(const double valores[], int n) {
    double media = calcular_media(valores, n);
    double soma_diferencas = 0.0;

    for (int i = 0; i < n; i++) {
        soma_diferencas += (valores[i] - media) * (valores[i] - media);
    }

    return sqrt(soma_diferencas / n);
}

/**
 * Apresenta o relatório final formatado na consola.
 */
void apresentar_resultados(const int sensores[], const double temperaturas[], int n) {
    printf("\nValores lidos (sensor -> temperatura):\n");
    for (int i = 0; i < n; i++) {
        printf("%d -> %.2f\n", sensores[i], temperaturas[i]);
    }

    printf("\nNúmero de valores lidos: %d\n", n);
    printf("Temperatura mínima:  %9.2f\n", calcular_minimo(temperaturas, n));
    printf("Temperatura máxima:  %9.2f\n", calcular_maximo(temperaturas, n));
    printf("Temperatura média:   %9.2f\n", calcular_media(temperaturas, n));
    printf("Desvio-padrão:       %9.2f\n", calcular_desvio_padrao(temperaturas, n));
}

/**
 * Requisito Adicional: Conjunto de testes unitários simples
 * Compares o resultado retornado com valores conhecidos.
 */
void executar_testes_unitarios(void) {
    // Teste de Conversão (Sensor 523 -> ~112.96)
    double temp_523 = converter_sensor_para_temp(523);
    
    // Teste de Validação
    int val_ok = temperatura_eh_valida(100.0);
    int val_err = temperatura_eh_valida(200.0);

    // Pequena verificação interna silenciosa ou informativa
    if (temp_523 < 112.90 || temp_523 > 113.00 || val_ok != 1 || val_err != 0) {
        printf("[AVISO DE TESTE]: Algum teste unitário falhou!\n");
    }
}
