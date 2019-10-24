#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int main (int argc, char *argv[]) {

	int k, n, i, j;
	int cr;
	char commande[50];
	struct timeval temps_debut, temps_fin;
	suseconds_t temps_ecoule;
	int somme_temps = 0;
	float *tab;
	int tube[2];
	
	

	if (argc != 4) {
		printf("%s : <nb  processus> <commande> <nb exec>.\n", argv[0]);
		exit(-1);
	}
	k = atoi(argv[1]);
	n = atoi(argv[3]);
	sprintf(commande, "%s", argv[2]);
	printf("%d %s %d\n", n, commande, k);
	tab = malloc(sizeof(float) * n);

	for (i = 0 ; i < n ; i++) {
		/* Lancement des n processus fils */
		switch(fork()) {
			case -1 : 
				perror("Problème dans le processus fils");
				exit(-1);
			case 0 : 
				for (j = 0 ; j < k ; j++) {
					/* Lancement des k exécutions de la commande */
					switch (fork()) {
						case -1 : 
							perror("Problème dans le processus fils : ");
							exit(-1);
						case 0 : 
							execlp(commande, argv[0], NULL);
							perror("Problème dans l'exécution de la commande : ");
							exit(-1);
						default : 
							/* Calcul du temps de la commande */
							gettimeofday(&temps_debut, NULL);
							wait(&cr);
							gettimeofday(&temps_fin, NULL);
							temps_ecoule = temps_fin.tv_usec - temps_debut.tv_usec;
							somme_temps += (int)temps_ecoule;
							
							//printf("Temps écoulé : %d microsecondes\n", (int)temps_ecoule);
					}
				}
				//printf("Moyenne du temps écoulé : %f", (float)somme_temps / k);
				tab[i] = (float)somme_temps / k;
				for (j = 0 ; j < n ; j++)
					printf("%f ", tab[j]);
				printf("\n");
				exit(0);
			default : 
				while(wait(&cr) != -1);
				tab[i] = (float)somme_temps / k;
		}
	}
	for (j = 0 ; j < n ; j++)
		printf("%f ", tab[j]);
	printf("\n");
	free(tab);
	return 0;
}
