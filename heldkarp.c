#include "heldkarp.h"

void heldkarp(Grafo grafo) {
  int n = grafo.vertices;
  int** distancias = grafo.matriz_adj;

  int num_mascaras = 1 << n;  // 2^n
  int** custos_dp;
  int** pai;

  custos_dp = (int**)malloc(num_mascaras * sizeof(int*));
  pai = (int**)malloc(num_mascaras * sizeof(int*));

  if (custos_dp == NULL || pai == NULL) {
    printf("Erro: Falha ao alocar memoria (linhas).\n");
    return;
  }

  for (int i = 0; i < num_mascaras; i++) {
    custos_dp[i] = (int*)malloc(n * sizeof(int));
    pai[i] = (int*)malloc(n * sizeof(int));

    if (custos_dp[i] == NULL || pai[i] == NULL) {
      printf("Erro: Falha ao alocar memoria (colunas).\n");
      return;
    }
  }

  for (int mascara = 0; mascara < num_mascaras; mascara++) {
    for (int cidade = 0; cidade < n; cidade++) {
      custos_dp[mascara][cidade] = INT_MAX;
      pai[mascara][cidade] = -1;
    }
  }

  custos_dp[1][0] = 0;

  for (int mascara = 0; mascara < num_mascaras; mascara++) {
    for (int ultimo = 0; ultimo < n; ultimo++) {
      if (!(mascara & (1 << ultimo))) {
        continue;
        ;
      }

      for (int proximo = 0; proximo < n; proximo++) {
        if (mascara & (1 << proximo)) {
          continue;
        }

        if (custos_dp[mascara][ultimo] == INT_MAX) {
          continue;
        }

        int nova_mascara = mascara | (1 << proximo);

        int novo_custo =
            custos_dp[mascara][ultimo] + distancias[ultimo][proximo];

        if (novo_custo < custos_dp[nova_mascara][proximo]) {
          custos_dp[nova_mascara][proximo] = novo_custo;
          pai[nova_mascara][proximo] = ultimo;
        }
      }
    }
  }

  int custo_minimo = INT_MAX;

  int cidade_final = -1;

  int mascara_completa = (1 << n) - 1;

  for (int ultimo = 1; ultimo < n; ultimo++) {
    if (custos_dp[mascara_completa][ultimo] == INT_MAX) {
      continue;
    }

    int custo_percurso =
        custos_dp[mascara_completa][ultimo] + distancias[ultimo][0];

    if (custo_percurso < custo_minimo) {
      custo_minimo = custo_percurso;
      cidade_final = ultimo;
    }
  }

  int percurso[n];
  int contador = 0;

  int mascara_atual = mascara_completa;
  int ultima_cidade = cidade_final;

  while (mascara_atual != 0) {
    percurso[contador] = ultima_cidade;
    contador++;

    int cidade_anterior = pai[mascara_atual][ultima_cidade];

    mascara_atual ^= (1 << ultima_cidade);

    ultima_cidade = cidade_anterior;
  }

  printf("\nMelhor caminho: ");
  for (int i = n - 1; i >= 0; i--) {
    printf("%d -> ", percurso[i]);
  }
  printf("0\n");

  printf("Custo minimo: %d\n", custo_minimo);

  for (int i = 0; i < num_mascaras; i++) {
    free(custos_dp[i]);
    free(pai[i]);
  }

  free(custos_dp);
  free(pai);
}