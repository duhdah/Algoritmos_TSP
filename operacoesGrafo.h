#ifndef OPERACOESGRAFO_H
#define OPERACOESGRAFO_H

#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct {
    int vertices;
    int **matriz_adj;
    int *pai;
    int *indices_impares;
} Grafo;

typedef struct {
    int origem, destino, peso;
} Aresta;

typedef struct {
    int *pai, *rank;
} Conjunto;


Grafo criaGrafo(char *caminho);
Grafo kruskal(Grafo grafo);
Grafo criaGrafoImpar(Grafo grafo, Grafo *mst);
void liberaGrafo(Grafo *grafo);
int *circuito_euleriano(int **contagem_arestas, int n);
void mergeSortArestas(Aresta *arestas, int n);

#endif