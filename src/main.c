#include "caverna.h"
#include "dp.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int gravar_resultado(int status, const Caverna *caverna,
                            const ResultadoDP *resultado)
{
    FILE *arquivo;
    size_t quantidade;
    size_t *caminho = NULL;
    size_t total = 0;
    size_t indice;
    int encontrou_inicio = 0;
    int sucesso = 0;

    arquivo = fopen("resultado.txt", "w");
    if (arquivo == NULL) {
        perror("resultado.txt");
        return 0;
    }

    if (status == DP_SEM_SOLUCAO) {
        sucesso = fputs("impossível\n", arquivo) >= 0;
        goto finalizar;
    }

    quantidade = (size_t)caverna->linhas * (size_t)caverna->colunas;
    if (quantidade > SIZE_MAX / sizeof(*caminho)) {
        fprintf(stderr, "Dimensoes da caverna muito grandes.\n");
        goto finalizar;
    }

    caminho = malloc(quantidade * sizeof(*caminho));
    if (caminho == NULL) {
        fprintf(stderr, "Nao foi possivel alocar memoria para o caminho.\n");
        goto finalizar;
    }

    indice = (size_t)caverna->linhaF * (size_t)caverna->colunas +
             (size_t)caverna->colunaF;
    while (total < quantidade) {
        int predecessor;

        caminho[total++] = indice;
        predecessor = resultado->predecessor[indice];
        if (predecessor == DP_INICIO) {
            encontrou_inicio = 1;
            break;
        }
        if (predecessor < 0 || (size_t)predecessor >= quantidade) {
            fprintf(stderr, "Predecessores invalidos na solucao.\n");
            goto finalizar;
        }
        indice = (size_t)predecessor;
    }

    if (!encontrou_inicio) {
        fprintf(stderr, "Nao foi possivel reconstruir o caminho.\n");
        goto finalizar;
    }

    while (total > 0) {
        size_t posicao = caminho[--total];
        if (fprintf(arquivo, "%zu %zu\n",
                    posicao / (size_t)caverna->colunas,
                    posicao % (size_t)caverna->colunas) < 0) {
            goto finalizar;
        }
    }
    sucesso = 1;

finalizar:
    free(caminho);
    if (fclose(arquivo) != 0) {
        perror("resultado.txt");
        sucesso = 0;
    }
    return sucesso;
}

int main(int argc, char *argv[])
{
    Caverna caverna = {0};
    ResultadoDP resultado = {0};
    CoordenadaDP inicio;
    CoordenadaDP fim;
    int *valores = NULL;
    int leitura;
    int status;
    int codigo_saida = EXIT_FAILURE;

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_da_caverna>\n", argv[0]);
        return EXIT_FAILURE;
    }

    leitura = lerCaverna(argv[1], &caverna);
    if (leitura != LEITURA_OK) {
        imprimirErroLeitura(leitura);
        return EXIT_FAILURE;
    }

    valores = achatarCaverna(&caverna);
    if (valores == NULL) {
        fprintf(stderr, "Nao foi possivel alocar memoria para a caverna.\n");
        goto finalizar;
    }

    inicio.linha = caverna.linhaI;
    inicio.coluna = caverna.colunaI;
    fim.linha = caverna.linhaF;
    fim.coluna = caverna.colunaF;

    status = resolver_dp(caverna.linhas, caverna.colunas, valores,
                         inicio, fim, caverna.vidaInicial, &resultado);
    if (status == DP_ERRO) {
        fprintf(stderr, "Erro ao calcular o caminho.\n");
        goto finalizar;
    }

    if (!gravar_resultado(status, &caverna, &resultado)) {
        goto finalizar;
    }
    codigo_saida = EXIT_SUCCESS;

finalizar:
    liberar_resultado_dp(&resultado);
    free(valores);
    liberarCaverna(&caverna);
    return codigo_saida;
}
