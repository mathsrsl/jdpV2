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
    unsigned int etat; // 0 carte de base, 1 si focus simple, 2 si selectionnée et 3 si les paires sont trouvée
    char var;
    // positions des cartes sauvegardée dans la structure, comme ça plus besoin d'index courrant
    int X;
    int Y;
};

// Donne une valeur a tout les attributs d'une carte
void AttributsInit(Carte *c, unsigned int etat);
// Affiche les cartes avec leurs bordure
void DisplayCard(Carte *c, int longeur, int largeur);

// Affiche les cartes en fonction des pointeurs et de leur état, cette fonction appel DispalyCard
void DisplayCardPtr(Carte *deck, Carte *current_focus, Carte *compared, int TAILLE);
// donne la position x de la carte n (n étant entre 0 et 11)
int PosCardX(int numCard);
// pareil que PosCardX mais avec y
int PosCardY(int numCard);
// fonction permettant de donner une lettre aleatoire dans un tableau de lettre donne a un set de carte donne
void LettreAlea(Carte *deck, char lettre[], int taille);

// fonction permettant de prendre les inputs de deplacement, ne prend pas la commande q
// pour que l'on puisse sortir de la boucle sans avoir a retourner quoi que ce soit
void ManageInput(Carte *deck, Carte **compared, Carte **current_focus, bool *freezeInput, double *elapsed_time, double *chronoCompare, char input, bool *br);

// cette fonction va être utilisée seulement quand on deux cartes sont comparée, elle permet
// de renvoyer les modifications à faire sur les cartes et les variables utilisées
void CompareCard(Carte **current_focus, Carte **compared, bool *freezeInput, double *chronoCompare, int *count);

// Crée le deck de carte et renvoie un pointeur sur le deck, les cartes dans le deck ont leur attributs initialisé sauf WINDOW
Carte *CreationDeck();
// libère le deck créé (évite des problèmes de mémoire)
void LibereDeck(Carte *deck);
// prend le deck de carte et renvoie la carte accessible la plus proche
Carte *rechercheCarteAccessible(Carte *deck, Carte *debut, Carte *fin, Carte *current_focus, char direction, int TAILLE);

#endif // CARTE_H_INCLUDED