#include "unix.h"
#define MAXLINE 1024
int data_socket;
int ret;
char buffer[BUFFER_SIZE];
void escreve_aux(FILE *faux, char x[100], int dados)//função para escrever num ficheiro uma string e um inteiro
{
    fprintf(faux, "%s", x);
    fprintf(faux, "%d\n", dados);
}

str_cli(int sockfd,int id, bool tipoPessoa, int prioridade, int probDesistir){ //editar dados que recebe e concatena
	int n;
    int buffer_len;
	//Composicao da stream de envio para socket
    buffer_len = snprintf(buffer, MAXLINE, "ID: %i, VIP: %s, Prioridade: %i, ProbDesistir: %i \n", id, (tipoPessoa) ? "Verdade": "Falso", prioridade, probDesistir);
	n = write(sockfd,buffer,buffer_len);
	if (n < 0) error("ERROR writing to socket");
}
str_cli2(int sockfd,int id){ //editar dados que recebe e concatena
	int n;
    int buffer_len;
	//Composicao da stream de envio para socket
    buffer_len = snprintf(buffer, MAXLINE, "Pessoa %i ", id);
	n = write(sockfd,buffer,buffer_len);
	if (n < 0) error("ERROR writing to socket");
}
