#ifndef AUTOPLAYER_H_INCLUDED
#define AUTOPLAYER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include <time.h>

#include "carte.h"


void autoplayer(int width);

double timer(double elapsed_time, WINDOW *chronoBox, struct timespec start_time, struct timespec current_time);


#endif // AUTOPLAYER_H_INCLUDED
