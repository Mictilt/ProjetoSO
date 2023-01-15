#include "unix.h"

#define SOCKET_NAME "/tmp/20819202078020"

struct mensagemMonitor{ //struct com todas as strings necessárias para aparecer no ecrã
	char* criouPessoa;
	char* ficouSalaIni;	
	char* desistiuSalaIni;	
	char* entrouSalaIni;	
	char* saiuSalaIni;	
	char* ficouSalaDanca; 
	char* desistiuSalaDanca;
	char* entrouSalaDanca;	
	char* saiuSalaDanca;	
	char* ficouWC;
	char* desistiuWC;			
	char* entrouWC;			
	char* saiuWC;					
	char* ficouBar;		
	char* desistiuBar;		
	char* entrouBar;			
	char* saiuBar;				
	char* ficouBil;				
	char* desistiuBil;				
	char* entrouBil;
	char* saiuBil;
	char* ficouVip;
	char* desistiuVip;
	char* entrouVip;
	char* saiuVip;
	char* expulso;
	char* jogoBil;
	char* fechouDisco;

}*apGlobMsgMonitor;

struct mensagemMonitor* carregaMonitor(){ //esta função lê o ficheiro "monitor.conf" e carrega seu conteúdo na struct "mensagemMonitor". A struct tem vários campos char* que correspondem a diferentes mensagens. 
//O código lê cada linha do arquivo usando a função getline e armazena-o no campo correspondente da struct. Ele também remove o caractere de nova linha no final de cada linha usando uma instrução if.
	size_t len = 0;
	FILE* abreFile = fopen("monitor.conf", "r");

	if(abreFile==NULL)
		printf("Erro ao abrir o ficheiro: monitor");
	else
	{
		struct mensagemMonitor *msgMonitor = malloc(sizeof(struct mensagemMonitor));
		len = getline(&(msgMonitor->criouPessoa), &len, abreFile);
		if((msgMonitor->criouPessoa)[len - 1] == '\n') {
			((msgMonitor->criouPessoa))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->ficouSalaIni), &len, abreFile);
		if((msgMonitor->ficouSalaIni)[len - 1] == '\n') {
			((msgMonitor->ficouSalaIni))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->desistiuSalaIni), &len, abreFile);
		if((msgMonitor->desistiuSalaIni)[len - 1] == '\n') {
			((msgMonitor->desistiuSalaIni))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->entrouSalaIni), &len, abreFile);
		if((msgMonitor->entrouSalaIni)[len - 1] == '\n') {
			((msgMonitor->entrouSalaIni))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->saiuSalaIni), &len, abreFile);
		if((msgMonitor->saiuSalaIni)[len - 1] == '\n') {
			((msgMonitor->saiuSalaIni))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->ficouSalaDanca), &len, abreFile);
		if((msgMonitor->ficouSalaDanca)[len - 1] == '\n') {
			((msgMonitor->ficouSalaDanca))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->desistiuSalaDanca), &len, abreFile);
		if((msgMonitor->desistiuSalaDanca)[len - 1] == '\n') {
			((msgMonitor->desistiuSalaDanca))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->entrouSalaDanca), &len, abreFile);
		if((msgMonitor->entrouSalaDanca)[len - 1] == '\n') {
			((msgMonitor->entrouSalaDanca))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->saiuSalaDanca), &len, abreFile);
		if((msgMonitor->saiuSalaDanca)[len - 1] == '\n') {
			((msgMonitor->saiuSalaDanca))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->ficouWC), &len, abreFile);
		if((msgMonitor->ficouWC)[len - 1] == '\n') {
			((msgMonitor->ficouWC))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->desistiuWC), &len, abreFile);
		if((msgMonitor->desistiuWC)[len - 1] == '\n') {
			((msgMonitor->desistiuWC))[len - 1] = '\0';
		}			
		len = getline(&(msgMonitor->entrouWC), &len, abreFile);
		if((msgMonitor->entrouWC)[len - 1] == '\n') {
			((msgMonitor->entrouWC))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->saiuWC), &len, abreFile);
		if((msgMonitor->saiuWC)[len - 1] == '\n') {
			((msgMonitor->saiuWC))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->ficouBar), &len, abreFile);
		if((msgMonitor->ficouBar)[len - 1] == '\n') {
			((msgMonitor->ficouBar))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->desistiuBar), &len, abreFile);
		if((msgMonitor->desistiuBar)[len - 1] == '\n') {
			((msgMonitor->desistiuBar))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->entrouBar), &len, abreFile);
		if((msgMonitor->entrouBar)[len - 1] == '\n') {
			((msgMonitor->entrouBar))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->saiuBar), &len, abreFile);
		if((msgMonitor->saiuBar)[len - 1] == '\n') {
			((msgMonitor->saiuBar))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->ficouBil), &len, abreFile);
		if((msgMonitor->ficouBil)[len - 1] == '\n') {
			((msgMonitor->ficouBil))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->desistiuBil), &len, abreFile);
		if((msgMonitor->desistiuBil)[len - 1] == '\n') {
			((msgMonitor->desistiuBil))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->entrouBil), &len, abreFile);
		if((msgMonitor->entrouBil)[len - 1] == '\n') {
			((msgMonitor->entrouBil))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->saiuBil), &len, abreFile);
		if((msgMonitor->saiuBil)[len - 1] == '\n') {
			((msgMonitor->saiuBil))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->ficouVip), &len, abreFile);
		if((msgMonitor->ficouVip)[len - 1] == '\n') {
			((msgMonitor->ficouVip))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->desistiuVip), &len, abreFile);
		if((msgMonitor->desistiuVip)[len - 1] == '\n') {
			((msgMonitor->desistiuVip))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->entrouVip), &len, abreFile);
		if((msgMonitor->entrouVip)[len - 1] == '\n') {
			((msgMonitor->entrouVip))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->saiuVip), &len, abreFile);
		if((msgMonitor->saiuVip)[len - 1] == '\n') {
			((msgMonitor->saiuVip))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->expulso), &len, abreFile);
		if((msgMonitor->expulso)[len - 1] == '\n') {
			((msgMonitor->expulso))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->jogoBil), &len, abreFile);
		if((msgMonitor->jogoBil)[len - 1] == '\n') {
			((msgMonitor->jogoBil))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->fechouDisco), &len, abreFile);
		if((msgMonitor->fechouDisco)[len - 1] == '\n') {
			((msgMonitor->fechouDisco))[len - 1] = '\0';
		}
		fclose(abreFile);
		return msgMonitor;

	}
}




