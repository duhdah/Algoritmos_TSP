#include "operacoesGrafo.h"
#include "wrapper_blossom.h"

int christofides(Grafo grafo) {

   // PRIMEIRO PASSO: Achar a árvore geradora mínima (MST)
    Grafo mst = kruskal(grafo);

    // SEGUNDO PASSO: Criar o grafo dos vértices de grau ímpar na MST   
    Grafo grafo_impar = criaGrafoImpar(grafo, &mst);

     // TERCEIRO PASSO: Encontrar o emparelhamento perfeito de custo mínimo do grafo dos vértices ímpares
    int *resultado = emparalhamento_minimo_perfeito(grafo_impar);

    // QUARTO PASSO: Adicionar as arestas do emparelhamento à MST
    int **contagem_arestas = malloc(mst.vertices * sizeof(int *));
    for (int i = 0; i < mst.vertices; i++) {
        contagem_arestas[i] = calloc(mst.vertices, sizeof(int));
    }

    for (int i = 0; i < mst.vertices; i++) {
        for (int j = i + 1; j < mst.vertices; j++) {
            if (mst.matriz_adj[i][j] > 0) {
                contagem_arestas[i][j]++;
                contagem_arestas[j][i]++;
            }
        }
    }

    for (int i = 0; i < grafo_impar.vertices; i++) {
        if (i < resultado[i]) {
            int v1 = mst.indices_impares[i];
            int v2 = mst.indices_impares[resultado[i]];
            contagem_arestas[v1][v2]++;
            contagem_arestas[v2][v1]++;
        }
    }

    // QUINTO PASSO: Encontrar circuito euleriano
    int *circuito = circuito_euleriano(contagem_arestas, mst.vertices);
    
    // SEXTO PASSO: Transformar circuito euleriano em hamiltoniano 
    int *visitado = calloc(mst.vertices, sizeof(int)), *caminho = calloc(mst.vertices + 1, sizeof(int));
    int tam_caminho = 0;
    for (int i = 0; circuito[i] != -1; i++) {
        int v = circuito[i];
        if (!visitado[v]) {
            caminho[tam_caminho++] = v;
            visitado[v] = 1;
        }
    }
    caminho[tam_caminho++] = caminho[0];
    printf("\nCaminho encontrado:\n");
    for (int i = 0; i < tam_caminho - 1; i++) {
        printf("%d -> ", caminho[i]);
    }
    printf("%d\n", caminho[tam_caminho - 1]);

    int custo_total = 0;
    for(int i = 0; i < tam_caminho - 1; i++) {
        custo_total += grafo.matriz_adj[caminho[i]][caminho[i + 1]];
    }
    printf("Custo total do caminho: %d\n", custo_total);

    if(mst.matriz_adj != NULL) liberaGrafo(&mst);
    if(grafo_impar.matriz_adj != NULL) liberaGrafo(&grafo_impar);

    free(resultado);
    free(circuito);
    free(visitado);
    free(caminho);
    free(contagem_arestas);
    return 0;
}
