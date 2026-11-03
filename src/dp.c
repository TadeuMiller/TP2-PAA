#include "dp.h"

#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

static int coordenada_valida(int linha, int coluna, int linhas, int colunas)
{
    return linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas;
}

static int somar_valor(int64_t vida, int valor, int64_t *resultado)
{
    if ((valor > 0 && vida > INT64_MAX - valor) ||
        (valor < 0 && vida < INT64_MIN - valor)) {
        return 0;
    }

    *resultado = vida + valor;
    return 1;
}

void liberar_resultado_dp(ResultadoDP *resultado)
{
    if (resultado == NULL) {
        return;
    }

    free(resultado->vida);
    free(resultado->predecessor);
    resultado->vida = NULL;
    resultado->predecessor = NULL;
    resultado->linhas = 0;
    resultado->colunas = 0;
    resultado->vida_final = 0;
}

int resolver_dp(int linhas, int colunas, const int *valores,
                CoordenadaDP inicio, CoordenadaDP fim,
                int64_t vida_inicial, ResultadoDP *resultado)
{
    size_t quantidade;
    size_t indice_inicio;
    size_t indice_fim;
    int linha;
    int coluna;

    if (resultado == NULL || valores == NULL || linhas <= 0 || colunas <= 0 ||
        vida_inicial <= 0 || !coordenada_valida(inicio.linha, inicio.coluna, linhas, colunas) ||
        !coordenada_valida(fim.linha, fim.coluna, linhas, colunas)) {
        return DP_ERRO;
    }

    resultado->linhas = 0;
    resultado->colunas = 0;
    resultado->vida = NULL;
    resultado->predecessor = NULL;
    resultado->vida_final = 0;

    quantidade = (size_t)linhas * (size_t)colunas;
    if (quantidade > (size_t)INT_MAX || quantidade > SIZE_MAX / sizeof(*resultado->vida) ||
        quantidade > SIZE_MAX / sizeof(*resultado->predecessor)) {
        return DP_ERRO;
    }

    resultado->vida = calloc(quantidade, sizeof(*resultado->vida));
    resultado->predecessor = malloc(quantidade * sizeof(*resultado->predecessor));
    if (resultado->vida == NULL || resultado->predecessor == NULL) {
        liberar_resultado_dp(resultado);
        return DP_ERRO;
    }

    resultado->linhas = linhas;
    resultado->colunas = colunas;
    for (size_t i = 0; i < quantidade; i++) {
        resultado->predecessor[i] = DP_SEM_PREDECESSOR;
    }

    indice_inicio = (size_t)inicio.linha * (size_t)colunas + (size_t)inicio.coluna;
    indice_fim = (size_t)fim.linha * (size_t)colunas + (size_t)fim.coluna;
    if (!somar_valor(vida_inicial, valores[indice_inicio], &resultado->vida[indice_inicio])) {
        liberar_resultado_dp(resultado);
        return DP_ERRO;
    }
    if (resultado->vida[indice_inicio] <= 0) {
        resultado->vida[indice_inicio] = 0;
        return DP_SEM_SOLUCAO;
    }
    resultado->predecessor[indice_inicio] = DP_INICIO;

    for (linha = inicio.linha; linha >= 0; linha--) {
        for (coluna = inicio.coluna; coluna >= 0; coluna--) {
            size_t atual = (size_t)linha * (size_t)colunas + (size_t)coluna;
            int proxima_linha[2] = {linha - 1, linha};
            int proxima_coluna[2] = {coluna, coluna - 1};

            if (resultado->vida[atual] <= 0) {
                continue;
            }

            for (int movimento = 0; movimento < 2; movimento++) {
                int nova_linha = proxima_linha[movimento];
                int nova_coluna = proxima_coluna[movimento];
                size_t proxima;
                int64_t vida;

                if (!coordenada_valida(nova_linha, nova_coluna, linhas, colunas)) {
                    continue;
                }

                proxima = (size_t)nova_linha * (size_t)colunas + (size_t)nova_coluna;
                if (!somar_valor(resultado->vida[atual], valores[proxima], &vida)) {
                    liberar_resultado_dp(resultado);
                    return DP_ERRO;
                }
                if (vida > 0 && vida > resultado->vida[proxima]) {
                    resultado->vida[proxima] = vida;
                    resultado->predecessor[proxima] = (int)atual;
                }
            }
        }
    }

    if (resultado->vida[indice_fim] <= 0) {
        return DP_SEM_SOLUCAO;
    }

    resultado->vida_final = resultado->vida[indice_fim];
    return DP_SUCESSO;
}
