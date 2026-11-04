#!/usr/bin/env python3
"""Compila, mede, salva CSV e produz o gráfico. Requer GCC e matplotlib."""
import subprocess
import sys
import tempfile
from pathlib import Path

BASE = Path(__file__).resolve().parent
RAIZ = BASE.parents[1]
with tempfile.TemporaryDirectory(prefix="paa_bench_") as tmp:
    binario = Path(tmp) / "benchmark_dp"
    cmd = ["gcc", "-std=c11", "-O2", "-Wall", "-Wextra", "-I", str(RAIZ / "include"),
           str(RAIZ / "src" / "dp.c"), str(BASE / "benchmark_dp.c"), "-o", str(binario)]
    subprocess.run(cmd, check=True)
    saida = subprocess.run([str(binario)], check=True, capture_output=True, text=True).stdout
    (BASE / "tempos_modulo_dp.csv").write_text(saida, encoding="utf-8")
    print(saida)
    subprocess.run([sys.executable, str(BASE / "plotar.py")], check=True)
