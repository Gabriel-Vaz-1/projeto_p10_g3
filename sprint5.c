#include <stdio.h>
#include <math.h>

#define MAX_LEITURAS 20
#define TEMP_MIN -10.0
#define TEMP_MAX 190.0

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
// FUNÇÃO PRINCIPAL
// ============================================================================
int main(void) {
    // Requisito Adicional 2: Executa testes unitários (silenciosos se passarem)
    executar_testes_unitarios();

    int sensores[MAX_LEITURAS];
    double temperaturas[MAX_LEITURAS];

    // 1. Leitura direta dos dados em silêncio
    int total_lidos = ler_sequencia_valores(sensores, temperaturas, MAX_LEITURAS);

    // 2. Apresentação do relatório final
    if (total_lidos > 0) {
        apresentar_resultados(sensores, temperaturas, total_lidos);
    }

    return 0;
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ============================================================================

/**
 * Converte um valor do sensor (0 a 1023) na temperatura correspondente.
 * Recebe: valor_sensor (int)
 * Devolve: temperatura calculada (double)
 */
double converter_sensor_para_temp(int valor_sensor) {
    return (260.0 * valor_sensor) / 1023.0 - 20.0;
}

/**
 * Verifica se uma temperatura está dentro da gama válida (-10ºC a 190ºC).
 * Recebe: temp (double)
 * Devolve: 1 se for válida, 0 caso contrário (int)
 */
int temperatura_eh_valida(double temp) {
    return (temp >= TEMP_MIN && temp <= TEMP_MAX);
}

/**
 * Lê sequencialmente os valores dos sensores a partir do teclado até ao limite 
 * ou até ser inserido o valor de terminação -1.
 * Recebe: sensores[], temperaturas[], max_elementos
 * Devolve: quantidade de valores efetivamente lidos (int)
 */
int ler_sequencia_valores(int sensores[], double temperaturas[], int max_elementos) {
    int total = 0;
    int valor_lido;

    while (total < max_elementos) {
        // Validação de entrada: rejeita texto (ex: "abc")
        if (scanf("%d", &valor_lido) != 1) {
            scanf("%*s");
            continue;
        }

        // Condição de terminação (sentinela -1)
        if (valor_lido == -1) {
            break;
        }

        double temp_calculada = converter_sensor_para_temp(valor_lido);

        // Imprime a resposta individual da conversão
        if (temperatura_eh_valida(temp_calculada)) {
            printf("%.2f\n", temp_calculada);
        } else {
            printf("Valor fora da gama\n");
        }

        sensores[total] = valor_lido;
        temperaturas[total] = temp_calculada;
        total++;
    }

    return total;
}

/**
 * Determina o valor mínimo das temperaturas registadas.
 * Recebe: array valores[], tamanho n
 * Devolve: valor mínimo (double)
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
 * Determina o valor máximo das temperaturas registadas.
 * Recebe: array valores[], tamanho n
 * Devolve: valor máximo (double)
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
 * Calcula a média aritmética das temperaturas registadas.
 * Recebe: array valores[], tamanho n
 * Devolve: média (double)
 */
double calcular_media(const double valores[], int n) {
    double soma = 0.0;
    for (int i = 0; i < n; i++) {
        soma += valores[i];
    }
    return soma / n;
}

/**
 * Calcula o desvio-padrão populacional das temperaturas registadas.
 * Recebe: array valores[], tamanho n
 * Devolve: desvio-padrão (double)
 */
double calcular_desvio_padrao(const double valores[], int n) {
    double media = calcular_media(valores, n);
    double soma_diferencas_sq = 0.0;

    for (int i = 0; i < n; i++) {
        soma_diferencas_sq += (valores[i] - media) * (valores[i] - media);
    }

    return sqrt(soma_diferencas_sq / n);
}

/**
 * Apresenta a lista de leituras e as estatísticas calculadas com alinhamento formatado.
 * Recebe: sensores[], temperaturas[], quantidade n
 * Devolve: nada (void)
 */
void apresentar_resultados(const int sensores[], const double temperaturas[], int n) {
    printf("\nValores lidos (sensor -> temperatura):\n");
    for (int i = 0; i < n; i++) {
        printf("%d -> %.2f\n", sensores[i], temperaturas[i]);
    }

    printf("\nNúmero de valores lidos: %d\n", n);
    printf("%-20s %8.2f\n", "Temperatura mínima:", calcular_minimo(temperaturas, n));
    printf("%-20s %8.2f\n", "Temperatura máxima:", calcular_maximo(temperaturas, n));
    printf("%-20s %8.2f\n", "Temperatura média:", calcular_media(temperaturas, n));
    printf("%-20s %8.2f\n", "Desvio-padrão:", calcular_desvio_padrao(temperaturas, n));
}

/**
 * Requisito Adicional 2: Testa diretamente as funções de conversão, validação e cálculo.
 * Funciona de forma silenciosa e só avisa se detetar um erro crítico de lógica.
 */
void executar_testes_unitarios(void) {
    double temp_523 = converter_sensor_para_temp(523);
    int val_valido = temperatura_eh_valida(100.0);
    int val_invalido = temperatura_eh_valida(200.0);

    // Verificação com tolerância adequada ao cálculo decimal real
    if (fabs(temp_523 - 112.92) > 0.05 || val_valido != 1 || val_invalido != 0) {
        printf("[ERRO CRÍTICO]: Teste unitario falhou!\n");
    }
}
