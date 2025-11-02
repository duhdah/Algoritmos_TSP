#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <time.h>

#include "operacoesGrafo.h"
#include "christofides.h"
#include "heldkarp.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Formato do comando: ./tsp <caminho_arquivo> <flag>");
        return 1;
    }

    char *caminho = argv[1];    
    char *flag = argv[2]; // 0 = exato, 1 = aproximativo

    Grafo grafo = criaGrafo(caminho);
    
    if(grafo.matriz_adj == NULL) {
        printf("ERRO: Nao foi possivel criar o grafo a partir do arquivo.\n");
        return 0;
    }

    if (strcmp(flag, "0") == 0) {
        clock_t inicio_hk = clock();
        heldkarp(grafo);
        clock_t fim_hk = clock();
        double tempo_hk = ((double)(fim_hk - inicio_hk)) / CLOCKS_PER_SEC;
        printf("\nTempo Held-Karp: %f segundos\n", tempo_hk);
    } else {
        clock_t inicio_hk = clock();
        christofides(grafo);
        clock_t fim_hk = clock();
        double tempo_hk = ((double)(fim_hk - inicio_hk)) / CLOCKS_PER_SEC;
        printf("\nTempo Christofides: %f segundos\n", tempo_hk);
    }
    if(grafo.matriz_adj != NULL) liberaGrafo(&grafo);
    return 0;
}
