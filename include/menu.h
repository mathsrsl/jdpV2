#include <ncurses.h>

#ifndef JDPV2_MENU_H
#define JDPV2_MENU_H

// Fonction permettant de créer le menu et de récupérer le choix du joueur
int menu(int height, int width);
// Fonction permettant d'afficher la banniere du jeu dans une fenetre et aux coordonnées données
void DisplayBanner(WINDOW *mainContainer, int posx, int posy);
// Foncton affichant le menu pour séléctionner si l'on veut rejouer au jeu ou non
int Rejouer(int width,int height);

#endif //JDPV2_MENU_H