#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <stdbool.h>

#define TAILLE_NOM 6
#define TAILLE_SCORE 10

//Fonction qui va remplir un fichier avec les scores
void filFile(FILE *fichier2, char nom1[TAILLE_NOM], char score1[TAILLE_SCORE], char nom2[TAILLE_NOM], char score2[TAILLE_SCORE],char nom3[TAILLE_NOM], char score3[TAILLE_SCORE]);
//Fonction qui va remplir un fichier vide avec juste le score du joueur
void filVoidFile(FILE * fichier2, char nom[TAILLE_NOM], char score[TAILLE_SCORE]);
//Fonction qui va gérer les résultats
int results(WINDOW * resultBox, float temps, bool game,int mode);

//fonctions en fonction de game
//Fonction si la partie est remportée
int winResults(WINDOW * resultBox, float temps, bool game);
//Fonction si la partie est perdue (on a appuyé sur q pour quitter)
int looseResults(WINDOW * resultBox, bool game,int mode);

//affiche simplement les resultats a la fin du jeu,
//perdu ou gagné
void displayResults(WINDOW * resultBox, bool game, char nom1[TAILLE_NOM], char score1[TAILLE_SCORE], char nom2[TAILLE_NOM], char score2[TAILLE_SCORE],char nom3[TAILLE_NOM], char score3[TAILLE_SCORE],int mode);


#endif // SCORE_H_INCLUDED
