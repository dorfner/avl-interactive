#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct noeud2 noeud2;
typedef noeud2* AVL;
struct noeud2 {
  unsigned val;
  AVL filsGauche;
  AVL filsDroit;
  AVL pere;
  int hauteur;
};

/**********************************************************************/
int max(int a, int b) {
  return a >= b ? a : b;
}

/**********************************************************************/
/**
* \fn			bool estvide(AVL arb)
* \brief	Renvoies vrai si un AVL placé en paramètre est vide, faux sinon.
* \param 	arb un AVL.
*/
bool estvide(AVL arb) { return arb == NULL; }

/**
* \fn			bool estfeuille(AVL noeud)
* \brief	Renvoies vrai si un noeud placé en paramètre est une feuille, faux sinon.
* \param 	noeud un AVL représentant un noeud.
*/
bool estfeuille(AVL noeud) {
  if (estvide(noeud)) return false;
  else if (estvide(noeud->filsGauche) && estvide(noeud->filsDroit)) return true;
  else return false;
}

bool estFilsGauche(AVL noeud) {
  return (noeud == (noeud->pere)->filsGauche);
}

/**
* \fn			int hauteur(AVL arb)
* \brief	Renvoies la hauteur d'un noeud d'un AVL.
* \param 	arb un arbre binaire de recherche équilibré
*/
int hauteur(AVL arb) {
	if (estvide(arb)) return -1;
  else return arb->hauteur;
}

/**
* \fn			unsigned nbnoeuds(AVL arb)
* \brief	Renvoies le nombre de noeuds d'un arbre binaire de recherche équilibré.
* \param 	arb un arbre binaire de recherche équilibré.
*/
unsigned nbnoeuds(AVL arb) {
  if (estvide(arb)) return 0;
  else return nbnoeuds(arb->filsGauche) + 1 + nbnoeuds(arb->filsDroit);
}

/**
* \fn			unsigned nbfeuilles(AVL arb)
* \brief	Renvoies le nombre de feuilles d'un arbre binaire de recherche équilibré.
* \param 	arb un arbre binaire de recherche équilibré.
*/
unsigned nbfeuilles(AVL arb) {
  if (estvide(arb)) return 0;
  else if (estfeuille(arb)) return 1;
  else return nbfeuilles(arb->filsGauche) + nbfeuilles(arb->filsDroit);
}

/**********************************************************************/
int desequilibre(AVL arb) {
  if (estvide(arb)) return 0;
  else return hauteur(arb->filsGauche) - hauteur(arb->filsDroit);
}

void rota_droite(AVL arb) {
  AVL fg = arb->filsGauche;
  if (estvide(fg)) return;

  // on change les pointeurs
  fg->pere = arb->pere;
  if (estFilsGauche(arb)) (arb->pere)->filsGauche = fg;
  else (arb->pere)->filsDroit = fg;
  if (!estvide(arb->filsGauche = fg->filsDroit))
    (arb->filsGauche)->pere = arb;
  fg->filsDroit = arb;
  arb->pere = fg;

  arb->hauteur = max(hauteur(arb->filsGauche), hauteur(arb->filsDroit)) + 1;
  fg->hauteur = max(hauteur(fg->filsGauche), hauteur(fg->filsDroit)) + 1;
}

void rota_gauche(AVL arb) {
  AVL fd = arb->filsDroit;
  if (estvide(fd)) return;

  // on change les pointeurs
  fd->pere = arb->pere;
  if (estFilsGauche(arb)) (arb->pere)->filsGauche = fd;
  else (arb->pere)->filsDroit = fd;
  if (!estvide(arb->filsDroit = fd->filsGauche))
    (arb->filsDroit)->pere = arb;
  fd->filsGauche = arb;
  arb->pere = fd;

  // on recalcule la hauteur
  arb->hauteur = max(hauteur(arb->filsGauche),hauteur(arb->filsDroit)) + 1;
  fd->hauteur = max(hauteur(fd->filsGauche),hauteur(fd->filsDroit)) + 1;
}

void rota_gauchedroite(AVL arb) {
  rota_gauche(arb->filsGauche);
  rota_droite(arb);
}

void rota_droitegauche(AVL arb) {
  rota_droite(arb->filsDroit);
  rota_gauche(arb);
}

/**
* \fn			AVL nouveauNoeud(unsigned x)
* \brief	Renvoies un noeud créé.
* \param 	x la valeur du noeud.
*/
AVL nouveauNoeud(unsigned x) {
  AVL nvNoeud = malloc(3*sizeof(AVL) + sizeof(unsigned) + sizeof(int));
  nvNoeud->val = x;
  nvNoeud->filsGauche = NULL;
  nvNoeud->filsDroit = NULL;
  nvNoeud->pere = NULL;
  nvNoeud->hauteur = 0;
  return nvNoeud;
}

/**
* \fn			void incrementer_hauteur(AVL noeud)
* \brief	Après avoir inséré un nouveau noeud dans un AVL, on incrémente
*  successivement la valeur de ses ascendants.
* \param 	noeud un AVL représentant un noeud.
*/
void remonter(AVL noeud) {
  AVL pere_noeud = noeud->pere;
  if (estvide(pere_noeud)) return;

  /* Rotations si arbre déséquilibré */
  if (desequilibre(pere_noeud) == 2) {
    if (desequilibre(noeud) == -1) rota_gauchedroite(pere_noeud);
    else rota_droite(pere_noeud);
  }
  else if (desequilibre(pere_noeud) == -2) {
    if (desequilibre(noeud) == 1) rota_droitegauche(pere_noeud);
    else rota_gauche(pere_noeud);
  }
  /* On incrémente peut etre la hauteur du pere */
  else if ( (estFilsGauche(noeud)
          && hauteur(noeud) > hauteur(pere_noeud->filsDroit))
        || (!estFilsGauche(noeud)
          && hauteur(noeud) > hauteur(pere_noeud->filsGauche)) ) {
    pere_noeud->hauteur = hauteur(pere_noeud) +1;
    remonter(pere_noeud);
  }
}

/**
* \fn			AVL insertion(AVL arb, unsigned x)
* \brief	Insère un noeud de valeur x à un arbre binaire de recherche équilibré.
* \param 	arb un arbre binaire de recherche équilibré.
* \param 	x la valeur du noeud à insérer.
*/
AVL insertion(AVL arb, unsigned x) {
  if (estvide(arb)) return nouveauNoeud(x);
  else if (x <= arb->val) {
    if (estvide(arb->filsGauche)) {
      AVL nvNoeud = nouveauNoeud(x);
      arb->filsGauche = nvNoeud;
      nvNoeud->pere = arb;
      remonter(nvNoeud);
      return nvNoeud;
    }
    else {
      insertion(arb->filsGauche, x);
    }
  }
  else {
    if (estvide(arb->filsDroit)) {
      AVL nvNoeud = nouveauNoeud(x);
      arb->filsDroit = nvNoeud;
      nvNoeud->pere = arb;
      remonter(nvNoeud);
      return nvNoeud;
    }
    else {
      insertion(arb->filsDroit, x);
    }
  }
  return NULL;
}

/**
* \fn			void destruction(AVL arb)
* \brief	Detruit en mémoire d'un arbre binaire de recherche équilibré.
* \param 	arb un arbre binaire de recherche équilibré.
*
* /!\ Ne pas utiliser arb après cet appel /!\
*/
void destruction(AVL arb) {
  if (!estvide(arb)) {
    destruction(arb->filsGauche);
    destruction(arb->filsDroit);
    free(arb);
  }
}
