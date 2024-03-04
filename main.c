#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include "affiche_ab.c"

/**********************************************************************/
/* Affiche une représentation par un tableau d'un arbre arb. */
void printABtab(AVL arb, size_t n) {
  if (n >= N) return;
  if (n == 0) {
    printf("{ }\n");
    return;
  }

  int t[N];
  size_t extremite_droite = 0;
  arbToTab(arb, t);
  putchar('{');
  for (size_t i=0; i < n-1; i++) {
    if (t[i] != -1) printf(" \033[1m\033[36m%d\033[0m", t[i]);
    else printf(" -");
    putchar(',');
    if (i == extremite_droite) {
      putchar('\n');putchar(' ');
      extremite_droite = 2*extremite_droite + 2;
    }
  }
  if (t[n-1] != -1) printf(" \033[1m\033[36m%d\033[0m }", t[n-1]);
  else printf(" - }");
  putchar('\n');
}

/**
 * \brief Afficher une aide.
 */
void aide()
{
  printf("  Commandes :\n");
  printf("\"a\"/\"aide\" :\tAffiche cette aide.\n");
  printf("\"insert <x>\" :\tAjoute l'entier x au tas ");
  printf("(deplace le noeud courant sur le noeud inséré).\n");
  printf("\"nbf\" :\t\tAffiche le nombre de feuilles du sous-arbre.\n");
  printf("\"nbn\" :\t\tAffiche le nombre de noeuds du sous-arbre.\n");
  printf("\"fg\" :\t\tDéplace le noeud courant sur son fils gauche.\n");
  printf("\"fd\" :\t\tDéplace le noeud courant sur son fils droit.\n");
  printf("\"pere\" :\tDéplace le noeud courant sur son fils père.\n");
  printf("\"racine\" :\tPlace le noeud courant a la racine de l'arbre.\n");
  printf("\"q\"/\"quit\" :\tTermine le programme.\n");
}

void printExtended(AVL arb, AVL noeud) {
  printAB(arb, noeud);
  printf("Hauteur: %d\n", hauteur(noeud));
}

/**********************************************************************/
int main(int argc, char* argv[]) {
	if(argc > 1) {
		printf("Usage: %s\n", argv[0]);
		exit(EXIT_FAILURE);
	}

  AVL arb = NULL;
  // indice d'un noeud pour voir quel noeud nous sommes en train de traiter
	AVL noeudcourant = arb;
	char buffer[2048];

	printf("##############################################################\n");
	printf(" Démonstration interactive de l'implémentation d'un AVL.\n");
  printf("(il est conseillé d'utiliser rlwrap)\n\n");
  printf("(Ca marche pas vraiment)\n\n");
  printf("(Beaucoup d'erreurs et des segfault pour certaines rotations, ");
  printf("notemment les double rotations)\n\n");
  aide();
  putchar('\n');

	for(printf("\033[1m>>>\033[0m ");
			  fgets(buffer, 2047, stdin) != NULL; printf("\033[1m>>>\033[0m "))
  {
    const char* ligne;
    for(ligne = buffer; *ligne == ' '; ++ligne);

    /* Ligne vide. */
    if(*ligne == '\n' || *ligne == '/') continue;

		if (ligne[0] == 'a' || !strncmp(ligne, "aide", 4)) {
			/* "a"/"aide" : Affiche une aide.\n" */
      aide();
		}
    else if (!strncmp(ligne, "insert", 6))
    {
      /* "insert <x>" : ajoute l'entier x au tas */
      int x;
      if (sscanf(ligne, "insert %d", &x) != 1)
        printf("Argument manquant ou incorrect\n");
      else if (x < 0) printf("Erreur: la valeur d'un noeud doit etre supérieure à 0.\n");
      else {
        noeudcourant = insertion(arb, x);
        if (estvide(arb)) arb = noeudcourant;
        else while (!estvide(arb->pere)) {
          arb = arb->pere;
        }
        printExtended(arb, noeudcourant);
      }
    }
    else if (!strncmp(ligne, "nbf", 3))
    {
      /* "nbf" : affiche le nombre de feuilles du sous-arbre dont la racine est à noeudcourant */
      printf("Nombre de feuilles: %d\n", nbfeuilles(noeudcourant));
    }
		else if (!strncmp(ligne, "nbn", 3))
    {
      /* "nbf" : affiche le nombre du sous-arbre dont la racine est à noeudcourant */
      printf("Nombre de noeuds: %d\n", nbnoeuds(noeudcourant));
    }
    else if (!strncmp(ligne, "fg", 2)) {
      /* "fg" : déplace le noeud courant sur son fils gauche */
      if(noeudcourant == NULL) printf("L'arbre est vide.\n");
			else if(noeudcourant->filsGauche == NULL)
				printf("Le noeud de valeur %d n'a pas de fils gauche.\n", noeudcourant->val);
			else noeudcourant = noeudcourant->filsGauche;
			printExtended(arb, noeudcourant);
		}
    else if (!strncmp(ligne, "fd", 2)) {
      /* "fd" : déplace le noeud courant sur son fils droit */
      if(noeudcourant == NULL) printf("L'arbre est vide.\n");
			else if(noeudcourant->filsDroit == NULL)
				printf("Le noeud de valeur %d n'a pas de fils droit.\n", noeudcourant->val);
			else noeudcourant = noeudcourant->filsDroit;
			printExtended(arb, noeudcourant);
		}
    else if (!strncmp(ligne, "pere", 4)) {
      /* "fg" : déplace le noeud courant sur son père */
      if(noeudcourant == NULL) printf("L'arbre est vide.\n");
			else if(noeudcourant == arb)
				printf("Le noeud courant est à la racine.\n");
			else noeudcourant = noeudcourant->pere;
			printExtended(arb, noeudcourant);
		}
		else if (!strncmp(ligne, "racine", 6)) {
			/* "racine" : place le noeud courant a la racine de l'arbre */
			noeudcourant = arb;
			printExtended(arb, noeudcourant);
		}
		else if (!strncmp(ligne, "tb", 2)) {
			/* "tb" : afficher l'arbre sous forme de tableau */
      printABtab(arb, 63); // on affiche 2^6 - 1 noeuds
		}
    else if (ligne[0] == 'q' || !strncmp(ligne, "quit", 4))
      /* "quit" : termine le programme.*/
      break;
    else {
      printf("Commande incorrecte\n");
      printf("(\"a\" ou \"aide\" pour obtenir de l'aide)\n");
		}
		putchar('\n');
  }

	putchar('\n');
	exit(EXIT_SUCCESS);
}
