#include "header.h"

//funzione per la conversione di una stringa(verrà usata per gli input) in minuscolo
void convertiMinuscolo(char* stringa){
	while(*stringa){
		*stringa= tolower(*stringa);
		stringa++;
	}
}

//funzione per il confronto tra due stringhe, per l'ordinamento alfanumerico tra esse
int ordinamento(const void *a, const void *b){
	const char* stringa1 = *(const char **)a;
	const char* stringa2 = *(const char **)b;
	while (*stringa1 && *stringa2){
		int differenza = tolower((unsigned char)* stringa1) - tolower((unsigned char)* stringa2);
		if (differenza != 0){return differenza;}
		stringa1++;
		stringa2++;
	}
	return *stringa1 - *stringa2;
}

//funzione per il calcolo della data di accesso usata nella funzione di stampa a formattazione l
void dataL(struct utmp *entry, char* date, char* zona){
	time_t tempoCorrente;
	time(&tempoCorrente);
	struct tm *oraAttuale = localtime(&tempoCorrente);
	int meseAttuale = oraAttuale->tm_mon;
	time_t tempo = entry->ut_tv.tv_sec;
	struct tm *informazioniTempo = localtime(&tempo);
	int meseAccesso = informazioniTempo->tm_mon;
	char time_buffer[80];
	/*se, sono passati meso di 6 mesi dall'ultimo accesso, la data viene settata nella forma di stringa in: nome del giorno della settimana, 
	nome del mese, numero del giorno e orario...*/
	if (abs(meseAttuale - meseAccesso) <= 6){strftime(time_buffer, sizeof(time_buffer), "%a %b %d %H:%M", informazioniTempo);}
	//...altrimenti, viene settata nella forma di stringa in: nome del giorno della settimana, nome del mese, numero del giorno e anno
	else{strftime(time_buffer, sizeof(time_buffer), "%a %b %d %Y", informazioniTempo);}
	//salva la data, in modo da poter essere stampata
	strcpy(date,time_buffer);
	char zone[strlen(informazioniTempo->tm_zone)];
	strcpy(zone, informazioniTempo->tm_zone);
	//salva la zona di fuso orario
	strcpy(zona, zone);
}

//funzione che produce e stampa il messaggio relativo allo stato di mail dell'utente (nella formattazione ad l)
void Mail(char* nome){
	FILE * pf;
	char appoggio1[100];
	char appoggio2[100];
	char dataMail[100];
	char dataAccesso[100];
	struct stat file;
	//creazione della directory dove è presente il possibile file relativo allo stato mail dell'utente
	char filename[] = "/var/mail/";
	strcat(filename, nome);
	//apertura in lettura del file
	pf = fopen(filename, "r");
	//se non è aperto correttamente, vuol dire che è inesistente, e quindi viene stampato il relativo risultato
	if (stat(filename, &file)!=0){
		printf("No mail.\n");
		return;
	}
	//altrimenti, calcola ultimo accesso dell'utente e ultimo momento di modifica del file (cioè ultima mail arrivata)
	stat(filename, &file);
	time_t ultimaMail = file.st_mtime;
	struct tm *informazioniTempo1 = localtime(&ultimaMail);
	strftime(appoggio1, sizeof(appoggio1), "%a %b %d %H:%M %Y", informazioniTempo1);
	char zone[strlen(informazioniTempo1->tm_zone)];
	strcpy(zone, informazioniTempo1->tm_zone);
	strcpy(dataMail, appoggio1);
	time_t ultimoAccesso = file.st_atime;
	struct tm *informazioniTempo2 = localtime(&ultimoAccesso);
	strftime(appoggio2, sizeof(appoggio2), "%a %b %d %H:%M %Y", informazioniTempo2);
	strcpy(dataAccesso, appoggio2);
	//se l'ultimo accesso è maggiore o uguale all'ultimo momento di modifica del file, allora stampa il momento di ultima lettura della mail
	if(ultimoAccesso>=ultimaMail){
		printf("Mail last read %s (%s)\n", dataAccesso, zone);
	}
	//altrimenti, stampa il momento in cui è arrivata l'ultima mail 
	else{
		printf("New mail received %s (%s)\n     Unread since %s (%s)\n", dataMail, zone, dataAccesso, zone);
	}
}

//funzione che produce e stampa il messaggio relativo allo stato dei file .pgpkey, .project e .plan dell'utente (nella formattazione ad l)
void Plan(char* nome){
	//stampa contenuto, se ce n'è, nel file .pgpkey
	int controllo = 0;
	FILE * pf;
	char filename[100] = "/home/";
	strcat(filename, nome);
	strcat(filename, "/");
	strcat(filename, ".pgpkey");
	char line[100];
	pf = fopen(filename, "r");
	if(pf!=NULL){
		printf("PGP Key:\n");
		while(fgets(line, sizeof(line), pf)){
			printf("%s", line);
			if (strcmp(line,"")!=0){
				controllo=1;
			}
		}
		if (controllo==0){
			printf("\n");
		}
	}
	//stampa contenuto, se ce n'è, nel file .project
	strcpy(filename,"/home/");
	strcat(filename, nome);
	strcat(filename, "/");
	strcat(filename, ".project");
	pf = fopen(filename, "r");
	controllo = 0;
	if(pf!=NULL){
		printf("Project:\n");
		while(fgets(line, sizeof(line), pf)){
			printf("%s", line);
			if (strcmp(line,"")!=0){
				controllo=1;
			}
		}
		if (controllo==0){
			printf("\n");
		}
	}
	//stampa contenuto, se ce n'è, nel file .plan
	strcpy(filename,"/home/");
	strcat(filename, nome);
	strcat(filename, "/");
	strcat(filename, ".plan");
	pf = fopen(filename, "r");
	controllo = 0;
	if(pf!=NULL){
		printf("Plan:\n");
		while(fgets(line, sizeof(line), pf)){
			printf("%s", line);
			if (strcmp(line,"")!=0){
				controllo=1;
			}
		}
		if (controllo==0){
			printf("\n");
		}
	}
	//se non esiste alcun file .plan, viene stampata a schermo "No Plan"
	else{
		printf("No Plan.\n");
	}
}

