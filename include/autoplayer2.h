#ifndef AUTOPLAYER2_H_INCLUDED
#define AUTOPLAYER2_H_INCLUDED

#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "carte.h"
#include "bandeau.h"

//Cette fonction permet de comparer les cartes pour l'auto player
int CompareCardAutoPlayer(Carte * deck,int indexCardA,int indexCardB,struct timespec start_time,double lastT);
//Cette fonction permet de savoir si une lettre est déjà connue en mode 1, permet de trouver le prochain endroit libre en mode 0
int SearchLetter(char letterFound[],int index_current,bool mode);
//Cette fonction contient tout pour l'autop
void AutoPlayer2();

#endif //AUTOPLAYER2_H_INCLUDED