#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TAM 10
#define LAMBDA ' '
#define TRUE 1
#define FALSE 0

char input[TAM];
typedef int BOOL;
int TAM_ESTADOS, TAM_ALFABETO, TAM_ESTADOS_FINAIS;
char *ESTADOS;
char *ALFABETO;
char ESTADO_INICIAL;
char ESTADOS_FINAIS[5];



typedef struct Transicao{
	char partida; 
	char chegada; 
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
	//puts ("\n\t--Lista--\n");
	while (lista){
		printf ("%c %c %c ;\n", lista->estado.partida, lista->estado.chegada, lista->estado.simbolo_consumido, lista->estado.partida);
		lista = lista->prox;
	}
}

TRANSICAO* buscaTransacaoNoConjuntoDeEstados (PtrLista estados, TRANSICAO* estado_atual, char letra){	
	if (estados == NULL)	return NULL;
	//puts ("\n\t--Lista--\n");
	while (estados){
		//printf ("lido: %c %c %c ;\n", estados->estado.partida, estados->estado.chegada, estados->estado.simbolo_consumido, estados->estado.partida);
		
		if (estados->estado.partida == estado_atual->partida && estados->estado.simbolo_consumido==letra){
			
			//printf ("\nencontrou! EP: %c EC: %c SIMBOLO: %c", estado_atual->partida, estados->estado.chegada, letra);
			return &estados->estado;
		}
		
		estados = estados->prox;
	}
	
	return NULL;
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
		
		fscanf(arq, " %c %c %c\n", &estado.partida, &estado.chegada, &estado.simbolo_consumido);	
		//printf(" %c %c %c\n", estado.partida, estado.chegada, estado.simbolo_consumido);	
		insere_lista(&conjunto_estados, cria_no_lista(estado));
	}
	//puts ("carregou as transacoes");
	fclose (arq);
}

void setEstadoAtual(TRANSICAO* atual, TRANSICAO transacao){
	atual->partida = transacao.partida;
	atual->chegada = transacao.chegada;
	atual->simbolo_consumido = transacao.simbolo_consumido;
}

void printStringComEspaco(const char *str){
	int i;
	
	for (i=0; i< strlen(str); i++)
		printf ("%c ", str[i]);
	puts (";");
}

BOOL faz_transicao(TRANSICAO* estado_atual, char letra_alfabeto){
	TRANSICAO* transicao = buscaTransacaoNoConjuntoDeEstados(conjunto_estados, estado_atual, letra_alfabeto);

	if (transicao != NULL){
		setEstadoAtual(estado_atual, *transicao);
		return TRUE;
	}

	return FALSE;
}

BOOL valida_transicao(TRANSICAO* estado_atual, char simbolo){
	
	estado_atual->partida = estado_atual->chegada;
	
	{
	
	/*
	
	if (simbolo == '0')	
		faz_transicao(estado_atual, 0);
	
	
	else if (simbolo == '1')
		faz_transicao(estado_atual, 1);

	else
		return FALSE;	
	
	return TRUE;
	
	*/
	}	
	
	if( strchr(ALFABETO, simbolo) == NULL){
		printf ("Nao pertence ao alfabeto: \'%c\' em \'%s\'", simbolo, input);
		return FALSE;
	}
	
	if( strchr(ESTADOS, estado_atual->partida) == NULL){
		printf ("Nao pertence ao conjunto de estados: \'%c\'", estado_atual->partida);
		return FALSE;
	}
	
	return TRUE;
}


int verifica_string(){
	int i;
	
	TRANSICAO estado_atual = { 'S', ESTADO_INICIAL, LAMBDA};
	
	printf  ("%10s ", input);
	for (i=0; input[i] != '\0'; i++)	{
		
		if (valida_transicao(&estado_atual, input[i]) == TRUE)
			faz_transicao(&estado_atual, input[i]);
		else{
			puts ("\n\n\t\tErro ");
			return FALSE;
		}
	}
	
	( strchr(ESTADOS_FINAIS, estado_atual.chegada) != NULL) ? puts ("sim") : puts ("nao");
}

int main(){
	FILE* arq = abre_arquivo("palavras.txt");

	carrega_estados();
	printStringComEspaco(ESTADOS);
	
	carrega_alfabeto();
	printStringComEspaco(ALFABETO);
	
	carrega_transicoes();
	print_lista(conjunto_estados);
	
	ESTADO_INICIAL = 'A';
	strcpy(ESTADOS_FINAIS, "BC");
	
	printf ("# ;\n%c ;\n", ESTADO_INICIAL);
	printStringComEspaco(ESTADOS_FINAIS);
	
	while (!feof(arq)){
		
		fscanf(arq, " %s", input);	
		verifica_string();	
	}
	
	
	libera_lista(&conjunto_estados);
	fclose(arq);
	puts ("\n");
	system("pause");
	return (0);
}
