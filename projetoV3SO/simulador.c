#include "unix.h"
#define SOCKET_NAME "/tmp/20819202078020"

#define MAX_THREADS 100

int valorSemSalaIni;
int valorSemSalaDanca;
int acabou;
int priority_q[20];
int nf = 0;
int createdThreads;
int pessoasParaJogo;
int acabaJogo;
int ganhouBil;
pthread_t thread;
//semáforos
sem_t sem_sala_ini;
sem_t sem_fila_sala_ini;
sem_t sem_sala_danca;
sem_t sem_fila_sala_danca;
sem_t sem_sala_bar;
sem_t sem_fila_sala_bar;
sem_t sem_sala_wc;
sem_t sem_sala_bil;
sem_t sem_sala_VIP;
//trincos
pthread_mutex_t mensagensMonitor;
pthread_mutex_t trincoVarGlobais;
pthread_mutex_t FIFO_salaInicial;
pthread_mutex_t trincoVeriJogo;

struct configs dados;

void init()//inicialização de tudo o que necessitamos para a simulação
{
    sem_init(&sem_sala_ini, 0, dados.num_sala_ini);
    sem_init(&sem_sala_danca, 0, dados.num_sala_danca);
    sem_init(&sem_sala_bar, 0, dados.num_sala_bar);
    sem_init(&sem_sala_wc, 0, dados.num_sala_wc);
    sem_init(&sem_sala_bil, 0, dados.num_sala_bil);
    sem_init(&sem_sala_VIP, 0, dados.num_sala_vip);
    pthread_mutex_init(&mensagensMonitor,NULL);
    pthread_mutex_init(&trincoVarGlobais,NULL);
    pthread_mutex_init(&FIFO_salaInicial, NULL);
    pthread_mutex_init(&trincoVeriJogo, NULL);
    dados.num_entra_sala_ini = 0;
    dados.num_desist_sala_ini = 0;
    dados.num_fila_sala_ini = 0;
    dados.num_entra_sala_danca = 0;
    dados.num_desist_sala_danca = 0;
    dados.num_fila_sala_danca = 0;
    dados.num_entra_bar = 0;
    dados.num_desist_bar = 0;
    dados.num_fila_bar = 0;
    dados.num_entra_wc = 0;
    dados.num_desist_wc = 0;
    dados.num_fila_wc = 0;
    dados.num_entra_bil = 0;
    dados.num_desist_bil = 0;
    dados.num_fila_bil = 0;
    dados.num_entra_vip = 0;
    dados.num_desist_vip = 0;
    dados.num_fila_vip = 0;
    dados.num_jogos_bil = 0;
    dados.num_bebados = 0;
    pessoasParaJogo = 0;
    ganhouBil = 0;
    acabaJogo = 0;
}
//push para a fila de prioridades feita com bubble sort
void push(int value, int priority) {//A função recebe o "id" e "prioridade" da thread e armazena-os na fila de prioridade. 
//Em seguida, ele classifica a fila de prioridade usando o algoritmo de ordenação bubble sort, de forma que os elementos com prioridade mais alta estejam na frente da fila.
    priority_q[nf] = value;
    priority_q[nf + 1] = priority;
    nf += 2;
    for (int i = 0; i < nf - 2; i += 2) {
        for (int j = 0; j < nf - 2 - i; j += 2) {
            if (priority_q[j + 1] > priority_q[j + 3]) {
                int temp = priority_q[j];
                priority_q[j] = priority_q[j + 2];
                priority_q[j + 2] = temp;
                temp = priority_q[j + 1];
                priority_q[j + 1] = priority_q[j + 3];
                priority_q[j + 3] = temp;
            }
        }
    }
}
//pop para a fila de prioridades feita com bubble sort
int pop() {// A função remove o elemento da frente da fila de prioridade e retorna seu valor. 
//A função desloca todos os elementos para a frente e diminui o número total de elementos na fila.
    int value = priority_q[0];
    for (int i = 0; i < nf - 2; i += 2) {
        priority_q[i] = priority_q[i + 2];
        priority_q[i + 1] = priority_q[i + 3];
    }
    nf -= 2;
    return value;
}