int main(int argc, char *argv[])
{	
    apGlobMsgMonitor = carregaMonitor();
    struct sockaddr_un addr;
    int i;
	int num, star;
    i = 1;
    /* Create data socket. */

    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (data_socket == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /*
     * For portability clear the whole structure, since some
     * implementations have additional (nonstandard) fields in
     * the structure.
     * */

    memset(&addr, 0, sizeof(struct sockaddr_un));

    /* Connect socket to socket address */

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    ret = connect(data_socket, (const struct sockaddr *)&addr,
                  sizeof(struct sockaddr_un));

    if (ret == -1)
    {
        fprintf(stderr, "The server is down.\n");
        exit(EXIT_FAILURE);
    }
    /* Receive result. */
    //------------------------------------------------------------------------------------------------------------

    do
    {
        memset(buffer, 0, BUFFER_SIZE);
        ret = read(data_socket, buffer, BUFFER_SIZE);
        if (ret == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
		if (sscanf(buffer, "%d*%n", &num, &star) >= 1) {
//Este código usa o sscanf para analisar um inteiro a partir do buffer e armazena-o na variável "num". Ele também usa uma variável chamada "star" para armazenar o número de caracteres lidos pelo sscanf. Em seguida, ele usa uma declaração de switch para determinar qual mensagem imprimir com base no valor de "num". As mensagens são armazenadas na struct "apGlobMsgMonitor" e são acessadas usando os campos da struct. Cada caso na declaração switch corresponde a uma mensagem diferente, e a mensagem adequada é impressa usando printf.
//Ele está lê o buffer, analisa o primeiro número e de acordo com esse número ele imprime uma string da struct global apGlobMsgMonitor. Esta struct que recebeu a informação com o load da função inicial.
    // buffer starts with a number
			switch(num){
				case 0:
					printf("%s\n", apGlobMsgMonitor->criouPessoa);
					break;
				case 1:
					printf("%s\n", apGlobMsgMonitor->ficouSalaIni);
					break;
				case 2:
					printf("%s\n", apGlobMsgMonitor->desistiuSalaIni);
					break;
				case 3:
					printf("%s\n", apGlobMsgMonitor->entrouSalaIni);
					break;
				case 4:
					printf("%s\n", apGlobMsgMonitor->saiuSalaIni);
					break;
				case 5:
					printf("%s\n", apGlobMsgMonitor->ficouSalaDanca);
					break;
				case 6:
					printf("%s\n", apGlobMsgMonitor->desistiuSalaDanca);
					break;
				case 7:
					printf("%s\n", apGlobMsgMonitor->entrouSalaDanca);
					break;
				case 8:
					printf("%s\n", apGlobMsgMonitor->saiuSalaDanca);
					break;
				case 9:
					printf("%s\n", apGlobMsgMonitor->ficouWC);
					break;
				case 10:
					printf("%s\n", apGlobMsgMonitor->desistiuWC);
					break;
				case 11:
					printf("%s\n", apGlobMsgMonitor->entrouWC);
					break;
				case 12:
					printf("%s\n", apGlobMsgMonitor->saiuWC);
					break;
				case 13:
					printf("%s\n", apGlobMsgMonitor->ficouBar);
					break;
				case 14:
					printf("%s\n", apGlobMsgMonitor->desistiuBar);
					break;
				case 15:
					printf("%s\n", apGlobMsgMonitor->entrouBar);
					break;
				case 16:
					printf("%s\n", apGlobMsgMonitor->saiuBar);
					break;
				case 17:
					printf("%s\n", apGlobMsgMonitor->ficouBil);
					break;
				case 18:
					printf("%s\n", apGlobMsgMonitor->desistiuBil);
					break;
				case 19:
					printf("%s\n", apGlobMsgMonitor->entrouBil);
					break;
				case 20:
					printf("%s\n", apGlobMsgMonitor->saiuBil);
					break;
				case 21:
					printf("%s\n", apGlobMsgMonitor->ficouVip);
					break;
				case 22:
					printf("%s\n", apGlobMsgMonitor->desistiuVip);
					break;
				case 23:
					printf("%s\n", apGlobMsgMonitor->entrouVip);
					break;
				case 24:
					printf("%s\n", apGlobMsgMonitor->saiuVip);
					break;
				case 25:
					printf("%s\n", apGlobMsgMonitor->expulso);
					break;
				case 26:
					printf("%s\n", apGlobMsgMonitor->jogoBil);
					break;
				case 27:
					printf("%s\n", apGlobMsgMonitor->fechouDisco);
					break;
				// Add more cases here
				default:
					// buffer starts with an unexpected number
					printf("%s", buffer);

			}
			if(buffer[star] == '*')
        		printf("Number %d followed by '*'\n", num);
		} else {
			// buffer does not start with a number
			printf("%s", buffer);
		}
    } while (i);

    close(data_socket);

    exit(EXIT_SUCCESS);
}