//funzione per il calcolo della data di accesso usata nella funzione di stampa a formattazione s
void dataS(struct utmp *entry, char* date, char* zona){
	time_t tempoCorrente;
	time(&tempoCorrente);
	struct tm *oraAttuale = localtime(&tempoCorrente);
	int meseAttuale = oraAttuale->tm_mon;
	time_t tempo = entry->ut_tv.tv_sec;
	struct tm *informazioniTempo = localtime(&tempo);
	int meseAccesso = informazioniTempo->tm_mon;
	char time_buffer[80];
	//se, sono passati meso di 6 mesi dall'ultimo accesso, la data viene settata nella forma di stringa in: nome del mese, numero del giorno e orario...
	if (abs(meseAttuale - meseAccesso) <= 6){strftime(time_buffer, sizeof(time_buffer), "%b %d %H:%M", informazioniTempo);}
	//...altrimenti, viene settata nella forma di stringa in: nome del mese, numero del giorno e anno
	else{strftime(time_buffer, sizeof(time_buffer), "%b %d %Y", informazioniTempo);}
	strcpy(date,time_buffer);
	char zone[strlen(informazioniTempo->tm_zone)];
	strcpy(zone, informazioniTempo->tm_zone);
	//salva la zona di fuso orario
	strcpy(zona, zone);
}

//funzione per l'estrazione di informazioni di un utente dal gecos (nome esteso, numero di ufficio, numero di telefono di ufficio e numero di telefono di casa)
void informazioniGecos(struct passwd *profile, char* office_number, char* nome_utente_esteso, char* office_phone, char* home_phone){
	char gecos[100];
	strcpy(gecos, profile->pw_gecos);
	int contatore_posizione = 0;
	int contatore_virgole = 0;
	int contatore_posizione_office_number = 0;
	int contatore_posizione_nome_esteso = 0;
	int contatore_posizione_office_phone = 0;
	int contatore_posizione_home_phone =0;
	//l'estrazione delle informazioni è fatto carattere carattere per carattere
	while (gecos[contatore_posizione] != '\0' && contatore_virgole <= 3 && gecos!=NULL){
		if (gecos[contatore_posizione]==','){
			contatore_virgole ++;
			contatore_posizione ++;
			continue;
		}
		//estrazione nome esteso dell'utente
		if (contatore_virgole == 0){
			nome_utente_esteso[contatore_posizione_nome_esteso] = gecos[contatore_posizione];
			contatore_posizione_nome_esteso ++;
		}
		//estrazione numero di ufficio dell'utente
		if (contatore_virgole == 1){
			office_number[contatore_posizione_office_number] = gecos[contatore_posizione];
			contatore_posizione_office_number ++;
		}
		//estrazione numero di telefono di ufficio dell'utente
		if (contatore_virgole == 2){
			office_phone[contatore_posizione_office_phone] = gecos[contatore_posizione];
			contatore_posizione_office_phone ++;
		}
		//estrazione numero di telefono di casa dell'utente
		if (contatore_virgole == 3){
			home_phone[contatore_posizione_home_phone] = gecos[contatore_posizione];
			contatore_posizione_home_phone ++;
		}
		contatore_posizione ++;
	}
	//setting del carattere di terminazione stringa per tutti i dati estratti (e non)
	office_number[contatore_posizione_office_number] = '\0';
	nome_utente_esteso[contatore_posizione_nome_esteso] = '\0';
	office_phone[contatore_posizione_office_phone] ='\0';
	home_phone[contatore_posizione_home_phone] = '\0';
	//modifica della formattazione del numero di telefono di ufficio
	switch (strlen(office_phone)){
		case 4: {
			memmove(office_phone+1, office_phone, strlen(office_phone)+1);
			office_phone[0] = 'x';
			break;
		}
		case 5: {
			memmove(office_phone+1, office_phone, strlen(office_phone)+1);
			office_phone[0] = 'x';
			memmove(office_phone+3, office_phone+2, strlen(office_phone)-1);
			office_phone[2] = '-';
			break;
		}
		case 7: {
			memmove(office_phone+4, office_phone+3, strlen(office_phone)-2);
			office_phone[3] = '-';
			break;
		}
		case 10: {
			memmove(office_phone+4, office_phone+3, strlen(office_phone)-2);
			office_phone[3] = '-';
			memmove(office_phone+8, office_phone+7, strlen(office_phone)-6);
			office_phone[7] = '-';
			break;
		}
		case 11: {
			memmove(office_phone+1, office_phone, strlen(office_phone)+1);
			office_phone[0] = '+';
			memmove(office_phone+3, office_phone+2, strlen(office_phone)-1);
			office_phone[2] = '-';
			memmove(office_phone+7, office_phone+6, strlen(office_phone)-5);
			office_phone[6] = '-';
			memmove(office_phone+11, office_phone+10, strlen(office_phone)-9);
			office_phone[10] = '-';
			break;
		}	
	}
	//modifica della formattazione del numero di telefono di casa
	switch (strlen(home_phone)){
		case 4: {
			memmove(home_phone+1, home_phone, strlen(home_phone)+1);
			home_phone[0] = 'x';
			break;
		}
		case 5: {
			memmove(home_phone+1, home_phone, strlen(home_phone)+1);
			home_phone[0] = 'x';
			memmove(home_phone+3, home_phone+2, strlen(home_phone)-1);
			home_phone[2] = '-';
			break;
		}
		case 7: {
			memmove(home_phone+4, home_phone+3, strlen(home_phone)-2);
			home_phone[3] = '-';
			break;
		}
		case 10: {
			memmove(home_phone+4, home_phone+3, strlen(home_phone)-2);
			home_phone[3] = '-';
			memmove(home_phone+8, home_phone+7, strlen(home_phone)-6);
			home_phone[7] = '-';
			break;
		}
		case 11: {
			memmove(home_phone+1, home_phone, strlen(home_phone)+1);
			home_phone[0] = '+';
			memmove(home_phone+3, home_phone+2, strlen(home_phone)-1);
			home_phone[2] = '-';
			memmove(home_phone+7, home_phone+6, strlen(home_phone)-5);
			home_phone[6] = '-';
			memmove(home_phone+11, home_phone+10, strlen(home_phone)-9);
			home_phone[10] = '-';
			break;
		}
	}
}

//funzione che estrae l'idletime di un utente e lo salva nella struct apposita
struct Idletime idletime(struct utmp *entry){
	//creazione della stringa rappresentante la directory dov'è presente il file della tty dove ha accesso l'utente
	char ttyVero[100] = "/dev/";
	char tty[sizeof(entry->ut_line)+1];
	tty[sizeof(entry->ut_line)]='\0';
	strncpy(tty, entry->ut_line, sizeof(entry->ut_line));
	strcat(ttyVero, tty);
    struct stat fileTty;
	//apertura dei dati relativi al file tty
	stat(ttyVero, &fileTty);
	//estrazione dell'idletime
    time_t tempo = fileTty.st_mtime;
	//conversione del dato estratto in intero, ma in secondi
	int idlesec = time(NULL) - tempo;
	//salvataggio nella struct apposita secondo tutte le unità di misura utili per la fase di printing
	struct Idletime res = { idlesec / (365 *24 * 3600), ( idlesec % ( 365 * 24 * 3600 )) / ( 24 * 3600 ), ( idlesec % ( 24 * 3600 )) / 3600, ( idlesec % 3600) / 60, idlesec % 60, idlesec};
	return res;
}

