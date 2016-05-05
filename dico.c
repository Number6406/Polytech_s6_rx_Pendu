/**
 * Abonnenc Bonhoure
 * Mai 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


typedef struct Dico{
	char** Mots;
	int nbMots;
} Dico;

void afficher_dico(Dico D){
	unsigned register int i;
	
	for(i=0;i<D.nbMots;i++){
		printf("%s\n",D.Mots[i]);
	}
}

Dico lire_dico(char *nomfich){
	register unsigned int i,j;
	Dico Res;
	int taille;
	FILE *f;
	
	f = fopen(nomfich,"r");
	
	if(f!=NULL){
		fscanf(f,"%d\n",&Res.nbMots); // La taille du dico est en début de fichier
		// On alloue le tableau
		Res.Mots = malloc(Res.nbMots*sizeof(char*));
		
		// on lit le mot char par char. avec leur taille en début de ligne
		for (i = 0; i < Res.nbMots; i++){ // Ligne
			fscanf(f,"%d ",&taille);
			// Allouer le mot
			Res.Mots[i]=malloc((taille+1)*sizeof(char));
			
			for(j=0;j<taille;j++){
				fscanf(f,"%c",&Res.Mots[i][j]);
			}
			fscanf(f,"\n");
			Res.Mots[i][j] = '\0';
		}
	}
	else{
		fprintf(stderr,"lire_dico : Fichier non ouvert\n");
		exit(1);
	}
	fclose(f);
	return Res;
}


char* recuperer_mot(Dico D){
	/**
	 * Renvoie un entier entre 0 et taille
	 */
	int rand_b(int taille){
		srand(time(NULL));
		return rand()%(taille);
	}
	
	int indice = rand_b(D.nbMots);
	return D.Mots[indice];
}

//~ int main(void){
	//~ Dico D;
	//~ D = lire_dico("dico1.txt");
	//~ afficher_dico(D);
	//~ 
	//~ printf("mot %s\n",recuperer_mot(D));
	//~ return 0;
//~ }


