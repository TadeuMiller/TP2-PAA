# Pessoa 3 — Integração, execução e saída

## Programa principal

O executável recebe exatamente um argumento: o caminho do arquivo da caverna. O módulo de leitura preenche a estrutura `Caverna`; em seguida, `achatarCaverna` cria o vetor de valores em ordem de linhas esperado por `resolver_dp`. As posições de `I` e `F` são passadas ao algoritmo como coordenadas com índices iniciados em zero.

Quando há solução, o programa parte de `F`, percorre a tabela de predecessores até `I` e grava as coordenadas na ordem do percurso. O arquivo `resultado.txt` é criado no diretório em que o executável foi iniciado. Cada linha contém `linha coluna`. Se não houver caminho válido, o arquivo contém somente `impossível`.

O caminho é reconstruído em memória antes de ser gravado. O programa libera a memória da caverna, do vetor achatado, das tabelas da programação dinâmica e do caminho antes de encerrar.

## Compilação e execução

Na raiz do repositório, com GCC e GNU Make instalados:

```sh
make
./programa caminho/para/caverna.txt
```

No PowerShell, a execução pode ser feita assim:

```powershell
make
.\programa.exe caminho\para\caverna.txt
```

Em ambos os casos, `resultado.txt` aparece no diretório atual. Para remover o executável:

```sh
make clean
```

## Arquivos integrados

- `src/main.c`: interface de linha de comando, integração dos módulos e gravação da saída.
- `src/caverna.c` e `include/caverna.h`: leitura e representação da entrada.
- `src/dp.c` e `include/dp.h`: programação dinâmica e predecessores.
- `Makefile`: compilação do programa principal.

O Makefile compila somente o programa básico do trabalho. O gerador de entradas existente continua separado e não é necessário para executar a solução principal.
