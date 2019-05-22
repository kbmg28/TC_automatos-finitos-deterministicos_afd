#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TAM 10
#define LAMBDA ' '
#define TRUE 1
#define FALSE 0

char input[TAM];
typedef int BOOL;


typedef struct Transicao{
	char estado_partida; 
	char estado_chegada; 
	char simbolo_consumido; 
	
} ESTADOS;

FILE* abre_arquivo (){
	FILE* arq = fopen("palavras.txt", "r+");
	
	if (arq == NULL){
		puts ("Erro de leitura");
		exit (0);
	}
	
	return arq;
}

void setEstadoAtual(ESTADOS* atual, ESTADOS transacao){
	atual->estado_partida = transacao.estado_partida;
	atual->estado_chegada = transacao.estado_chegada;
	atual->simbolo_consumido = transacao.simbolo_consumido;
}

BOOL faz_transicao(ESTADOS* estado_atual, int pos){
	ESTADOS transicoes[2][4] = {  
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

BOOL valida_transicao(ESTADOS* estado_atual, char simbolo){
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
	
	ESTADOS estado_atual = { 'S', 'A', LAMBDA};
	
	printf  ("%10s ", input);
	for (i=0; input[i] != '\0'; i++)	{
		
		if (valida_transicao(&estado_atual, input[i]) == TRUE){
			// tratamento se necessario
		}
			
	}
	
	( estado_atual.estado_chegada == 'C' || estado_atual.estado_chegada == 'B') ? puts ("sim") : puts ("nao");
		
}

int main(){
	FILE* arq = abre_arquivo();
	
	while (!feof(arq)){
		
		fscanf(arq, "%s", input);	
		verifica_string();	
	}
	
	fclose(arq);
	puts ("\n");
	system("pause");
}
