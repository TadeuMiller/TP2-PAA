#!/usr/bin/env python3
"""Gera 30 casos deterministas e gabaritos de referencia por enumeracao exaustiva."""
import json
from pathlib import Path
from oraculo import ler_caverna, oraculo_exaustivo

BASE = Path(__file__).resolve().parent
CASOS = BASE / "casos"
ESPERADOS = BASE / "esperados"


def montar(linhas, colunas, vida, inicio, fim, alteracoes=None, padrao=0):
    assert inicio != fim
    grade = [[padrao for _ in range(colunas)] for _ in range(linhas)]
    for (i, j), valor in (alteracoes or {}).items():
        grade[i][j] = valor
    grade[inicio[0]][inicio[1]] = "I"
    grade[fim[0]][fim[1]] = "F"
    corpo = "\n".join(" ".join(str(c) for c in linha) for linha in grade)
    return f"{linhas} {colunas} {vida}\n{corpo}\n"


def construir():
    a = []
    def add(nome, descricao, linhas, colunas, vida, inicio, fim, alteracoes=None, padrao=0):
        a.append((nome, descricao, montar(linhas, colunas, vida, inicio, fim, alteracoes, padrao)))

    add("01_quadrada_simples", "2x2 com caminho possivel", 2, 2, 10, (1, 1), (0, 0))
    a.append(("02_exemplo_enunciado", "Exemplo oficial 4x5 do enunciado", "4 5 40\n-20 0 -20 0 0\nF -10 -10 -10 -10\n-20 0 0 0 +20\n0 0 -10 0 I\n"))
    add("03_retangular_larga", "Caverna 2x6", 2, 6, 12, (1, 5), (0, 0), {(1, 3): 7, (0, 4): -5, (1, 1): -3})
    add("04_retangular_alta", "Caverna 6x2", 6, 2, 12, (5, 1), (0, 0), {(3, 1): 8, (2, 0): -4, (4, 0): -6})
    add("05_linha_unica", "Uma linha, deslocamento a esquerda", 1, 6, 9, (0, 5), (0, 0), {(0, 3): -4, (0, 1): 2})
    add("06_coluna_unica", "Uma coluna, deslocamento para cima", 6, 1, 9, (5, 0), (0, 0), {(3, 0): -4, (1, 0): 2})
    add("07_mesma_linha", "I e F na mesma linha interna", 3, 6, 7, (2, 5), (2, 1), {(2, 3): 5})
    add("08_mesma_coluna", "I e F na mesma coluna", 6, 4, 7, (5, 3), (2, 3), {(4, 3): -5})
    add("09_fora_de_direcao", "Fim abaixo ou a direita do inicio", 3, 3, 20, (0, 2), (2, 0))
    add("10_dano_letal", "Todas as opcoes iniciais letais", 2, 2, 5, (1, 1), (0, 0), {(0, 1): -5, (1, 0): -5})
    add("11_zero_intermediario", "Vida fica exatamente zero durante o trajeto", 1, 4, 5, (0, 3), (0, 0), {(0, 2): -5, (0, 1): 20})
    add("12_zero_na_saida", "Vida fica exatamente zero em F após perda", 1, 3, 5, (0, 2), (0, 0), {(0, 1): -5})
    add("13_vida_minima", "Vida inicial igual a um", 2, 3, 1, (1, 2), (0, 0))
    add("14_pocao_necessaria", "Recuperacao de vida em rota sobrevivente", 2, 3, 4, (1, 2), (0, 0), {(1, 1): 8, (0, 2): -4, (0, 1): -7, (1, 0): -6})
    add("15_otimo_diferente", "Duas rotas viaveis com diferentes vidas finais", 2, 3, 10, (1, 2), (0, 0), {(1, 1): 12, (0, 2): -4, (0, 1): -2})
    add("16_empate", "Multiplas rotas otimas por empate", 3, 3, 8, (2, 2), (0, 0))
    add("17_rota_letal_vs_viavel", "Maior ganho final isolado nao salva caminho letal", 3, 3, 3, (2, 2), (0, 0), {(1, 2): -3, (2, 1): 4, (1, 1): -2, (0, 2): 20})
    add("18_pocao_apos_inicio", "Pocao positiva adjacente ao inicio", 2, 2, 3, (1, 1), (0, 0), {(0, 1): 10, (1, 0): -2})
    add("19_pocao_tardia", "Pocao depois de perdas toleraveis", 1, 5, 12, (0, 4), (0, 0), {(0, 3): -6, (0, 2): 15, (0, 1): -10})
    add("20_inicio_borda_superior", "Inicio na borda superior", 3, 5, 20, (0, 4), (0, 0), {(0, 2): -7})
    add("21_inicio_borda_esquerda", "Inicio na borda esquerda", 5, 3, 20, (4, 0), (0, 0), {(2, 0): -7})
    add("22_saida_canto", "Saida no canto superior esquerdo", 4, 4, 15, (3, 3), (0, 0), {(3, 2): 3, (2, 3): 4, (1, 1): -5})
    add("23_todas_zero", "Mapa inteiramente neutro", 4, 4, 9, (3, 3), (0, 0))
    add("24_negativos_vida_alta", "Danos em todas as celulas, vida suficiente", 4, 4, 100, (3, 3), (0, 0), padrao=-3)
    add("25_negativos_vida_baixa", "Danos em todas as celulas, vida insuficiente", 4, 4, 2, (3, 3), (0, 0), padrao=-3)
    add("26_gargalo", "Passagem critica de vida baixa", 3, 3, 5, (2, 2), (0, 0), {(1, 1): -4, (2, 1): 5, (1, 2): -4, (0, 1): -3, (1, 0): 1})
    add("27_larga_7", "Retangulo 3x7", 3, 7, 20, (2, 6), (0, 0), {(2, 4): -10, (1, 5): 4, (0, 3): 3})
    add("28_alta_7", "Retangulo 7x3", 7, 3, 20, (6, 2), (0, 0), {(4, 2): -10, (5, 1): 4, (3, 0): 3})
    add("29_minimo_linha", "Entrada minima 1x2", 1, 2, 1, (0, 1), (0, 0))
    add("30_minimo_coluna", "Entrada minima 2x1", 2, 1, 1, (1, 0), (0, 0))
    assert len(a) == 30, len(a)
    CASOS.mkdir(parents=True, exist_ok=True)
    ESPERADOS.mkdir(parents=True, exist_ok=True)
    manifesto = []
    for nome, descricao, texto in a:
        arquivo = CASOS / f"{nome}.txt"
        arquivo.write_text(texto, encoding="utf-8")
        melhor, caminho = oraculo_exaustivo(ler_caverna(arquivo))
        exemplo = "impossível\n" if caminho is None else "".join(f"{i} {j}\n" for i, j in caminho)
        (ESPERADOS / f"{nome}.txt").write_text(exemplo, encoding="utf-8")
        manifesto.append({"caso": nome, "descricao": descricao, "solucao": melhor is not None, "vida_final_maxima": melhor})
    (BASE / "manifesto.json").write_text(json.dumps(manifesto, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
    print(f"Gerados {len(a)} casos, gabaritos e manifesto.")

if __name__ == "__main__":
    construir()
