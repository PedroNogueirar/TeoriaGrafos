#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<memory.h>

 * Estrutura de dados para representar grafos
typedef struct a{ /* Celula de uma lista de arestas */
	int    vizinho;
	struct a *prox;
} Aresta;

typedef struct v{
	int nome;
	int marcado;   /* Usado por eConexo: 0 = Nao visitado, 1 = Visitado */
	Aresta *prim;
} Vertice;

/* Declaracoes das funcoes para manipulacao de grafos */
void criaGrafo(Vertice **G, int ordem);
void destroiGrafo(Vertice **G, int ordem);
int  acrescentaAresta(Vertice G[], int ordem, int v1, int v2);
void imprimeGrafo(Vertice G[], int ordem);
int  eConexo(Vertice G[], int ordem);
 

/* Criacao de um grafo com ordem predefinida (passada como argumento), e, inicilamente, sem nenhuma aresta */
void criaGrafo(Vertice **G, int ordem){
	int i;
	*G= (Vertice*) malloc(sizeof(Vertice)*ordem); /* Alcacao dinamica de um vetor de vertices */
	
	for(i=0; i<ordem; i++){
		(*G)[i].nome= i;
		(*G)[i].prim= NULL;    /* Cada vertice sem nenhuma aresta incidente */
	}
}

/* Desaloca a memoria dinamica usada para armazenar um grafo */
void destroiGrafo(Vertice **G, int ordem){
	int i;
    Aresta *a, *n;
    
	for(i=0; i<ordem; i++){ /* Remove lista de adjacencia de cada vertice */
	    a= (*G)[i].prim;
        while (a!= NULL){
              n= a->prox;
              free(a);
              a= n;
        }
	}
    free(*G);  /* Remove o vetor de vertices */
}

/*  
 * Acrescenta uma nova aresta em um grafo previamente criado. 
 *   Devem ser passados os extremos v1 e v2 da aresta a ser acrescentada 
 * Como o grafo nao e orientado, para uma aresta com extremos i e j, quando
 *   i != j, serao criadas, na estrutura de dados, arestas (i,j) e (j,i) .
 */
int acrescentaAresta(Vertice G[], int ordem, int v1, int v2){
    Aresta *A1, *A2;
    
	if (v1<0 || v1 >= ordem) /* Testo se vertices sao validos */
	   return 0;
	if (v2<0 || v2 >= ordem)
	   return 0;
	
	/* Acrescento aresta na lista do vertice v1 */
	A1= (Aresta *) malloc(sizeof(Aresta));
	A1->vizinho= v2;
	A1->prox= G[v1].prim;
	G[v1].prim= A1;

	if (v1 == v2) return 1; /* Aresta e um laco */

	/* Acrescento aresta na lista do vertice v2 se v2 != v1 */	
	A2= (Aresta *) malloc(sizeof(Aresta));
	A2->vizinho= v1;
	A2->prox= G[v2].prim;
	G[v2].prim= A2;
	
	return 1;
}


/* Imprime um grafo com uma notacao similar a uma lista de adjacencia */
void imprimeGrafo(Vertice G[], int ordem){
	int i;
	Aresta *aux;

	printf("\nOrdem:   %d",ordem);
	printf("\nLista de Adjacencia:\n");

	for (i=0; i<ordem; i++){
		printf("\n    v%d: ", i);
		aux= G[i].prim;
		for( ; aux != NULL; aux= aux->prox)
			printf("  v%d", aux->vizinho);
	}
	printf("\n\n");

}

/*
 * Recebe um grafo G nao vazio como argumento e retorna:
 *   1, se o grafo for conexo; ou
 *   0, se o grafo nao for conexo.
 *
 * Utiliza o campo 'marcado' da struct Vertice como marcacao:
 *   0 = nao visitado, 1 = visitado.
 */

int eConexo(Vertice G[], int ordem){
    int i;
    int marcouNovo;   /* Controla se algum vertice novo foi marcado na iteracao */
    Aresta *aux;      /* Ponteiro auxiliar para percorrer a lista de arestas */

    /* Passos 1 e 2: marca v0 com 1 e todos os demais com 0 */
    G[0].marcado = 1;
    for(i = 1; i < ordem; i++)
        G[i].marcado = 0;

    /* Passo 3: repete ate que nenhum novo vertice seja marcado */
    do {
        marcouNovo = 0;

        /* Percorre todos os vertices vi marcados com 1 */
        for(i = 0; i < ordem; i++){
            if(G[i].marcado == 1){

                /* Percorre cada vizinho vj de vi pela lista de arestas */
                aux = G[i].prim;
                while(aux != NULL){
                    /* Se vj esta marcado com 0, marca-o com 1 */
                    if(G[aux->vizinho].marcado == 0){
                        G[aux->vizinho].marcado = 1;
                        marcouNovo = 1;
                    }
                    aux = aux->prox;
                }

            }
        }

    } while(marcouNovo);

    /* Passo 4: verifica se todos os vertices foram marcados com 1 */
    for(i = 0; i < ordem; i++)
        if(G[i].marcado == 0)
            return 0; /* Grafo nao e conexo */

    return 1; /* Grafo e conexo */
}

/*
 * Programa simples para testar a representacao de grafo e a funcao eConexo
 */
int main(int argc, char *argv[]) {
    int i;
	Vertice *G;
	int ordemG= 10; /* Vertices identificado de 0 ate 9 */
		
	criaGrafo(&G, ordemG);
	acrescentaAresta(G,ordemG,0,1);
	acrescentaAresta(G,ordemG,0,2);
	acrescentaAresta(G,ordemG,0,7);
	acrescentaAresta(G,ordemG,2,4);
	acrescentaAresta(G,ordemG,2,2);
	acrescentaAresta(G,ordemG,2,5);
	acrescentaAresta(G,ordemG,3,5);
	acrescentaAresta(G,ordemG,4,6);
	acrescentaAresta(G,ordemG,3,6);
	acrescentaAresta(G,ordemG,7,7);
	acrescentaAresta(G,ordemG,8,9);

	imprimeGrafo(G, ordemG);

    /* Testa eConexo */
    if(eConexo(G, ordemG))
        printf("O grafo E conexo.\n");
    else
        printf("O grafo NAO e conexo.\n");
	
	destroiGrafo(&G, ordemG);

	printf("\nPressione uma tecla para terminar\n");
    getchar();
	return(0);
}