//funzione che controlla se un input rappresenta il nome esteso di un utente, e aggiorna i valori primo e secondo in caso siano più piccoli (formattazione s)
void trova_nome(struct passwd *profile, char* argv[], int i, int* controllo_nome, int* controllo_esistono_utenti, int* primo, int* secondo, int argc){
	setpwent();
	char nomeAppoggio[100];
	strcpy(nomeAppoggio, argv[i]);
	convertiMinuscolo(nomeAppoggio);
	//trova il nome per esteso, carattere per carattere, e lo confronta ad ogni possibile spazio con l'input analizzato
	while ((profile=getpwent())!=NULL){
		char user[100];
		strcpy(user, profile->pw_name);
		char nome_utente_esteso[100];
		char gecos[100];
		strcpy(gecos, profile->pw_gecos);
		int contatore_posizione = 0;
		int contatore_posizione_nome_esteso = 0;
		while (gecos[contatore_posizione] != '\0' && gecos!=NULL){
			convertiMinuscolo(nome_utente_esteso);
			//confronto tra il nome esteso e input analizzato
			if (gecos[contatore_posizione]==','){
				//se il confronto riesce, allora è confermata la validità dell'input, e si procede con il possibile aggiornamento dei valori primo e secondo
				if (strcmp(nome_utente_esteso, nomeAppoggio)==0){
					*controllo_nome=1;
					*controllo_esistono_utenti=1;
					contatore_posizione ++;
					if (*primo<strlen(user)){*primo = strlen(argv[i]);}
					if (*secondo<strlen(argv[i])){*secondo = strlen(argv[i]);}
				}
				break;
			}
			//confronto tra parte di nome esteso e input analizzato
			if (gecos[contatore_posizione]==' '){
				//se il confronto riesce, allora è confermata la validità dell'input, e si procede con il possibile aggiornamento dei valori primo e secondo
				if (strcmp(nome_utente_esteso, nomeAppoggio)==0){
					*controllo_nome=1;
					*controllo_esistono_utenti=1;
					if (*primo<strlen(user)){*primo = strlen(argv[i]);}
					if (*secondo<strlen(argv[i])){*secondo = strlen(argv[i]);}
					break;
				}
				else{
					strcpy(nome_utente_esteso, "");
					contatore_posizione_nome_esteso = 0;
					contatore_posizione++;
					continue;
				}
			}
			nome_utente_esteso[contatore_posizione_nome_esteso] = gecos[contatore_posizione];
			contatore_posizione_nome_esteso ++;
			contatore_posizione ++;
		}
		nome_utente_esteso[contatore_posizione_nome_esteso] = '\0';
		/*confronto tra il nome esteso e input analizzato. Se il confronto riesce, allora è confermata la validità dell'input, 
		e si procede con il possibile aggiornamento dei valori primo e secondo*/
		if (strcmp(nome_utente_esteso, nomeAppoggio)==0){
			*controllo_nome=1;
			*controllo_esistono_utenti=1;
			if (*primo<strlen(user)){*primo = strlen(argv[i]);}
			if (*secondo<strlen(argv[i])){*secondo = strlen(argv[i]);}
			break;
		}
		strcpy(user, "");
		strcpy(nome_utente_esteso, "");
		strcpy(gecos, "");
	}
	endpwent();
	//se l'input non è associato ad alcun nome esteso degli utenti, allora non è un input valido, perciò la funzione stampa un messaggio di errore
	if (*controllo_nome==0){printf("finger: %s: no such user.\n", argv[i]);}
}

//funzione che controlla se un input rappresenta il nome esteso di un utente (formattazione l)
void trova_nomeL(struct passwd *profile, char* argv[], int i, int* controllo_nome, int* controllo_esistono_utenti, int argc){
	setpwent();
	char nomeAppoggio[100];
	strcpy(nomeAppoggio, argv[i]);
	convertiMinuscolo(nomeAppoggio);
	//trova il nome per esteso, carattere per carattere, e lo confronta ad ogni possibile spazio con l'input analizzato
	while ((profile=getpwent())!=NULL){
		char user[100];
		strcpy(user, profile->pw_name);
		char nome_utente_esteso[100];
		char gecos[100];
		strcpy(gecos, profile->pw_gecos);
		int contatore_posizione = 0;
		int contatore_posizione_nome_esteso = 0;
		while (gecos[contatore_posizione] != '\0' && gecos!=NULL){
			convertiMinuscolo(nome_utente_esteso);
			//confronto tra il nome esteso e input analizzato
			if (gecos[contatore_posizione]==','){
				//se il confronto riesce, allora è confermata la validità dell'input
				if (strcmp(nome_utente_esteso, nomeAppoggio)==0){
					*controllo_nome=1;
					*controllo_esistono_utenti=1;
					contatore_posizione ++;
				}
				break;
			}
			//confronto tra parte di nome esteso e input analizzato
			if (gecos[contatore_posizione]==' '){
				//se il confronto riesce, allora è confermata la validità dell'input
				if (strcmp(nome_utente_esteso, nomeAppoggio)==0){
					*controllo_nome=1;
					*controllo_esistono_utenti=1;
					break;
				}
				else{
					strcpy(nome_utente_esteso, "");
					contatore_posizione_nome_esteso = 0;
					contatore_posizione++;
					continue;
				}
			}
			nome_utente_esteso[contatore_posizione_nome_esteso] = gecos[contatore_posizione];
			contatore_posizione_nome_esteso ++;
			contatore_posizione ++;
		}
		nome_utente_esteso[contatore_posizione_nome_esteso] = '\0';
		//confronto tra il nome esteso e input analizzato. se il confronto riesce, allora è confermata la validità dell'input
		if (strcmp(nome_utente_esteso, nomeAppoggio)==0){
			*controllo_nome=1;
			*controllo_esistono_utenti=1;
			break;
		}
		strcpy(user, "");
		strcpy(nome_utente_esteso, "");
		strcpy(gecos, "");
	}
	endpwent();
	//se l'input non è associato ad alcun nome esteso degli utenti, allora non è un input valido, perciò la funzione stampa un messaggio di errore
	if (*controllo_nome==0){printf("finger: %s: no such user.\n", argv[i]);}
}

