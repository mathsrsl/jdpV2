#ifndef BANDEAU_H_INCLUDED
#define BANDEAU_H_INCLUDED

#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//fonction affichant les deux bandeaux : celui du temps écoulé et celui du texte
void AfficheBandeau(double elapsedTime);
//fonction retournant le temps écoulé depuis le début du jeu
double CalcElapsed_Time(struct timespec start_time);

#endif