/* Teste de compatibilidade entre a leitura (Pessoa 1) e a programacao
   dinamica (Pessoa 2). Nao e o main do grupo: so mostra que
   lerCaverna + achatarCaverna + resolver_dp funcionam juntos.
   Uso: ./teste_integracao arquivo.txt */
#include <stdio.h>
#include <stdlib.h>
#include "caverna.h"
#include "dp.h"

int main(int argc, char *argv[]) {
    Caverna c;
    ResultadoDP res = {0};
    CoordenadaDP inicio, fim;
    int *vetor;
    int status, r, k, total, indice;
    int *caminho;

    if (argc != 2) {
        printf("Uso: %s arquivo_de_entrada\n", argv[0]);
        return 1;
    }

    r = lerCaverna(argv[1], &c);
    if (r != LEITURA_OK) {
        imprimirErroLeitura(r);
        return 1;
    }

    vetor = achatarCaverna(&c);
    if (vetor == NULL) {
        liberarCaverna(&c);
        return 1;
    }

    inicio.linha = c.linhaI;  inicio.coluna = c.colunaI;
    fim.linha = c.linhaF;     fim.coluna = c.colunaF;

    status = resolver_dp(c.linhas, c.colunas, vetor, inicio, fim, c.vidaInicial, &res);

    if (status == DP_ERRO) {
        printf("Erro no modulo de programacao dinamica.\n");
    } else if (status == DP_SEM_SOLUCAO) {
        printf("impossível\n");
    } else {
        /* segue os predecessores de F ate I e imprime na ordem certa */
        caminho = (int *) malloc((size_t) c.linhas * c.colunas * sizeof(int));
        if (caminho != NULL) {
            total = 0;
            indice = fim.linha * c.colunas + fim.coluna;
            while (indice != DP_INICIO) {
                caminho[total++] = indice;
                indice = res.predecessor[indice];
            }
            for (k = total - 1; k >= 0; k--) {
                printf("%d %d\n", caminho[k] / c.colunas, caminho[k] % c.colunas);
            }
            printf("vida final = %lld\n", (long long) res.vida_final);
            free(caminho);
        }
    }

    liberar_resultado_dp(&res);
    free(vetor);
    liberarCaverna(&c);
    return status == DP_ERRO ? 1 : 0;
}