void lerConfig()// função adiciona à estrutura dos dados toda a informação presente no simulador.conf. 
//Ele tenta encontrar strings com o mesmo nome usando o strcmp() e depois dá o número à frente o valor ao dado da estrutura correspondente
{
    FILE *FileIn;
    char vect[100];
    char n[5];
    FileIn = fopen("simulador.conf", "r");

    if (NULL == FileIn)
    {
        printf("o ficheiro nao abre \n");
        exit(1);
    }

    while (fscanf(FileIn, "%s = %s", vect, n) != EOF)
    {
        if (strcmp(vect, "tempo_disco") == 0)
            dados.tempo_disco = atoi(n);
        if (strcmp(vect, "tempo_med_sala_ini") == 0)
            dados.tempo_med_sala_ini = atoi(n);
        if (strcmp(vect, "num_sala_ini") == 0)
            dados.num_sala_ini = atoi(n);
        if (strcmp(vect, "tempo_disco") == 0)
            dados.tempo_disco = atoi(n);
        if (strcmp(vect, "num_salas") == 0)
            dados.num_salas = atoi(n);
        if (strcmp(vect, "num_sala_danca") == 0)
            dados.num_sala_danca = atoi(n);
        if (strcmp(vect, "tempo_med_sala_danca") == 0)
            dados.tempo_med_sala_danca = atoi(n);
        if (strcmp(vect, "num_sala_wc") == 0)
            dados.num_sala_wc = atoi(n);
        if (strcmp(vect, "tempo_med_wc") == 0)
            dados.tempo_med_wc = atoi(n);
        if (strcmp(vect, "fila_wc") == 0)
            dados.fila_wc = atoi(n);
        if (strcmp(vect, "num_sala_bar") == 0)
            dados.num_sala_bar = atoi(n);
        if (strcmp(vect, "tempo_med_bar") == 0)
            dados.tempo_med_bar = atoi(n);
        if (strcmp(vect, "prob_beb") == 0)
            dados.prob_beb = atoi(n);
        if (strcmp(vect, "num_sala_bil") == 0)
            dados.num_sala_bil = atoi(n);
        if (strcmp(vect, "tempo_pausa_bil") == 0)
            dados.tempo_pausa_bil = atoi(n);
        if (strcmp(vect, "num_sala_vip") == 0)
            dados.num_sala_vip = atoi(n);
        if (strcmp(vect, "tempo_med_vip") == 0)
            dados.tempo_med_vip = atoi(n);
        if (strcmp(vect, "fila_sala_ini") == 0)
            dados.fila_sala_ini = atoi(n);
        if (strcmp(vect, "fila_sala_danca") == 0)
            dados.fila_sala_danca = atoi(n);
        if (strcmp(vect, "fila_bar") == 0)
            dados.fila_bar = atoi(n);
        if (strcmp(vect, "fila_sala_bil") == 0)
            dados.fila_sala_bil = atoi(n);
        if (strcmp(vect, "fila_sala_VIP") == 0)
            dados.fila_sala_VIP = atoi(n);
    }
    fclose(FileIn);
}

