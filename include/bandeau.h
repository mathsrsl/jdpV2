#ifndef BANDEAU_H_INCLUDED
#define BANDEAU_H_INCLUDED

#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void AfficheBandeau(double elapsedTime);
double CalcElapsed_Time(struct timespec start_time);

#endif