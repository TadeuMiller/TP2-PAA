#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "caverna.h"

/* Aloca a matriz de inteiros linhas x colunas */
static int **alocarMatriz(int linhas, int colunas) {
    int i;
    int **m = (int **) malloc(linhas * sizeof(int *));
    if (m == NULL) return NULL;

    for (i = 0; i < linhas; i++) {
        m[i] = (int *) calloc(colunas, sizeof(int));
        if (m[i] == NULL) {
            /* deu erro: libera as linhas que ja foram alocadas */
            while (--i >= 0) free(m[i]);
            free(m);
            return NULL;
        }
    }
    return m;
}

static void liberarMatriz(int **m, int linhas) {
    int i;
    if (m == NULL) return;
    for (i = 0; i < linhas; i++) free(m[i]);
    free(m);
}

int lerCaverna(const char *caminho, Caverna *c) {
    FILE *arq;
    char token[64];
    char *fim;
    int i, j;
    int achouI = 0, achouF = 0;

    arq = fopen(caminho, "r");
    if (arq == NULL) return ERRO_ARQUIVO;

    if (fscanf(arq, "%d %d %d", &c->linhas, &c->colunas, &c->vidaInicial) != 3
        || c->linhas <= 0 || c->colunas <= 0) {
        fclose(arq);
        return ERRO_CABECALHO;
    }

    c->valores = alocarMatriz(c->linhas, c->colunas);
    if (c->valores == NULL) {
        fclose(arq);
        return ERRO_MEMORIA;
    }

    for (i = 0; i < c->linhas; i++) {
        for (j = 0; j < c->colunas; j++) {
            if (fscanf(arq, "%63s", token) != 1) {
                liberarMatriz(c->valores, c->linhas);
                fclose(arq);
                return ERRO_CELULA;
            }

            if (strcmp(token, "I") == 0) {
                c->linhaI = i;
                c->colunaI = j;
                c->valores[i][j] = 0;
                achouI = 1;
            } else if (strcmp(token, "F") == 0) {
                c->linhaF = i;
                c->colunaF = j;
                c->valores[i][j] = 0;
                achouF = 1;
            } else {
                c->valores[i][j] = (int) strtol(token, &fim, 10);
                if (*fim != '\0') {  
                    liberarMatriz(c->valores, c->linhas);
                    fclose(arq);
                    return ERRO_CELULA;
                }
            }
        }
    }

    fclose(arq);

    if (!achouI || !achouF) {
        liberarMatriz(c->valores, c->linhas);
        return ERRO_SEM_I_OU_F;
    }
    return LEITURA_OK;
}

void liberarCaverna(Caverna *c) {
    liberarMatriz(c->valores, c->linhas);
    c->valores = NULL;
}

int dentroDaCaverna(const Caverna *c, int i, int j) {
    return i >= 0 && i < c->linhas && j >= 0 && j < c->colunas;
}

int valorCelula(const Caverna *c, int i, int j) {
    return c->valores[i][j];
}

void imprimirErroLeitura(int codigo) {
    switch (codigo) {
        case ERRO_ARQUIVO:    printf("Erro: nao foi possivel abrir o arquivo.\n"); break;
        case ERRO_CABECALHO:  printf("Erro: primeira linha invalida.\n"); break;
        case ERRO_MEMORIA:    printf("Erro: falta de memoria.\n"); break;
        case ERRO_CELULA:     printf("Erro: celula invalida ou faltando no arquivo.\n"); break;
        case ERRO_SEM_I_OU_F: printf("Erro: o arquivo nao tem I ou F.\n"); break;
        default:              printf("Erro desconhecido.\n");
    }
}

int *achatarCaverna(const Caverna *c) {
    int i, j;
    int *v = (int *) malloc((size_t) c->linhas * c->colunas * sizeof(int));
    if (v == NULL) return NULL;

    for (i = 0; i < c->linhas; i++)
        for (j = 0; j < c->colunas; j++)
            v[i * c->colunas + j] = c->valores[i][j];
    return v;
}

void imprimirCaverna(const Caverna *c) {
    int i, j;
    printf("Caverna %d x %d, vida inicial = %d\n", c->linhas, c->colunas, c->vidaInicial);
    printf("I em (%d, %d) | F em (%d, %d)\n", c->linhaI, c->colunaI, c->linhaF, c->colunaF);
    for (i = 0; i < c->linhas; i++) {
        for (j = 0; j < c->colunas; j++) {
            if (i == c->linhaI && j == c->colunaI)      printf("    I");
            else if (i == c->linhaF && j == c->colunaF) printf("    F");
            else                                        printf("%5d", c->valores[i][j]);
        }
        printf("\n");
    }
}
