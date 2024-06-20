#include "rainhas.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct tabuleiro
{
    casa **c;
    unsigned int **proibida;
} tabuleiro;

typedef struct Nodo
{
    unsigned int vertice;
    unsigned int proibida;
    struct Nodo *prox;
} Nodo;

typedef struct grafo
{
    Nodo **listasAdj;
    unsigned int tam;
    unsigned int tam2;
} Grafo;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Protótipos

// Backtracking
void inicializaTabuleiro(unsigned int n, tabuleiro *tab, unsigned int k, casa *proibida);
void imprimeTabuleiro(unsigned n, tabuleiro *tab);
unsigned int casaProibida(unsigned int linha, unsigned int coluna, tabuleiro *tab);
unsigned int rainhaPodeSerColocada(unsigned int n, unsigned int linha, unsigned int coluna, unsigned int *r);
unsigned int rainhasBacktrackingRec(unsigned int n, unsigned int linha, unsigned int *r, tabuleiro *tab);


// Grafo

// Cria um novo Nodo
Nodo *criaNodo(unsigned int v);
// Cria um novo Grafo
Grafo *criaGrafo(unsigned int numVertices);
// Insere uma aresta entre dois vértices
void insereAresta(Grafo *Grafo,  unsigned int v1, unsigned int v2);
// Verifica se um vértice é adjacente a outro
int ehAdjacente(Grafo *Grafo, unsigned int v1,unsigned int v2);
// Imprime o Grafo
void imprimeGrafo(Grafo *Grafo);
// Copia um Nodo
Nodo* copiaNodo(Nodo *Nodo);

// Remove um vértice do Grafo e retorna uma copia dele
int removeNodo(Grafo *grafo, unsigned int v);

// Retorna o ultimo vértice do Grafo
unsigned int encontraUltimo(Grafo *g);

// Monta um conjunto com todos os vertices do grafo que não sejam adjacentes a nenhum vertice do grafo independente
Grafo* conjuntoNaoAdjacente(Grafo *grafo, Grafo *independente);

// Adiciona um vértice ao grafo independente
unsigned int adicionaVertice(Grafo *grafo, Grafo *independente, unsigned int v);

unsigned int *conjIndependente(unsigned int n, Grafo *grafo, Grafo *Independente, Grafo *Conjunto, unsigned int *r, casa *Proibidas);
casa *ordenaVetorDeTuplas(casa *vetor, unsigned int n);
void imprimeVetorDeTuplas(casa *vetor, unsigned int n);
casa *copiaVetorDeTuplas(casa *vetor, unsigned int n);
unsigned int montaConjunto(Grafo *g, Grafo *c, Grafo *Independente);
void montaGrafo(Grafo *g, casa *proibidas);
int casaProibidaDois(unsigned int i, unsigned int j, casa *proibidas);
unsigned int calcSqrt(unsigned int n);
void liberaGrafo(Grafo *g);
void liberaVetorDeTuplas(casa *vetor);
void adicionaAoConjunto(Grafo *g, Grafo *Independente, Nodo *n);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

            if (rainhasBacktrackingRec(n, linha + 1, r, tab))
            {
                return 1;
            }

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Funções para nRainhas com grafo

Nodo *criaNodo(unsigned int v){
    Nodo *novo = (Nodo *)malloc(sizeof(Nodo));
    novo->vertice = v;
    novo->prox = NULL;
    return novo;
}

Grafo *criaGrafo(unsigned int numVertices){
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));

    grafo->tam = numVertices * numVertices;
    grafo->tam2 = 0;
    grafo->listasAdj = (Nodo **)malloc(grafo->tam * sizeof(Nodo *));

    
    for(unsigned int i = 0; i < grafo->tam; i++)
        grafo->listasAdj[i] = NULL;
    
    return grafo;
}

void insereAresta(Grafo *grafo, unsigned int v1, unsigned int v2){
    // Insere o vértice v2 na lista de adjacência de v1
    Nodo *novo = criaNodo(v2);
    novo->prox = grafo->listasAdj[v1];
    grafo->listasAdj[v1] = novo;

    // Insere o vértice v1 na lista de adjacência de v2
    novo = criaNodo(v1);
    novo->prox = grafo->listasAdj[v2];
    grafo->listasAdj[v2] = novo;
}

