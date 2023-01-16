#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>



#define UNIXSTR_PATH "/tmp/s.unixstr"
#define UNIXDG_PATH "/tmp/s.unixdgx"
#define UNIXDG_TMP "/tmp/dg2081920"
#define BUFFER_SIZE 1024

extern pthread_mutex_t mensagensMonitor;
extern pthread_mutex_t trincoVarGlobais;
//pthread_mutex_t mensagemSalaIni;
void escreve_aux(FILE *faux, char x[], int dados);
extern int data_socket;
extern int ret;
extern char buffer[BUFFER_SIZE];
extern int valorSemSalaIni;
extern int valorSemSalaDanca;
//flags
//int flagMessageSalaIni;
typedef struct configs
{
    int tempo_disco;
    int num_salas;
    int num_sala_ini;
    int tempo_med_sala_ini;
    int num_sala_danca;
    int tempo_med_sala_danca;
    int num_sala_wc;
    int tempo_med_wc;
    int num_sala_bar;
    int tempo_med_bar;
    int prob_beb;
    int num_sala_bil;
    int tempo_pausa_bil;
    int num_sala_vip;
    int fila_wc;
    int tempo_med_vip;
    int fila_sala_ini;
    int fila_sala_danca;
    int fila_bar;
    int fila_sala_bil;
    int fila_sala_VIP;
    int num_entra_sala_ini;
    int num_desist_sala_ini;
    int num_fila_sala_ini;
    int num_entra_sala_danca;
    int num_desist_sala_danca;
    int num_fila_sala_danca;
    int num_entra_bar;
    int num_desist_bar;
    int num_fila_bar;
    int num_entra_wc;
    int num_desist_wc;
    int num_fila_wc;
    int num_entra_bil;
    int num_desist_bil;
    int num_fila_bil;
    int num_entra_vip;
    int num_desist_vip;
    int num_fila_vip;
    int num_bebados;
    int num_jogos_bil;
} configs;

/*
    @param Momento SALA
        SALA 0 - Sala inicial
        SALA 1 - Pista de danca
        SALA 2 - Casa de banho
        SALA 3 - Bar
        SALA 4 - Sala de Bilhar
        SALA 5 - Sala VIP
*/

struct mensagemMonitor;