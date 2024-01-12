#ifndef AUTOPLAYER2_H_INCLUDED
#define AUTOPLAYER2_H_INCLUDED

#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "carte.h"
#include "bandeau.h"

#define SAUT printf("\n");

int CompareCardAutoPlayer(Carte * deck,int indexCardA,int indexCardB,struct timespec start_time,double lastT);
int SearchLetter(char letterFound[],int index_current,bool mode);
void AutoPlayer2();

#endif //AUTOPLAYER2_H_INCLUDED