/*funzione che stampa a video la prima riga della formattazione a s, secondo i valori primo e secondo, che rappresentano la lunghezza massima di 
nome utente e utente esteso degli utenti di cui si vuole stampare i dati*/
void funzionePrintInizialeS(int primo, int secondo){
	int lunghezzaLogin = 5;
	int lunghezzaName = 4;
	printf("Login");
	if (primo>lunghezzaLogin){
		for (int i = 1; lunghezzaLogin+i<=primo; i++){
			printf(" ");
		}
	}
	printf("   Name");
	if (secondo>lunghezzaName){
		for (int i = 1; lunghezzaName+i<=secondo; i++){
			printf(" ");
		}
	}
	printf("   Tty      Idle  Login Time   Office     Office Phone\n");
}


//funzione che si occupa della fase di stampa a video dei dati di un utente
void funzionePrintS(char* user, char* nomeEsteso, char* tty, struct Idletime idle, char* loginTime, char* host, char* office_number, char* office_phone, int primo, int secondo){
	//stampa del nome utente e degli spazi successivi, in base al valore primo
	printf("%s", user);
	if (strlen(user)<primo){
		for (int i = 1; strlen(user)+i<=primo; i++){
			printf(" ");
		}
	}
	//stampa del nome esteso e degli spazi successivi, in base al valore secondo
	printf("   %s", nomeEsteso);
	if (strlen(nomeEsteso)<secondo){
		for (int i = 1; strlen(nomeEsteso)+i<=secondo; i++){
			printf(" ");
		}
	}
	//stampa nella formattazione s corretta, in base all'idle time dell'utente
	if (idle.years == 0 && idle.days == 0 && idle.hours == 0){printf("   %-9s%4d  %-13s", tty, idle.min, loginTime);}
	else if (idle.years == 0 && idle.days == 0 && idle.hours != 0 && idle.hours <=9 && idle.min <= 9){printf("  %-9s%d:0%d  %-13s", tty, idle.hours, idle.min, loginTime);}
	else if (idle.years == 0 && idle.days == 0 && idle.hours != 0 && idle.hours >9 && idle.min <= 9){printf("  %-8s%d:0%d  %-13s", tty, idle.hours, idle.min, loginTime);}
	else if (idle.years == 0 && idle.days == 0 && idle.hours != 0 && idle.hours <=9 && idle.min > 9){printf("  %-9s%d:%2d  %-13s", tty, idle.hours, idle.min, loginTime);}
	else if (idle.years == 0 && idle.days == 0 && idle.hours != 0 && idle.hours >9 && idle.min > 9){printf("  %-8s%d:%2d  %-13s", tty, idle.hours, idle.min, loginTime);}
	else if (idle.years == 0 && idle.days != 0){printf("  %-9s%3dd  %-13s", tty, idle.days, loginTime);}
	else if (idle.years != 0){printf("  %-9s%4d  %-13s", tty, idle.years, loginTime);}
	//se il campo host ha una lunghezza maggiore di 0,, allora la stampa...
	if (strlen(host)!=0){printf("(%s)\n", host);}
	//...altrimenti stampa anche il numero di ufficio e il numero di telefono di ufficio
	else{printf("%-11s%s\n", office_number, office_phone);}
}

//funzione di stampa di dati di un utente che non ha fatto mai accesso in formattazione s
void funzionePrintAlternativaS(char* user, char* nomeEsteso, char* tty, char* idletime_init, char* date, char* office_number, char* office_phone, int primo, int secondo){
	//stampa del nome utente e degli spazi successivi, in base al valore primo
	printf("%s", user);
	if (strlen(user)<primo){
		for (int i = 1; strlen(user)+i<=primo; i++){
			printf(" ");
		}
	}
	//stampa del nome esteso e degli spazi successivi, in base al valore secondo
	printf("   %s", nomeEsteso);
	if (strlen(nomeEsteso)<secondo){
		for (int i = 1; strlen(nomeEsteso)+i<=secondo; i++){
			printf(" ");
		}
	}
	//stampa asterischi sostitutivi e la scritta No logins, ed eventuali dati come numero di ufficio e il numero di telefono di ufficio
	printf("   %-9s", tty);
	printf("%4s  %-13s%-11s%s\n",idletime_init, date, office_number, office_phone);
}

/*funzione che svolge l'indirizzamento verso una seconda funzione di printing in formattazione s dei dati di un utente, in base a che 
tipo di utente è (ad esempio, se loggato o non)*/
void FunzioneStampaS(struct passwd *profile, struct utmp *entry, char* nome, int primo, int secondo){
	//estrazione di quasi tutti i dati relativi a tale utente
	char nomeEsteso[100];
	char tty[] = "*";
	char idletime_init[] = "*";
	char date[] = "No logins";
	char office_number[100];
	char office_phone[100];
	char home_phone[100];
	informazioniGecos(profile, office_number, nomeEsteso, office_phone, home_phone);
	struct Idletime idle ={0,0,0,0,0,0};
	int userProcess = 0;
	int controlloUP = 0;
	//controllo che l'utente abbia una sessione attiva, ed in caso trovo gli ultimi dati ad esso relativi e invio la fase di printing
	setutent();
	while ((entry= getutent())!=NULL){
		if (entry->ut_type==USER_PROCESS){
			char nome_utente2[sizeof(entry->ut_user)+1];
			nome_utente2[sizeof(entry->ut_user)]='\0';
			strncpy(nome_utente2, entry->ut_user, sizeof(entry->ut_user));
			if (strcmp(nome, nome_utente2)==0){
				userProcess = 1;
				idle = idletime(entry);
				char zona[100];
				dataS(entry, date, zona);
				char tty[sizeof(entry->ut_line)+1];
				tty[sizeof(entry->ut_line)]='\0';
				strncpy(tty, entry->ut_line, sizeof(entry->ut_line));
				funzionePrintS(nome, nomeEsteso, entry->ut_line, idle, date, entry->ut_host, office_number, office_phone, primo, secondo);
			}
		}
	}
	endutent();
	//se l'utente non ha una sessione attiva, allora stampo a schermo i dati precedentemente raccolti attraverso una funzione alternativa
	if (userProcess==0){funzionePrintAlternativaS(nome, nomeEsteso, tty, idletime_init, date, office_number, office_phone, primo, secondo);}
}