void escreve_log(){// função adiciona ao ficheiro resultado.log os dados que forma recebidos ao longo da simulação
	FILE *f = fopen("resultado.log","a");
	if (!f){
        printf("Erro a abrir o ficheiro!\n");
		return;
	}
	escreve_aux(f,"Tempo de duração da discoteca(segundos): ",dados.tempo_disco);

	fprintf(f, "\n--Discoteca Informação Geral--\n");
    printf("\n--Discoteca Informação Geral--\n");
	escreve_aux(f,"Número de Salas na Discoteca: ", dados.num_salas);
	escreve_aux(f,"Capacidade máxima sala inicial: ",dados.num_sala_ini);
	escreve_aux(f,"Capacidade máxima da pista de dança : ",dados.num_sala_danca);
	escreve_aux(f,"Capacidade máxima do bar: ",dados.num_sala_bar);
	escreve_aux(f,"Capacidade máxima da casa de banho: ",dados.num_sala_wc);
	escreve_aux(f,"Capacidade máxima da sala de bilhar: ",dados.num_sala_bil);
	escreve_aux(f,"Capacidade máxima da sala VIP: ",dados.num_sala_vip);
    escreve_aux(f,"Tempo na sala inicial: ",dados.tempo_med_sala_ini);
    escreve_aux(f,"Tempo na pista de dança: ",dados.tempo_med_sala_danca);
    escreve_aux(f,"Tempo na casa de banho: ",dados.tempo_med_wc);
    escreve_aux(f,"Tempo no bar: ",dados.tempo_med_bar);
    escreve_aux(f,"Tempo de pausa entre jogos de bilhar: ",dados.tempo_pausa_bil);
    escreve_aux(f,"Tempo na sala VIP: ",dados.tempo_med_vip);
    escreve_aux(f,"Probabilidade de alguém ficar bêbado (percentagem): ",dados.prob_beb);

	fprintf(f, "\n--Discoteca Informação das salas na simulação--\n");
    printf("\n--Discoteca Informação das salas na simulação--\n");
    escreve_aux(f,"Número total de pessoas que estiveram na noite: ",createdThreads);
	escreve_aux(f,"Pessoas que Entraram na discoteca: ",dados.num_sala_ini);
    escreve_aux(f,"Pessoas que ficaram na fila para entrar na discoteca: ",dados.num_fila_sala_ini);
	escreve_aux(f,"Pessoas que Desistiram de entrar na discoteca: ",dados.num_desist_sala_ini);
	escreve_aux(f,"Pessoas que Entraram na pista de dança: ",dados.num_entra_sala_danca);
	escreve_aux(f,"Pessoas que ficaram na fila da pista de dança: ",dados.num_fila_sala_danca);
	escreve_aux(f,"Pessoas que Desistiram de entrar na pista de dança: ",dados.num_desist_sala_danca);
	escreve_aux(f,"Pessoas que Entraram no bar: ",dados.num_entra_bar);
	escreve_aux(f,"Pessoas que ficaram na fila do bar: ",dados.num_fila_bar);
	escreve_aux(f,"Pessoas que Desistiram de entrar no bar: ",dados.num_desist_bar);
    escreve_aux(f,"Pessoas que Entraram na casa de banho: ",dados.num_entra_wc);
	escreve_aux(f,"Pessoas que ficaram na fila da casa de banho: ",dados.num_fila_wc);
	escreve_aux(f,"Pessoas que Desistiram de entrar na casa de banho: ",dados.num_desist_wc);
    escreve_aux(f,"Pessoas que Entraram na sala de bilhar: ",dados.num_entra_bil);
	escreve_aux(f,"Pessoas que ficaram na fila da sala de bilhar: ",dados.num_fila_bil);
	escreve_aux(f,"Pessoas que Desistiram de entrar na sala de bilhar: ",dados.num_desist_bil);
    escreve_aux(f,"Pessoas que Entraram na sala VIP: ",dados.num_entra_vip);
	escreve_aux(f,"Pessoas que ficaram na fila da sala VIP: ",dados.num_fila_vip);
	escreve_aux(f,"Pessoas que Desistiram de entrar na sala VIP: ",dados.num_desist_vip);
	
	fprintf(f, "\n--Outros Dados--\n");
    printf("\n--Outros Dados--\n");
    int numbers[] = {dados.num_entra_bar, dados.num_entra_sala_danca, dados.num_entra_bil, dados.num_entra_wc, dados.num_entra_vip};
    int largest = numbers[0];
    char salaMaisVisitada[100];

    for (int i = 1; i < 5; i++) {
        if (numbers[i] > largest) {
            largest = numbers[i];
        }
    }
    if(largest == numbers[0]){
        strcpy(salaMaisVisitada,"Bar");
    }
    else if(largest == numbers[1]){
        strcpy(salaMaisVisitada,"Pista de Dança");
    }
    else if(largest == numbers[2]){
        strcpy(salaMaisVisitada,"Sala de Bilhar");
    }
    else if(largest == numbers[3]){
        strcpy(salaMaisVisitada,"Casa de Banho");
    }
    else if(largest == numbers[4]){
        strcpy(salaMaisVisitada,"Sala VIP");
    }
    fprintf(f,"Sala mais visitada (sem contar com a sala inicial): ");
    fprintf(f,"%s\n", salaMaisVisitada);
    escreve_aux(f,"Número de vezes que essa sala foi visitada: ", largest);
	escreve_aux(f,"Número de Jogos Ocorridos na sala de bilhar: ",dados.num_jogos_bil);
    escreve_aux(f,"Número de Pessoas expulsas por estarem bêbadas: ",dados.num_bebados);
    fprintf(f, "\n-----------------------FIM DA INFORMAÇÃO DA SIMULAÇÃO-----------------------\n");

	fclose(f);
}

