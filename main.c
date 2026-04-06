/*
============================================================
            PROJETO DE TEORIA DOS GRAFOS
                COMPONENTES E CORTES
============================================================

Integrantes do grupo:
Bruna Soncini                  RA: 10428267
Gustavo Galhardo Rodrigues     RA: 10403091
Pedro Nogueira Ribeiro         RA: 10324924
Tales Hernandes                RA: 10408846
============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>

/* Estrutura de dados para representar grafos */
typedef struct a
{ /* Celula de uma lista de arestas */
    int vizinho;
    struct a *prox;
} Aresta;

typedef struct v
{
    int nome;
    int marcado; /* Usado por eConexo: 0 = Nao visitado, 1 = Visitado */
    Aresta *prim;
} Vertice;

/* Declaracoes das funcoes para manipulacao de grafos */
void criaGrafo(Vertice **G, int ordem);
void destroiGrafo(Vertice **G, int ordem);
int acrescentaAresta(Vertice G[], int ordem, int v1, int v2);
void imprimeGrafo(Vertice G[], int ordem);
int eConexo(Vertice G[], int ordem);
int quantidadeComponentesConexas(Vertice G[], int ordem);

/* Vertices e Arestas de Corte */
int componentesGrafoSemVertice(Vertice G[], int ordem, int v_ignorado);
void exibeVerticesDeCorte(Vertice G[], int ordem);
int componentesGrafoSemAresta(Vertice G[], int ordem, int v1_ignorado, int v2_ignorado);
void exibeArestasDeCorte(Vertice G[], int ordem);

/* Criacao de um grafo com ordem predefinida (passada como argumento), e, inicilamente, sem nenhuma aresta */
void criaGrafo(Vertice **G, int ordem)
{
    int i;
    *G = (Vertice *)malloc(sizeof(Vertice) * ordem); /* Alocacao dinamica de um vetor de vertices */

    for (i = 0; i < ordem; i++)
    {
        (*G)[i].nome = i;
        (*G)[i].marcado = 0;
        (*G)[i].prim = NULL; /* Cada vertice sem nenhuma aresta incidente */
    }
}

/* Desaloca a memoria dinamica usada para armazenar um grafo */
void destroiGrafo(Vertice **G, int ordem)
{
    int i;
    Aresta *a, *n;

    for (i = 0; i < ordem; i++)
    { /* Remove lista de adjacencia de cada vertice */
        a = (*G)[i].prim;
        while (a != NULL)
        {
            n = a->prox;
            free(a);
            a = n;
        }
    }
    free(*G); /* Remove o vetor de vertices */
}

/* * Acrescenta uma nova aresta em um grafo previamente criado.
 * Devem ser passados os extremos v1 e v2 da aresta a ser acrescentada
 * Como o grafo nao e orientado, para uma aresta com extremos i e j, quando
 * i != j, serao criadas, na estrutura de dados, arestas (i,j) e (j,i) .
 */
int acrescentaAresta(Vertice G[], int ordem, int v1, int v2)
{
    Aresta *A1, *A2;

    if (v1 < 0 || v1 >= ordem) /* Testo se vertices sao validos */
        return 0;
    if (v2 < 0 || v2 >= ordem)
        return 0;

    /* Acrescento aresta na lista do vertice v1 */
    A1 = (Aresta *)malloc(sizeof(Aresta));
    A1->vizinho = v2;
    A1->prox = G[v1].prim;
    G[v1].prim = A1;

    if (v1 == v2)
        return 1; /* Aresta e um laco */

    /* Acrescento aresta na lista do vertice v2 se v2 != v1 */
    A2 = (Aresta *)malloc(sizeof(Aresta));
    A2->vizinho = v1;
    A2->prox = G[v2].prim;
    G[v2].prim = A2;

    return 1;
}

/* Imprime um grafo com uma notacao similar a uma lista de adjacencia */
void imprimeGrafo(Vertice G[], int ordem)
{
    int i;
    Aresta *aux;

    printf("\nOrdem:   %d", ordem);
    printf("\nLista de Adjacencia:\n");

    for (i = 0; i < ordem; i++)
    {
        printf("    v%d: ", i);
        aux = G[i].prim;
        for (; aux != NULL; aux = aux->prox)
            printf("  v%d", aux->vizinho);
        printf("\n");
    }
    printf("\n");
}