//funzione per il printing dei dati di un utente nella formattazione l
void FunzioneStampaL(struct passwd* profile,  struct utmp* entry, char* nome, int* controllo_m, int* controllo_p, int* controlloPrimaStampa){
	//estrazione dei dati dell'utente
	char directory[80];
	strcpy(directory, profile->pw_dir);
	char shell[80];
	strcpy(shell, profile->pw_shell);
	char nomeEsteso[100];
	char tty[]="";
	struct Idletime idle = {0,0,0,0,0,0};
	char date[] = "";
	char office_number[100];
	char office_phone[100];
	char home_phone[100];
	char zona[100];
	char host[100];
	int controllo_accesso = 0;
	informazioniGecos(profile, office_number, nomeEsteso, office_phone, home_phone);
	//controllo se l'utente ha una sessione attiva
	setutent();
	while ((entry= getutent())!=NULL){
		//avendo una sessione attiva, la funzione estrae gli ultimi dati utili per la stampa dell'utente, e poi li stampa secondo la formattazione a l
		if (entry->ut_type==USER_PROCESS){
			char nome_utente2[sizeof(entry->ut_user)+1];
			nome_utente2[sizeof(entry->ut_user)]='\0';
			strncpy(nome_utente2, entry->ut_user, sizeof(entry->ut_user));
			if (strcmp(nome, nome_utente2)==0){
				controllo_accesso = 1;
				idle = idletime(entry);
				dataL(entry, date, zona);
				char tty[sizeof(entry->ut_line)+1];
				tty[sizeof(entry->ut_line)]='\0';
				strncpy(tty, entry->ut_line, sizeof(entry->ut_line));
				char host[sizeof(entry->ut_host)+1];
				host[sizeof(entry->ut_host)]='\0';
				strncpy(host, entry->ut_host, sizeof(entry->ut_host));
				if (*controlloPrimaStampa==1){*controlloPrimaStampa=0;}
				else{printf("\n");}
				printf("Login: %-33sName: %s\nDirectory: %-29sShell: %s\n", nome, nomeEsteso, directory, shell);
				if (strcmp(office_number,"")!=0 || strcmp(office_phone,"")!=0 || strcmp(home_phone, "")!=0){
					if (strcmp(office_number,"")!=0 && strcmp(office_phone,"")!=0){
						strcat(office_number, ", ");
						strcat(office_number, office_phone);
						printf("Office: %-32s", office_number);
					}
					else if (strcmp(office_number,"")!=0 || strcmp(office_phone,"")!=0){
						strcat(office_number, office_phone);
						printf("Office: %-32s", office_number);
					}
					if (strcmp(home_phone, "")!=0){
						printf("Home Phone: %s", home_phone);
					}
					printf("\n");
				}
				printf("On since %s (%s) on %s", date, zona, tty);
				if (strcmp(host, "")!=0){printf(" from %s", host);}
				printf("   ");
				if (idle.years == 1){printf("%d year idle\n", idle.years);}
				else if (idle.years > 1){printf("%d years idle\n\n", idle.years);}
				else if (idle.years == 0 && idle.days ==1){printf("%d day idle\n", idle.days);}
				else if (idle.years == 0 && idle.days >1){printf("%d days idle\n", idle.days);}
				else if (idle.years == 0 && idle.days == 0 && idle.hours ==1 && idle.min ==1){printf("%d hour %d minute idle\n", idle.hours, idle.min);}
				else if (idle.years == 0 && idle.days == 0 && idle.hours >1 && idle.min ==1){printf("%d hours %d minute idle\n", idle.hours, idle.min);}
				else if (idle.years == 0 && idle.days == 0 && idle.hours ==1 && idle.min >1){printf("%d hour %d minutes idle\n", idle.hours, idle.min);}
				else if (idle.years == 0 && idle.days == 0 && idle.hours >1 && idle.min >1){printf("%d hours %d minutes idle\n", idle.hours, idle.min);}
				else if (idle.years == 0 && idle.days == 0 && idle.hours ==0 && idle.min ==1 && idle.sec ==1){printf("%d minute %d second idle\n", idle.min, idle.sec);}
				else if (idle.years == 0 && idle.days == 0 && idle.hours ==0 && idle.min >1 && idle.sec ==1){printf("%d minutes %d second idle\n", idle.min, idle.sec);}
				else if (idle.years == 0 && idle.days == 0 && idle.hours ==0 && idle.min ==1 && idle.sec >1){printf("%d minute %d seconds idle\n", idle.min, idle.sec);}
				else if (idle.years == 0 && idle.days == 0 && idle.hours ==0 && idle.min >1 && idle.sec >1){printf("%d minutes %d seconds idle\n", idle.min, idle.sec);}
				//stampa lo stato utente mail e plan, se non presente -p tra gli input
				Mail(nome);
				if (*controllo_p==0){Plan(nome);}
			}
		}
	}
	//se non c'è alcuna sessione attiva, avvia la stampa dei dati ottenuti precedentemente secondo la formattazione a l
	if (controllo_accesso == 0){
		if (*controlloPrimaStampa==1){*controlloPrimaStampa=0;}
		else{printf("\n");}
		printf("Login: %-33sName: %s\nDirectory: %-29sShell: %s\n", nome, nomeEsteso, directory, shell);
		if (strcmp(office_number,"")!=0 || strcmp(office_phone,"")!=0 || strcmp(home_phone, "")!=0){
			if (strcmp(office_number,"")!=0 && strcmp(office_phone,"")!=0){
				strcat(office_number, ", ");
				strcat(office_number, office_phone);
				printf("Office: %-32s", office_number);
			}
			else if (strcmp(office_number,"")!=0 || strcmp(office_phone,"")!=0){
				strcat(office_number, office_phone);
				printf("Office: %-32s", office_number);
			}
			if (strcmp(home_phone, "")!=0){
				printf("Home Phone: %s", home_phone);
			}
			printf("\n");
		}
		if (strcmp(date, "")!=0){printf("Last login %s (%s) on %s\n", date, zona, tty);}
		else{printf("Never logged in.\n");}
		//stampa lo stato utente mail e plan, se non presente -p tra gli input
		Mail(nome);
		if (*controllo_p==0){Plan(nome);}
	}
}

