# Pessoa 2 — Programação dinâmica e complexidade

## Objetivo

Encontrar um caminho da posição `I` até `F`, movendo somente para cima ou para a esquerda, que mantenha a vida estritamente positiva em todas as células e maximize a vida restante em `F`.

## Estado e recorrência

Para cada célula `(i, j)`, `vida[i][j]` guarda a maior quantidade de vida com que é possível chegar até ela sem ter ficado com vida menor ou igual a zero. O valor zero representa uma célula inalcançável por um caminho válido.

Na posição inicial, a vida é a vida inicial somada ao valor da própria célula. Para cada célula alcançável, são avaliados os movimentos para `(i - 1, j)` e `(i, j - 1)`. Para uma célula seguinte `v`, calcula-se `candidata = vida[i][j] + valor[v]`. A candidata só é considerada quando é maior que zero e supera a melhor vida já registrada para `v`. Ao atualizar a melhor vida, também é guardado o índice da célula de origem em `predecessor[v]`.

Como os movimentos sempre reduzem a linha ou a coluna, o grafo de movimentos não possui ciclos. O preenchimento percorre as linhas e colunas em ordem decrescente a partir de `I`, de modo que cada célula seja processada depois de receber as atualizações de baixo e da direita.

## Correção da escolha

Todos os caminhos que chegam à mesma célula recebem os mesmos valores nas células seguintes. Portanto, entre caminhos válidos que chegam a uma célula, o que chega com mais vida nunca terá desempenho inferior ao que chega com menos vida: ele pode seguir qualquer continuação viável do outro caminho e terminar com vida igual ou maior. Assim, basta guardar a maior vida por célula, sem perder uma solução ótima.

## Reconstrução do caminho

`ResultadoDP.predecessor` tem uma entrada por célula, em ordem de linhas. Cada valor é o índice linear da célula anterior no caminho. `DP_INICIO` marca a posição inicial e `DP_SEM_PREDECESSOR` marca células sem caminho válido. A integração pode partir de `F`, seguir os predecessores até `I` e inverter a sequência. O índice linear de `(i, j)` é `i * colunas + j`.

`resolver_dp` retorna `DP_SUCESSO` se `F` for alcançável com vida positiva, `DP_SEM_SOLUCAO` caso contrário e `DP_ERRO` para argumentos inválidos, falha de alocação ou soma fora do intervalo de `int64_t`. O chamador deve iniciar `ResultadoDP` com `{0}` e liberar as tabelas alocadas com `liberar_resultado_dp` após sucesso ou ausência de solução.

## Estruturas e complexidade

O módulo recebe os valores em um vetor contíguo em ordem de linhas, junto com dimensões, posições inicial e final e vida inicial. `ResultadoDP` fornece as tabelas de vida e predecessores, além da vida final quando há solução. As posições `I` e `F`, por serem marcadores sem valor numérico no formato do trabalho, devem ser representadas com valor zero no vetor.

Para uma caverna de `L` linhas e `C` colunas, o tempo é `O(L × C)` e o espaço adicional é `O(L × C)`. A implementação percorre apenas a região acima e à esquerda de `I`, portanto pode visitar menos células, sem alterar esses limites superiores.

## Integração pendente

A interface usa vetor de inteiros em ordem de linhas para não depender da representação escolhida pelo módulo de leitura. Antes da integração, combinar com as Pessoas 1 e 3 se essa representação atende ao projeto e se os marcadores `I` e `F` serão convertidos em valor zero. Paredes e movimentos nas quatro direções são extras fora desta implementação básica.
