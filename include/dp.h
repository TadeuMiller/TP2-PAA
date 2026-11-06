#ifndef DP_H
#define DP_H

#include <stdint.h>

typedef struct {
    int linha;
    int coluna;
} CoordenadaDP;

typedef struct {
    int linhas;
    int colunas;
    int64_t *vida;
    int *predecessor;
    int64_t vida_final;
} ResultadoDP;

enum {
    DP_ERRO = -1,
    DP_SEM_SOLUCAO = 0,
    DP_SUCESSO = 1
};

#define DP_SEM_PREDECESSOR (-2)
#define DP_INICIO (-1)

int resolver_dp(int linhas, int colunas, const int *valores, CoordenadaDP inicio, CoordenadaDP fim, int64_t vida_inicial, ResultadoDP *resultado);
void liberar_resultado_dp(ResultadoDP *resultado);

#endif