void *criarClientes(void *in, pthread_t* appthread)
{
    int index = *(int *)in;
    srand(time(NULL));
    int id = index;
    bool VIP;
    int random_number = rand() % 100 + 1;
    
    if (random_number < 16)
    {
        VIP = true;
    }
    else
    {
        VIP = false;
    }
    //tratarThreads();
    int prioridade = rand() % 100 + 1;
    int momento_sala = 0;
    int probalidadeDesistir = rand() % 6 + 5;
    pthread_mutex_lock(&mensagensMonitor);
    str_cli(data_socket,id,VIP,prioridade,probalidadeDesistir);
    pthread_mutex_unlock(&mensagensMonitor);
   /* printf("Sala que se encontra é %s", (momento_sala ? "inicial" : "outra"));*/
    printf("Prioridade: %d\n", prioridade);
    printf("A pessoa com o id %i entrou e %s \n", id, (VIP ? "é VIP" : "não é VIP"));
    //var locais
    int counterSalaIni = 0;
    int counterSalaDanca = 0;
    int flagMessageSalaIni = 0;
    int flagMessageSalaIni2 = 0;
    int flagDesitiuSalaInicial = 0;
    int flagMessageSalaDanca = 0;
    int flagDesitiuSalaDanca = 0;
    int flagFilaPrio = 0;
    int numSalas = 0;
    int valorSemWC;
    int flagWC = 0;
    int flag2WC = 0;
    int ganhar = 0;
    
    sem_getvalue(&sem_sala_ini, &valorSemSalaIni);
    sem_getvalue(&sem_sala_danca, &valorSemSalaDanca);
    while (1) {
        pthread_mutex_lock(&trincoVarGlobais);
        sem_getvalue(&sem_sala_ini, &valorSemSalaIni);
        pthread_mutex_unlock(&trincoVarGlobais);
        if(flagMessageSalaIni == 0){ // a mensagem só aparece uma vez
            printf("Semaforo Sala Ini: %i", valorSemSalaIni);
            flagMessageSalaIni = 1;
        }
        if(valorSemSalaIni>0 && flagMessageSalaIni2 == 0) {
            pthread_mutex_lock(&FIFO_salaInicial);//trinco para o FIFO
            sem_wait(&sem_sala_ini);
            pthread_mutex_unlock(&FIFO_salaInicial);//trinco para o FIFO
            pthread_mutex_lock(&trincoVarGlobais);
            dados.num_entra_sala_ini++;
            sem_getvalue(&sem_sala_ini, &valorSemSalaIni);
            pthread_mutex_unlock(&trincoVarGlobais);
            
            break;
        }
        if(valorSemSalaIni>0 && flagMessageSalaIni2 == 1) {
            sem_wait(&sem_sala_ini);
            pthread_mutex_lock(&trincoVarGlobais);
            dados.num_entra_sala_ini++;
            sem_getvalue(&sem_sala_ini, &valorSemSalaIni);
            pthread_mutex_unlock(&trincoVarGlobais);
            break;
        }
        
        else{
            if(flagMessageSalaIni2 == 0){ // a mensagem só aparece uma vez
                pthread_mutex_lock(&mensagensMonitor);
                str_cli2(data_socket,id);
                sleep(1);
                int buffer_len = snprintf(buffer,BUFFER_SIZE, "1*");//meter em códigos para o monitor no conf. Neste exemplo é ficou na fila da sala inicial
                int n = write(data_socket, buffer, buffer_len);
                pthread_mutex_unlock(&mensagensMonitor);
                pthread_mutex_lock(&trincoVarGlobais);
                dados.num_fila_sala_ini++;
                pthread_mutex_unlock(&trincoVarGlobais);
                flagMessageSalaIni2 = 1;
                pthread_mutex_lock(&FIFO_salaInicial);//trinco para o FIFO
            }     
        }
        if(counterSalaIni == probalidadeDesistir) {
            pthread_mutex_lock(&mensagensMonitor);
            str_cli2(data_socket,id);
            sleep(1);
            int buffer_len = snprintf(buffer,BUFFER_SIZE, "2*");//desistiu de entrar na sala inicial
            int n = write(data_socket, buffer, buffer_len);
            pthread_mutex_unlock(&mensagensMonitor);
            pthread_mutex_lock(&trincoVarGlobais);
            dados.num_desist_sala_ini++;
            pthread_mutex_unlock(&trincoVarGlobais);
            flagMessageSalaIni2 = 0;
            flagDesitiuSalaInicial = 1;//mete flag a 1 para dizer que saíu da fila
            pthread_mutex_unlock(&FIFO_salaInicial);//trinco para deixar a próxima thread
            return;
        }
        counterSalaIni++;
        sleep(1);
    }
    if (flagDesitiuSalaInicial == 0){// se não desistiu da fila entra na sala inicial
        flagMessageSalaIni = 0;
        flagMessageSalaIni2 = 0;
        pthread_mutex_lock(&mensagensMonitor);
        str_cli2(data_socket,id);
        sleep(1);
        int buffer_len = snprintf(buffer,BUFFER_SIZE, "3*");//entrou na sala inicial
        int n = write(data_socket, buffer, buffer_len);
        pthread_mutex_unlock(&mensagensMonitor);
        sleep(dados.tempo_med_sala_ini); // tempo à espera
        sem_post(&sem_sala_ini);
        pthread_mutex_unlock(&FIFO_salaInicial);
        pthread_mutex_lock(&mensagensMonitor);
        str_cli2(data_socket,id);
        sleep(1);
        buffer_len = snprintf(buffer,BUFFER_SIZE, "4*");//saiu da sala inicial
        n = write(data_socket, buffer, buffer_len);
        pthread_mutex_unlock(&mensagensMonitor);
    }
    
 while (!acabou)
    {

        int prox_sala = rand() % 5;
        if(prox_sala == 0){//vai para a pista de dança
            while (1) {
                pthread_mutex_lock(&trincoVarGlobais);
                sem_getvalue(&sem_sala_danca, &valorSemSalaDanca);
                pthread_mutex_unlock(&trincoVarGlobais);
                if (flagFilaPrio == 0){
                    pthread_mutex_lock(&trincoVarGlobais);
                    push(id, prioridade);//mete a thread na fila da prioridade e organiza consoante a prioridade
                    pthread_mutex_unlock(&trincoVarGlobais);
                    flagFilaPrio = 1;//mete a flag a 1 para não voltar a entrar no if e não voltar a entrar na fila
                }
                if(valorSemSalaDanca>0) {        
                    if(priority_q[1] == prioridade){//verifica se tem a maior prioridade
                        sem_wait(&sem_sala_danca);
                        pthread_mutex_lock(&trincoVarGlobais);
                        pop();//tira a thread de maior prioridade da fila
                        sem_getvalue(&sem_sala_danca, &valorSemSalaDanca);
                        dados.num_entra_sala_danca++;
                        pthread_mutex_unlock(&trincoVarGlobais);
                        break;
                    }
                }
                else{
                    if(flagMessageSalaDanca == 0){ // a mensagem só aparece uma vez
                        pthread_mutex_lock(&mensagensMonitor);
                        str_cli2(data_socket,id);
                        sleep(1);
                        int buffer_len = snprintf(buffer,BUFFER_SIZE, "5*");//ficou na fila da pista de dança
                        int n = write(data_socket, buffer, buffer_len);
                        pthread_mutex_unlock(&mensagensMonitor);
                        pthread_mutex_lock(&trincoVarGlobais);
                        dados.num_fila_sala_danca++;
                        pthread_mutex_unlock(&trincoVarGlobais);
                        flagMessageSalaDanca = 1;
                    }     
                }
                if(counterSalaDanca == probalidadeDesistir) {
                    pthread_mutex_lock(&mensagensMonitor);
                    str_cli2(data_socket,id);
                    sleep(1);
                    int buffer_len = snprintf(buffer,BUFFER_SIZE, "6*");//desitiu da fila da pista de dança
                    int n = write(data_socket, buffer, buffer_len);
                    pthread_mutex_unlock(&mensagensMonitor);
                    pthread_mutex_lock(&trincoVarGlobais);
                    dados.num_desist_sala_danca++;
                    pthread_mutex_unlock(&trincoVarGlobais);
                    flagMessageSalaDanca = 0;
                    flagDesitiuSalaDanca = 1;
                    break;
                }
                counterSalaDanca++;
                sleep(1);
            }
            if (flagDesitiuSalaDanca == 0){// se não desistiu da fila entra na sala inicial
                flagMessageSalaDanca = 0;
                pthread_mutex_lock(&mensagensMonitor);
                str_cli2(data_socket,id);
                sleep(1);
                int buffer_len = snprintf(buffer,BUFFER_SIZE, "7*");//entrou na pista de dança
                int n = write(data_socket, buffer, buffer_len);
                pthread_mutex_unlock(&mensagensMonitor);
                sleep(dados.tempo_med_sala_danca); // tempo a dançar
                sem_post(&sem_sala_ini);
                pthread_mutex_lock(&mensagensMonitor);
                str_cli2(data_socket,id);
                sleep(1);
                buffer_len = snprintf(buffer,BUFFER_SIZE, "8*");//saiu da pista de dança
                n = write(data_socket, buffer, buffer_len);
                pthread_mutex_unlock(&mensagensMonitor);
            }
            else flagDesitiuSalaDanca = 0;
        }
        flagFilaPrio = 0;
        if (prox_sala == 1) // Ira para o bar
        {

            sem_wait(&sem_sala_bar);
            pthread_mutex_lock(&mensagensMonitor);//trinco para enviar mensagem que entrou no bar 
            str_cli2(data_socket, id);
            sleep(1);
            int buffer_len = snprintf(buffer, BUFFER_SIZE, "15*");//entrou no bar
            int n = write(data_socket, buffer, buffer_len);
            pthread_mutex_unlock(&mensagensMonitor);
            pthread_mutex_lock(&trincoVarGlobais);
            dados.num_entra_bar++;
            pthread_mutex_unlock(&trincoVarGlobais);
            sleep(dados.tempo_med_bar); // tempo no bar
            int beb = rand() % dados.prob_beb + 1;
            if(beb < 26){//vai ser expulso por estar a fazer confusão e estar bêbado
                pthread_mutex_lock(&mensagensMonitor); //trinco para enviar mensagem que saiu do bar
                str_cli2(data_socket, id);
                sleep(1);
                buffer_len = snprintf(buffer, BUFFER_SIZE, "25*");//foi expulso
                n = write(data_socket, buffer, buffer_len);
                pthread_mutex_unlock(&mensagensMonitor);
                pthread_mutex_lock(&trincoVarGlobais);
                dados.num_bebados++; //adiciona à contagem o a pessoa bêbada
                pthread_mutex_unlock(&trincoVarGlobais);
                return;//sai da discoteca
            }
            pthread_mutex_lock(&mensagensMonitor); //trinco para enviar mensagem que saiu do bar
            str_cli2(data_socket, id);
            sleep(1);
            buffer_len = snprintf(buffer, BUFFER_SIZE, "16*");//saiu do bar
            n = write(data_socket, buffer, buffer_len);
            pthread_mutex_unlock(&mensagensMonitor);

            sem_post(&sem_sala_bar);
        }

        if (prox_sala == 2)
        { // IR A WC
            while (1)
            {
                pthread_mutex_lock(&trincoVarGlobais);
                sem_getvalue(&sem_sala_wc, &valorSemWC);
                pthread_mutex_unlock(&trincoVarGlobais);
                if (valorSemWC > 0)
                {
                    sem_wait(&sem_sala_wc);
                    pthread_mutex_lock(&trincoVarGlobais);
                    sem_getvalue(&sem_sala_wc, &valorSemWC);
                    pthread_mutex_unlock(&trincoVarGlobais);
                    break;
                }
                else
                {
                    if (!flag2WC)
                    {
                        pthread_mutex_lock(&mensagensMonitor);
                        str_cli2(data_socket, id);
                        sleep(1);
                        int buffer_len = snprintf(buffer, BUFFER_SIZE, "9*");//ficou na fila da casa de banho
                        int n = write(data_socket, buffer, buffer_len);
                        pthread_mutex_unlock(&mensagensMonitor);
                        pthread_mutex_lock(&trincoVarGlobais);
                        dados.num_fila_wc++;
                        pthread_mutex_unlock(&trincoVarGlobais);
                        flagWC = 1;
                        flag2WC = 1;
                        break;
                    }
                }

                sleep(1);
            }

            if (flagWC == 0)
            {
                pthread_mutex_lock(&mensagensMonitor);
                str_cli2(data_socket, id);
                sleep(1);
                int buffer_len = snprintf(buffer, BUFFER_SIZE, "11*");//entrou na casa de banho
                int n = write(data_socket, buffer, buffer_len);
                pthread_mutex_unlock(&mensagensMonitor);
                pthread_mutex_lock(&trincoVarGlobais);
                dados.num_entra_wc++;
                pthread_mutex_unlock(&trincoVarGlobais);
                sleep(dados.tempo_med_wc); // tempo na casa de banho
                pthread_mutex_lock(&mensagensMonitor);
                str_cli2(data_socket, id);
                sleep(1);
                buffer_len = snprintf(buffer, BUFFER_SIZE, "12*");//saiu da casa de baho
                n = write(data_socket, buffer, buffer_len);
                pthread_mutex_unlock(&mensagensMonitor);
                sem_post(&sem_sala_wc);
            }
        }
        if (prox_sala == 3){
            if (VIP) // IRA SALA VIP
                {
                    sem_wait(&sem_sala_VIP);
                    pthread_mutex_lock(&mensagensMonitor);
                    str_cli2(data_socket, id);
                    sleep(1);
                    int buffer_len = snprintf(buffer, BUFFER_SIZE, "23*");//entrou na sala VIP
                    int n = write(data_socket, buffer, buffer_len);
                    pthread_mutex_unlock(&mensagensMonitor);
                    pthread_mutex_lock(&trincoVarGlobais);
                    dados.num_entra_vip++;
                    pthread_mutex_unlock(&trincoVarGlobais);
                    sleep(dados.tempo_med_vip);
                    pthread_mutex_lock(&mensagensMonitor);
                    str_cli2(data_socket, id);
                    sleep(1);
                    buffer_len = snprintf(buffer, BUFFER_SIZE, "24*");//saiu da sala VIP
                    n = write(data_socket, buffer, buffer_len);
                    pthread_mutex_unlock(&mensagensMonitor);

                    sem_post(&sem_sala_VIP);
                }
            }
            if (prox_sala == 4)//vai para a sala de bilhar
        {
            sem_wait(&sem_sala_bil);
            pthread_mutex_lock(&trincoVarGlobais);
            pessoasParaJogo++;
            pthread_mutex_unlock(&trincoVarGlobais);
            pthread_mutex_lock(&mensagensMonitor);//trinco para enviar mensagem que entrou no bar 
            str_cli2(data_socket, id);
            sleep(1);
            int buffer_len = snprintf(buffer, BUFFER_SIZE, "19*");//entra na sala de bilhar
            int n = write(data_socket, buffer, buffer_len);
            pthread_mutex_unlock(&mensagensMonitor);
            pthread_mutex_lock(&trincoVarGlobais);
            dados.num_entra_bil++;
            pthread_mutex_unlock(&trincoVarGlobais);
             while (!acabou){
                    while(1){
                    if(pessoasParaJogo < 2){
                        sleep(dados.tempo_pausa_bil); // tempo à espera de alguém para jogar 
                    }
                    pthread_mutex_lock(&trincoVeriJogo); //trinco para haver verificação uma thread de cada vez
                    if (ganhouBil == 0 && pessoasParaJogo >= 2)// verifica se tem pessoas para um jogo e se o jogo acabou
                    {   
                        pthread_mutex_lock(&trincoVarGlobais); 
                        ganhouBil = 1;//altera variável global para dizer que o jogo acabou
                        pthread_mutex_unlock(&trincoVarGlobais);
                        ganhar = 1; //altera variável local para dizer que a thread ganhou
                        pthread_mutex_unlock(&trincoVeriJogo);
                        break;
                    }
                    pthread_mutex_unlock(&trincoVeriJogo);
                    if(ganhouBil == 1) {;break;}//se já foi anunciado que o jogo acabou a thread sai
                }
                    if(ganhar == 1){//se ganhou ganha 1 de prioridade
                        prioridade++;
                        pthread_mutex_lock(&trincoVarGlobais);
                        acabaJogo++;//diz para acabar o jogo
                        pthread_mutex_unlock(&trincoVarGlobais);
                        while(1){
                         
                            if(acabaJogo >=2){//se as 2 threads já disseram para acabar o jogo é adicionado para as estatísticas esse jogo e sai do while
                                pthread_mutex_lock(&trincoVarGlobais);
                                dados.num_jogos_bil++;
                                pthread_mutex_unlock(&trincoVarGlobais);
                                break;
                            }
                        }
                    }
                    if(ganhar == 0){//se perdeu decrementa um valor na prioridade
                        prioridade--;
                        pthread_mutex_lock(&trincoVarGlobais);
                        acabaJogo++;//diz para acabar o jogo
                        pthread_mutex_unlock(&trincoVarGlobais);
                        while(1){
                            if(acabaJogo >=2){//se as 2 threads já disseram para acabar o jogo sai do while
                                break;
                            }
                        }
                }
            pthread_mutex_lock(&mensagensMonitor); //trinco para enviar mensagem que ocorreu um jogo
            str_cli2(data_socket, id);
            sleep(1);
            buffer_len = snprintf(buffer, BUFFER_SIZE, "26*");//ocorreu um jogo
            n = write(data_socket, buffer, buffer_len);
            pthread_mutex_unlock(&mensagensMonitor);
            pthread_mutex_lock(&trincoVarGlobais);
            ganhouBil = 0;
            acabaJogo = 0;
            pessoasParaJogo = 0;
            pthread_mutex_unlock(&trincoVarGlobais);
            ganhar = 0;
            }
            pthread_mutex_lock(&mensagensMonitor); //trinco para enviar mensagem que saiu da sala de bilhar
            str_cli2(data_socket, id);
            sleep(1);
            buffer_len = snprintf(buffer, BUFFER_SIZE, "20*");//sai da sala de bilhar
            n = write(data_socket, buffer, buffer_len);
            pthread_mutex_unlock(&mensagensMonitor);

            sem_post(&sem_sala_bil);
        }

    }
        return;
}

