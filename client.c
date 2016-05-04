/******************************************************************************/
/*			Application: ...					*/
/******************************************************************************/
/*									      */
/*			 programme  CLIENT				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs : Alicia ABONNENC		  */
/*				  Gilles BONHOURE		  */
/*									      */
/******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curses.h> 		/* Primitives de gestion d'�cran */
#include <sys/signal.h>
#include <sys/wait.h>

#include "fon.h"   		/* primitives de la boite a outils */

#define SERVICE_DEFAUT "1111"
#define PROTOCOLE_DEFAUT "tcp"
#define SERVEUR_DEFAUT "localhost"
#define BUFFER_LEN 800
#define MAXHOSTNAMELEN 255

void client_appli (char *serveur, char *service, char *protocole);


/*****************************************************************************/
/*--------------- programme client -----------------------*/

int main(int argc, char *argv[])
{

	char *serveur= SERVEUR_DEFAUT; /* serveur par defaut */
	char *service= SERVICE_DEFAUT; /* numero de service par defaut (no de port) */
	char *protocole= PROTOCOLE_DEFAUT; /* protocole par defaut */

	/* Permet de passer un nombre de parametre variable a l'executable */
	switch(argc)
	{
 	case 1 :		/* arguments par defaut */
		  printf("serveur par defaut: %s\n",serveur);
		  printf("service par defaut: %s\n",service);
		  printf("protocole par defaut: %s\n",protocole);
		  break;
  	case 2 :		/* serveur renseigne  */
		  serveur=argv[1];
		  printf("service par defaut: %s\n",service);
		  printf("protocole par defaut: %s\n",protocole);
		  break;
  	case 3 :		/* serveur, service renseignes */
		  serveur=argv[1];
		  service=argv[2];
		  printf("protocole par defaut: %s\n",protocole);
		  break;
  	case 4:			/* serveur, service, protocole renseignes */
		  serveur=argv[1];
		  service=argv[2];
		  protocole=argv[3];
		  break;
	default:
		  printf("Usage:client serveur(nom ou @IP)  service (nom ou port)  protocole (TCP ou UDP)\n");
		  return 1;
	}

	/* serveur est le nom (ou l'adresse IP) auquel le client va acceder */
	/* service le numero de port sur le serveur correspondant au  */
	/* service desire par le client */
	/* protocole le protocole qui sera utilise pour la communication */

	client_appli(serveur,service,protocole);

	return 0;
}

//Vérification de si le mot complet a été trouvé
bool motTrouve(char *mot, int lgMot) {
	int i = 0;
	while (i<lgMot && mot[i] != '_') { //Si tous les caractères sont différents de '_' c'est que le mot a été trouvé
		i+=1;
		if(i>=lgMot) return true;
	}
	return false;
}

//Vérification d'une fin de partie : mot trouvé ou nombre de coups = 0
bool finPartie(char *mot, int lgMot, int nbCoupsRestants) {
	if(nbCoupsRestants == 0) return true;
	return motTrouve(mot, lgMot);
}

void penduClient(int socket) {

	//Buffer de réception
	char buffer[800];
	//Choix de la difficulté
	char choice[1];
	int nbCoupsRestants = 20;

	printf("|===========================================|\n");
	//Affichage du début de la partie.
	h_reads(socket,buffer, BUFFER_LEN);
	printf("%s", buffer);

	//Choix de la diffulté
	h_reads(socket,buffer, BUFFER_LEN);
	printf("%s", buffer);

	int diff;
	//Choix de la difficulté, avec répétition si on se "trompe" de choix
	scanf("%d", &diff);
	while( diff <= 0 || diff > 3 ) {
		printf("La difficulté ne peut être comprise qu'entre 1 et 3. Choisir à nouveau : ");
		scanf("%d", &diff);
	}
	//Passage en char et envoie au serveur
	choice[0] = diff + '0';
	h_writes(socket, choice,1);
	printf("|===========================================|\n");

	//Ecriture "Début de partie"
	h_reads(socket, buffer, BUFFER_LEN);
	printf("%s", buffer);

	int lgMot;
	//récupération de la longueur du mots
	h_reads(socket, buffer, 15);
	lgMot = atoi(buffer);
	char *mot = malloc(sizeof(char) * lgMot);
	printf("|===========================================|\n");

	char lettre[2]; //Variable pour récupérer les lettres enregistrées par le client
	//début de boucle de jeu
	do {
		//Verification du nombre de coups restants auprès du serveur
		h_reads(socket, buffer, 15);
		nbCoupsRestants = atoi(buffer);
		printf("Nombre de coups : %d\n", nbCoupsRestants);

		//Récupération de l'état du mot
		h_reads(socket, mot, lgMot);
		printf("Etat du mot : %s\n", mot);
		if(finPartie(mot, lgMot, nbCoupsRestants)) break; //On quitte la partie si jamais les conditions sont remplies

		//Choix et envoie de la lettre
		printf("Saisir une lettre : ");
		scanf("%ls", lettre);
		h_writes(socket, &lettre[0], 1);
		printf("|===========================================|\n");

	} while(true); //tourne en boucle

	printf("|===========================================|\n");
	//Message de fin en fonction du l'événement qui l'a provoqué
	if(motTrouve(mot, lgMot)) {
			printf("Bravo ! Vous avez trouvé le mot qui était : %s\n", mot);
	}	else if(nbCoupsRestants == 0) {
		//fin de partie : perdu car toutes les tentatives ont échoué
		printf("Vous avez perdu car vous n'avez plus d'essai disponible.\n");
	} else {
		printf("Dommage, vous avez abandonné la partie :c\n");
	}


}

/*****************************************************************************/
void client_appli (char *serveur,char *service,char *protocole)
/* procedure correspondant au traitement du client de votre application */
{

  int soc_client;
  int mode;
	char hostname[MAXHOSTNAMELEN + 1];

	struct sockaddr_in *p_adr_serveur = malloc(sizeof(struct sockaddr_in));
	struct sockaddr_in *p_adr_local = malloc(sizeof(struct sockaddr_in));

	p_adr_serveur->sin_family = AF_INET;

  // Initialisation du mode
  if((protocole=="udp")||(protocole=="UDP")){
		mode = SOCK_DGRAM;
  } else {
		mode = SOCK_STREAM;
  }

	// Création du socket
  soc_client = h_socket(PF_INET,mode);

	gethostname(hostname,MAXHOSTNAMELEN);
	adr_socket(service,hostname,protocole, p_adr_local);

	while(1)
	{
		adr_socket(service,hostname,protocole, p_adr_serveur);//remplit p_adr_socket
		//CONNECT
		h_connect(soc_client,p_adr_serveur);
		//JOUER
		penduClient(soc_client);
		//CLOSE CLIENT
		h_close(soc_client);
		exit(0);
		break;
	}

 }
/*****************************************************************************/
