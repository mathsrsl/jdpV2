#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <stdbool.h>

#define TAILLE_NOM 6
#define TAILLE_SCORE 10

void filFile(FILE *fichier2, char nom1[TAILLE_NOM], char score1[TAILLE_SCORE], char nom2[TAILLE_NOM], char score2[TAILLE_SCORE],char nom3[TAILLE_NOM], char score3[TAILLE_SCORE]);
void filVoidFile(FILE * fichier2, char nom[TAILLE_NOM], char score[TAILLE_SCORE]);
void resultats(WINDOW * resultBox, float temps,bool game);

//fonction pour recrire dans un fichier
void WriteResults(float temps);

//affiche simplement les resultats a la fin du jeu,
//perdu ou gagné
void DisplayResults();


#endif // SCORE_H_INCLUDED