void create_threads(int SECONDS_TO_RUN) {
    struct sched_param param;
    
    int policy;
    int i;
    int id = 0;
    pthread_mutex_lock(&trincoVarGlobais);
    acabou = 0;
    pthread_mutex_unlock(&trincoVarGlobais);
    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    createdThreads = 0;
    // create the threads
    while (1) {
        // set the scheduling policy and priority of the thread
        policy = SCHED_OTHER;
        param.sched_priority = i+1;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setschedpolicy(&attr, policy);
        pthread_attr_setschedparam(&attr, &param);
        if(pthread_create(&thread, &attr, &criarClientes, &id)!= 0)
         {
                    printf("Errou ao criar pessoa\n");
                    return 1;
                }
                else
                {   
                    pthread_mutex_lock(&mensagensMonitor);
                    strcpy(buffer, "0*");
                    ret = write(data_socket, buffer, BUFFER_SIZE);
                    pthread_mutex_unlock(&mensagensMonitor);
                    if (ret == -1)
                    {
                        perror("read");
                        exit(EXIT_FAILURE);
                    }
                }; 
                sleep(1);
        id++;
        createdThreads++;
        struct timespec current_time;
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        double elapsed_time = (current_time.tv_sec - start_time.tv_sec) + 
                              (current_time.tv_nsec - start_time.tv_nsec) / 1e9;
        if (elapsed_time > SECONDS_TO_RUN) {
            pthread_mutex_lock(&trincoVarGlobais);
            acabou = 1;
            pthread_mutex_unlock(&trincoVarGlobais);
            break;

        }
}
}

