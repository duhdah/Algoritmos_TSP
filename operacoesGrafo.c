#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "operacoesGrafo.h"

Grafo criaGrafo(char *caminho){
    FILE *arquivo = fopen(caminho, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return (Grafo){0, NULL, NULL, NULL};
    }

    int linhas = 0, c;
    while ((c = fgetc(arquivo)) != EOF) {
        if (c == '\n') {
            if((c = fgetc(arquivo)) != '\n' && c != EOF){
               linhas++;
            }
        }
    }
    linhas++; 

    rewind(arquivo);

    int **matriz = malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = malloc(linhas * sizeof(int));
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < linhas; j++) {
            if (fscanf(arquivo, "%d", &matriz[i][j]) != 1) {
                printf("Erro na leitura do arquivo");
                return (Grafo){0, NULL, NULL, NULL};
            }
        }
    }
    Grafo grafo;
    grafo.vertices = linhas;
    grafo.matriz_adj = malloc(linhas * sizeof(int *));
    grafo.pai = NULL;
    grafo.indices_impares = NULL;
    for (int i = 0; i < linhas; i++) {
        grafo.matriz_adj[i] = malloc(linhas * sizeof(int));
        for (int j = 0; j < linhas; j++) {
            grafo.matriz_adj[i][j] = matriz[i][j];
        }
    }
    
    for (int i = 0; i < linhas; i++)
        free(matriz[i]);
    free(matriz);
    fclose(arquivo);

    return grafo;

}

Conjunto *criaConjunto(int n) {
    Conjunto *conjunto = malloc(sizeof(Conjunto));
    conjunto->pai = malloc(n * sizeof(int));
    conjunto->rank = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        conjunto->pai[i] = i;
        conjunto->rank[i] = 0;
    }
    return conjunto;
}

int kruskal_encontraRaiz(Conjunto *conjunto, int i) {
    if (conjunto->pai[i] != i)
        conjunto->pai[i] = kruskal_encontraRaiz(conjunto, conjunto->pai[i]);
    return conjunto->pai[i];
}

void kruskal_uniao(Conjunto *conjunto, int x, int y) {
    int raiz_x = kruskal_encontraRaiz(conjunto, x);
    int raiz_y = kruskal_encontraRaiz(conjunto, y);

    if (raiz_x == raiz_y) return;

    if (conjunto->rank[raiz_x] < conjunto->rank[raiz_y])
        conjunto->pai[raiz_x] = raiz_y;
    else if (conjunto->rank[raiz_x] > conjunto->rank[raiz_y])
        conjunto->pai[raiz_y] = raiz_x;
    else {
        conjunto->pai[raiz_y] = raiz_x;
        conjunto->rank[raiz_x]++;
    }

}

Grafo kruskal(Grafo grafo) {
    int **matriz = grafo.matriz_adj;
    Aresta *arestas = malloc(grafo.vertices * grafo.vertices * sizeof(Aresta));
    int qtdArestas = 0;

    for (int i = 0; i < grafo.vertices; i++) {
        for (int j = i + 1; j < grafo.vertices; j++) {
            if (matriz[i][j] != 0) {
                arestas[qtdArestas].origem = i;
                arestas[qtdArestas].destino = j;
                arestas[qtdArestas].peso = matriz[i][j];
                qtdArestas++;
            }
        }
    }

    mergeSortArestas(arestas, qtdArestas);

    Grafo mst;
    mst.vertices = grafo.vertices;
    mst.matriz_adj = malloc(mst.vertices * sizeof(int *));
    mst.pai = NULL;
    mst.indices_impares = NULL;
    for (int i = 0; i < mst.vertices; i++) {
        mst.matriz_adj[i] = calloc(mst.vertices, sizeof(int));
    }
    Conjunto *conjunto = criaConjunto(grafo.vertices);
    int arestasIncluidas = 0;

    for (int i = 0; i < qtdArestas && arestasIncluidas < grafo.vertices - 1; i++) {
        int u = arestas[i].origem;
        int v = arestas[i].destino;

        int raiz_u = kruskal_encontraRaiz(conjunto, u);
        int raiz_v = kruskal_encontraRaiz(conjunto, v);

        if (raiz_u != raiz_v) {
            mst.matriz_adj[u][v] = arestas[i].peso;
            mst.matriz_adj[v][u] = arestas[i].peso;
            kruskal_uniao(conjunto, raiz_u, raiz_v);
            arestasIncluidas++;
        }
    }
    free(arestas);
    free(conjunto->pai);
    free(conjunto->rank);
    free(conjunto);
    return mst;
}

