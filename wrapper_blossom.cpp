#include "blossom5/PerfectMatching.h"
#include <vector>
#include <cstdlib>

#include "operacoesGrafo.h"

extern "C" {
    int *emparalhamento_minimo_perfeito(Grafo grafo_impar) {
        int n_arestas = grafo_impar.vertices * (grafo_impar.vertices - 1) / 2;
        int *resultado = (int *) malloc(grafo_impar.vertices * sizeof(int));
        PerfectMatching *PM = new PerfectMatching(grafo_impar.vertices, n_arestas);
        
        for (int i = 0; i < grafo_impar.vertices; i++) {
            for (int j = i + 1; j < grafo_impar.vertices; j++) {
                    PM->AddEdge(i, j, grafo_impar.matriz_adj[i][j]);
            }
        }

        PM->Solve();

        for (int i = 0; i < grafo_impar.vertices; i++) {
            resultado[i] = PM->GetMatch(i);
        }

        delete PM;
        return resultado;
    }
}