/*funzione usata per verificare che l'input è associato ad una parte o a tutto il nome esteso di un utente. Nel caso, reindirizza i dati 
verso una funzione di stampa in formattazione s*/
void soluzione_s_caso_nome_esteso(int argc, struct passwd *profile, char* argv[], int i, struct utmp *entry, int* controlloDoppioni, int* contatore_stringhe, char (*stringhe)[40], int primo, int secondo){
	setpwent();
	char nomeAppoggio[100];
	strcpy(nomeAppoggio, argv[i]);
	convertiMinuscolo(nomeAppoggio);
	//inizio controllo della verifica dell'input scorrendo ogni utente disponibile
	while ((profile=getpwent())!=NULL){
		int controllo_esistono_utenti=0;
		char nomeEsteso[100];
		char gecos[100];
		strcpy(gecos, profile->pw_gecos);
		int contatore_posizione = 0;
		int contatore_posizione_nome_esteso = 0;
		//verifica sviluppata carattere per carattere
		while (gecos[contatore_posizione] != '\0' && gecos!=NULL){
			convertiMinuscolo(nomeEsteso);
			//se l'input è uguale al nome esteso di un utente, allora segna che può essere controllato che non sia un doppione
			if (gecos[contatore_posizione]==','){
				if (strcmp(nomeEsteso, nomeAppoggio)==0){
					controllo_esistono_utenti=1;
					contatore_posizione ++;
				}
				break;
			}
			//se l'input è uguale ad una parte del nome esteso di un utente, allora segna che può essere controllato che non sia un doppione
			if (gecos[contatore_posizione]==' '){
				if (strcmp(nomeEsteso, nomeAppoggio)==0){
					controllo_esistono_utenti=1;
					break;
				}
				else{
					strcpy(nomeEsteso, "");
					contatore_posizione_nome_esteso=0;
					contatore_posizione ++;
					continue;
				}
			}
			nomeEsteso[contatore_posizione_nome_esteso] = gecos[contatore_posizione];
			contatore_posizione_nome_esteso ++;
			contatore_posizione ++;
		}
		nomeEsteso[contatore_posizione_nome_esteso] = '\0';
		//una volta stabilito che l'utente è potenzialmente valido, verifica se è un doppione o no, e nel caso non lo sia, lo manda in stampa
		if (controllo_esistono_utenti==1){
			char nome[80];
			strcpy(nome, profile->pw_name);
			for (int c = 0; c < argc; c++){
						if (strcmp(stringhe[c], nome)==0){
							*controlloDoppioni = 1;
						}
					}
					if (*controlloDoppioni==1){
						continue;
					}
					strcpy(stringhe[*contatore_stringhe], nome);
					*contatore_stringhe++;
			FunzioneStampaS(profile, entry, nome, primo, secondo);
		}
	}
	endpwent();
}

/*funzione usata per verificare che l'input è associato ad una parte o a tutto il nome esteso di un utente. Nel caso, reindirizza i dati 
verso una funzione di stampa in formattazione l*/
void soluzione_l_caso_nome_esteso(int argc, struct passwd *profile, char* argv[], int i, struct utmp *entry, int* controlloDoppioni, int* contatore_stringhe, char (*stringhe)[40], int* controllo_m, int* controllo_p, int* controlloPrimaStampa){
	setpwent();
	char nomeAppoggio[100];
	strcpy(nomeAppoggio, argv[i]);
	convertiMinuscolo(nomeAppoggio);
	//inizio controllo della verifica dell'input scorrendo ogni utente disponibile
	while ((profile=getpwent())!=NULL){
		int controllo_esistono_utenti=0;
		char nomeEsteso[100];
		char gecos[100];
		strcpy(gecos, profile->pw_gecos);
		int contatore_posizione = 0;
		int contatore_posizione_nome_esteso = 0;
		//verifica sviluppata carattere per carattere
		while (gecos[contatore_posizione] != '\0' && gecos!=NULL){
			convertiMinuscolo(nomeEsteso);
			//se l'input è uguale al nome esteso di un utente, allora segna che può essere controllato che non sia un doppione
			if (gecos[contatore_posizione]==','){
				if (strcmp(nomeEsteso, nomeAppoggio)==0){
					controllo_esistono_utenti=1;
					contatore_posizione ++;
				}
				break;
			}
			//se l'input è uguale ad una parte del nome esteso di un utente, allora segna che può essere controllato che non sia un doppione
			if (gecos[contatore_posizione]==' '){
				if (strcmp(nomeEsteso, nomeAppoggio)==0){
					controllo_esistono_utenti=1;
					break;
				}
				else{
					strcpy(nomeEsteso, "");
					contatore_posizione_nome_esteso=0;
					contatore_posizione ++;
					continue;
				}
			}
			nomeEsteso[contatore_posizione_nome_esteso] = gecos[contatore_posizione];
			contatore_posizione_nome_esteso ++;
			contatore_posizione ++;
		}
		nomeEsteso[contatore_posizione_nome_esteso] = '\0';
		//una volta stabilito che l'utente è potenzialmente valido, verifica se è un doppione o no, e nel caso non lo sia, lo manda in stampa
		if (controllo_esistono_utenti==1){
			char nome[80];
			strcpy(nome, profile->pw_name);
			for (int c = 0; c < argc; c++){
						if (strcmp(stringhe[c], nome)==0){
							*controlloDoppioni = 1;
						}
					}
					if (*controlloDoppioni==1){
						continue;
					}
					strcpy(stringhe[*contatore_stringhe], nome);
					*contatore_stringhe++;
			FunzioneStampaL(profile, entry, nome, controllo_m, controllo_p, controlloPrimaStampa);
		}
	}
	endpwent();
}


