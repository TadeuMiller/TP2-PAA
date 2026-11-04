#!/usr/bin/env python3
"""Executa testes fixos e aleatorios com oraculo independente por forca bruta."""
import argparse
import json
import random
import subprocess
import sys
import tempfile
from pathlib import Path
from oraculo import ler_caverna, oraculo_exaustivo, validar_saida

BASE = Path(__file__).resolve().parent
RAIZ = BASE.parents[1]


def compilar_driver(destino):
    cmd = ["gcc", "-std=c11", "-O2", "-Wall", "-Wextra", "-Wpedantic",
           "-I", str(RAIZ / "include"), str(RAIZ / "src" / "dp.c"),
           str(BASE / "driver_dp.c"), "-o", str(destino)]
    proc = subprocess.run(cmd, capture_output=True, text=True)
    if proc.returncode:
        raise RuntimeError(f"Falha na compilacao:\n{proc.stderr}")
    return proc.stderr


def executar(executavel, arquivo, modo, pasta):
    if modo == "integrado":
        destino = pasta / "resultado.txt"
        destino.unlink(missing_ok=True)
    proc = subprocess.run([str(executavel), str(arquivo)], cwd=pasta,
                          capture_output=True, text=True, timeout=20)
    if proc.returncode:
        return None, f"processo encerrou com codigo {proc.returncode}: {proc.stderr.strip()}"
    if modo == "integrado":
        if not destino.exists():
            return None, "arquivo resultado.txt nao foi criado"
        return destino.read_text(encoding="utf-8-sig"), None
    return proc.stdout, None


def casos_aleatorios(total=150, seed=20261104):
    rng = random.Random(seed)
    for num in range(total):
        linhas, colunas = rng.randint(2, 6), rng.randint(2, 6)
        vida = rng.randint(1, 80)
        grade = [[rng.randint(-24, 20) for _ in range(colunas)] for _ in range(linhas)]
        if num % 3 == 0:
            inicio = (rng.randrange(linhas), colunas - 1)
        else:
            inicio = (linhas - 1, rng.randrange(colunas))
        if num % 10 == 0:
            fim = (rng.randrange(linhas), rng.randrange(colunas))
            if fim == inicio:
                fim = (0, 0)
        else:
            fim = (rng.randint(0, inicio[0]), rng.randint(0, inicio[1]))
            if fim == inicio:
                fim = (0, 0)
        grade[inicio[0]][inicio[1]] = "I"
        grade[fim[0]][fim[1]] = "F"
        texto = f"{linhas} {colunas} {vida}\n"
        texto += "\n".join(" ".join(map(str, linha)) for linha in grade) + "\n"
        yield f"aleatorio_{num+1:03d}", texto


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--executavel", help="Programa final do grupo com resultado.txt")
    parser.add_argument("--aleatorios", type=int, default=150)
    parser.add_argument("--somente-fixos", action="store_true")
    args = parser.parse_args()
    modo = "integrado" if args.executavel else "isolado"
    resultado = []
    with tempfile.TemporaryDirectory(prefix="paa_pessoa4_") as tmp:
        pasta = Path(tmp)
        executavel = Path(args.executavel).resolve() if args.executavel else pasta / "driver_dp"
        if not args.executavel:
            avisos = compilar_driver(executavel)
            if avisos.strip():
                print("AVISOS DO COMPILADOR:", avisos.strip(), sep="\n")
        entradas = [(c.stem, c) for c in sorted((BASE / "casos").glob("*.txt"))]
        if not args.somente_fixos:
            for nome, conteudo in casos_aleatorios(args.aleatorios):
                arquivo = pasta / f"{nome}.txt"
                arquivo.write_text(conteudo, encoding="utf-8")
                entradas.append((nome, arquivo))
        manifesto = {item["caso"]: item for item in json.loads((BASE / "manifesto.json").read_text(encoding="utf-8"))}
        for nome, arquivo in entradas:
            caverna = ler_caverna(arquivo)
            melhor, _ = oraculo_exaustivo(caverna)
            if nome in manifesto and melhor != manifesto[nome]["vida_final_maxima"]:
                print("FALHA: manifesto divergente", nome)
                sys.exit(1)
            saida, erro = executar(executavel, arquivo, modo, pasta)
            if erro:
                ok, detalhe = False, erro
            else:
                ok, detalhe = validar_saida(caverna, saida, melhor)
            resultado.append({"caso": nome, "ok": ok, "detalhe": detalhe})
            if not ok:
                print(f"FALHA {nome}: {detalhe}")
        aprovados = sum(item["ok"] for item in resultado)
        print(f"Modo: {modo} | Fixos: {len(list((BASE / 'casos').glob('*.txt')))} | "
              f"Aleatorios: {0 if args.somente_fixos else args.aleatorios} | "
              f"Aprovados: {aprovados}/{len(resultado)}")
        if aprovados != len(resultado):
            sys.exit(1)

if __name__ == "__main__":
    main()
