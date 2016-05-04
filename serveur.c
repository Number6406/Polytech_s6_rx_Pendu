/******************************************************************************/
/*			Application: socket Pendu serveur    																	*/
/******************************************************************************/
/*									      																										*/
/*			 programme  SERVEUR 				      																		*/
/*									    																										  */
/******************************************************************************/
/*									      																										*/
/*		Auteurs :  Abonnenc Alicia 			Bonhoure Gilles										      */
/*		Date :  Avril 2016			      																					*/
/*									      																										*/
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

#include <sys/signal.h>
#include <sys/wait.h>

#include "fon.h"     		/* Primitives de la boite a outils */


#define SERVICE_DEFAUT "1111"
#define PROTOCOLE_DEFAUT "tcp"
#define BUFFER_LEN 800
#define MAXHOSTNAMELEN 255

void serveur_appli (char *service, char* protocole);   /* programme serveur */


/******************************************************************************/
/*---------------- programme serveur ------------------------------*/

main(int argc,char *argv[])
{

	char *service= SERVICE_DEFAUT; /* numero de service par defaut */
	char *protocole= PROTOCOLE_DEFAUT; /* protocole par defaut */


	/* Permet de passer un nombre de parametre variable a l'executable */
	switch (argc)
 	{
   	case 1:
		  printf("defaut service = %s\n", service);
		  printf("defaut protocole = %s\n", protocole);
		  break;
		case 2:
		  service=argv[1];
		  printf("defaut protocole = %s\n", protocole);
		  break;
		case 3 :
		  service=argv[1];
		  protocole=argv[2];
		  break;

   	default :
		  printf("Usage:serveur service (nom ou port) protocole (TCP ou UDP)\n");
		  return 1;
 	}

	/* service est le service (ou num�ro de port) auquel sera affect�
	ce serveur*/
	/* protocole est le protocole qui sera utilis� */

	serveur_appli(service, protocole);
}

bool finPartie(char *etat, int nbCoupsRestants) {
	if(nbCoupsRestants == 0) return true;
	int lg = strlen(etat);
	int i = 0;

	while (i<lg && etat[i] != '_') i++;

	if(i>=lg) return true;
	else return false;
}

void initTable(char *tab) {
	int i=0;
	for(i; i<26; i++)
		tab[i] = '0';
}

void initEtat(char *etat, int lgMot) {
	int i;
	for(i=0; i<lgMot; i++) etat[i] = '_';
}

void majEtat(char *etat, char *mot, char lettre, int lgMot) {
	int i;
	printf("%d\n", lgMot);
	for(i=0; i<lgMot; i++) {
		if(mot[i] == lettre) etat[i] = lettre;
	}
}

void penduServeur(int socket) {

	char reponse;
	char *mot = "BIBOUP";
	char *etat;
	int nbCoupsRestants;
	int lgMot;
	char valeur_num;

	//Envoi du message de début de partie.
	h_writes(socket, "Bienvenue dans le jeu de pendu.\nVeuillez choisir le niveau de difficulté :\n1- Facile : 20 essais\n2- Moyen : 15 essais\n3- Difficile : 10 essais\n", BUFFER_LEN);

	//Réception de la réponse client
	h_writes(socket, "Faire son choix : ", BUFFER_LEN);
	h_reads(socket, &reponse, 1);
	valeur_num = reponse - '0';

	nbCoupsRestants = 25 - 5*valeur_num;
	printf("Coups restants : %d\n", nbCoupsRestants);

	h_writes(socket, "Sélection du mot...\nLa partie peut commencer :", BUFFER_LEN);

	lgMot = strlen(mot);
	etat = malloc(sizeof(char) * lgMot);
	initEtat(etat, lgMot);
	//On envoie la longueur du mot au client
	char lgMotS[15];
	sprintf(lgMotS, "%d", lgMot);
	h_writes(socket, lgMotS, 15);


	char *buffer;
	char tableauLettres[26];
	initTable(tableauLettres);
	int indiceLettre;
	char lettre;
	while(!finPartie(etat,nbCoupsRestants)) {

		char nbRest[15];
		sprintf(nbRest, "%d", nbCoupsRestants);
		h_writes(socket, nbRest, 15);

		//Envoie de l'état du mot courrant
		h_writes(socket, etat, lgMot);

		//récupération de la lettre tapée par le client
		h_reads(socket, &lettre, 1);
		lettre = toupper(lettre);
		indiceLettre = lettre - 'A';
		printf("Indice : %d\n", indiceLettre);

		if(tableauLettres[indiceLettre] == '0') {
			majEtat(etat, mot, lettre, lgMot);
			tableauLettres[indiceLettre] = '1';
			nbCoupsRestants--;
		}
		printf("Coups : %d\n", nbCoupsRestants);

	}
	//Nécessaire pour finir la partie
	char nbRest[15];
	sprintf(nbRest, "%d", nbCoupsRestants);
	h_writes(socket, nbRest, 15);
	h_writes(socket, etat, lgMot);

}

/******************************************************************************/
void serveur_appli(char *service, char *protocole)

/* Procedure correspondant au traitement du serveur de votre application */

{
	int pid =1;
	int nbMaxRq = 10;
	char hostname[MAXHOSTNAMELEN + 1];
	int soc_serveur, soc_client; //sockets instanciées par le serveur et le client
	struct sockaddr_in *p_adr_serveur = malloc(sizeof(struct sockaddr_in));
	struct sockaddr_in *p_adr_client = malloc(sizeof(struct sockaddr_in)); //identité du client

	p_adr_serveur->sin_family = AF_INET;

	int mode; //Mode du serveur
	if(protocole == PROTOCOLE_DEFAUT) mode = SOCK_STREAM;
	else mode = SOCK_DGRAM;

	//Ouverture d'un socket
	soc_serveur = h_socket(AF_INET, mode);

	//Récupération de l'adresse du serveur
	gethostname(hostname, MAXHOSTNAMELEN);
	adr_socket(service, hostname, protocole, p_adr_serveur);

	//Binder le socket en fonction de l'adresse du serveur
	h_bind(soc_serveur, p_adr_serveur);

	//Ecoute les connexions client
	h_listen(soc_serveur, nbMaxRq);

	//Tant que le processus courrant est la fonction principal, alors le serveur attend une demande de connexion
	while(1) {
		//Récupération du socket client
		soc_client = h_accept(soc_serveur, p_adr_client);
		pid = fork();
		if(pid==0)
		{
			srand(time(NULL));
			//Suppression du socket serveur
			h_close(soc_serveur);
			//Lancement du jeu
			penduServeur(soc_client);

			//Fermeture du socket client
			h_close(soc_client);

			exit(0);
		}
		else
		{
			//Fermeture du socket client
			h_close(soc_client);
		}
	}
	//Fermeture du socket serveur
	h_close(soc_serveur);
	exit(0);
}

/******************************************************************************/
