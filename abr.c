#include <stdio.h>
#include <stdlib.h>

typedef struct noeud noeud;
typedef noeud* arbreBinaire;
struct noeud {
  unsigned val;
  arbreBinaire filsGauche;
  arbreBinaire filsDroit;
  arbreBinaire pere;
};

/**********************************************************************/

int max(int a, int b) {
  return a >= b ? a : b;
}

/**********************************************************************/

/**
* \fn			int hauteur(arbreBinaire arb)
* \brief	Renvoies la hauteur d'un arbre binaire de recherche.
* \param 	arb un arbre binaire de recherche
*/
int hauteur(arbreBinaire arb) {
	if (arb == NULL) return -1;
  else return max(hauteur(arb->filsGauche), hauteur(arb->filsDroit)) + 1;
}

/**
* \fn			unsigned nbnoeuds(arbreBinaire arb)
* \brief	Renvoies le nombre de noeuds d'un arbre binaire de recherche.
* \param 	arb un arbre binaire de recherche.
*/
unsigned nbnoeuds(arbreBinaire arb) {
  if (arb == NULL) return 0;
  else return nbnoeuds(arb->filsGauche) + 1 + nbnoeuds(arb->filsDroit);
}

/**
* \fn			unsigned nbfeuilles(arbreBinaire arb)
* \brief	Renvoies le nombre de feuilles d'un arbre binaire de recherche.
* \param 	arb un arbre binaire de recherche.
*/
unsigned nbfeuilles(arbreBinaire arb) {
  if (arb == NULL) return 0;
  else if (arb->filsGauche == NULL && arb->filsDroit == NULL) return 1;
  else return nbfeuilles(arb->filsGauche) + nbfeuilles(arb->filsDroit);
}

/**********************************************************************/

/**
* \fn			arbreBinaire nouveauNoeud(unsigned x)
* \brief	Renvoies un noeud créé.
* \param 	x la valeur du noeud.
*/
arbreBinaire nouveauNoeud(unsigned x) {
  arbreBinaire nvNoeud = malloc(3*sizeof(arbreBinaire) + sizeof(unsigned));
  nvNoeud->val = x;
  nvNoeud->filsGauche = NULL;
  nvNoeud->filsDroit = NULL;
  nvNoeud->pere = NULL;
  return nvNoeud;
}

/**
* \fn			arbreBinaire insertion(arbreBinaire arb, unsigned x)
* \brief	Insère un noeud de valeur x à un arbre binaire de recherche.
* \param 	arb un arbre binaire de recherche.
* \param 	x la valeur du noeud à insérer.
*/
arbreBinaire insertion(arbreBinaire arb, unsigned x) {
  if (arb == NULL) return nouveauNoeud(x);
  else if (x <= arb->val) {
    if (arb->filsGauche == NULL){
      arbreBinaire nvNoeud = nouveauNoeud(x);
      arb->filsGauche = nvNoeud;
      nvNoeud->pere = arb;
      return nvNoeud;
    }
    else insertion(arb->filsGauche, x);
  }
  else {
    if (arb->filsDroit == NULL){
      arbreBinaire nvNoeud = nouveauNoeud(x);
      arb->filsDroit = nvNoeud;
      nvNoeud->pere = arb;
      return nvNoeud;
    }
    else insertion(arb->filsDroit, x);
  }
  return NULL;
}

/**
* \fn			void destruction(arbreBinaire arb)
* \brief	Detruit en mémoire d'un arbre binaire de recherche.
* \param 	arb un arbre binaire de recherche.
*
* /!\ Ne pas utiliser arb après cet appel /!\
*/
void destruction(arbreBinaire arb) {
  if (arb != NULL) {
    destruction(arb->filsGauche);
    destruction(arb->filsDroit);
    free(arb);
  }
}
