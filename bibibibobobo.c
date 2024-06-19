#include "rainhas.h"
#include <stdio.h>
#include <stdlib.h>


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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Grafo
nodo *criaNodo(unsigned int vertice, nodo *prox);
grafo *criaGrafo(unsigned int n);
void imprimeGrafo(grafo *g);
unsigned int *conjIndependente(unsigned int n, grafo *Grafo, grafo *Independente, grafo *Conjunto, unsigned int *r, casa *Proibidas);
casa *ordenaVetorDeTuplas(casa *vetor, unsigned int n);
void imprimeVetorDeTuplas(casa *vetor, unsigned int n);
casa *copiaVetorDeTuplas(casa *vetor, unsigned int n);
void adicionaAresta(grafo *g, unsigned int u, unsigned int v);
void montaConjunto(grafo *g, grafo *c, grafo *Independente);
void montaGrafo(grafo *g, casa *proibidas);
int casaProibidaDois(unsigned int i, unsigned int j, casa *proibidas);
unsigned int calcSqrt(unsigned int n);
int ehAdjacente(nodo *n, nodo *listaIndependente);
void liberaGrafo(grafo *g);
void liberaVetorDeTuplas(casa *vetor);
nodo *removeVertice(grafo *g, unsigned int vertice);
void adicionaAoConjunto(grafo *g, grafo *Independente, nodo *n);


// Funções para nRainhas com grafos
nodo *criaNodo(unsigned int vertice, nodo *prox)
{
    nodo *n = (nodo *)malloc(sizeof(nodo));
    if(!n){
        printf("Erro ao alocar memória\n");
        exit(1);
    }
    n->vertice = vertice;
    n->prox = prox;
    return n;
}

grafo *criaGrafo(unsigned int n) {
    grafo *g = (grafo *)malloc(sizeof(grafo));
    if (!g) {
        printf("Erro ao alocar memória para o grafo\n");
        exit(1);
    }
    g->tam = n * n;
    // printf("Tamanho do grafo: %d\n", g->tam);
    g->listasAdj = (nodo **)malloc(g->tam * sizeof(nodo *));
    if (!g->listasAdj) {
        printf("Erro ao alocar memória para listasAdj\n");
        exit(1);
    }
    for (unsigned int i = 0; i < g->tam; i++) {
        g->listasAdj[i] = NULL;
    }
    return g;
}

void montaGrafo(grafo *g, casa *proibidas) {
    unsigned int tamanhoTabuleiro = calcSqrt(g->tam);
    for (unsigned int i = 0; i < g->tam; i++) {
        unsigned int iLinha = i / tamanhoTabuleiro;
        unsigned int iColuna = i % tamanhoTabuleiro;

        for (unsigned int j = i + 1; j < g->tam; j++) {
            if (casaProibidaDois(i, j, proibidas)) {
                continue;
            }

            unsigned int jLinha = j / tamanhoTabuleiro;
            unsigned int jColuna = j % tamanhoTabuleiro;

            if (iLinha == jLinha || // mesma linha
                iColuna == jColuna || // mesma coluna
                iLinha - jLinha == iColuna - jColuna) // mesma diagonal
            {
                // printf("Adicionando aresta entre %d e %d\n", i, j);
                adicionaAresta(g, i, j);
            }
        }
    }
}

void copiaNodo(grafo *g, unsigned int vertice){
    nodo *novo = criaNodo(vertice, g->listasAdj[vertice]);
    novo->prox = g->listasAdj[vertice];
    g->listasAdj[vertice] = novo;
    g->tam++;
}

void imprimeGrafo(grafo *g)
{
    printf("\n");
    for (unsigned int i = 0; i < g->tam; i++)
    {
        if (g->listasAdj[i] == NULL)
        {
            printf("Vértice %d: NULL\n", i);
        }
        else
        {
            printf("Vértice %d: ", i);
            nodo *n = g->listasAdj[i];
            while (n != NULL)
            {
                printf("%d ", n->vertice);
                n = n->prox;
            }
            printf("\n");
        }
    }
}

