# Experimento de desempenho — Pessoa 4

O teste mede a chamada completa `resolver_dp`, incluindo alocação e liberação das estruturas alocadas pelo módulo. Não inclui leitura do arquivo nem escrita de `resultado.txt`, pois o programa principal ainda não foi integrado.

## Reprodução

Na raiz do repositório:

```bash
python benchmarks/pessoa4/executar.py
```

Requer Python 3, GCC e Matplotlib (`python -m pip install matplotlib`). O programa compila o benchmark em C, executa três lotes por tamanho, usa a **mediana das três médias** e grava:

- `benchmarks/pessoa4/tempos_modulo_dp.csv`
- `docs/figuras/pessoa4_complexidade.png`

Os tamanhos variam de 32×32 a 1024×1024. As entradas são geradas em memória com valores determinísticos de -3 a +3 e 1 bilhão de pontos de vida iniciais, evitando mortes precoces que encurtariam o percurso. O início é o canto inferior direito e o final o superior esquerdo.

A medida usa `clock()` da biblioteca padrão C (tempo de CPU), compilação `gcc -O2`. A memória é realocada a cada execução. Os tempos variam com o equipamento, compilador, temperatura e carga do sistema.

## Interpretação

O eixo x mostra a quantidade de células `L × C` e o eixo y o tempo médio de execução, em milissegundos. A linha pontilhada é apenas uma referência `O(L × C)` normalizada no último ponto, não uma regressão estatística. A tendência é compatível com o custo assintótico linear no número de células, mas pode haver desvios devido a alocação, cache e ruído de medição.

O tempo **total do programa**, incluindo E/S, deverá ser medido novamente caso o grupo queira afirmar a conformidade literal com o extra 5 do enunciado.
