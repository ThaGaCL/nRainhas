#include "rainhas.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct tabuleiro{
    casa** c;
    unsigned int** proibida;
}tabuleiro;

unsigned int mesmaDiagonal(unsigned int linha1, unsigned int coluna1, unsigned int linha2, unsigned int coluna2){
    if(abs(linha1 - linha2) == abs(coluna1 - coluna2)){
        return 1;
    }
    return 0;
}

unsigned int existeRainha(int linha, int coluna, tabuleiro* tab, int n) {
    for (unsigned int i = 0; i < n; i++) {
        if (i != linha && tab->proibida[i][coluna] == 3) {
            return 1;
        }
        if (i != coluna && tab->proibida[linha][i] == 3) {
            return 1;
        }
    }
    return 0;
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
    // Verificar se a coluna está vazia
    for (unsigned int i = 0; i < n; i++) {
        if (r[i] == coluna) {
            return 0;
        }
    }
    
    // Verificar diagonais superiores
    for (unsigned int i = 0; i < linha; i++) {
        if (r[i] != 0 && abs(r[i] - coluna) == abs(i - linha)) {
            return 0;
        }
    }
    
    return 1;
}

unsigned int rainhasBacktrackingRec(unsigned int n, unsigned int linha, unsigned int* r, tabuleiro* tab){
    if(linha == n){
        return 1;
    }

    for(unsigned int i = 0; i < n; i++){
        if(casaProibida(linha, i, tab)){
            continue;
        }
        if(rainhaPodeSerColocada(n, linha, i, r, tab)){
            r[i] = i+1;

            printf("Rainha colocada na linha %d, coluna %d\n", linha + 1, i + 1);

            // Proibe a linha, a coluna e as diagonais
            for(unsigned int j = 0; j < n; j++){
                if(tab->proibida[linha][j] == 0 && tab->proibida[j][i] == 0){
                    tab->proibida[linha][j] = 2;
                    tab->proibida[j][i] = 2;
                }
            }

            for(unsigned int j = 0; j < n; j++){
                for(unsigned int k = 0; k < n; k++){
                    if(mesmaDiagonal(linha, i, j, k) && tab->proibida[j][k] == 0 ){
                        tab->proibida[j][k] = 2;
                    }
                }
            }

            tab->proibida[linha][i] = 3;

            imprimeTabuleiro(n, tab);
            if(rainhasBacktrackingRec(n, linha+1, r, tab)){
                return 1;
            }

            // Se não encontrou solução, remove a rainha e restaura o tabuleiro
            printf("Rainha removida da linha %d, coluna %d\n", linha + 1, i + 1);
            r[i] = 0;
            for(unsigned int j = 0; j < n; j++){
                if(tab->proibida[linha][j] == 2){
                    tab->proibida[linha][j] = 0;
                    tab->proibida[j][i] = 0;
                }
            }
            tab->proibida[linha][i] = 0; // Restaure a célula removida
        }
    }

    return 0;
}


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

unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r) {

  n = n;
  k = k;
  c = c;

  return r;
}
