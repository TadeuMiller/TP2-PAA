#!/usr/bin/env python3
"""Plota os tempos medidos do modulo DP e uma referencia linear."""
import csv
from pathlib import Path
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

BASE = Path(__file__).resolve().parent
ARQUIVO = BASE / "tempos_modulo_dp.csv"
LINHA = BASE.parents[1] / "docs" / "figuras" / "pessoa4_complexidade.png"

with ARQUIVO.open(encoding="utf-8") as f:
    dados = list(csv.DictReader(f))
x = [int(d["celulas"]) for d in dados]
y = [float(d["tempo_ms"]) for d in dados]
referencia = [y[-1] * n / x[-1] for n in x]
plt.figure(figsize=(9.1, 5.4))
plt.plot(x, y, "o-", label="Tempo medido do módulo DP", linewidth=2)
plt.plot(x, referencia, "--", label="Referência O(L × C) ajustada ao último ponto", alpha=0.75)
plt.xlabel("Tamanho da entrada: linhas × colunas (células)")
plt.ylabel("Tempo médio da chamada DP (ms)")
plt.title("TP2 de PAA — Medição da programação dinâmica")
plt.ticklabel_format(style="sci", axis="x", scilimits=(0,0))
plt.grid(True, alpha=0.25)
plt.legend()
plt.tight_layout()
LINHA.parent.mkdir(parents=True, exist_ok=True)
plt.savefig(LINHA, dpi=180)
print(LINHA)
