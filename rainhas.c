#include "rainhas.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct tabuleiro
{
    casa **c;
    unsigned int **proibida;
} tabuleiro;

typedef struct nodo
{
    unsigned int vertice;
    unsigned int proibida;
    struct nodo *prox;
} nodo;

typedef struct grafo
{
    nodo **listasAdj;
    unsigned int tam;
} grafo;

// Protótipos
void inicializaTabuleiro(unsigned int n, tabuleiro *tab, unsigned int k, casa *proibida);
void imprimeTabuleiro(unsigned n, tabuleiro *tab);
unsigned int casaProibida(unsigned int linha, unsigned int coluna, tabuleiro *tab);
unsigned int rainhaPodeSerColocada(unsigned int n, unsigned int linha, unsigned int coluna, unsigned int *r);
unsigned int rainhasBacktrackingRec(unsigned int n, unsigned int linha, unsigned int *r, tabuleiro *tab);
nodo *criaNodo(unsigned int vertice, nodo *prox);
grafo *criaGrafo(unsigned int n);
unsigned int conjIndependente(unsigned int n, unsigned int *Grafo, unsigned int *Independente, unsigned int *Conjunto, unsigned int *r);
casa *ordenaVetorDeTuplas(casa *vetor, unsigned int n);
void imprimeVetorDeTuplas(casa *vetor, unsigned int n);
casa *copiaVetorDeTuplas(casa *vetor, unsigned int n);

// Funções para nRainhas com backtracking
void inicializaTabuleiro(unsigned int n, tabuleiro *tab, unsigned int k, casa *proibida)
{
    tab->c = (casa **)malloc(n * sizeof(casa *));
    tab->proibida = (unsigned int **)malloc(n * sizeof(unsigned int *));

    for (unsigned int i = 0; i < n; i++)
    {
        tab->c[i] = (casa *)malloc(n * sizeof(casa));
        tab->proibida[i] = (unsigned int *)malloc(n * sizeof(unsigned int));
        for (unsigned j = 0; j < n; j++)
        {
            tab->c[i][j].linha = i;
            tab->c[i][j].coluna = j;
            tab->proibida[i][j] = 0;
        }
    }

    for (unsigned i = 0; i < k; i++)
    {
        tab->proibida[proibida[i].linha - 1][proibida[i].coluna - 1] = 1;
    }
}

void imprimeTabuleiro(unsigned n, tabuleiro *tab)
{
    printf("\n");
    for (unsigned int i = 0; i < n; i++)
    {
        for (unsigned int j = 0; j < n; j++)
        {
            printf("%d ", tab->proibida[i][j]);
        }
        printf("\n");
    }
}

unsigned int casaProibida(unsigned int linha, unsigned int coluna, tabuleiro *tab)
{
    if (tab->proibida[linha][coluna] >= 1)
    {
        return 1;
    }
    return 0;
}

unsigned int rainhaPodeSerColocada(unsigned int n, unsigned int linha, unsigned int coluna, unsigned int *r)
{
    // Verificar se a linha já está ocupada
    if (r[linha] != 0)
    {
        // printf("Linha %d já está ocupada\n", linha);
        return 0;
    }

    // Verificar se a coluna está vazia
    for (unsigned int i = 0; i < n; i++)
    {
        if (r[i] == coluna + 1)
        { // coluna é base 0, r é base 1
            // printf("Coluna %d já está ocupada\n", coluna);
            return 0;
        }
    }

    // Verificar diagonais superiores
    for (unsigned int i = 0; i < linha; i++)
    {
        if (r[i] != 0 && r[i] - 1 - coluna == i - linha)
        {
            // printf("Diagonais superior conflitante na linha %d\n", i);
            return 0;
        }
    }

    return 1;
}

