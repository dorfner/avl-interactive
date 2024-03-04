# Interactive AVL Tree

An interesting project I made based on a practical work I had to do in the second year of my Bachelor's degree for the subject "Data structures and Algorithmics II". This project revolves around implementing and interacting with an AVL tree, which is a self-balancing binary search tree.

## Overview

The AVL tree implementation provided here is a self-balancing binary search tree where the heights of the two child subtrees of any node differ by at most one. It's designed to provide efficient insertion, deletion, and search operations while automatically maintaining balance to ensure logarithmic time complexity for these operations.

## Features

- Interactive interface for inserting elements into the AVL tree
- Visualization of the tree structure using ASCII art
- Supports various tree operations such as finding the number of nodes and leaves, rotating nodes, and more

## Note

This project is kinda broken, I haven't touched it since I did it years ago and I would probably be able to easily correct it if I had time today. The Comments, documentation, and interface are in French.

## Usage

Compile the `main.c` file using any C compiler and run the executable. Follow the prompts to interact with the AVL tree.

```bash
gcc main.c -o avl
./avl
```

The help menu (in French):
```
  Commandes :
"a"/"aide" :	Affiche cette aide.
"insert <x>" :	Ajoute l'entier x au tas (deplace le noeud courant sur le noeud inséré).
"nbf" :		Affiche le nombre de feuilles du sous-arbre.
"nbn" :		Affiche le nombre de noeuds du sous-arbre.
"fg" :		Déplace le noeud courant sur son fils gauche.
"fd" :		Déplace le noeud courant sur son fils droit.
"pere" :	Déplace le noeud courant sur son fils père.
"racine" :	Place le noeud courant a la racine de l'arbre.
"q"/"quit" :	Termine le programme.
```
