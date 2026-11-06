# Pessoa 1 — Entrada e estruturas de dados

## 1. Formato de entrada

O programa recebe por linha de comando o caminho de um arquivo texto com a caverna. A primeira linha tem três inteiros separados por espaço: número de linhas, número de colunas e vida inicial. As linhas seguintes descrevem a caverna, uma linha do arquivo para cada linha da caverna, com as células separadas por espaço. Cada célula pode ser:

- um número inteiro, com ou sem sinal (`-20`, `0`, `+20`): negativo é monstro (tira vida), positivo é poção (dá vida) e zero é célula vazia;
- a letra `I`, que marca a posição inicial do estudante;
- a letra `F`, que marca a saída.

O exemplo do enunciado (`caverna1.txt`) tem 4 linhas, 5 colunas e vida inicial 40:

```
4 5 40
-20 0 -20 0 0
F -10 -10 -10 -10
-20 0 0 0 +20
0 0 -10 0 I
```

A caverna pode ser retangular (linhas diferentes de colunas). O enunciado diz que os arquivos seguem fielmente o formato, então a leitura não trata variações do formato, mas foram colocadas algumas verificações simples de erro (seção 5).

## 2. Estruturas de dados

Toda a caverna fica em uma única `struct`, definida em `include/caverna.h`:

```c
typedef struct {
    int linhas;
    int colunas;
    int vidaInicial;
    int **valores;
    int linhaI, colunaI;
    int linhaF, colunaF;
} Caverna;
```

- `linhas`, `colunas` e `vidaInicial` vêm da primeira linha do arquivo.
- `valores` é uma matriz alocada dinamicamente: um vetor de ponteiros em que cada ponteiro aponta para uma linha com `colunas` inteiros. Assim o tamanho só precisa ser conhecido em tempo de execução e a matriz pode ser retangular.
- `linhaI`/`colunaI` e `linhaF`/`colunaF` guardam onde estão o início e a saída. Nas posições de `I` e `F` a matriz guarda o valor 0, pois essas células não tiram nem dão vida. Com isso quem usa a matriz (a programação dinâmica) só lida com números, sem caracteres.

Os índices começam em zero, como pede o formato de saída: `valores[i][j]` é a célula da linha `i` e coluna `j`, e o canto superior esquerdo é `(0, 0)`.

## 3. Implementação

O módulo tem `include/caverna.h` (declarações) e `src/caverna.c` (implementação).

**Leitura (`lerCaverna`).** O arquivo é aberto com `fopen` e a primeira linha é lida com `fscanf("%d %d %d")`. Depois a matriz é alocada e cada célula é lida como texto (`fscanf("%s")`), porque só dá para saber se ela é número, `I` ou `F` depois de lida. Se o texto for `I` ou `F`, a posição é guardada e a célula recebe 0. Caso contrário, o texto é convertido com `strtol`, que aceita `-10` e também `+20` (o sinal `+` aparece no exemplo do enunciado). Se sobrar algum caractere que não seja número depois da conversão, a célula é considerada inválida. O arquivo é percorrido uma única vez, então a leitura leva tempo `O(L × C)` e usa espaço `O(L × C)` para a matriz.

**Alocação e liberação.** `alocarMatriz` faz um `malloc` para o vetor de ponteiros e um `calloc` para cada linha. Se alguma alocação falhar, as linhas que já tinham sido alocadas são liberadas antes de retornar o erro, para não vazar memória. `liberarCaverna` dá `free` em cada linha e depois no vetor de ponteiros, e coloca `valores = NULL`. Se `lerCaverna` retornar erro, ela mesma já liberou tudo o que alocou, então não é preciso chamar `liberarCaverna` nesse caso.

**Vetor para a programação dinâmica (`achatarCaverna`).** O módulo da Pessoa 2 (`resolver_dp`) recebe os valores em um vetor único, em ordem de linhas, e não em uma matriz de ponteiros. Por isso foi feita a função `achatarCaverna`, que aloca um vetor com `linhas * colunas` inteiros e copia a matriz para ele, de forma que a célula `(i, j)` fique na posição `i * colunas + j`. Essa é a mesma fórmula de índice linear usada pela Pessoa 2 nos predecessores. A função retorna `NULL` se faltar memória, e quem a chamou deve dar `free` no vetor.

## 4. Interface com as Pessoas 2 e 3

