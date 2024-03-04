#include <stdlib.h>
#include <stdio.h>

#include "avl.c"

#define N 81920

/**** fonctions utiles             ************************************/
/**
* \brief Afficher n fois un caractère c.
*/
void putcharn(char c, int n) {
	for(int i=0; i < n; i++) putchar(c);
}

/**
* \brief Renvoie x à la puissance n.
*/
unsigned powerofx(int n, int x) {
	if(n == 0) return 1;
	else if(n < 0) return 0;
	return x*powerofx(n-1,x);
}

/**
* \fn			int digitI(int n, unsigned i)
* \brief	Renvoie le ieme chiffre d'un nombre
* \param 	n le nombre
* \param 	i l'indice du chiffre a renvoyer
* \prec		<em>i >= 0</em>
* (i=0 pour l'unité, i=1 pour le chiffre des dizaines...)
*/
int digitI(int n, unsigned i) {
	if(i==0) return n%10;
	else {
		return (n%(powerofx(i+1, 10)))/powerofx(i, 10);
	}
}

/**
* \brief	Renvoie le nb de chiffres dans une nombre n.
*/
int lennum(int n) {
	if(n < 10) return 1;
	else return lennum(n/10) +1;
}

/**
* \brief Renvoie 2 à la puissance n.
*/
unsigned powerof2(int n) {
	return powerofx(n, 2);
}

/* Pour la recurrence de arbToTab() */
void arbToTab2(AVL arb, int *tab, int i) {
	if (arb != NULL) {
		tab[i] = arb->val;
		arbToTab2(arb->filsGauche, tab, 2*i+1);
		arbToTab2(arb->filsDroit, tab, 2*i+2);
	}
}
/* tab sera une représentation par tas de l'arbre arb */
void arbToTab(AVL arb, int *tab) {
	for (int i = 0; i < N; i++) {
		tab[i] = -1;
	}
	arbToTab2(arb, tab, 0);
}

/* Retournes l'indice indice du noeud noeud de l'arbre arb */
int indiceNoeud(AVL arb, AVL noeud, unsigned indice) {
	if (arb == noeud) {
		return indice;
	}
	else if (arb != NULL) {
		int fg = indiceNoeud(arb->filsGauche, noeud, 2*indice+1);
		int fd = indiceNoeud(arb->filsDroit, noeud, 2*indice+2);
		if (fg != -1) return fg;
		else return fd;
	}
	else return -1;
}

int hauteur1(AVL arb) {
	if (estvide(arb)) return -1;
	else return max(hauteur1(arb->filsGauche),hauteur1(arb->filsDroit))+1;
}

/**** fonction printABP              ************************************/
/**
 * \fn		printAB(AVL arb, AVL noeud)
 * \brief	Affiche un arbre binaire dont le pointeur de la racine est passé en argument.
 * \param arb l'arbre à afficher
 * \param noeud le noeud à 'surligner'
 */
