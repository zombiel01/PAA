#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PACOTES 100

// Estrutura para armazenar os pacotes
typedef struct {
    int numero_pacote;
    int tamanho;
    char* dados;
} Pacote;

// Função para comparar dois pacotes para ordenação
int comparar(const void *a, const void *b) {
    return ((Pacote*)a)->numero_pacote - ((Pacote*)b)->numero_pacote;
}

// Função para ler o arquivo de entrada e processar os pacotes
void processar_pacotes(const char* entrada, const char* saida, int num_pacotes_ordenacao) {
    FILE *arquivo_entrada = fopen(entrada, "r");
    if (arquivo_entrada == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return;
    }

    // Leitura do número total de pacotes e quantidade de pacotes recebidos
    int total_pacotes;
    fscanf(arquivo_entrada, "[#%d %*d]\n", &total_pacotes);
    
    Pacote pacotes[MAX_PACOTES];
    int contagem_pacotes = 0;
    
    // Leitura dos pacotes
    while (fscanf(arquivo_entrada, "[%d #%d", &pacotes[contagem_pacotes].numero_pacote, &pacotes[contagem_pacotes].tamanho) == 2) {
        pacotes[contagem_pacotes].dados = malloc(pacotes[contagem_pacotes].tamanho);
        for (int i = 0; i < pacotes[contagem_pacotes].tamanho; i++) {
            fscanf(arquivo_entrada, " %c", &pacotes[contagem_pacotes].dados[i]);
        }
        fscanf(arquivo_entrada, "]\n");
        contagem_pacotes++;
    }

    fclose(arquivo_entrada);

    // Ordenação dos pacotes, quando o número de pacotes recebidos for suficiente
    if (contagem_pacotes >= num_pacotes_ordenacao) {
        qsort(pacotes, contagem_pacotes, sizeof(Pacote), comparar);

        // Escreve no arquivo de saída
        FILE *arquivo_saida = fopen(saida, "w");
        if (arquivo_saida == NULL) {
            perror("Erro ao abrir o arquivo de saída");
            return;
        }

        // Concatenar os dados dos pacotes ordenados para exibir a parte inicial
        char dados_completos[512 * MAX_PACOTES] = {0};
        int total_bytes = 0;

        for (int i = 0; i < contagem_pacotes; i++) {
            memcpy(dados_completos + total_bytes, pacotes[i].dados, pacotes[i].tamanho);
            total_bytes += pacotes[i].tamanho;
        }

        // Exibe no arquivo de saída a parte inicial dos dados que já foram recebidos
        fwrite(dados_completos, 1, total_bytes, arquivo_saida);

        fclose(arquivo_saida);
    } else {
        printf("A quantidade de pacotes recebidos é insuficiente para ordenar.\n");
    }

    // Libera a memória dos pacotes
    for (int i = 0; i < contagem_pacotes; i++) {
        free(pacotes[i].dados);
    }
}

int main() {
    // Arquivo de entrada e saída
    const char* entrada = "entrada.txt";
    const char* saida = "saida.txt";
    
    // Número de pacotes necessários para iniciar a ordenação
    int num_pacotes_ordenacao = 3; // Altere esse valor conforme necessário

    processar_pacotes(entrada, saida, num_pacotes_ordenacao);

    return 0;
}