//funzione che gestisce il caso in cui la funzionen finger debba stampare a schermo secondo la formattazione a s
int caso_finger_s(int argc, char* argv[], int controllo_m){
	struct utmp *entry;
	struct passwd *profile;
	int primo = 0;
	int secondo = 0;
	int calcolo = 0;
	//verifica, oltre a parole chiave come "-s", "-m" e "-p", quanti input sono stati dati dall'utente
	if (argc>1){
		for (int i = 1; i < argc; i++){
			if (strcmp(argv[i], "-s")!=0 && strcmp(argv[i], "-m")!=0 && strcmp(argv[i], "-p")!=0){calcolo++;}
		}
	}
	//se non ci sono ulteriori input, allora stampa solo gli utenti attualmente attivi
	if (calcolo == 0){
		setutent();
		while((entry= getutent())!=NULL){
			if(entry->ut_type== USER_PROCESS){
				//salvataggio del nome utente
				char nome[sizeof(entry->ut_user)+1];
				nome[sizeof(entry->ut_user)]='\0';
				strncpy(nome, entry->ut_user, sizeof(entry->ut_user));
				char nomeEsteso[100];
				char office_number[100];
				char office_phone[100];
				char home_phone[100];
				profile= getpwnam(nome);
				//estrazione degli altri dati, cioè il nome esteso, il numero di ufficio, il numero di telefono di ufficio e il numero di telefono di casa
				informazioniGecos(profile, office_number, nomeEsteso, office_phone, home_phone);
				//setting di due valori, primo e secondo, che indicheranno la massima lunghezza tra i nomi utenti e i nomi estesi
				if (primo<strlen(nome)){primo = strlen(nome);}
				if (secondo<strlen(nomeEsteso)){secondo = strlen(nomeEsteso);}
			}
		}
		endutent();
		//stampa della prima linea della formattazione s, secondo i valori primo e secondo rilevati prima (per rendere la visibilità gradevole)
		funzionePrintInizialeS(primo, secondo);
		while((entry= getutent())!=NULL){
			if(entry->ut_type== USER_PROCESS){
				//creazione della struct che rappresenta l'idle time dell'utente
				struct Idletime idle = idletime(entry);
				char date[80];
				char zona[100];
				//estrazione della data di accesso dell'utente
				dataS(entry, date, zona);
				//salvataggio del nome utente
				char nome[sizeof(entry->ut_user)+1];
				nome[sizeof(entry->ut_user)]='\0';
				strncpy(nome, entry->ut_user, sizeof(entry->ut_user));
				char nomeEsteso[100];
				char office_number[100];
				char office_phone[100];
				char home_phone[100];
				profile= getpwnam(nome);
				//estrazione degli altri dati, cioè il nome esteso, il numero di ufficio, il numero di telefono di ufficio e il numero di telefono di casa
				informazioniGecos(profile, office_number, nomeEsteso, office_phone, home_phone);
				//stampa degli utenti nella formattazione s
				funzionePrintS(nome, nomeEsteso, entry->ut_line, idle, date, entry->ut_host, office_number, office_phone, primo, secondo);
			}
		}
		return 1;
	}
	else{
		//caso in cui è presente negli input "-m", che implica il dover eliminare casi di input che non richiamano nome utente
		if (controllo_m==1){
			char stringhe[argc][40];
			char controlloEsistenzaElementi[100] = "False";
			/*eliminazione degli input non validi, con annessa stampa del messaggio di errore. Inoltre, è presente il calcolo di user e nomi estesi validi,
			in modo da aggiornare i valori primo e secondo.*/
			for (int i = 1; i < argc; i++){
				if ( strcmp(argv[i], "-s")==0 || strcmp(argv[i], "-m")==0 || strcmp(argv[i], "-p")==0 ){continue;}
				else if ((profile= getpwnam(argv[i]))==NULL){
					printf("finger: %s: no such user.\n", argv[i]);
					continue;
				}
				else {
					strcpy(controlloEsistenzaElementi, "True");
					if (primo < strlen(argv[i])){
						primo = strlen(argv[i]);
					}
					profile = getpwnam(argv[i]);
					char nomeEsteso[100];
					char gecos[100];
					strcpy(gecos, profile->pw_gecos);
					int i = 0;
					while (gecos[i] != '\0' && gecos!=NULL && gecos[i] != ','){
						nomeEsteso[i]=gecos[i];
						i++;
					}
					nomeEsteso[i]='\0';
					if (secondo < strlen(nomeEsteso)){secondo = strlen(nomeEsteso);}
				}
			}
			//ordinamento degli input, in modo che possano essere successivamente stampati in ordine alfabetico
			if (argc>2){qsort(argv+1, argc-1, sizeof(char*), ordinamento);}
			//se esistono utenti validi, allora inizia la fase di stampa
			if (strcmp(controlloEsistenzaElementi, "True")==0){
				//stampa della prima linea della formattazione s, secondo i valori primo e secondo rilevati prima (per rendere la visibilità gradevole)
				funzionePrintInizialeS(primo, secondo);
				int contatore_stringhe = 0;
				//controllo della presenza di nomi non accettabili o doppioni
				for (int i = 1; i < argc; i++){
					if ((profile= getpwnam(argv[i]))==NULL){continue;}
					int controlloDoppioni = 0;
					profile= getpwnam(argv[i]);
					char nome[80];
					strcpy(nome, profile->pw_name);
					for (int c = 0; c < argc; c++){
						if (strcmp(stringhe[c], nome)==0){
							controlloDoppioni = 1;
						}
					}
					//nel caso l'input non è nè inaccettabile nè doppione, viene salvato in un array di controllo
					if (controlloDoppioni==1){continue;}
					strcpy(stringhe[contatore_stringhe], nome);
					contatore_stringhe++;
					//stampa dell'utente valido
					FunzioneStampaS(profile, entry, nome, primo, secondo);
				}
			}
		}
		//caso in cui non è presente negli input "-m", che implica il dover controllare casi di input che non richiamano nome utente e nome esteso
		else{
			int controllo_esistono_utenti= 0;
			char stringhe[argc][40];
			/*eliminazione degli input non validi, con annessa stampa del messaggio di errore. Inoltre, è presente il calcolo di user e nomi estesi validi,
			in modo da aggiornare i valori primo e secondo.*/
			for (int i = 1; i < argc; i++){
				if ( strcmp(argv[i], "-s")==0 || strcmp(argv[i], "-p")==0){continue;}
				if ((profile= getpwnam(argv[i]))==NULL){
					int controllo_nome = 0;
					trova_nome(profile, argv, i, &controllo_nome, &controllo_esistono_utenti, &primo, &secondo, argc);
				}
				else{
					controllo_esistono_utenti=1;
					if (primo < strlen(argv[i])){
						primo = strlen(argv[i]);
					}
					profile = getpwnam(argv[i]);
					char nomeEsteso[100];
					char gecos[100];
					strcpy(gecos, profile->pw_gecos);
					int i = 0;
					while (gecos[i] != '\0' && gecos!=NULL && gecos[i] != ','){
						nomeEsteso[i]=gecos[i];
						i++;
					}
					nomeEsteso[i]='\0';
					if (secondo < strlen(nomeEsteso)){secondo = strlen(nomeEsteso);}
				}
			}
			//se esistono utenti validi, allora inizia la fase di stampa
			if (controllo_esistono_utenti==1){
				//stampa della prima linea della formattazione s, secondo i valori primo e secondo rilevati prima (per rendere la visibilità gradevole)
				funzionePrintInizialeS(primo, secondo);
				//ordinamento degli input, in modo che possano essere successivamente stampati in ordine alfabetico
				if (argc>2){qsort(argv+1, argc-1, sizeof(char*), ordinamento);}
				//controllo che l'input sia valido (anche caso di doppioni)
				int contatore_stringhe = 0;
				for (int i = 1; i < argc; i++){
					int controlloDoppioni = 0;
					if (strcmp(argv[i], "-s")==0 || strcmp(argv[i], "-p")==0){continue;}
					//controllo specifico nel caso in cui l'input è relativo al nome esteso di un utente, nel caso lo stampa
					if ((profile= getpwnam(argv[i]))==NULL){
						soluzione_s_caso_nome_esteso(argc , profile, argv, i, entry, &controlloDoppioni, &contatore_stringhe, stringhe, primo, secondo);
						continue;
					}
					profile = getpwnam(argv[i]);
					char nome[80];
					strcpy(nome, profile->pw_name);
					for (int c = 0; c < argc; c++){
						if (strcmp(stringhe[c], nome)==0){
							controlloDoppioni = 1;
						}
					}
					//se l'input è associato al nome utente, e non è un doppione, allora lo stampa
					if (controlloDoppioni==0){
						strcpy(stringhe[contatore_stringhe], nome);
						contatore_stringhe++;
						FunzioneStampaS(profile, entry, nome, primo, secondo);
					}
				}
			}
		}
	}
	return 1;
}


