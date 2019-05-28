#include "afd_generico.h"

/* Lista onde contem todas as transicoes possiveis do automato */
PtrLista conjunto_estados;

/* Aloca dinamicamente o espaco na memoria baseado na quantidade de 
   de transicoes descrita no arquivo script.txt */
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
/* Libera a memoria alocada para cada no da lista */
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

/* Verifica se existe uma transacao do estado de partida para o simbolo desejado
         Se existir: retorna qual a transacao sera realizada
		 Senao: retorna um ponteiro nulo     */
TRANSICAO* buscaTransacaoNoConjuntoDeEstados (PtrLista estados, TRANSICAO* estado_atual, char simbolo){	
	if (estados == NULL)	return NULL;
	
	while (estados){
		if (estados->estado.partida == estado_atual->partida && estados->estado.simbolo_consumido==simbolo)
			return &estados->estado;
		
		estados = estados->prox;
	}
	
	return NULL;
}
/* Abre um arquivo em formato de leitura/escrita e verifica se foi 
    aberto com sucesso, caso haja algum problema na abertura, para 
	a execucao do programa */
FILE* abre_arquivo (const char* nome_arquivo){
	FILE* arq = fopen(nome_arquivo, "r+");
	
	if (arq == NULL){
		puts ("Erro de leitura do arquivo");
		exit (0);
	}
	
	return arq;
}
/* Insere os valores na variaveis globais ESTADOS, ALFABETO,
    ESTADO_INICIAL,ESTADOS_FINAIS com base nas linhas do arquivo */
void carrega_script (){
	FILE *arq = abre_arquivo("script.txt");
	int i;
	char linha_arq[50];
	
	for (i=1; !feof(arq); i++){
		TRANSICAO estado;
		
		fgets(linha_arq, 50, arq);
		printf(linha_arq);

		if (!strcmp(linha_arq, "#\n")) break;
		
		escolheOpcao(i, linha_arq);
	}
	ESTADO_INICIAL = fgetc(arq);
	fgetc(arq);
	printf ("%c\n", ESTADO_INICIAL);

	fgets (linha_arq, 50, arq);
	puts(linha_arq);
	carrega_estados_finais(linha_arq);

	fclose (arq);
}
/* Salva o conteudo da linha sem espacos para fins de simplicidade
   no instante de validar as informacoes */
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
/* Simplificacao de codigo para evitar reescrita e definicao 
   onde inserir a linha do arquivo nas variaveis globais */
void escolheOpcao(int opcao, char *informacao){
	switch(opcao){
		case 1: carrega_estados(informacao); break;
		case 2: carrega_alfabeto(informacao); break;
		default: carrega_transicoes(informacao); 
	}
}

BOOL carrega_estados(char linha_arq[]){
	defineConfiguracao(linha_arq, &ESTADOS);
//	printf ("\nESTADOS: %s", ESTADOS);	
}

BOOL carrega_alfabeto(char linha_arq[]){
	defineConfiguracao(linha_arq, &ALFABETO);
//	printf ("\nALFABETO: %s", ALFABETO);
}

BOOL carrega_transicoes (char linha_arq[]){
	TRANSICAO estado;
	
	estado.partida = linha_arq[0];
	estado.chegada = linha_arq[2];
	estado.simbolo_consumido = linha_arq[4];

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

/* No momento de um estado para outro, verifica se os valores estao validos */
BOOL valida_transicao(TRANSICAO* estado_atual, char simbolo){
	
	estado_atual->partida = estado_atual->chegada;
	
	if (validaEstadosAndAlfabeto(estado_atual->partida, estado_atual->chegada, simbolo) == FALSE){
		return FALSE;
	}
	return TRUE;
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
/* Se existe uma transicao valida, o estado atual vai para o proximo estado */
BOOL faz_transicao(TRANSICAO* estado_atual, char letra_alfabeto){
	TRANSICAO* transicao = buscaTransacaoNoConjuntoDeEstados(conjunto_estados, estado_atual, letra_alfabeto);

	if (transicao != NULL){
		setEstadoAtual(estado_atual, *transicao);
		return TRUE;
	}

	return FALSE;
}
/* Atualiza os valores do controlador de estado */
void setEstadoAtual(TRANSICAO* atual, TRANSICAO transacao){
	atual->partida = transacao.partida;
	atual->chegada = transacao.chegada;
	atual->simbolo_consumido = transacao.simbolo_consumido;
}
/* Verifica a palavra salva em input (varivel global) e retorna:
        sim: se a cadeia for valida, ou seja, o automato leu e chegou ao estado final
		ERRO 02: se a cadeia nao terminar em um dos ESTADOS FINAIS */
void verifica_string(){
	int i;
	
	TRANSICAO estado_atual = { 'S', ESTADO_INICIAL, LAMBDA};
	
	printf  ("%20s  ", input);
	for (i=0; input[i] != '\0'; i++)	{
		
		if (valida_transicao(&estado_atual, input[i]) == TRUE)
			faz_transicao(&estado_atual, input[i]);
		else
			return;
	}
	( strchr(ESTADOS_FINAIS, estado_atual.chegada) != NULL) ? puts ("sim") : puts ("ERRO 02");
}
/* O parametro entrada eh uma linha de arquivo ou do teclado com todas as palavras que serao 
   verificadas e testa cada uma separadamente */
void separaPalavraEtestaStrings(char* entrada){
	char*ptr;
	ptr = strtok(entrada, " ");
	puts ("\n\nResultado: ");
	do{
		if(ptr) {
			strcpy(input, ptr);
			verifica_string();
		}
		ptr = strtok('\0', " ");

	}while(ptr);
}
/* Testa as cadeias do arquivo para mostrar um exemplo com um segundo de delay*/
void afd_via_arquivo(){
	FILE* arq = abre_arquivo("palavras.txt");
	char linha_arquivo[TAM];
	
	fgets(linha_arquivo, TAM, arq);
	printf ("\t\t%s",linha_arquivo);
	sleep(1);
	separaPalavraEtestaStrings(linha_arquivo);
	fclose(arq);
}
/* Recebe a entrada do usuario para testar se o automato le as palavras passadas */
void afd_via_console(){
	char lido_do_console[TAM];

	puts ("\n Insira as palavras por espaco e enter para iniciar conforme o exemplo abaixo\n");
	afd_via_arquivo();
	printf ("\n\n> ");
	fgets(lido_do_console, TAM, stdin);	
	lido_do_console[strlen(lido_do_console) - 1] = '\0';
	separaPalavraEtestaStrings(lido_do_console);
}

int main(){

	carrega_script();
	afd_via_console();

	libera_lista(&conjunto_estados);
	free(ALFABETO);
	free(ESTADOS);
	free(ESTADOS_FINAIS);
	return (0);
}