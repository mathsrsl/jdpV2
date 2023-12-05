#ifndef CARTE_H_INCLUDED
#define CARTE_H_INCLUDED
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define LONGUEUR 7
#define LARGEUR 11
#define TAILLE_DECK 12

typedef struct Carte Carte;
struct Carte
{
    WINDOW *carte;
    unsigned int etat;    //0 si rien, 1 si focus simple, 2 si selectionne et 3 si les paires sont trouvee
    char  var;
};
//deux fonction differente, une pour l'initialisation de la carte attributs
//l'autre pour le display (check les attributs pour des displays differents


//Donne une valeur a tout les attributs d'une carte
void AttributsInit(Carte * c,unsigned int etat);
//Affiche les cartes avec leurs bordure
void DisplayCard(Carte * c,int longeur,int largeur,int numCard);

void AttributsPointeur(Carte * deck,Carte * current_focus,Carte * compared,int TAILLE);
//donne la position x de la carte n (n etant entre 0 et 11)
int PosCardX(int numCard);
//pareil mais avec y
int PosCardY(int numCard);
//fonction permettant de donner une lettre aleatoire dans un tableau de lettre donne a un set de carte donne
void LettreAlea(Carte * deck,char lettre[],int taille);

//Crée le deck de carte et renvoie un pointeur sur le deck
Carte * CreationDeck();
//libere le deck cree
void LibereDeck(Carte * deck);
//prend le deck de carte et renvoie la carte accessible la plus proche
Carte * rechercheCarteAccessible(Carte * deck,Carte * debut,Carte * fin,Carte * current_focus,char direction,int * current_index,int TAILLE);
#endif // CARTE_H_INCLUDED