void adicionaAresta(grafo *g, unsigned int u, unsigned int v)
{
    nodo *novo = criaNodo(v, g->listasAdj[u]);
    novo->prox = g->listasAdj[u];
    g->listasAdj[u] = novo;

    novo = criaNodo(u, g->listasAdj[v]);
    novo->prox = g->listasAdj[v];
    g->listasAdj[v] = novo;
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

unsigned int *conjIndependente(unsigned int n, grafo *Grafo, grafo *Independente, grafo *Conjunto, unsigned int *r, casa *Proibidas){
    if (Independente->tam == n)
    {
        // printf("Conjunto independente encontrado\n");
        for (unsigned int i = 0; i < n; i++)
        {
            r[i] = Independente->listasAdj[i]->vertice;
        }
        return r;
    }

    if (Independente->tam + Conjunto->tam < n)
    {
        // printf("Conjunto independente não encontrado\n");
        return NULL;
    }
    // printf("Tamanho do conjunto: %d\n", Conjunto->tam-1);
    unsigned int vertice = Conjunto->listasAdj[Conjunto->tam-1]->vertice;
    
    nodo *nodoR = removeVertice(Conjunto, vertice);
    // printf("Nodo Novo %d\n", nodoR->vertice);
    if (nodoR == NULL) {
        // printf("Erro ao remover vértice\n");
        return NULL;
    }

    adicionaAoConjunto(Grafo, Independente, nodoR);
    Conjunto = criaGrafo(n);

    montaConjunto(Grafo, Conjunto, Independente);

    printf("I:\n");
    imprimeGrafo(Independente);

    printf("C:\n");
    imprimeGrafo(Conjunto);

    r = conjIndependente(n, Grafo, Independente, Conjunto, r, Proibidas);

    if(r && r[0] != 0){
        // printf("tem r\n");
        return r;
    }
    
    
    return conjIndependente(n, Grafo, Independente, Conjunto, r, Proibidas);
}

// TODO: mudar o nome da função 
int casaProibidaDois(unsigned int i, unsigned int j, casa *proibidas)
{
    for (unsigned int k = 0; k < 8; k++)
    {
        if (proibidas[k].linha == i && proibidas[k].coluna == j)
        {
            return 1;
        }
    }
    return 0;
}

unsigned int calcSqrt(unsigned int n)
{
    unsigned int i = 1;
    while (i * i < n)
    {
        i++;
        if (i * i == n)
        {
            return i;
        }
    }

    return 0;
}

// Recebe um nodo e a lista de adjacencia de outro nodo
// Percorre a lista de adjacencia e verifica se o nodo é adjacente a algum nodo da lista
// Se for, retorna 1, senão retorna 0
int ehAdjacente(nodo *n, nodo *listaIndependente) {
    while (listaIndependente != NULL) {
        if (n->vertice == listaIndependente->vertice) {
            return 1;
        }
        listaIndependente = listaIndependente->prox;
    }
    return 0;
}



// Checa se cada nodo n do grafo g é adjacente a algum nodo da lista de adjacencia do nodo i do grafo Independente
// Se não for, adiciona a aresta ao grafo c
void montaConjunto(grafo *g, grafo *c, grafo *Independente) {
    // Pega o primeiro nodo do grafo Independente
    nodo *n = Independente->listasAdj[0];

    for (unsigned int i = 0; i < g->tam; i++) {
        nodo *temp = g->listasAdj[i];
        while (temp != NULL) {
            if (!ehAdjacente(temp, n)) {
                // adicionaAresta(c, i, temp->vertice);
                copiaNodo(c, i);
            }
            temp = temp->prox;
        }
    }

}
void liberaGrafo(grafo *g) {
    for (unsigned int i = 0; i < g->tam; i++) {
        nodo *n = g->listasAdj[i];
        while (n != NULL) {
            nodo *temp = n;
            n = n->prox;
            free(temp);
        }
    }
    free(g->listasAdj);
    free(g);
}

void liberaVetorDeTuplas(casa *vetor) {
    
    free(vetor);

}

void adicionaAoConjunto(grafo *g, grafo *Independente, nodo *n) {
    Independente->listasAdj[Independente->tam] = n;
    Independente->tam++;

    for (unsigned int i = 0; i < g->tam; i++) {
        nodo *temp = g->listasAdj[i];
        while (temp != NULL) {
            if (temp->vertice == n->vertice) {
                temp->proibida = 1;
            }
            temp = temp->prox;
        }
    }



}

nodo *removeVertice(grafo *g, unsigned int verticeR) {
    nodo *n = g->listasAdj[verticeR];
    g->listasAdj[verticeR] = n->prox;
    return n;
}

unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r) {
    for (unsigned int i = 0; i < n; i++) {
        r[i] = 0;
    }

    casa *cProibida = copiaVetorDeTuplas(c, k);
    grafo *g = criaGrafo(n);
    montaGrafo(g, cProibida);

    grafo *Independente = criaGrafo(n);
    Independente->listasAdj[0] = g->listasAdj[0];
    Independente->tam = 1;

    grafo *Conjunto = criaGrafo(n);
    montaConjunto(g, Conjunto, Independente);

    conjIndependente(n, g, Independente, Conjunto, r, cProibida);
    liberaGrafo(g);
    liberaGrafo(Independente);
    liberaGrafo(Conjunto);
    liberaVetorDeTuplas(cProibida);

    return r;
}
