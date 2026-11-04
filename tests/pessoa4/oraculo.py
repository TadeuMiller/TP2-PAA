#!/usr/bin/env python3
"""Oraculo independente: enumera todos os caminhos monotônicos em cavernas pequenas."""
from pathlib import Path


def ler_caverna(caminho):
    tokens = Path(caminho).read_text(encoding="utf-8").split()
    if len(tokens) < 3:
        raise ValueError("cabecalho ausente")
    linhas, colunas, inicial = map(int, tokens[:3])
    if linhas <= 0 or colunas <= 0 or inicial <= 0:
        raise ValueError("dimensoes ou vida invalidas")
    if len(tokens) != 3 + linhas * colunas:
        raise ValueError("quantidade incorreta de celulas")
    mapa = []
    inicio = fim = None
    for k, valor in enumerate(tokens[3:]):
        i, j = divmod(k, colunas)
        if valor == "I":
            if inicio is not None:
                raise ValueError("mais de um inicio")
            inicio = (i, j)
            mapa.append(0)
        elif valor == "F":
            if fim is not None:
                raise ValueError("mais de um fim")
            fim = (i, j)
            mapa.append(0)
        else:
            mapa.append(int(valor))
    if inicio is None or fim is None:
        raise ValueError("marcadores I e F obrigatorios")
    return linhas, colunas, inicial, mapa, inicio, fim


def oraculo_exaustivo(caverna):
    """Independente do DP: percorre cada caminho e devolve melhor vida e um exemplo."""
    linhas, colunas, vida_inicial, valores, inicio, fim = caverna
    melhor = None
    caminho_melhor = None

    def explorar(pos, vida, caminho):
        nonlocal melhor, caminho_melhor
        if pos == fim:
            if melhor is None or vida > melhor:
                melhor, caminho_melhor = vida, list(caminho)
            return
        i, j = pos
        for novo in ((i - 1, j), (i, j - 1)):
            ni, nj = novo
            if ni < 0 or nj < 0:
                continue
            vida_seguinte = vida + valores[ni * colunas + nj]
            if vida_seguinte <= 0:
                continue
            explorar(novo, vida_seguinte, caminho + [novo])

    explorar(inicio, vida_inicial, [inicio])
    return melhor, caminho_melhor


def validar_saida(caverna, saida, melhor_esperado):
    linhas, colunas, vida_inicial, valores, inicio, fim = caverna
    linhas_saida = saida.strip().splitlines()
    if melhor_esperado is None:
        if saida.strip() != "impossível":
            return False, "sem solucao: esperado apenas 'impossível'"
        return True, "impossivel confirmado"
    if not linhas_saida or saida.strip() == "impossível":
        return False, "saida impossivel/vazia para caso com solucao"
    caminho = []
    for l in linhas_saida:
        partes = l.split()
        if len(partes) != 2:
            return False, "linha de saida diferente de 'linha coluna'"
        try:
            par = tuple(map(int, partes))
        except ValueError:
            return False, "coordenada nao inteira"
        if not (0 <= par[0] < linhas and 0 <= par[1] < colunas):
            return False, "coordenada fora da matriz"
        caminho.append(par)
    if caminho[0] != inicio or caminho[-1] != fim:
        return False, "extremidades I/F incorretas"
    vida = vida_inicial
    for indice, (i, j) in enumerate(caminho):
        if indice > 0:
            anterior = caminho[indice - 1]
            if (i - anterior[0], j - anterior[1]) not in ((-1, 0), (0, -1)):
                return False, "movimento diferente de cima/esquerda"
        vida += valores[i * colunas + j]
        if vida <= 0:
            return False, "vida nao positiva em algum passo"
    if vida != melhor_esperado:
        return False, f"vida final nao otima: obtida={vida}, esperada={melhor_esperado}"
    return True, f"caminho valido e otimo, vida final={vida}"