void imprimeGrafo(Grafo *grafo){
    for(unsigned int i = 0; i < grafo->tam; i++){
        Nodo *nodo = grafo->listasAdj[i];
        if(nodo == NULL)
            continue;

        printf("Lista de adjacência do vértice %d\n", i);
        while(nodo){
            printf("%d -> ", nodo->vertice);
            nodo = nodo->prox;
        }
        printf("NULL\n");
    }
}

Nodo* copiaNodo(Nodo *nodo){
    if(nodo == NULL)
        return NULL;
    
    Nodo *novo = criaNodo(nodo->vertice);
    novo->prox = copiaNodo(nodo->prox);
    return novo;
}

int ehAdjacente(Grafo *grafo,unsigned int v1,unsigned int v2){
    if(v1 == v2){
        // printf("%d é adjacente a %d\n", v1, v2);
        return 1;
    }

    Nodo *nodo = grafo->listasAdj[v1];
    while(nodo){
        if(nodo->vertice == v2){
            // printf("%d é adjacente a %d\n", v1, v2);
            return 1;
        }
        nodo = nodo->prox;
    }
    // printf("%d não é adjacente a %d\n", v1, v2);
    return 0;
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


void montaGrafo(Grafo *g, casa *proibidas) {
    unsigned int tamanhoTabuleiro = calcSqrt(g->tam);
    for(int i = 0; i < g->tam; i++){
        int linha = i / tamanhoTabuleiro;
        int coluna = i % tamanhoTabuleiro;
        
        if(casaProibidaDois(linha, coluna, proibidas)){
            continue;
        }

        for(int j = i+1; j < g->tam; j++){
            int linha2 = j / tamanhoTabuleiro;
            int coluna2 = j % tamanhoTabuleiro;

            if(casaProibidaDois(linha2, coluna2, proibidas)){
                continue;
            }

            if(linha == linha2 || coluna == coluna2 || linha - coluna == linha2 - coluna2 || linha + coluna == linha2 + coluna2){
                insereAresta(g, i, j);
            }
        }

    }
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

unsigned int encontraUltimo(Grafo *g){
    unsigned int ultimo = g->tam2;
    // printf("Tam %d\n", g->tam2);
    for(unsigned int i = 0; i < g->tam; i++){
        Nodo *nodo = g->listasAdj[i];
        // printf("Nodo %d\n", i);
        if(nodo == NULL)
            continue;
        else
            ultimo = i;
    }

    // printf("Ultimo %d\n", ultimo);
    return ultimo;
}

unsigned int *conjIndependente(unsigned int n, Grafo *grafo, Grafo *Independente, Grafo *Conjunto, unsigned int *r, casa *Proibidas){
    // printf("Grafo tam %d\n", grafo->tam);   
    if (Independente->tam2 == n)
    {
        // printf("Conjunto independente encontrado\n");
        unsigned int j = 0;

        for(int i = 0; i < Independente->tam; i++){
            if(Independente->listasAdj[i] != NULL){
                unsigned int iColuna = i % calcSqrt(grafo->tam);
                unsigned int iLinha = i / calcSqrt(grafo->tam);

                r[j] = iColuna + 1;
                j++;
            }
            else{
                r[j] =  0;
            }
        }
        return r;
    }

    if (Conjunto->tam2 + Independente->tam2 < n)
    {
        // printf("Conjunto independente não encontrado\n");
        return r;
    }

    // printf("Conjunto tam %d\n", Conjunto->tam2);
    unsigned int vertice = encontraUltimo(Conjunto);
    if(vertice > Conjunto->tam){
        return r;
    }

    int nodoR = removeNodo(Conjunto, vertice);
    // printf("Nodo Novo %d\n", nodoR);
    if (nodoR == NULL) {
        return r;
    }

    adicionaVertice(grafo, Independente, nodoR);


    Conjunto = conjuntoNaoAdjacente(grafo, Independente);


    r = conjIndependente(n, grafo, Independente, Conjunto, r, Proibidas);

    if(r && r[0] != 0){
        // printf("tem r\n");
        return r;
    }
    
    // printf("Não tem r\n");
    return conjIndependente(n, grafo, Independente, Conjunto, r, Proibidas);
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



// Checa se cada Nodo n do Grafo g é adjacente a algum Nodo da lista de adjacencia do Nodo i do Grafo Independente
// Se não for, adiciona a aresta ao Grafo c
unsigned int montaConjunto(Grafo *g, Grafo *c, Grafo *Independente) {
    c->tam2 = 0;
    // printf("Montando Conjunto\n");

    // Iterar sobre todos os vértices do grafo g
    for (unsigned int i = 0; i < g->tam; i++) {
        unsigned int adjacente = 0;

        // Verificar se o vértice i é adjacente a qualquer vértice de Independente
        for (unsigned int j = 0; j < Independente->tam; j++) {
            if (Independente->listasAdj[j] != NULL && ehAdjacente(g, i, j)) {
                adjacente = 1;
                break;
            }
        }

        // Se o vértice i não é adjacente a nenhum vértice de Independente, adicionar ao grafo c
        if (!adjacente) {
            c->listasAdj[i] = copiaNodo(g->listasAdj[i]);
            c->tam2++;
        }
    }

    return c->tam2;
}

Grafo* conjuntoNaoAdjacente(Grafo *grafo, Grafo *independente) {
    Grafo *naoAdjacente = criaGrafo(grafo->tam);
    naoAdjacente->tam2 = 0;

    int adjacente = 0;
    for (unsigned int i = 0; i < grafo->tam; i++) {
        adjacente = 0;
        for (unsigned int j = 0; j < independente->tam; j++) {
            // printf("Verificando se vértice %d é adjacente ao vértice %d do grafo independente\n", i, j);
            if (independente->listasAdj[j] != NULL && ehAdjacente(grafo, i, j)) {
                adjacente = 1;
                // printf("Vértice %d é adjacente ao vértice %d do grafo independente\n", i, j);
                break;
            }
        }
        if (!adjacente) {
            // printf("Vértice %d não é adjacente a nenhum vértice do grafo independente\n", i);
            naoAdjacente->listasAdj[i] = copiaNodo(grafo->listasAdj[i]);
            naoAdjacente->tam2++;
        }
    }

    return naoAdjacente;
}

void liberaGrafo(Grafo *g) {
    for (unsigned int i = 0; i < g->tam; i++) {
        Nodo *n = g->listasAdj[i];
        while (n != NULL) {
            Nodo *temp = n;
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

void adicionaAoConjunto(Grafo *g, Grafo *Independente, Nodo *n) {
    Independente->listasAdj[Independente->tam] = n;
    Independente->tam2++;

    for (unsigned int i = 0; i < g->tam; i++) {
        Nodo *temp = g->listasAdj[i];
        while (temp != NULL) {
            if (temp->vertice == n->vertice) {
                break;
            }
            temp = temp->prox;
        }
    }
}

unsigned int adicionaVertice(Grafo *grafo, Grafo *independente, unsigned int v){
    if (independente->listasAdj[v] != NULL) {
        // printf("Vértice %d já está no grafo independente\n", v);
        return 0;
    }

    if (independente->listasAdj[v] == NULL) {
        // printf("Vértice %d adicionado ao grafo independente\n", v);
        independente->listasAdj[v] = copiaNodo(grafo->listasAdj[v]);
        independente->tam2++;
        return 1;
    }
    return 0;
}

int removeNodo(Grafo *grafo, unsigned int v){
    Nodo *nodo = grafo->listasAdj[v];
    grafo->listasAdj[v] = NULL;
    return v;
}


unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r)
{
    // zera o vetor resposta
    for (unsigned int i = 0; i < n; i++)
    {
        r[i] = 0;
    }

    // Cria o vetor de casas proibidas ordenado
    casa *cProibida = copiaVetorDeTuplas(c, k);
    // imprimeVetorDeTuplas(cProibida, k);

    // Cria o Grafo
    Grafo *g = criaGrafo(n);
    montaGrafo(g, cProibida);
    // printf("Grafo\n");
    // imprimeGrafo(g);

    // Cria o conjunto independente
    Grafo *Independente = criaGrafo(n);
    Independente->listasAdj[0] = g->listasAdj[0];
    Independente->tam2 = 1;



    // printf("Conjunto Independente\n");
    // imprimeGrafo(Independente);

    Grafo *Conjunto = conjuntoNaoAdjacente(g, Independente);
    // printf("Conjunto\n");
    // imprimeGrafo(Conjunto);
    
    conjIndependente(n, g, Independente, Conjunto, r, cProibida);
   // Cria o vetor de casas proibidas ordenado
    liberaGrafo(g);

    return r;
}
