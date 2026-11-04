# Testes da Pessoa 4 — TP2 de PAA

Este diretório é independente do `main.c` do grupo. **Não altera** `src/dp.c`, `include/dp.h` ou os arquivos de outros integrantes.

## Conteúdo

- `casos/`: 30 entradas fixas em formato idêntico ao do enunciado.
- `esperados/`: uma saída válida de referência por caso. **Empates podem gerar caminhos diferentes e igualmente corretos.**
- `manifesto.json`: descrição, possibilidade de solução e vida final ótima de cada caso.
- `oraculo.py`: verificador independente por enumeração de todos os caminhos para cavernas pequenas.
- `gerar_casos.py`: reproduz as 30 entradas e seus gabaritos.
- `driver_dp.c`: executa apenas o módulo `resolver_dp`, sem assumir que o `main.c` do grupo esteja pronto.
- `verificar.py`: testa saída, legalidade dos movimentos, vida em todas as células e otimalidade.

## Teste atual (módulo isolado)

Na raiz do repositório, com Python 3 e GCC disponíveis:

```bash
python tests/pessoa4/verificar.py
```

O script compila `src/dp.c` com o driver de teste e executa **30 casos fixos e 150 casos pseudoaleatórios** (`seed=20261104`). A verificação dos casos aleatórios utiliza o oráculo independente.

## Quando o programa final estiver disponível

Compile o programa do grupo usando o `Makefile` dos colegas. Em seguida:

```bash
python tests/pessoa4/verificar.py --executavel ./programa
```

No Windows, substitua o executável por um caminho para o `.exe` (por exemplo, `--executavel .\programa.exe`). O teste executará cada entrada em diretório temporário e lerá o arquivo `resultado.txt` criado pelo programa integrado.

## Critérios de aceitação

- Caminho começa em `I` e termina em `F`.
- Cada movimento é exclusivamente para cima ou esquerda.
- As coordenadas estão dentro da caverna e são baseadas em zero.
- A vida permanece **estritamente maior que zero**, inclusive nas células intermediárias e em `F`.
- A vida final é máxima entre todos os caminhos possíveis.
- Em casos sem solução, o conteúdo da saída é somente `impossível`.
- Caminhos empatados não são comparados caractere por caractere com um único gabarito.

**Atenção:** a versão isolada testa o algoritmo e a reconstrução do driver auxiliar. Ela **não comprova** ainda o comportamento do `main.c`, sua interface de linha de comando ou a geração de `resultado.txt` pelo programa definitivo.
