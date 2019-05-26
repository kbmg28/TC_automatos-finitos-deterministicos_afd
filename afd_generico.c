#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TAM 10
#define LAMBDA ' '
#define TRUE 1
#define FALSE 0

char input[TAM];
typedef int BOOL;
int TAM_ESTADOS, TAM_ALFABETO;
char *ESTADOS;
char *ALFABETO;


typedef struct Transicao{
	char estado_partida; 
	char estado_chegada; 
	char simbolo_consumido; 
	
} TRANSICAO;

typedef struct ListTransicao{
	TRANSICAO estado;
	struct ListTransicao *prox;
	
} LIST_TRANSICAO;

typedef LIST_TRANSICAO *PtrLista;

PtrLista conjunto_estados;

PtrLista cria_no_lista (TRANSICAO estado){
	PtrLista novo = (PtrLista) malloc(sizeof(LIST_TRANSICAO));
	novo->estado = estado; 
	novo->prox = NULL;
	return novo;
}

void insere_lista (PtrLista *lista, PtrLista novo){
	PtrLista atual;
	
	
	//puts ("\n\t\t--Inserir na lista--\n");
	if (!*lista)	*lista = novo;
	else{
		atual = *lista;
		
		while (atual->prox)
			atual = atual->prox;
		atual->prox = novo;
	}
	//printf ("\n\tElemento %d inserido no fim com sucesso!!\n\n", novo->chave);
}

void print_lista (PtrLista lista){	
	if (lista == NULL)	return;
	puts ("\n\t--Lista--\n");
	while (lista){
		printf ("-> %c ", lista->estado.simbolo_consumido);
		lista = lista->prox;
	}
	printf ("\n\n");
}

void libera_lista (PtrLista *lista){
	PtrLista atual;
	
	while(*lista){
		atual = *lista;
		*lista = (*lista)->prox;
		free (atual);
	}
	free (*lista);
	
	*lista = NULL;
}




FILE* abre_arquivo (const char* nome_arquivo){
	FILE* arq = fopen(nome_arquivo, "r+");
	
	if (arq == NULL){
		puts ("Erro de leitura");
		exit (0);
	}
	
	return arq;
}


BOOL carrega_estados(){
	FILE* arq = abre_arquivo("estados.txt");
	int i;
	
	for (TAM_ESTADOS=0; !feof(arq); TAM_ESTADOS++){
		char estado;
		fscanf(arq, " %c\n", &estado);		
	}
	
	fseek(arq, 0, SEEK_SET);
	ESTADOS = (char*) malloc(sizeof(TAM_ESTADOS+1));
	
	for (i=0; !feof(arq); i++)
		fscanf(arq, " %c\n", &ESTADOS[i]);
	
	ESTADOS[TAM_ESTADOS]='\0';
	fclose(arq);
	
}


BOOL carrega_alfabeto(){
	FILE* arq = abre_arquivo("alfabeto.txt");
	int i;
	
	for (TAM_ALFABETO=0; !feof(arq); TAM_ALFABETO++){
		char letra;
		fscanf(arq, " %c\n", &letra);		
	}
	
	fseek(arq, 0, SEEK_SET);
	ALFABETO = (char*) malloc(sizeof(TAM_ALFABETO+1));
	
	for (i=0; !feof(arq); i++)
		fscanf(arq, " %c\n", &ALFABETO[i]);
	
	ALFABETO[TAM_ALFABETO]='\0';
	fclose(arq);
	
}



BOOL carrega_transicoes (){
	FILE *arq = abre_arquivo("transicoes.txt");
	
	while (!feof(arq)){
		TRANSICAO estado;
		
		fscanf(arq, " %c %c %c\n", &estado.estado_partida, &estado.estado_chegada, &estado.simbolo_consumido);	
		insere_lista(&conjunto_estados, cria_no_lista(estado));
	}
	
	fclose (arq);
}

void setEstadoAtual(TRANSICAO* atual, TRANSICAO transacao){
	atual->estado_partida = transacao.estado_partida;
	atual->estado_chegada = transacao.estado_chegada;
	atual->simbolo_consumido = transacao.simbolo_consumido;
}

BOOL faz_transicao(TRANSICAO* estado_atual, int pos){
	TRANSICAO transicoes[2][4] = {  
		{	{'A', 'B', '0'}, {'B', 'A', '0'}, {'C', 'D', '0'}, {'D', 'C', '0'} }, 
		{	{'A', 'C', '1'}, {'B', 'D', '1'}, {'C', 'A', '1'}, {'D', 'B', '1'} }	};	
	
	
	switch (estado_atual->estado_partida){
		case 'A': setEstadoAtual(estado_atual, transicoes[pos][0]); break;
		case 'B': setEstadoAtual(estado_atual, transicoes[pos][1]); break;
		case 'C': setEstadoAtual(estado_atual, transicoes[pos][2]); break;
		case 'D': setEstadoAtual(estado_atual, transicoes[pos][3]); break;
		default : return FALSE;
	}
	return TRUE;
}

BOOL valida_transicao(TRANSICAO* estado_atual, char simbolo){
	estado_atual->estado_partida = estado_atual->estado_chegada;
	
	if (simbolo == '0')	
		faz_transicao(estado_atual, 0);
	
	
	else if (simbolo == '1')
		faz_transicao(estado_atual, 1);

	else
		return FALSE;	
	
	return TRUE;
}


int verifica_string(){
	int i;
	
	TRANSICAO estado_atual = { 'S', 'A', LAMBDA};
	
	printf  ("%10s ", input);
	for (i=0; input[i] != '\0'; i++)	{
		
		if (valida_transicao(&estado_atual, input[i]) == TRUE){
			// tratamento se necessario
		}
			
	}
	
	( estado_atual.estado_chegada == 'C' || estado_atual.estado_chegada == 'B') ? puts ("sim") : puts ("nao");
		
}

int main(){
/*	FILE* arq = abre_arquivo("palavras.txt");
	
	while (!feof(arq)){
		
		fscanf(arq, "%s", input);	
		verifica_string();	
	}
	
	fclose(arq);

	carrega_transicoes();
	print_lista(conjunto_estados);
	libera_lista(&conjunto_estados);
*/
	carrega_estados();
	carrega_alfabeto();
	
	puts (ALFABETO);
	puts(ESTADOS);
	
	puts ("\n");
	system("pause");
}