/*
 * Recebe um grafo G nao vazio como argumento e retorna:
 * 1, se o grafo for conexo; ou
 * 0, se o grafo nao for conexo.
 */
int eConexo(Vertice G[], int ordem)
{
    int i;
    int marcouNovo; /* Controla se algum vertice novo foi marcado na iteracao */
    Aresta *aux;    /* Ponteiro auxiliar para percorrer a lista de arestas */

    G[0].marcado = 1;
    for (i = 1; i < ordem; i++)
        G[i].marcado = 0;

    do
    {
        marcouNovo = 0;
        for (i = 0; i < ordem; i++)
        {
            if (G[i].marcado == 1)
            {
                aux = G[i].prim;
                while (aux != NULL)
                {
                    if (G[aux->vizinho].marcado == 0)
                    {
                        G[aux->vizinho].marcado = 1;
                        marcouNovo = 1;
                    }
                    aux = aux->prox;
                }
            }
        }
    } while (marcouNovo);

    for (i = 0; i < ordem; i++)
        if (G[i].marcado == 0)
            return 0; /* Grafo nao e conexo */

    return 1; /* Grafo e conexo */
}

/*Retorna a quantidade de componentes conexas do grafo.*/
int quantidadeComponentesConexas(Vertice G[], int ordem)
{
    int i;
    int componentes = 0;
    int marcouNovo;
    Aresta *aux;

    if (ordem <= 0)
        return 0;

    for (i = 0; i < ordem; i++)
        G[i].marcado = 0;

    for (i = 0; i < ordem; i++)
    {
        int j;
        if (G[i].marcado == 1)
            continue;

        componentes++;
        G[i].marcado = 1;

        do
        {
            marcouNovo = 0;
            for (j = 0; j < ordem; j++)
            {
                if (G[j].marcado == 1)
                {
                    aux = G[j].prim;
                    while (aux != NULL)
                    {
                        if (G[aux->vizinho].marcado == 0)
                        {
                            G[aux->vizinho].marcado = 1;
                            marcouNovo = 1;
                        }
                        aux = aux->prox;
                    }
                }
            }
        } while (marcouNovo);
    }
    return componentes;
}

/* Conta componentes ignorando o vertice 'v_ignorado'*/
int componentesGrafoSemVertice(Vertice G[], int ordem, int v_ignorado)
{
    int i, j, componentes = 0, marcouNovo;
    Aresta *aux;

    if (ordem <= 0)
        return 0;

    for (i = 0; i < ordem; i++)
        G[i].marcado = 0;

    /* Marca o vertice que queremos ignorar como 'visitado' logo de cara,
       assim o algoritmo pula ele e finge que ele nao existe */
    if (v_ignorado >= 0 && v_ignorado < ordem)
        G[v_ignorado].marcado = 1;

    for (i = 0; i < ordem; i++)
    {
        if (G[i].marcado == 1)
            continue;

        componentes++;
        G[i].marcado = 1;

        do
        {
            marcouNovo = 0;
            for (j = 0; j < ordem; j++)
            {
                if (j == v_ignorado)
                    continue; /* Protecao extra para pular o vertice */

                if (G[j].marcado == 1)
                {
                    aux = G[j].prim;
                    while (aux != NULL)
                    {
                        /* Nao podemos "viajar" para o vertice ignorado */
                        if (aux->vizinho != v_ignorado && G[aux->vizinho].marcado == 0)
                        {
                            G[aux->vizinho].marcado = 1;
                            marcouNovo = 1;
                        }
                        aux = aux->prox;
                    }
                }
            }
        } while (marcouNovo);
    }

    return componentes;
}

