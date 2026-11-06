#ifndef CAVERNA_H
#define CAVERNA_H

typedef struct {
    int linhas;        /* numero de linhas da caverna */
    int colunas;       /* numero de colunas da caverna */
    int vidaInicial;   /* pontos de vida no comeco */
    int **valores;     /* matriz linhas x colunas (monstro < 0, pocao > 0, vazio = 0) */
    int linhaI;        /* posicao do inicio (I) */
    int colunaI;
    int linhaF;        /* posicao da saida (F) */
    int colunaF;
} Caverna;

/* Codigos de retorno de lerCaverna */
#define LEITURA_OK            0
#define ERRO_ARQUIVO         -1
#define ERRO_CABECALHO       -2
#define ERRO_MEMORIA         -3
#define ERRO_CELULA          -4
#define ERRO_SEM_I_OU_F      -5

/* Le o arquivo e preenche a caverna. Retorna LEITURA_OK ou um codigo de erro */
int lerCaverna(const char *caminho, Caverna *c);

/* Libera a matriz alocada por lerCaverna */
void liberarCaverna(Caverna *c);

/* Retorna 1 se (i, j) esta dentro da caverna, 0 caso contrario */
int dentroDaCaverna(const Caverna *c, int i, int j);

/* Retorna o valor da celula (i, j)*/
int valorCelula(const Caverna *c, int i, int j);

/* Escreve uma mensagem de erro de acordo com o codigo */
void imprimirErroLeitura(int codigo);

/* Copia a matriz para um vetor unico em ordem de linhas (indice i*colunas+j) */
int *achatarCaverna(const Caverna *c);

/* Imprime a caverna na tela */
void imprimirCaverna(const Caverna *c);


#endif