unsigned int rainhasBacktrackingRec(unsigned int n, unsigned int linha, unsigned int *r, tabuleiro *tab)
{
    if (linha == n)
    {
        return 1;
    }
    for (unsigned int j = 0; j < n; j++)
    {
        if (!casaProibida(linha, j, tab) && rainhaPodeSerColocada(n, linha, j, r))
        {
            r[linha] = j + 1;

            // printf("Rainha colocada na linha %d, coluna %d\n", linha + 1, j + 1);

            if (rainhasBacktrackingRec(n, linha + 1, r, tab))
            {
                return 1;
            }

            // Se não encontrou solução, remove a rainha e restaura o tabuleiro
            // printf("Rainha removida da linha %d, coluna %d\n", linha + 1, j + 1);
            r[linha] = 0;
        }
    }

    return 0;
}

unsigned int *rainhas_bt(unsigned int n, unsigned int k, casa *c, unsigned int *r)
{
    tabuleiro tab;
    inicializaTabuleiro(n, &tab, k, c);

    if (!rainhasBacktrackingRec(n, 0, r, &tab))
    {
        printf("Não foi possível encontrar uma solução\n");
        imprimeTabuleiro(n, &tab);
        return NULL;
    }

    return r;
}

// Funções para nRainhas com grafos
nodo *criaNodo(unsigned int vertice, nodo *prox)
{
    nodo *novo = (nodo *)malloc(sizeof(nodo));
    novo->vertice = vertice;
    novo->prox = prox;
    return novo;
}

grafo *criaGrafo(unsigned int n)
{
    grafo *g = (grafo *)malloc(sizeof(grafo));
    g->tam = n;
    g->listasAdj = (nodo **)malloc(n * sizeof(nodo *));

    for (unsigned int i = 0; i < n; i++)
    {
        g->listasAdj[i] = NULL;
    }

    return g;
}

casa *ordenaVetorDeTuplas(casa *vetor, unsigned int n)
{
    casa *c = (casa *)malloc(n * sizeof(casa));
    for (unsigned int i = 0; i < n; i++)
    {
        c[i] = vetor[i];
    }

    for (unsigned int i = 0; i < n; i++)
    {
        for (unsigned int j = i + 1; j < n; j++)
        {
            if (c[i].linha > c[j].linha)
            {
                casa temp = c[i];
                c[i] = c[j];
                c[j] = temp;
            }
            else if (c[i].linha == c[j].linha && c[i].coluna > c[j].coluna)
            {
                casa temp = c[i];
                c[i] = c[j];
                c[j] = temp;
            }
        }
    }



    return c;
}

void imprimeVetorDeTuplas(casa *vetor, unsigned int n)
{
    printf("\n");
    for (unsigned int i = 0; i < n; i++)
    {
        printf("(%d, %d) ", vetor[i].linha, vetor[i].coluna);
    }
    printf("\n");
}

casa *copiaVetorDeTuplas(casa *vetor, unsigned int n)
{
    casa *c = (casa *)malloc(n * sizeof(casa));
    for (unsigned int i = 0; i < n; i++)
    {
        c[i] = vetor[i];
    }
    return ordenaVetorDeTuplas(c, n);
}

//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n
// rainhas com casas proibidas usando a modelagem do problema como
// conjunto independente de um grafo
//
// n, c e r são como em rainhas_bt()

// O Algoritmo ConjIndep(G, n, I, C) abaixo recebe um grafo G, um inteiro positivo n, um conjunto I ⊆ V(G) independente em G e um conjunto
// C ⊆ V(G) − I satisfazendo ΓG(I)∩C = ∅, e devolve um conjunto R independente em G com n vértices satisfazendo I ⊆ R ⊆ I∪C, ou "não" caso não exista tal conjunto.
unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r)
{
    // zera o vetor resposta
    for (unsigned int i = 0; i < n; i++)
    {
        r[i] = 0;
    }

    // Cria o grafo
    grafo *g = criaGrafo(n);
    g = g; // só para evitar warning de variável não usada

    // Cria o vetor de casas proibidas ordenado
    casa *cProibida = copiaVetorDeTuplas(c, k);
    // imprimeVetorDeTuplas(cProibida, k);
    
    // Cria o vetor de casas proibidas ordenado


    return NULL;
}