| Função | O que faz |
|---|---|
| `int lerCaverna(const char *caminho, Caverna *c)` | Lê o arquivo. Retorna `LEITURA_OK` (0) ou um código de erro negativo |
| `void liberarCaverna(Caverna *c)` | Libera a memória da matriz |
| `int *achatarCaverna(const Caverna *c)` | Copia a matriz para um vetor em ordem de linhas (`i * colunas + j`) |
| `int dentroDaCaverna(const Caverna *c, int i, int j)` | Retorna 1 se `(i, j)` está dentro da caverna |
| `int valorCelula(const Caverna *c, int i, int j)` | Retorna o valor da célula (0 em `I` e `F`) |
| `void imprimirErroLeitura(int codigo)` | Mostra a mensagem do erro |
| `void imprimirCaverna(const Caverna *c)` | Imprime a caverna (usado nos testes) |

**Como o `main` da Pessoa 3 usa o módulo junto com `resolver_dp`:**

```c
Caverna c;
ResultadoDP res = {0};
CoordenadaDP inicio, fim;

if (lerCaverna(argv[1], &c) != LEITURA_OK) { /* tratar erro */ }
int *vetor = achatarCaverna(&c);

inicio.linha = c.linhaI;  inicio.coluna = c.colunaI;
fim.linha    = c.linhaF;  fim.coluna    = c.colunaF;

int status = resolver_dp(c.linhas, c.colunas, vetor, inicio, fim, c.vidaInicial, &res);
/* ... reconstruir o caminho com res.predecessor e gravar resultado.txt ... */

liberar_resultado_dp(&res);
free(vetor);
liberarCaverna(&c);
```

A vida inicial (`int`) é convertida sozinha para o `int64_t` que `resolver_dp` espera. O arquivo `tests/pessoa1/teste_integracao.c` é um exemplo completo desse uso (sem gravar o `resultado.txt`) e serve de base para o `main`.

**Decisão sobre `I` e `F`.** O enunciado diz que o estudante pode começar de qualquer ponto das bordas, mas também diz que a posição inicial é a marcada com `I`. Como a entrada traz um `I` explícito, o grupo adotou que o caminho começa na célula `I` lida do arquivo; a leitura só guarda essa posição, e `I` e `F` podem estar em qualquer célula (não só nas bordas). Isso deve ser confirmado com o professor. Se for preciso mudar, a leitura continua igual e só o módulo da Pessoa 2 é afetado.

## 5. Tratamento de erros

`lerCaverna` retorna um código para cada problema:

| Código | Significado |
|---|---|
| `ERRO_ARQUIVO` | o arquivo não pôde ser aberto |
| `ERRO_CABECALHO` | primeira linha inválida ou com linhas/colunas menores que 1 |
| `ERRO_MEMORIA` | falta de memória na alocação |
| `ERRO_CELULA` | célula que não é número, `I` nem `F`, ou arquivo com células faltando |
| `ERRO_SEM_I_OU_F` | o arquivo não tem `I` ou não tem `F` |

## 6. Extra: gerador de arquivos de entrada

Para facilitar os testes (principalmente os de tempo da Pessoa 4, que precisam de cavernas de vários tamanhos), foi feito um programa à parte, `src/gerador.c`. Ele tem seu próprio `main` e não faz parte do programa principal, então não interfere na execução básica.

```
./gerador <linhas> <colunas> <vida> <valorMin> <valorMax> <arquivo_saida> [semente [linhaI colunaI linhaF colunaF]]
```

| Parâmetro | Obrigatório | Descrição |
|---|---|---|
| `linhas`, `colunas` | sim | Dimensões da caverna (maiores que zero). Podem ser diferentes |
| `vida` | sim | Vida inicial, escrita na primeira linha do arquivo |
| `valorMin`, `valorMax` | sim | Intervalo (inclusive) dos valores sorteados. Ex.: `-20 20` |
| `arquivo_saida` | sim | Nome do arquivo que será criado |
| `semente` | não | Semente do `rand`. Com a mesma semente e os mesmos parâmetros o arquivo é sempre igual. Sem ela, usa o horário |
| `linhaI colunaI` | não | Posição do `I`. Padrão: canto inferior direito |
| `linhaF colunaF` | não | Posição do `F`. Padrão: canto superior esquerdo |

As posições só podem ser informadas juntas (os quatro valores) e depois da semente.

**Funcionamento.** O programa confere os argumentos (dimensões positivas, `valorMin <= valorMax`, `I` e `F` dentro da caverna e em células diferentes) e, se algo estiver errado, mostra a mensagem e não cria o arquivo. Depois passa a semente ao `srand`, grava a primeira linha e, para cada célula, grava `I` ou `F` na posição correspondente ou sorteia um valor com `valorMin + rand() % (valorMax - valorMin + 1)`. Valores positivos são gravados com sinal `+`, como no exemplo do enunciado, o que também testa a leitura desse formato.

