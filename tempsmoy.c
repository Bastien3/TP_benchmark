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
	float *tab, moyenne;
	int tube[2];
	pid_t pid_fils;

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
		pipe(tube);
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
							
							/*printf("Temps écoulé : %d microsecondes\n", (int)temps_ecoule);*/
					}
				}
				moyenne = (float)somme_temps / k;
				printf("%f\n", moyenne);
				printf("Tube du processus %d : lecture %d écriture %d\n", getpid(), tube[0], tube[1]);
				close(tube[0]);
				write(tube[1], &moyenne, sizeof(float));
				close(tube[1]);
				exit(0);
			default : 
				while((pid_fils = wait(&cr)) != -1) {
					printf("pid fils : %d\n", (int)pid_fils);
					printf("Tube du processus %d : lecture %d écriture %d\n", getpid(), tube[0], tube[1]);
					close(tube[1]);
					read(tube[0], &moyenne, sizeof(float));
					printf("%f\n", moyenne);
				}
		}
	}
	/*for (j = 0 ; j < n ; j++)
		printf("%f ", tab[j]);*/
	printf("\n");
	free(tab);
	return 0;
}
