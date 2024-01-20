#ifndef AUTOPLAYER_H_INCLUDED
#define AUTOPLAYER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include <time.h>

#include "carte.h"

#define LONGUEUR 7
#define LARGEUR 11
#define TAILLE_DECK 12

//Fonction qui permet d'afficher le jeu en fct des cartes comparées
void displayGameAuto (Carte *deck, Carte *compared1, Carte *compared2, int TAILLE);
//Fonction qui va gérer la fin du jeu avec l'affichage des scores
void endGame (Carte *deck, float elapsed_time, bool game, int width);
//Fonction qui va comparer les valeurs de deux cartes selectionnées
int compare(Carte *compared1, Carte *compared2, Carte *deck, int count);
//Fonction du mode de jeu autoplayer
void autoplayer(int width);

#endif // AUTOPLAYER_H_INCLUDED
