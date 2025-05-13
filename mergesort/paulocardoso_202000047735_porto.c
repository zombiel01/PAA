#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    char codigo[20];
    char cnpj[20];
    int peso;
} Container;

typedef struct HashNode {
    Container container;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode **buckets;
    int size;
} HashTable;

unsigned int hashFunction(const char *key, int size) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31 + *key) % size;
        key++;
    }
    return hash;
}

HashTable *createHashTable(int size) {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    table->size = size;
    table->buckets = (HashNode **)calloc(size, sizeof(HashNode *));
    return table;
}

void insertHashTable(HashTable *table, Container container) {
    unsigned int index = hashFunction(container.codigo, table->size);
    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
    newNode->container = container;
    newNode->next = table->buckets[index];
    table->buckets[index] = newNode;
}

Container *findHashTable(HashTable *table, const char *codigo) {
    unsigned int index = hashFunction(codigo, table->size);
    HashNode *current = table->buckets[index];
    while (current) {
        if (strcmp(current->container.codigo, codigo) == 0) {
            return &current->container;
        }
        current = current->next;
    }
    return NULL;
}

void freeHashTable(HashTable *table) {
    for (int i = 0; i < table->size; i++) {
        HashNode *current = table->buckets[i];
        while (current) {
            HashNode *toFree = current;
            current = current->next;
            free(toFree);
        }
    }
    free(table->buckets);
    free(table);
}

void lerConteineres(FILE *arquivo, Container **containers, int *quantidade) {
    fscanf(arquivo, "%d", quantidade);
    *containers = (Container *)malloc(*quantidade * sizeof(Container));

    for (int i = 0; i < *quantidade; i++) {
        fscanf(arquivo, "%s %s %d", (*containers)[i].codigo, (*containers)[i].cnpj, &(*containers)[i].peso);
    }
}

void verificarDivergencias(Container *todosContainers, int totalContainers, Container *containersSelecionados, int totalSelecionados, const char *arquivoSaida) {
    FILE *saida = fopen(arquivoSaida, "w");
    if (saida == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        return;
    }

    HashTable *hashTable = createHashTable(totalContainers);

    // Insere todos os contêineres em uma tabela hash
    for (int i = 0; i < totalContainers; i++) {
        insertHashTable(hashTable, todosContainers[i]);
    }

    int maiorDiferencaPercentual = 0;
    char codigoMaiorDiferenca[20][20];
    int diferencaPeso[20];
    int count = 0;

    int encontrouDivergencia = 0;

    // Verifica os contêineres selecionados usando a tabela hash
    for (int i = 0; i < totalSelecionados; i++) {
        Container *encontrado = findHashTable(hashTable, containersSelecionados[i].codigo);

        if (encontrado) {
            if (strcmp(encontrado->cnpj, containersSelecionados[i].cnpj) != 0) {
                fprintf(saida, "%s: %s<->%s\n", encontrado->codigo, encontrado->cnpj, containersSelecionados[i].cnpj);
                encontrouDivergencia = 1;
            } else {
                float diferenca = abs(encontrado->peso - containersSelecionados[i].peso);
                float media = (encontrado->peso + containersSelecionados[i].peso) / 2.0;
                int diferencaPercentual = (int)((diferenca / media) * 100.0 + 0.5);

                if (diferencaPercentual > maiorDiferencaPercentual) {
                    maiorDiferencaPercentual = diferencaPercentual;
                    strcpy(codigoMaiorDiferenca[0], encontrado->codigo);
                    diferencaPeso[0] = (int)diferenca;
                    count = 1;
                } else if (diferencaPercentual == maiorDiferencaPercentual) {
                    strcpy(codigoMaiorDiferenca[count], encontrado->codigo);
                    diferencaPeso[count] = (int)diferenca;
                    count++;
                }
            }
        }
    }

    if (count > 0) {
        for (int i = 0; i < count; i++) {
            fprintf(saida, "%s: %dkg(%d%%)\n", codigoMaiorDiferenca[i], diferencaPeso[i], maiorDiferencaPercentual);
        }
    } else {
        fprintf(saida, "Nenhuma diferença de peso encontrada.\n");
    }

    if (!encontrouDivergencia) {
        fprintf(saida, "Nenhuma divergência encontrada.\n");
    }

    fclose(saida);
    freeHashTable(hashTable);
    printf("Arquivo de saída '%s' gerado com sucesso.\n", arquivoSaida);
}

int main(int argc, char *argv[]) {
    FILE *arquivo = fopen(argv[1], "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    Container *todosContainers = NULL;
    int totalContainers = 0;

    Container *containersSelecionados = NULL;
    int totalSelecionados = 0;

    lerConteineres(arquivo, &todosContainers, &totalContainers);
    lerConteineres(arquivo, &containersSelecionados, &totalSelecionados);

    fclose(arquivo);

    verificarDivergencias(todosContainers, totalContainers, containersSelecionados, totalSelecionados, argv[2]);

    free(todosContainers);
    free(containersSelecionados);

    return 0;
}
