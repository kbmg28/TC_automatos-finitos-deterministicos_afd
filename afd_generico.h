#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define TAM 200 //maximo valor lido pelos vetores
#define LAMBDA ' ' //auxiliar para simbolo vazio
#define TRUE 1
#define FALSE 0

char input[TAM]; //vetor
typedef int BOOL; //verificador booleano
char *ESTADOS; //array dinamico
char *ALFABETO; //array dinamico
char ESTADO_INICIAL; // variavel
char *ESTADOS_FINAIS; //array dinamico

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

PtrLista cria_no_lista (TRANSICAO estado);
void insere_lista (PtrLista *lista, PtrLista novo);
void libera_lista (PtrLista *lista);

TRANSICAO* buscaTransacaoNoConjuntoDeEstados (PtrLista estados, TRANSICAO* estado_atual, char simbolo);

FILE* abre_arquivo (const char* nome_arquivo);
void carrega_script ();

void defineConfiguracao(char linha_arq[], char**ptr);
void escolheOpcao(int opcao, char *informacao);
BOOL carrega_estados(char linha_arq[]);
BOOL carrega_alfabeto(char linha_arq[]);
BOOL carrega_transicoes (char linha_arq[]);
BOOL carrega_estados_finais(char linha_arq[]);

BOOL valida_transicao(TRANSICAO* estado_atual, char simbolo);
BOOL validaEstadosAndAlfabeto(char estadoPartida, char estadoChegada, char simbolo);
BOOL faz_transicao(TRANSICAO* estado_atual, char letra_alfabeto);
void setEstadoAtual(TRANSICAO* atual, TRANSICAO transacao);

void verifica_string();
void separaPalavraEtestaStrings(char* entrada);

void afd_via_arquivo();
void afd_via_console();