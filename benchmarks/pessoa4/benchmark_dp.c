/* Tempo de CPU da chamada completa resolver_dp (inclui alocacao e liberacao). */
#include "dp.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static uint32_t estado = 20261104u;
static uint32_t sorteio(void)
{
    estado ^= estado << 13;
    estado ^= estado >> 17;
    estado ^= estado << 5;
    return estado;
}

static double medir(int linhas, int colunas, int repeticoes, int *valores)
{
    clock_t inicio = clock();
    for (int k = 0; k < repeticoes; k++) {
        ResultadoDP resultado = {0};
        CoordenadaDP origem = {linhas - 1, colunas - 1};
        CoordenadaDP fim = {0, 0};
        int rc = resolver_dp(linhas, colunas, valores, origem, fim, 1000000000LL, &resultado);
        if (rc != DP_SUCESSO) {
            fprintf(stderr, "Falha no benchmark: status=%d\n", rc);
            liberar_resultado_dp(&resultado);
            exit(2);
        }
        liberar_resultado_dp(&resultado);
    }
    return (1000.0 * (double)(clock() - inicio) / CLOCKS_PER_SEC) / (double)repeticoes;
}

int main(void)
{
    const int lados[] = {32, 48, 64, 96, 128, 192, 256, 384, 512, 768, 1024};
    const int total = (int)(sizeof(lados) / sizeof(lados[0]));
    puts("celulas,linhas,colunas,tempo_ms");
    for (int i = 0; i < total; i++) {
        int n = lados[i];
        size_t quantidade = (size_t)n * (size_t)n;
        int *valores = malloc(quantidade * sizeof(*valores));
        if (!valores) return 2;
        for (size_t j = 0; j < quantidade; j++) {
            valores[j] = (int)(sorteio() % 7u) - 3;
        }
        valores[0] = valores[quantidade - 1] = 0;
        medir(n, n, 1, valores);
        int repeticoes = (n <= 128) ? 100 : (n <= 384 ? 25 : 7);
        double t1 = medir(n, n, repeticoes, valores);
        double t2 = medir(n, n, repeticoes, valores);
        double t3 = medir(n, n, repeticoes, valores);
        double ordenados[] = {t1, t2, t3};
        if (ordenados[0] > ordenados[1]) { double tmp=ordenados[0]; ordenados[0]=ordenados[1]; ordenados[1]=tmp; }
        if (ordenados[1] > ordenados[2]) { double tmp=ordenados[1]; ordenados[1]=ordenados[2]; ordenados[2]=tmp; }
        if (ordenados[0] > ordenados[1]) { double tmp=ordenados[0]; ordenados[0]=ordenados[1]; ordenados[1]=tmp; }
        printf("%zu,%d,%d,%.6f\n", quantidade, n, n, ordenados[1]);
        free(valores);
    }
    return 0;
}
