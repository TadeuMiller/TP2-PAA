/*
 * Gerador de arquivos de entrada (Pessoa 1).
 * Cria um arquivo no formato aceito pelo programa principal.
 *
 * Uso:
 *   ./gerador <linhas> <colunas> <vida> <valorMin> <valorMax> <arquivo_saida> [semente [linhaI colunaI linhaF colunaF]]
 *
 * Parametros:
 *   linhas, colunas    dimensoes da caverna (inteiros > 0)
 *   vida               vida inicial do estudante
 *   valorMin, valorMax intervalo dos valores das celulas (inclusive). Ex: -20 e 20
 *   arquivo_saida      nome do arquivo que sera criado
 *   semente            (opcional) semente do rand, para gerar sempre o mesmo arquivo.
 *                      Se nao for informada, usa o horario atual.
 *   linhaI colunaI     (opcional) posicao do inicio. Padrao: canto inferior direito
 *   linhaF colunaF     (opcional) posicao da saida.  Padrao: canto superior esquerdo
 *
 * Valores positivos sao gravados com sinal + (como no exemplo do enunciado).
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mostrarUso(const char *nome) {
    printf("Uso: %s <linhas> <colunas> <vida> <valorMin> <valorMax> <arquivo_saida> [semente [linhaI colunaI linhaF colunaF]]\n", nome);
    printf("Exemplo: %s 4 5 40 -20 20 caverna.txt 1\n", nome);
}

int main(int argc, char *argv[]) {
    int linhas, colunas, vida, vMin, vMax;
    int linhaI, colunaI, linhaF, colunaF;
    unsigned int semente;
    int i, j, valor;
    FILE *arq;

    if (argc != 7 && argc != 8 && argc != 12) {
        mostrarUso(argv[0]);
        return 1;
    }

    linhas  = atoi(argv[1]);
    colunas = atoi(argv[2]);
    vida    = atoi(argv[3]);
    vMin    = atoi(argv[4]);
    vMax    = atoi(argv[5]);

    if (linhas <= 0 || colunas <= 0) {
        printf("Erro: linhas e colunas devem ser maiores que zero.\n");
        return 1;
    }
    if (vMin > vMax) {
        printf("Erro: valorMin nao pode ser maior que valorMax.\n");
        return 1;
    }

    if (argc >= 8) semente = (unsigned int) atoi(argv[7]);
    else           semente = (unsigned int) time(NULL);
    srand(semente);

    /* posicoes padrao: I embaixo a direita, F em cima a esquerda */
    linhaI = linhas - 1;
    colunaI = colunas - 1;
    linhaF = 0;
    colunaF = 0;
    if (argc == 12) {
        linhaI  = atoi(argv[8]);
        colunaI = atoi(argv[9]);
        linhaF  = atoi(argv[10]);
        colunaF = atoi(argv[11]);
    }

    if (linhaI < 0 || linhaI >= linhas || colunaI < 0 || colunaI >= colunas ||
        linhaF < 0 || linhaF >= linhas || colunaF < 0 || colunaF >= colunas) {
        printf("Erro: posicao de I ou F fora da caverna.\n");
        return 1;
    }
    if (linhaI == linhaF && colunaI == colunaF) {
        printf("Erro: I e F nao podem ficar na mesma celula.\n");
        return 1;
    }

    arq = fopen(argv[6], "w");
    if (arq == NULL) {
        printf("Erro: nao foi possivel criar o arquivo %s.\n", argv[6]);
        return 1;
    }

    fprintf(arq, "%d %d %d\n", linhas, colunas, vida);
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            if (i == linhaI && j == colunaI) {
                fprintf(arq, "I");
            } else if (i == linhaF && j == colunaF) {
                fprintf(arq, "F");
            } else {
                valor = vMin + rand() % (vMax - vMin + 1);
                if (valor > 0) fprintf(arq, "+%d", valor);
                else           fprintf(arq, "%d", valor);
            }
            if (j < colunas - 1) fprintf(arq, " ");
        }
        fprintf(arq, "\n");
    }

    fclose(arq);
    printf("Arquivo %s gerado (%d x %d, vida %d, semente %u).\n", argv[6], linhas, colunas, vida, semente);
    return 0;
}
