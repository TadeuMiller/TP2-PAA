/* Driver auxiliar para testes isolados. Nao substitui o main.c do grupo. */
#include "dp.h"
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parse_int(const char *texto, int *valor)
{
    char *fim;
    long numero;
    errno = 0;
    numero = strtol(texto, &fim, 10);
    if (errno || *fim || texto == fim || numero < INT_MIN || numero > INT_MAX) {
        return 0;
    }
    *valor = (int)numero;
    return 1;
}

int main(int argc, char **argv)
{
    FILE *arquivo;
    int linhas, colunas, inicio_encontrado = 0, fim_encontrado = 0;
    long long vida_inicial;
    int *valores = NULL;
    CoordenadaDP inicio = {0, 0}, fim = {0, 0};
    ResultadoDP resultado = {0};
    int status, erro = 1;
    size_t quantidade;

    if (argc != 2 || (arquivo = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Uso: driver_dp arquivo_de_entrada.txt\n");
        return 2;
    }
    if (fscanf(arquivo, "%d %d %lld", &linhas, &colunas, &vida_inicial) != 3 ||
        linhas <= 0 || colunas <= 0 || vida_inicial <= 0 ||
        (size_t)linhas > SIZE_MAX / (size_t)colunas) {
        fprintf(stderr, "Cabecalho invalido\n");
        goto limpar;
    }
    quantidade = (size_t)linhas * (size_t)colunas;
    if (quantidade > SIZE_MAX / sizeof(*valores)) {
        goto limpar;
    }
    valores = malloc(quantidade * sizeof(*valores));
    if (!valores) {
        goto limpar;
    }
    for (size_t k = 0; k < quantidade; ++k) {
        char token[64];
        if (fscanf(arquivo, "%63s", token) != 1) {
            fprintf(stderr, "Caverna incompleta\n");
            goto limpar;
        }
        if (!strcmp(token, "I")) {
            if (inicio_encontrado++) goto limpar;
            inicio = (CoordenadaDP){(int)(k / colunas), (int)(k % colunas)};
            valores[k] = 0;
        } else if (!strcmp(token, "F")) {
            if (fim_encontrado++) goto limpar;
            fim = (CoordenadaDP){(int)(k / colunas), (int)(k % colunas)};
            valores[k] = 0;
        } else if (!parse_int(token, &valores[k])) {
            fprintf(stderr, "Valor da caverna invalido\n");
            goto limpar;
        }
    }
    if (inicio_encontrado != 1 || fim_encontrado != 1) {
        fprintf(stderr, "Faltando I ou F\n");
        goto limpar;
    }
    status = resolver_dp(linhas, colunas, valores, inicio, fim, (int64_t)vida_inicial, &resultado);
    if (status == DP_ERRO) {
        fprintf(stderr, "Erro no modulo DP\n");
        goto limpar;
    }
    if (status == DP_SEM_SOLUCAO) {
        puts("impossível");
    } else {
        size_t indice = (size_t)fim.linha * (size_t)colunas + (size_t)fim.coluna;
        size_t total = 0;
        size_t *caminho = malloc(quantidade * sizeof(*caminho));
        if (!caminho) goto limpar;
        while (total < quantidade) {
            caminho[total++] = indice;
            if (resultado.predecessor[indice] == DP_INICIO) break;
            if (resultado.predecessor[indice] < 0 ||
                (size_t)resultado.predecessor[indice] >= quantidade) {
                free(caminho);
                goto limpar;
            }
            indice = (size_t)resultado.predecessor[indice];
        }
        if (total == quantidade && resultado.predecessor[indice] != DP_INICIO) {
            free(caminho);
            goto limpar;
        }
        while (total > 0) {
            size_t atual = caminho[--total];
            printf("%zu %zu\n", atual / (size_t)colunas, atual % (size_t)colunas);
        }
        free(caminho);
    }
    erro = 0;
limpar:
    liberar_resultado_dp(&resultado);
    free(valores);
    fclose(arquivo);
    return erro ? 2 : 0;
}
