/**
 * Abonnenc Bonhoure
 * Mai 2016
 */

#ifndef DICO_H
#define DICO_H

#include <stdlib.h>
#include <stdio.h>

typedef struct Dico{
	char** Mots;
	int nbMots;
}Dico;

Dico lire_dico(char *nomfich);

void afficher_dico(Dico D);

// Récupérer un mot aléatoire
char* recuperer_mot(Dico D);

#endif
