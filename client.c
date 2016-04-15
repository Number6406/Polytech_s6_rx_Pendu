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
#include <curses.h> 		/* Primitives de gestion d'�cran */
#include <sys/signal.h>
#include <sys/wait.h>

#include "fon.h"   		/* primitives de la boite a outils */

#define SERVICE_DEFAUT "1111"
#define PROTOCOLE_DEFAUT "tcp"
#define SERVEUR_DEFAUT "localhost"

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

/*****************************************************************************/
void client_appli (char *serveur,char *service,char *protocole)
/* procedure correspondant au traitement du client de votre application */
{

	/*
	struct sockaddr_in adr_serveur;
	    adr_serveur.sin_family = AF_INET;

	    int idsocket = h_socket(AF_INET,SOCK_STREAM);

	    adr_socket(service, serveur, protocole, &adr_serveur);

	    h_connect(idsocket, &adr_serveur);
	*/

  int soc_client;
  int mode;

	struct sockaddr_in *p_adr_serveur = malloc(sizeof(struct sockaddr_in));

	p_adr_serveur->sin_family = AF_INET;

  // Initialisation du mode
  if((protocole=="udp")||(protocole=="UDP")){
		mode = SOCK_DGRAM;
  } else {
		mode = SOCK_STREAM;
  }

	// Cr�ation de la socket
  soc_client = h_socket(AF_INET,mode);
	adr_socket(service,serveur,protocole,p_adr_serveur);

	h_connect(soc_client, p_adr_serveur);

	char *tampon;
	int len;

	len = h_reads(soc_client, tampon, 800);

	printf("%s", tampon);

	len = h_writes(soc_client, tampon, 1);

 }

/*****************************************************************************/
