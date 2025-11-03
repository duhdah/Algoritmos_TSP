# Algoritmo Aproximativo para TSP
**Trabalho 1 de Algoritmos e Estruturas de Dados III**  

O projeto desenvolvido inclui implementações dos algoritmos Held-Karp e Christofides com o objetivo de encontrar respostas ótimas ou subótimas para instâncias do Problema do Caixeiro Viajante (TSP).

## Como compilar?

No Linux: 

```bash
make
```

No Windows: 

```PowerShell
g++ -Iblossom5 -c wrapper_blossom.cpp -o output/wrapper_blossom.o ; gcc -c christofides.c -o output/christofides.o ; gcc -c heldkarp.c -o output/heldkarp.o ; gcc -c main.c -o output/main.o ; gcc -c operacoesGrafo.c -o output/operacoesGrafo.o ; g++ blossom5/misc.cpp blossom5/PMduals.cpp blossom5/PMexpand.cpp blossom5/PMinit.cpp blossom5/PMinterface.cpp blossom5/PMmain.cpp blossom5/PMrepair.cpp blossom5/PMshrink.cpp blossom5/MinCost/MinCost.cpp blossom5/GEOM/GPMinit.cpp blossom5/GEOM/GPMinterface.cpp blossom5/GEOM/GPMkdtree.cpp blossom5/GEOM/GPMmain.cpp output/wrapper_blossom.o output/operacoesGrafo.o output/main.o output/christofides.o output/heldkarp.o -o tsp -lpthread
```
* OBS: No Windows, também é necessário criar uma pasta "output" antes de compilar. 

## Como executar?

No Windows ou no Linux, as instâncias pode ser executadas da seguinte forma: 

### Algoritmo exato
* ./tsp instancias/tsp1_253.txt 0
* ./tsp instancias/tsp2_1248.txt 0
* ./tsp instancias/tsp3_1194.txt 0
* ./tsp instancias/tsp4_7013.txt 0
* ./tsp instancias/tsp5_27603.txt 0

### Algoritmo aproximado
* ./tsp instancias/tsp1_253.txt 1
* ./tsp instancias/tsp2_1248.txt 1
* ./tsp instancias/tsp3_1194.txt 1
* ./tsp instancias/tsp4_7013.txt 1
* ./tsp instancias/tsp5_27603.txt 1
