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

void autoplayer(int width);

void displayGameAuto (Carte *deck, Carte *compared1, Carte *compared2, int TAILLE);

double timer(double elapsed_time, WINDOW *chronoBox, struct timespec start_time, struct timespec current_time);


#endif // AUTOPLAYER_H_INCLUDED