/* Conta componentes ignorando a aresta entre v1_ignorado e v2_ignorado */
int componentesGrafoSemAresta(Vertice G[], int ordem, int v1_ignorado, int v2_ignorado)
{
    int i, j, componentes = 0, marcouNovo;
    Aresta *aux;

    if (ordem <= 0)
        return 0;

    for (i = 0; i < ordem; i++)
        G[i].marcado = 0;

    for (i = 0; i < ordem; i++)
    {
        if (G[i].marcado == 1)
            continue;

        componentes++;
        G[i].marcado = 1;

        do
        {
            marcouNovo = 0;

            for (j = 0; j < ordem; j++)
            {
                if (G[j].marcado == 1)
                {
                    aux = G[j].prim;

                    while (aux != NULL)
                    {
                        /* Ignora a aresta escolhida nos dois sentidos:
                           (v1_ignorado, v2_ignorado) e (v2_ignorado, v1_ignorado) */
                        if (!((j == v1_ignorado && aux->vizinho == v2_ignorado) ||
                              (j == v2_ignorado && aux->vizinho == v1_ignorado)))
                        {

                            if (G[aux->vizinho].marcado == 0)
                            {
                                G[aux->vizinho].marcado = 1;
                                marcouNovo = 1;
                            }
                        }

                        aux = aux->prox;
                    }
                }
            }

        } while (marcouNovo);
    }

    return componentes;
}

/* Testa todos os vertices e exibe os que sao de corte*/
void exibeVerticesDeCorte(Vertice G[], int ordem)
{
    int i;
    int qtdOriginal = quantidadeComponentesConexas(G, ordem);
    int encontrouAlgum = 0;

    printf("Vertices de Corte: ");
    for (i = 0; i < ordem; i++)
    {
        int qtdSemVertice = componentesGrafoSemVertice(G, ordem, i);

        /* O vertice i e de corte se remove-lo fragmenta o grafo,
           ou seja, aumenta o numero de componentes originais. */
        if (qtdSemVertice > qtdOriginal)
        {
            printf("v%d ", i);
            encontrouAlgum = 1;
        }
    }

    if (!encontrouAlgum)
    {
        printf("Nenhum");
    }
    printf("\n");
}

/* Testa todas as arestas e exibe as que sao de corte */
void exibeArestasDeCorte(Vertice G[], int ordem)
{
    int i;
    int qtdOriginal = quantidadeComponentesConexas(G, ordem);
    int encontrouAlguma = 0;
    Aresta *aux;

    printf("Arestas de Corte: ");
    for (i = 0; i < ordem; i++)
    {
        aux = G[i].prim;

        while (aux != NULL)
        {
            /* Para nao testar a mesma aresta duas vezes,
               so consideramos quando i < vizinho */
            if (i < aux->vizinho)
            {
                int qtdSemAresta = componentesGrafoSemAresta(G, ordem, i, aux->vizinho);

                if (qtdSemAresta > qtdOriginal)
                {
                    printf("(v%d,v%d) ", i, aux->vizinho);
                    encontrouAlguma = 1;
                }
            }

            aux = aux->prox;
        }
    }

    if (!encontrouAlguma)
    {
        printf("Nenhuma");
    }

    printf("\n");
}

/*
 * Programa principal
 */
int main(int argc, char *argv[])
{
    int qtdComponentes;
    Vertice *G;
    int ordemG = 5; /* Vertices identificado de 0 ate 9 */

    printf("Criando grafo...\n");
    
    criaGrafo(&G, ordemG);
    acrescentaAresta(G, 5, 0, 1);
    acrescentaAresta(G, 5, 1, 2);
    acrescentaAresta(G, 5, 2, 3);
    acrescentaAresta(G, 5, 3, 4);
    
    printf("Imprimindo grafo...\n");
    imprimeGrafo(G, 4);

    /* Testa eConexo */
    if (eConexo(G, ordemG))
        printf("O grafo E conexo.\n");
    else
        printf("O grafo NAO e conexo.\n");

    /* Quantidade de Componentes */
    qtdComponentes = quantidadeComponentesConexas(G, ordemG);
    printf("Quantidade de componentes conexas: %d\n", qtdComponentes);

    /* Vértices e Arestas de Corte */
    exibeVerticesDeCorte(G, ordemG);
    exibeArestasDeCorte(G, ordemG);

    destroiGrafo(&G, ordemG);

    printf("\nPressione ENTER para terminar\n");
    getchar();
    return (0);
}
