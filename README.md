# TC_automatos-finitos-deterministicos_afd
  Dado uma quíntupla qualquer e um conjunto de palavras, retornar se a cadeia é aceita ou não.
  
# Pré-requisitos
	Linguagem de Programação: C
	Compilador: GCC
        @see: https://terminaldeinformacao.com/2015/10/08/como-instalar-e-configurar-o-gcc-no-windows-mingw/

# Executar via console
	$ gcc afd_generico.c -o main
	$ ./main (linux)
	$ main (windows)

  # Bibliotecas
        Mais informações: https://www.tutorialspoint.com/c_standard_library/stdlib_h.htm
        
        
				#include<stdio.h>   //entrada e saida padrao
            
            Macros
                    FILE: armazena informação de um arquivo
                    stdin: ponteiro FILE que indica a entrada padrão (teclado)
            Funções
                    FILE* fopen(const char* filename, const char* mode): Abre um arquivo com o modo que deve ser usado
                    int fclose(FILE* stream) : fecha o arquivo com segurança
                    int printf(const char* format, ...) : mostrar na tela um conteudo formatado
                    int scanf(const char* format, ...) : lê do teclado alguma informação
                    int fgetc(FILE* stream) : Lê o próximo caractere do fluxo
                    char* fgets(char* str, int n, FILE* stream) : Lê uma linha do fluxo especificado e a armazena na sequência apontada por str
                    int puts (const char* str) : mostra o conteudo da string na tela com uma quebra de linha
                    int feof (FILE* stream) : verifica se o arquivo está no fim
            
            
				#include<stdlib.h>  //biblioteca padrao
        
            Macros
                    NULL: constante de ponteiro nulo
            Funções
                    void* malloc(size_t size) : aloca memória dinamicamente e retorna um ponteiro generico
                    void free(void *ptr) : libera memória alocada
                    void exit(int status) : interrompe a execução do programa
                    
				  
				#include<string.h> // manipulação de strings
            
            Funções
                    char *strchr(const char *str, int c) : busca na string o caractere solicitado
                    int strcmp(const char *str1, const char *str2) : compara a str2 com str1
                    char *strcpy(char *dest, const char *src) : copia a string de src para dest
                    char *strtok(char *str, const char *delim) : string str subdividida em uma série de tokens separados por delim .
                    

				#include <unistd.h> //padrão os SO
      
            Função
                    void sleep(u_size tam) : faz uma pausa no programa em segundos com base no tam passado
	
  
	Sistemas Operacionais Testados: Windows 10 (x64) e Linux 4.19.16-1-MANJARO (x64) 

# Informações #

	* @author Kevin Gomes
	* @contribuitors: Enio Gama
	* @version 1.0
	* @since 2019
  
