#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int main (int argc, char *argv[]) {

	int k, n, i, j, mediane;
	int cr;
	char commande[50];
	struct timeval temps_debut, temps_fin;
	suseconds_t temps_ecoule;
	int somme_temps = 0;
	float *tab, moyenne, en_cours, m;
	int tube[2], tube2[2];
	pid_t pid_fils;

	if (argc != 4) {
		printf("%s : <nb  processus> <commande> <nb exec>.\n", argv[0]);
		exit(-1);
	}
	k = atoi(argv[1]);
	n = atoi(argv[3]);
	mediane = n / 2;
	sprintf(commande, "%s", argv[2]);
	tab = malloc(sizeof(float) * (unsigned int)n);

	for (i = 0 ; i < n ; i++) {

		/* Lancement des n processus fils */
		pipe(tube);
		switch(fork()) {

			case -1 : 
				perror("Problème dans le processus fils.\n");
				exit(-1);

			case 0 : 
				for (j = 0 ; j < k ; j++) {
					/* Lancement des k exécutions de la commande */
					pipe(tube2);
					switch (fork()) {
						case -1 : 
							perror("Problème dans le processus fils : ");
							exit(-1);
						case 0 : 
							/* Redirection de la partie lecture du tube vers l'entrée standard */
							/* Objectif : éviter l'affichage de la commande à l'écran */
							close(tube2[1]);
							close(0);
							dup(tube2[0]);
							close(tube2[0]);

							execlp(commande, argv[0], NULL);
							perror("Problème dans l'exécution de la commande : ");
							exit(-1);
						default : 
							/* Redirection de la partie écriture du tube vers la sortie standard */
							close(tube2[0]);
							close(1);
							dup(tube2[1]);
							close(tube2[1]);

							/* Calcul du temps de la commande */
							gettimeofday(&temps_debut, NULL);
							wait(&cr);
							gettimeofday(&temps_fin, NULL);
							temps_ecoule = temps_fin.tv_usec - temps_debut.tv_usec;
							somme_temps += (int)temps_ecoule;

							/* Cas où la commande s'est terminée anormalement */
							if (cr << 8 == 0 && cr >> 8 != 0) /* Code de retour de la commande != 0 */
								printf("Échec de la commande : retour %d.\n", cr >> 8);
							else if (cr << 8 != 0) /* Commande interrompue par un signal */
								printf("Commande interrompue par le signal %d.\n", cr << 8);
					}
				}

				moyenne = (float)somme_temps / (float)k;

				/* Écriture de la moyenne calculée par le ième fils dans le tube */
				close(tube[0]);
				write(tube[1], &moyenne, sizeof(float));
				close(tube[1]);
				exit(0);

			default : 
				while((pid_fils = wait(&cr)) != -1) {

					/* Récupération de la moyenne */
					close(tube[1]);
					read(tube[0], &moyenne, sizeof(float));
					tab[i] = moyenne;
				}
		}
	}
	
	/* Tri du tableau par insertion */
	for (i = 1 ; i < n ; i++) {
		en_cours = tab[i];
		for (j = i ; j > 0 && tab[j - 1] > en_cours ; j--)
			tab[j] = tab[j - 1];
		tab[j] = en_cours;
	}

	/* Calcul du temps moyen d'exécution par la médiane */
	if (n % 2 == 0)
		m = (tab[mediane - 1] + tab[mediane]) / 2;
	else
		m = tab[mediane];

	printf("\n");
	printf("Temps moyen : %.3f millisecondes.\n", m / 1000);
	free(tab);
	return 0;
}