void printAB(AVL arb, AVL noeud) {
	int h = hauteur1(arb);
	int x;
	int _2pX;
	int _2pXm1;
	int _2pI;
	unsigned index = 0;  //l'indexe du prochain element de l'AB a afficher
	unsigned indexj;
	unsigned curseur;
	int j, k;  //des itérateurs
	int lenn;	//longueur en chiffres maximale des elts a afficher d'un 'niveau'

	if (h == -1) return;

	curseur = indiceNoeud(arb, noeud, 0);

	int t[N];
	arbToTab(arb, t);

	putchar('\n');
	/* On itère h fois sur chaque 'niveau' de l'arbre:
	 * (à la racine: i=0, au niveau avant les feuilles: i=h-1) */
	for(int i = 0; i < h; i++) {
		x = h-i;
		_2pX = powerof2(x);
		_2pXm1 = powerof2(x-1);
		_2pI = (int) powerof2(i);

		/* On vérifie le nb de lignes qu'on va devoir imprimer:
	   * (si des elts du niveau ont plus de 1 chiffre ils seront affichés
		 * à la verticale) */
		lenn = 1;
		for(j=0; j < _2pI; j++) {
			indexj = index +j;
			if(t[indexj] != -1 && lennum(t[indexj]) > lenn) lenn = lennum(t[indexj]);
		}
		/* On imprime les elts au niveau i: */
		for(j=lenn-1; j > 0; j--) {
			for(k=0; k < _2pI; k++) {
				putcharn(' ', _2pX-1);
				if (t[index] == -1) putchar(' ');
				else if (digitI(t[index], j) == 0 && t[index] < (int) powerofx(j, 10)) {
					putchar('|');
				}
				else if (index == curseur) {
					printf("\033[1m\033[35m%d\033[0m", digitI(t[index], j));
				}
				else printf("%d", digitI(t[index], j));
				putcharn(' ', _2pX);
				index++;
			}
			if(j != 0) index -= _2pI;
			putchar('\n');
		}
		/* j == 0 (on imprime la ligne des unités) */
		for(k=0; k < _2pI; k++) {
			putcharn(' ', _2pXm1+x-1);
			if ((2*index +1) < N && t[2*index +1] != -1) putcharn('_', _2pXm1-x);
			else putcharn(' ', _2pXm1-x);
			if (t[index] == -1) putchar(' ');
			else if (index == curseur) printf("\033[1m\033[35m%d\033[0m",
					digitI(t[index], j));
			else printf("%d", digitI(t[index], j));
			if ((2*index +2) < N && t[2*index +2] != -1) putcharn('_', _2pXm1-x);
			else putcharn(' ', _2pXm1-x);
			putcharn(' ', _2pXm1+x);
			index++;
		}
		putchar('\n');

		/* On imprime les branches sous la ligne des l'elts: */
		if (i == h-2) {	//à l'avant dernier niveau
			for (j=0; j < (int) powerof2(h-2); j++) {
				if (t[index] == -1) putcharn(' ', 4);
				else printf("  / ");
				index++;
				if (t[index] == -1) putcharn(' ', 4);
				else printf("\\   ");
				index++;
			}
			index -= powerof2(h-1);
			putchar('\n');
		}
		else if(i == h-1) {	//au dernier niveau
			for (j=0; j < (int) powerof2(h-1); j++) {
				if (t[index] == -1) putcharn(' ', 2);
				else printf("/ ");
				index++;
				if (t[index] == -1) putcharn(' ', 2);
				else printf("\\ ");
				index++;
			}
			index -= powerof2(h);
			putchar('\n');
		}
		else {	//dans les autres cas (i > h-2)
			for (k=2; k <= x; k++) {
				for (j=0; j < _2pI; j++) {
					//on imprime (2 puissance x-1 +x -j)espaces
					putcharn(' ', _2pXm1+x-k);
					if (t[index] == -1) putchar(' ');
					else if (index%2 == 1) putchar('/');
					index++;
					//on imprime (2 puissance x -2x +2j -3) espaces
					putcharn(' ', _2pX-2*x+2*k-3);
					if (t[index] == -1) putchar(' ');
					else if (index%2 == 0) putchar('\\');
					putcharn(' ', _2pXm1+x-k);
					putchar(' ');
					index++;
				}
				index -= _2pI*2;
				putchar('\n');
			}
		}
	}

	lenn = 1;
	for(j=0; j < (int) powerof2(h); j++) {
		indexj = index +j;
		if (t[indexj] != -1 && lennum(t[indexj]) > lenn) lenn = lennum(t[indexj]);
	}
	/* On imprime les elts au niveau i==h: */
	for(j=lenn-1; j >= 0; j--) {
		for(k=0; k < (int) powerof2(h); k++) {
			if (t[index] == -1) putchar(' ');
			else if (j != 0 && digitI(t[index], j) == 0
					&& t[index] < (int) powerofx(j, 10))
				putchar('|');
			else if (index == curseur)
				printf("\033[1m\033[35m%d\033[0m", digitI(t[index], j));
			else printf("%d", digitI(t[index], j));
			putchar(' ');
			index++;
		}
		index -= powerof2(h);
		putchar('\n');
	}
}