Grafo criaGrafoImpar(Grafo grafo, Grafo *mst){
    int n = grafo.vertices;

    int vertices_impares = 0;
    mst->indices_impares = malloc(n * sizeof(int)); 
    
    for(int i = 0; i < n; i++) {
        int contador = 0;
        for(int j = 0; j < n; j++) {
            if(mst->matriz_adj[i][j] != 0) {
                contador++;
            }
        }
        if(contador % 2 != 0) {
            mst->indices_impares[vertices_impares] = i;
            vertices_impares++;
        }
    }
    
    Grafo subgrafo_impar;
    subgrafo_impar.vertices = vertices_impares;
    subgrafo_impar.matriz_adj = malloc(vertices_impares * sizeof(int *));
    subgrafo_impar.pai = NULL;
    subgrafo_impar.indices_impares = NULL; 

    for(int i = 0; i < vertices_impares; i++) {
        subgrafo_impar.matriz_adj[i] = malloc(vertices_impares * sizeof(int));
    }

    int **distancias = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        distancias[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            if (i == j) {
                distancias[i][j] = 0;
            } else if (grafo.matriz_adj[i][j] > 0) {
                distancias[i][j] = grafo.matriz_adj[i][j]; 
            } else {
                distancias[i][j] = INT_MAX / 2; 
            }
        }
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (distancias[i][k] + distancias[k][j] < distancias[i][j]) {
                    distancias[i][j] = distancias[i][k] + distancias[k][j];
                }
            }
        }
    }    

    for(int i = 0; i < vertices_impares; i++) {
        for(int j = 0; j < vertices_impares; j++) {
            int original_i = mst->indices_impares[i];
            int original_j = mst->indices_impares[j];
            subgrafo_impar.matriz_adj[i][j] = distancias[original_i][original_j];
        }
    }
    for (int i = 0; i < n; i++) {
        free(distancias[i]);
    }
    free(distancias);

    return subgrafo_impar;
}

void liberaGrafo(Grafo *grafo) {

    if(grafo->matriz_adj != NULL) {
        for (int i = 0; i < grafo->vertices; i++) {
            free(grafo->matriz_adj[i]);
        }
        free(grafo->matriz_adj);
        grafo->matriz_adj = NULL;
    }

    if(grafo->pai) {
        free(grafo->pai);
        grafo->pai = NULL;
    }

    if(grafo->indices_impares) {
        free(grafo->indices_impares);
        grafo->indices_impares = NULL;
    }

    grafo->vertices = 0;
}

int *circuito_euleriano(int **contagem_arestas, int n) {
    if (n == 0) {
        int *circuito_vazio = malloc(sizeof(int)); 
        if (circuito_vazio) { 
            circuito_vazio[0] = -1; 
        }
        return circuito_vazio;
    }

    int *pilha = malloc(n * n * 2 * sizeof(int)), *circuito, topo = -1; 
    int *resultado = malloc(n * n * 2 * sizeof(int)), tam_resultado = 0;

    pilha[++topo] = 0; 

    while (topo >= 0) {
        
        int i = pilha[topo];
        int encontrou = 0;

        for (int j = 0; j < n; j++) {
            if (contagem_arestas[i][j] > 0) {
                contagem_arestas[i][j]--;
                contagem_arestas[j][i]--;
                pilha[++topo] = j;
                encontrou = 1;
                break;
            }
        }

        if (!encontrou) {
            resultado[tam_resultado++] = i;
            topo--;
        }
    }

    circuito = malloc((tam_resultado + 1) * sizeof(int));
    for (int i = 0; i < tam_resultado; i++)
        circuito[i] = resultado[tam_resultado - i - 1];
    circuito[tam_resultado] = -1;

    free(pilha);
    free(resultado);

    return circuito;
}

void merge(Aresta *arestas, Aresta *temp, int esq, int meio, int dir) {
    int i = esq;   
    int j = meio + 1; 
    int k = esq;       

    while (i <= meio && j <= dir) {
        if (arestas[i].peso <= arestas[j].peso) {
            temp[k++] = arestas[i++];
        } else {
            temp[k++] = arestas[j++];
        }
    }

    while (i <= meio) {
        temp[k++] = arestas[i++];
    }

    while (j <= dir) {
        temp[k++] = arestas[j++];
    }

    for (i = esq; i <= dir; i++) {
        arestas[i] = temp[i];
    }
}

void mergeSortRecursivo(Aresta *arestas, Aresta *temp, int esq, int dir) {
    if (esq < dir) {
        int meio = esq + (dir - esq) / 2;
        mergeSortRecursivo(arestas, temp, esq, meio);
        mergeSortRecursivo(arestas, temp, meio + 1, dir);
        merge(arestas, temp, esq, meio, dir);
    }
}

void mergeSortArestas(Aresta *arestas, int n) {
    if (n < 2) {
        return;
    }
    Aresta *temp = (Aresta *)malloc(n * sizeof(Aresta));
    mergeSortRecursivo(arestas, temp, 0, n - 1);
    free(temp);
}