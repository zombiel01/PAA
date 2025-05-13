#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int32_t trocas = 0;
int32_t chamadas = 0;

// Função auxiliar para trocar dois elementos
void trocar(int32_t *a, int32_t *b) {
    int32_t temp = *a;
    *a = *b;
    *b = temp;
    trocas++;
}

// Particionamento por Lomuto
int32_t lomuto(int32_t* V, int32_t i, int32_t j) {
    int32_t P = V[j], x = i - 1, y = i;
    for (y = i; y < j; y++) {
        if(V[y] <= P) {
            trocar(&V[++x], &V[y]);
        }
    }
    trocar(&V[++x], &V[j]);
    return x;
}

// Particionamento por Hoare
int32_t hoare(int32_t* V, int32_t i, int32_t j) {
    int32_t P = V[i], x = i - 1, y = j + 1;
    while (1) {
        do {
            y--;
        } while (V[y] > P);

        do {
            x++;
        } while (V[x] < P);

        if (x >= y) {
            return y;
        }
        trocar(&V[x], &V[y]);
    }
}



//Lomuto Padrao 
void quicksort_lomuto(int32_t* V, int32_t i, int32_t j) {
    chamadas++;
    if(i < j) {
        int32_t p = lomuto(V, i, j);
        quicksort_lomuto(V, i, p - 1);
        quicksort_lomuto(V, p + 1, j);
    }
}

void quicksort_hoare(int32_t* V, int32_t i, int32_t j) {
    chamadas++;
    if(i < j) {
        int32_t p = hoare(V, i, j);
        quicksort_hoare(V, i, p);
        quicksort_hoare(V, p + 1, j);
    }
}


void ler_arquivo(const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    int32_t n, tamanho_vetor;
    fscanf(arquivo, "%d", &n);  // Lê o número total de vetores

    for (int32_t i = 0; i < n; i++) {
        // Lê o tamanho do vetor
        fscanf(arquivo, "%d", &tamanho_vetor);
        int32_t* vetor = (int32_t*)malloc(tamanho_vetor * sizeof(int32_t));

        // Lê os elementos do vetor
        for (int32_t j = 0; j < tamanho_vetor; j++) {
            fscanf(arquivo, "%d", &vetor[j]);
        }

        // Testa o Quicksort com Lomuto
        trocas = 0;
        chamadas = 0;
        printf("\n%d:", i);
        printf("N(%d),", tamanho_vetor);
        quicksort_lomuto(vetor, 0, tamanho_vetor - 1);
        printf("LP(%d)\n", trocas+chamadas);

        


        // Libera a memória do vetor
        free(vetor);
    }

    fclose(arquivo);
}

int main() {
    // Substitua pelo caminho do arquivo
    ler_arquivo("entrada.txt");  
    return 0;
}

// Inacabado