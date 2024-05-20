#include "rainhas.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct tabuleiro{
    casa** c;
    unsigned int** proibida;
}tabuleiro;

void inicializaTabuleiro(unsigned int n, tabuleiro* tab, unsigned int k, casa *proibida){
    tab->c = (casa**)malloc(n*sizeof(casa*));
    tab->proibida = (unsigned int**)malloc(n*sizeof(unsigned int*));

    for(unsigned int i = 0; i < n; i++){
        tab->c[i] = (casa*)malloc(n*sizeof(casa));
        tab->proibida[i] = (unsigned int*)malloc(n*sizeof(unsigned int));
        for(unsigned j = 0; j < n; j++){
            tab->c[i][j].linha = i;
            tab->c[i][j].coluna = j;
            tab->proibida[i][j] = 0;
        }
    }

    for(unsigned i = 0; i < k; i++){
        tab->proibida[proibida[i].linha-1][proibida[i].coluna-1] = 1;
    }

}



void imprimeTabuleiro(unsigned n, tabuleiro* tab){
    printf("\n");
    for(unsigned int i = 0; i < n; i++){
        for(unsigned int j = 0; j < n; j++){
            printf("%d ", tab->proibida[i][j]);
        }
        printf("\n");
    }
}

unsigned int casaProibida(unsigned int linha, unsigned int coluna, tabuleiro* tab){
    if(tab->proibida[linha][coluna] >= 1){
        return 1;
    }
    return 0;
}

unsigned int rainhaPodeSerColocada(unsigned int n, unsigned int linha, unsigned int coluna, unsigned int* r, tabuleiro* tab){
    
    if(r[linha] != 0){
        printf("Linha %d já está ocupada\n", linha);
        return 0;
    }

    // Verificar se a coluna está vazia
    for (unsigned int i = 0; i < n; i++) {
        if ((r[i] == coluna + 1)) {
            printf("Coluna %d já está ocupada\n", coluna);
            return 0;
        }
    }    
    
    // Verificar diagonais superiores
    for (unsigned int i = 0; i < linha; i++) {
        if (r[i] != 0 && abs(r[i] - coluna - 1) == abs(i - linha)) {
            return 0;
        }
    }
    
    return 1;
}

//    r é um vetor de n posições (já alocado) a ser preenchido com a resposta:
//      r[i] = j > 0 indica que a rainha da linha i+1 fica na coluna j;
//      r[i] = 0     indica que não há rainha nenhuma na linha i+1
unsigned int rainhasBacktrackingRec(unsigned int n, unsigned int linha, unsigned int* r, tabuleiro* tab){
    if(linha == n - 1){
        return 1;
    }
    for(unsigned int j = 0; j < n; j++){
        if(!casaProibida(linha, j, tab) && rainhaPodeSerColocada(n, linha, j, r, tab)){
            r[linha] = j+1;

            printf("Rainha colocada na linha %d, coluna %d\n", linha, j);

            if(rainhasBacktrackingRec(n, linha+1, r, tab)){
                return 1;
            }

            // Se não encontrou solução, remove a rainha e restaura o tabuleiro
            printf("Rainha removida da linha %d, coluna %d\n", linha, j);
            r[linha] = 0;

        }
    }

    return 0;
}


unsigned int *rainhas_bt(unsigned int n, unsigned int k, casa *c, unsigned int *r) {
    tabuleiro tab;
    inicializaTabuleiro(n, &tab, k, c);

    if(!rainhasBacktrackingRec(n, 0, r, &tab)){
        printf("Não foi possível encontrar uma solução\n");
        imprimeTabuleiro(n, &tab);
        return NULL;
    }

    return r;

}





//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n
// rainhas com casas proibidas usando a modelagem do problema como
// conjunto independente de um grafo
//
// n, c e r são como em rainhas_bt()

// O Algoritmo ConjIndep(G, n, I, C) abaixo recebe um grafo G, um inteiro positivo n, um conjunto I ⊆ V(G) independente em G e um conjunto 
// C ⊆ V(G) − I satisfazendo ΓG(I)∩C = ∅, e devolve um conjunto R independente em G com n vértices satisfazendo I ⊆ R ⊆ I∪C, ou "não" caso não exista tal conjunto.
unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r) {
    return NULL;

}
