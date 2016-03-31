/******************************************************************************/
/*			Application: ....			              */
/******************************************************************************/
/*									      */
/*			 programme  SERVEUR 				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs :  ....						      */
/*		Date :  ....						      */
/*									      */
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

	int pid =1;
	int soc_parent, soc_enfant;
	struct sockadrr_in *p_adr_client; //identité du client

	int mode; //Mode du serveur
	if(protocole == PROTOCOLE_DEFAUT) mode =  SOCK_STREAM;
	else mode =  SOCK_DGRAM;

	soc_parent = h_socket(AF_INET, mode);

	printf("%d", soc_parent);

	//Tant que le processus courrant est la fonction principal, alors le serveur attend une demande de connexion
	while(1) {
		soc_enfant = h_accept (soc_parent, p_adr_client);
		pid = fork();
		if(pid == 0) { break; }
	}

	printf("PID : %d, NUM_SOC : %d FIN DE FILS", pid, soc_enfant);

}

/******************************************************************************/