A posição padrão (`I` embaixo à direita, `F` em cima à esquerda) foi escolhida porque os movimentos permitidos são só para cima e para a esquerda. Informando as posições à mão dá para gerar casos sem solução geométrica (por exemplo, `F` à direita ou abaixo do `I`). O gerador também não garante vida suficiente: com monstros fortes e vida inicial baixa a caverna pode dar "impossível", o que é útil como caso de teste.

Exemplo: `./gerador 4 5 40 -20 20 gerado_4x5.txt 1` gera

```
4 5 40
F -15 +16 -20 +3
-13 -1 -14 +12 +3
-3 +7 -14 -3 -14
-12 +2 -6 -1 I
```

## 7. Como compilar e executar

Na raiz do projeto:

```bash
# teste da leitura
gcc -std=c99 -Wall -Wextra -Iinclude -o teste_leitura tests/pessoa1/teste_leitura.c src/caverna.c
./teste_leitura tests/pessoa1/entradas/caverna1.txt

# teste de integração leitura + programação dinâmica
gcc -std=c99 -Wall -Wextra -Iinclude -o teste_integracao tests/pessoa1/teste_integracao.c src/caverna.c src/dp.c
./teste_integracao tests/pessoa1/entradas/caverna1.txt

# gerador
gcc -std=c99 -Wall -Wextra -Iinclude -o gerador src/gerador.c
./gerador 4 5 40 -20 20 caverna_gerada.txt 1
```

A opção `-Iinclude` é necessária porque os `.c` usam `#include "caverna.h"`.

## 8. Testes realizados

Os arquivos usados estão em `tests/pessoa1/entradas/` (os gerados, em `gerados/`).

**Leitura.** O `teste_leitura` lê o arquivo, imprime a caverna e confere se o vetor de `achatarCaverna` tem os mesmos valores da matriz.

| Arquivo | Objetivo | Resultado |
|---|---|---|
| `caverna1.txt` | Exemplo do enunciado (4x5, com `+20`) | Correto: `I` em (3,4) e `F` em (1,0) |
| `retangular_3x6.txt` | Mais colunas que linhas | Correto |
| `retangular_5x2.txt` | Mais linhas que colunas | Correto |
| `linha_unica.txt` | Caverna 1x5 | Correto |
| `coluna_unica.txt` | Caverna 4x1 | Correto |
| `gerados/gerado_4x5.txt` | Gerado com semente 1 | Correto |
| `gerados/gerado_6x9.txt` | Retangular, `I` e `F` informados | Correto |
| `gerados/gerado_8x3_IF_meio.txt` | `I` e `F` fora dos cantos | Correto |
| `gerados/gerado_1x6.txt` | Linha única gerada | Correto |
| `erro_sem_I_F.txt` | Arquivo sem `I` e `F` | Erro detectado (`ERRO_SEM_I_OU_F`) |
| `erro_celula.txt` | Célula com texto `abc` | Erro detectado (`ERRO_CELULA`) |
| `erro_faltando.txt` | Faltam células | Erro detectado (`ERRO_CELULA`) |

Saída para `caverna1.txt`:

```
Caverna 4 x 5, vida inicial = 40
I em (3, 4) | F em (1, 0)
  -20    0  -20    0    0
    F  -10  -10  -10  -10
  -20    0    0    0   20
    0    0  -10    0    I
Vetor achatado: confere com a matriz
```

**Integração com a programação dinâmica.** O `teste_integracao` lê a caverna, achata a matriz e chama `resolver_dp`. Para `caverna1.txt` o resultado foi o caminho abaixo, com vida final 50 (a mesma da solução do enunciado, que usa o par `4 4` no início, sendo que aqui os índices começam em zero):

```
3 4
2 4
2 3
2 2
2 1
1 1
1 0
```

Nos 30 casos da Pessoa 4 (`tests/pessoa4/casos/`), a combinação leitura + achatamento + `resolver_dp` deu a vida final máxima esperada em todos os casos com solução e `impossível` nos demais (30 de 30 de acordo com o `manifesto.json`). Os testes também foram compilados com AddressSanitizer e UndefinedBehaviorSanitizer, sem nenhum erro de memória. No `retangular_3x6.txt` o resultado é `impossível` porque o `F` está abaixo e à direita do `I`, e só é permitido ir para cima ou para a esquerda.

**Gerador.** Verificou-se que os arquivos gerados são lidos corretamente, que a mesma semente produz exatamente o mesmo arquivo e que os erros de parâmetros (dimensão zero, `valorMin > valorMax`, `I` igual a `F`) são detectados.
