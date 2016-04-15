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
#include <curses.h>

#include <sys/signal.h>
#include <sys/wait.h>

#include "fon.h"     		/* Primitives de la boite a outils */


#define SERVICE_DEFAUT "1111"
#define PROTOCOLE_DEFAUT "tcp"

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


/******************************************************************************/
void serveur_appli(char *service, char *protocole)

/* Procedure correspondant au traitemnt du serveur de votre application */

{

/*	struct sockaddr_in adr_serveur,adr_client;
	    adr_client.sin_family = AF_INET;

	    //creation de la socket
	    int idsocket = h_socket(AF_INET,SOCK_STREAM);

	    adr_socket(service, INADDR_ANY, protocole, &adr_serveur);
	    //association de la socket a son adresse
	    h_bind(idsocket, &adr_serveur);

	    //ecoute de la socket
	    h_listen(idsocket,50);

	    //acceptation de la socket (attente)
	    int id_socket_talk = h_accept(idsocket, &adr_client);
*/

	int pid =1;
	int nbMaxRq = 10;
	int soc_parent, soc_client; //sockets instanciées par le serveur et le client
	struct sockaddr_in *p_adr_serveur = malloc(sizeof(struct sockaddr_in));
	struct sockaddr_in *p_adr_client = malloc(sizeof(struct sockaddr_in)); //identité du client

	p_adr_serveur->sin_family = AF_INET;

	int mode; //Mode du serveur
	if(protocole == PROTOCOLE_DEFAUT) mode =  SOCK_STREAM;
	else mode =  SOCK_DGRAM;

	soc_parent = h_socket(AF_INET, mode);
	adr_socket(service, INADDR_ANY, protocole, p_adr_serveur);
	h_bind(soc_parent, p_adr_serveur);
	h_listen(soc_parent, nbMaxRq);

	//Tant que le processus courrant est la fonction principal, alors le serveur attend une demande de connexion
	while(1) {
		soc_client = h_accept(soc_parent, p_adr_client);
		pid = fork();
		if(pid == 0) { break; }
	}

	char *tampon = malloc(2000);
	int len;

	len = h_writes(soc_client, "Début de partie de pendu\n", 800);
	len = h_writes(soc_client, "Test frère\n", 800);

	printf("Choisir le niveau de difficulté : \n");
	printf("1- Facile : 20 essais\n");
	printf("2- Moyen : 15 essais\n");
	printf("3- Difficile : 10 essais\n");

	len = h_reads(soc_client, tampon, 1);

	printf("coucou");

}

/******************************************************************************/
