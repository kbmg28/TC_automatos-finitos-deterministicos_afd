#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TAM 100
#define LAMBDA ' '
#define TRUE 1
#define FALSE 0

char input[TAM];
typedef int BOOL;
char *ESTADOS;
char *ALFABETO;
char ESTADO_INICIAL;
char *ESTADOS_FINAIS;

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
	
	if (!*lista)	*lista = novo;
	else{
		atual = *lista;
		
		while (atual->prox)
			atual = atual->prox;
		atual->prox = novo;
	}
}

void print_lista (PtrLista lista){	
	if (lista == NULL)	return;
	while (lista){
		printf ("%c %c %c\n", lista->estado.partida, lista->estado.chegada, lista->estado.simbolo_consumido, lista->estado.partida);
		lista = lista->prox;
	}
}

TRANSICAO* buscaTransacaoNoConjuntoDeEstados (PtrLista estados, TRANSICAO* estado_atual, char simbolo){	
	if (estados == NULL)	return NULL;
	
	while (estados){
		if (estados->estado.partida == estado_atual->partida && estados->estado.simbolo_consumido==simbolo)
			return &estados->estado;
		
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
		puts ("Erro de leitura do arquivo");
		exit (0);
	}
	
	return arq;
}

void defineConfiguracao(char linha_arq[], char**ptr){
	int i, j;
	
	*ptr = (char*) malloc(sizeof( strlen(linha_arq) / 2) + 1);
	
	for (i=0, j=0; i < strlen(linha_arq)-1; i++){
		if (linha_arq[i] != ' '){
			(*ptr)[j] = linha_arq[i];
			j++;
		}
	}

	(*ptr)[j]='\0';
}

BOOL carrega_estados(char linha_arq[]){
	defineConfiguracao(linha_arq, &ESTADOS);
//	printf ("\nESTADOS: %s", ESTADOS);	
}

BOOL carrega_alfabeto(char linha_arq[]){
	defineConfiguracao(linha_arq, &ALFABETO);
//	printf ("\nALFABETO: %s", ALFABETO);
}

BOOL validaEstadosAndAlfabeto(char estadoPartida, char estadoChegada, char simbolo){
	
	if( strchr(ALFABETO, simbolo) == NULL){
		if (simbolo == LAMBDA)	return TRUE;
		//printf ("\t\'%c\' Nao pertence ao alfabeto\n", simbolo);
		puts ("ERRO 01");
		return FALSE;
	}
	
	if( strchr(ESTADOS, estadoPartida) == NULL){
		printf ("\t Estado de partida \'%c\' nao pertence ao conjunto de estados\n", estadoPartida);
		return FALSE;
	}
	
	if( strchr(ESTADOS, estadoChegada) == NULL){
		printf ("\t Estado de chegada \'%c\' nao pertence ao conjunto de estados\n", estadoChegada);
		return FALSE;
	}
	
	return TRUE;
}

BOOL carrega_transicoes (char linha_arq[]){
	TRANSICAO estado;
	
	estado.partida = linha_arq[0];
	estado.chegada = linha_arq[2];
	estado.simbolo_consumido = linha_arq[4];
//	printf ("\t%c %c %c\n", estado.partida, estado.chegada, estado.simbolo_consumido);
	if (validaEstadosAndAlfabeto(estado.partida, estado.chegada, estado.simbolo_consumido) == FALSE){
		puts("\t Erro transicao");
		exit(1);
	}

	insere_lista(&conjunto_estados, cria_no_lista(estado));
}


BOOL carrega_estados_finais(char linha_arq[]){
	defineConfiguracao(linha_arq, &ESTADOS_FINAIS);
//	printf ("\nESTADOS FINAIS: %s", ESTADOS_FINAIS);
}

char* carrega_script (){
	FILE *arq = abre_arquivo("script.txt");
	int i;
	char* palavras_para_verificar;
	char linha_arq[50];
	
	for (i=1; !feof(arq); i++){
		TRANSICAO estado;
		
		fgets(linha_arq, 50, arq);
		printf(linha_arq);

		if (!strcmp(linha_arq, "#\n")) break;
		
		switch(i){
			case 1: carrega_estados(linha_arq); break;
			case 2: carrega_alfabeto(linha_arq); break;
			default: carrega_transicoes(linha_arq); 
		}
	}
	ESTADO_INICIAL = fgetc(arq);
	fgetc(arq);
	printf ("%c\n", ESTADO_INICIAL);

	fgets (linha_arq, 50, arq);
	printf(linha_arq);
	carrega_estados_finais(linha_arq);

	fgets (linha_arq, 50, arq);
	puts(linha_arq);
	palavras_para_verificar = (char*) malloc(sizeof(strlen(linha_arq)));
	strcpy(palavras_para_verificar, linha_arq);

	fclose (arq);
	return palavras_para_verificar;
}

void setEstadoAtual(TRANSICAO* atual, TRANSICAO transacao){
	atual->partida = transacao.partida;
	atual->chegada = transacao.chegada;
	atual->simbolo_consumido = transacao.simbolo_consumido;
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
	
	if (validaEstadosAndAlfabeto(estado_atual->partida, estado_atual->chegada, simbolo) == FALSE){
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
			return FALSE;
		}
	}
	
	( strchr(ESTADOS_FINAIS, estado_atual.chegada) != NULL) ? puts ("sim") : puts ("ERRO 02");
}

int main(){
	char* palavras_para_verificar;
	char*ptr;

	palavras_para_verificar = carrega_script();
	
	ptr = strtok(palavras_para_verificar, " ");
	do{
		if(ptr) {
			strcpy(input, ptr);
			verifica_string();
		}
		ptr = strtok('\0', " ");

	}while(ptr);


	libera_lista(&conjunto_estados);
	free(ALFABETO);
	free(ESTADOS);
	free(ESTADOS_FINAIS);
	return (0);
}