int main(int argc, char *argv[])
{
    // OBTER DADOS DO CONFIG

    lerConfig();
    init();

    struct sockaddr_un name;
    int connection_socket;
    int result;
    int data;

    /*In case the program exited inadvertently on the last run,
     *remove the socket.
     **/

    unlink(SOCKET_NAME);

    /* Create Master socket. */

    /*SOCK_DGRAM for Datagram based communication*/
    connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (connection_socket == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /*initialize*/
    memset(&name, 0, sizeof(struct sockaddr_un));

    /*Specify the socket Cridentials*/
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    /* Bind socket to socket name.*/
    /* Purpose of bind() system call is that application() dictate the underlying
     * operating system the criteria of recieving the data. Here, bind() system call
     * is telling the OS that if sender process sends the data destined to socket "/tmp/DemoSocket",
     * then such data needs to be delivered to this server process (the server process)*/
    ret = bind(connection_socket, (const struct sockaddr *)&name,
               sizeof(struct sockaddr_un));

    if (ret == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /*
     * Prepare for accepting connections. The backlog size is set
     * to 20. So while one request is being processed other requests
     * can be waiting.
     * */

    ret = listen(connection_socket, 20);
    if (ret == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* This is the main loop for handling connections. */

    for (;;)
    {
        data_socket = accept(connection_socket, NULL, NULL);
        if (data_socket == -1)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        for (;;)
        {
            result = 0;
            memset(buffer, 0, BUFFER_SIZE);

            //strcpy(buffer, "Hello World\n");

            ret = write(data_socket, buffer, BUFFER_SIZE);
            if (ret == -1)
            {
                perror("read");
                exit(EXIT_FAILURE);
            }
            create_threads(dados.tempo_disco);
            sleep(30);
            if (pthread_join(thread, NULL)!= 0)
                        {
                            printf("Errou uma pessoa");
                            return 2;
                        }
                        else
                        {   
                            pthread_mutex_lock(&mensagensMonitor);
                            strcpy(buffer, "27*");
                            ret = write(data_socket, buffer, BUFFER_SIZE);
                            pthread_mutex_unlock(&mensagensMonitor);
                            if (ret == -1)
                            {
                                perror("read");
                                exit(EXIT_FAILURE);
                            }
                        }
                        
                        sleep(1);
            //escrever log (função)
            escreve_log();
            break;
        }
        pthread_mutex_destroy(&trincoVarGlobais);
        pthread_mutex_destroy(&mensagensMonitor);
        pthread_mutex_destroy(&FIFO_salaInicial);
        pthread_mutex_destroy(&trincoVeriJogo);
        sem_destroy(&sem_sala_ini);
        sem_destroy(&sem_sala_bar);
        sem_destroy(&sem_sala_danca);
        sem_destroy(&sem_sala_bil);
        sem_destroy(&sem_sala_wc);
        sem_destroy(&sem_sala_VIP);
        sleep(1);
    }
    close(data_socket);

    /*close the master socket*/
    close(connection_socket);

    /* Server should release resources before getting terminated.
     * Unlink the socket. */

    unlink(SOCKET_NAME);
    exit(EXIT_SUCCESS);
}