//funzione che gestisce il caso in cui la funzionen finger debba stampare a schermo secondo la formattazione a l
void caso_finger_l(int argc, char *argv[], int controllo_m, int controllo_p){
	struct utmp *entry;
	struct passwd *profile;
	//caso in cui è presente negli input "-m", che implica il dover eliminare casi di input che non richiamano nome utente
	if (controllo_m==1){
		char stringhe[argc][40];
		//eliminazione degli input non validi, con annessa stampa del messaggio di errore
		for (int i = 1; i < argc; i++){
			if ( strcmp(argv[i], "-s")==0 || strcmp(argv[i], "-m")==0 || strcmp(argv[i], "-p")==0 || strcmp(argv[i], "-l")==0){continue;}
			else if ((profile= getpwnam(argv[i]))==NULL){
				printf("finger: %s: no such user.\n", argv[i]);
				continue;
			}
		}
		int contatore_stringhe = 0;
		int controlloPrimaStampa= 1;
		//controllo della presenza di nomi non accettabili o doppioni
		for (int i = 1; i < argc; i++){
			if ((profile= getpwnam(argv[i]))==NULL){continue;}
			int controlloDoppioni = 0;
			profile= getpwnam(argv[i]);
			char nome[80];
			strcpy(nome, profile->pw_name);
			for (int c = 0; c < argc; c++){
				if (strcmp(stringhe[c], nome)==0){
					controlloDoppioni = 1;
				}
			}
			//nel caso l'input non è nè inaccettabile nè doppione, viene salvato in un array di controllo
			if (controlloDoppioni==1){continue;}
			strcpy(stringhe[contatore_stringhe], nome);
			contatore_stringhe++;
			//stampa dell'utente valido
			FunzioneStampaL(profile, entry, nome, &controllo_m, &controllo_p, &controlloPrimaStampa);
		}
	}
	//caso in cui non è presente negli input "-m", che implica il dover controllare casi di input che non richiamano nome utente e nome esteso
	else{
		int controllo_esistono_utenti= 0;
		char stringhe[argc][40];
		//eliminazione degli input non validi, con annessa stampa del messaggio di errore
		for (int i = 1; i < argc; i++){
			if (strcmp(argv[i], "-s")==0 || strcmp(argv[i], "-p")==0 || strcmp(argv[i], "-l")==0){continue;}
			if ((profile= getpwnam(argv[i]))==NULL){
				int controllo_nome = 0;
				trova_nomeL(profile, argv, i, &controllo_nome, &controllo_esistono_utenti, argc);
			}
			else{
				controllo_esistono_utenti=1;
			}
		}
		//se esistono utenti validi, allora inizia la fase di stampa
		if (controllo_esistono_utenti==1){
			//ordinamento degli input, in modo che possano essere successivamente stampati in ordine alfabetico
			if (argc>2){qsort(argv+1, argc-1, sizeof(char*), ordinamento);}
			//controllo che l'input sia valido (anche caso di doppioni)
			int contatore_stringhe = 0;
			int controlloPrimaStampa= 1;
			for (int i = 1; i < argc; i++){
				int controlloDoppioni = 0;
				if (strcmp(argv[i], "-s")==0 || strcmp(argv[i], "-p")==0 || strcmp(argv[i], "-l")==0){continue;}
				//controllo specifico nel caso in cui l'input è relativo al nome esteso di un utente, nel caso lo stampa
				if ((profile= getpwnam(argv[i]))==NULL){
					soluzione_l_caso_nome_esteso(argc , profile, argv, i, entry, &controlloDoppioni, &contatore_stringhe, stringhe, &controllo_m, &controllo_p, &controlloPrimaStampa);
					continue;
				}
				profile = getpwnam(argv[i]);
				char nome[80];
				strcpy(nome, profile->pw_name);
				for (int c = 0; c < argc; c++){
					if (strcmp(stringhe[c], nome)==0){
						controlloDoppioni = 1;
					}
				}
				//se l'input è associato al nome utente, e non è un doppione, allora lo stampa
				if (controlloDoppioni==0){
					strcpy(stringhe[contatore_stringhe], nome);
					contatore_stringhe++;
					FunzioneStampaL(profile, entry, nome, &controllo_m, &controllo_p, &controlloPrimaStampa);
				}
			}
		}
	}
}

//il main si occupa dello smistamento degli input nella giusta funzione di elaborazione e stampa dei dati
int main(int argc, char* argv[]){
	//controllo della presenza negli input delle parole chiave "-l", "-s", "-m" e "-p"
	int controllo_l = 0;
	int controllo_s = 0;
	int controllo_m = 0;
	int controllo_p = 0;
	for (int i = 1; i < argc; i++){
		if (strcmp(argv[i], "-l")==0){
			controllo_l = 1;
		}
		else if (strcmp(argv[i], "-s")==0){
			controllo_s = 1;
		}
		else if (strcmp(argv[i], "-m")==0){
			controllo_m = 1;
		}
		else if (strcmp(argv[i], "-p")==0){
			controllo_p = 1;
		}
	//smistamento effettivo in base alle parole chiave in input
	}
	if (argc==1 || (controllo_l==0 && controllo_s==1)){
		caso_finger_s(argc, argv, controllo_m);
		return 1;
	}
	else{
		caso_finger_l(argc, argv, controllo_m, controllo_p);
		return 1;
	}
}