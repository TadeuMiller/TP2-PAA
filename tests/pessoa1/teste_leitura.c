/* Programa de teste da leitura (Pessoa 1).
   Le o arquivo, imprime a caverna e confere se o vetor achatado
   (usado pela programacao dinamica) tem os mesmos valores da matriz.
   Uso: ./teste_leitura arquivo.txt */
#include <stdio.h>
#include <stdlib.h>
#include "caverna.h"

int main(int argc, char *argv[]) {
    Caverna c;
    int r, i, j;
    int *vetor;
    int iguais = 1;

    if (argc != 2) {
        printf("Uso: %s arquivo_de_entrada\n", argv[0]);
        return 1;
    }

    r = lerCaverna(argv[1], &c);
    if (r != LEITURA_OK) {
        imprimirErroLeitura(r);
        return 1;
    }

    imprimirCaverna(&c);

    vetor = achatarCaverna(&c);
    if (vetor == NULL) {
        printf("Erro: falta de memoria ao achatar a caverna.\n");
        liberarCaverna(&c);
        return 1;
    }
    for (i = 0; i < c.linhas; i++) {
        for (j = 0; j < c.colunas; j++) {
            if (vetor[i * c.colunas + j] != valorCelula(&c, i, j)) iguais = 0;
        }
    }
    printf("Vetor achatado: %s\n", iguais ? "confere com a matriz" : "DIFERENTE da matriz");

    free(vetor);
    liberarCaverna(&c);
    return iguais ? 0 : 1;
}
