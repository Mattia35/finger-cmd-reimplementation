#include <stdio.h>
#include <utmp.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>

struct Idletime {
	int years;
	int days;
	int hours;
	int min;
	int sec;
	int tot;
};

void convertiMinuscolo(char* stringa);
int ordinamento(const void *a, const void *b);
void dataL(struct utmp *entry, char* date, char* zona);
void Mail(char* nome);
void Plan(char* nome);
void dataS(struct utmp *entry, char* date, char* zona);
void informazioniGecos(struct passwd *profile, char* office_number, char* nome_utente_esteso, char* office_phone, char* home_phone);
struct Idletime idletime(struct utmp *entry);
void trova_nome(struct passwd *profile, char* argv[], int i, int* controllo_nome, int* controllo_esistono_utenti, int* primo, int* secondo, int argc);
void trova_nomeL(struct passwd *profile, char* argv[], int i, int* controllo_nome, int* controllo_esistono_utenti, int argc);
void funzionePrintInizialeS(int primo, int secondo);
void funzionePrintS(char* user, char* nomeEsteso, char* tty, struct Idletime idle, char* loginTime, char* host, char* office_number, char* office_phone, int primo, int secondo);
void funzionePrintAlternativaS(char* user, char* nomeEsteso, char* tty, char* idletime_init, char* date, char* office_number, char* office_phone, int primo, int secondo);
void FunzioneStampaS(struct passwd *profile, struct utmp *entry, char* nome, int primo, int secondo);
void FunzioneStampaL(struct passwd* profile,  struct utmp* entry, char* nome, int* controllo_m, int* controllo_p, int* controlloPrimaStampa);
void soluzione_s_caso_nome_esteso(int argc, struct passwd *profile, char* argv[], int i, struct utmp *entry, int* controlloDoppioni, int* contatore_stringhe, char (*stringhe)[40], int primo, int secondo);
void soluzione_l_caso_nome_esteso(int argc, struct passwd *profile, char* argv[], int i, struct utmp *entry, int* controlloDoppioni, int* contatore_stringhe, char (*stringhe)[40], int* controllo_m, int* controllo_p, int* controlloPrimaStampa);
int caso_finger_s(int argc, char* argv[], int controllo_m);
void caso_finger_l(int argc, char *argv[], int controllo_m, int controllo_p);