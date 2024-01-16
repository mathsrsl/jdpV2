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

void displayGameAuto (Carte *deck, Carte *compared1, Carte *compared2, int TAILLE);

void endGame (Carte *deck, float elapsed_time, bool game, int width);

int compare(Carte *compared1, Carte *compared2, Carte *deck, int count);

void autoplayer(int width);

#endif // AUTOPLAYER_H_INCLUDED
