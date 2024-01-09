#include "../include/bandeau.h"

void AfficheBandeau(double elapsed_time)
{
    //static permet de ne pas avoir a les redefinir a chaque appel
    static WINDOW * titleBox = NULL;
    static WINDOW * chronoBox = NULL;
    if (titleBox == NULL && chronoBox == NULL)
    {
        titleBox = subwin(stdscr, 4, 60, 0, 0);
        chronoBox = subwin(stdscr, 4, 19, 0, 61);
        
        box(titleBox, ACS_VLINE, ACS_HLINE);
        box(chronoBox, ACS_VLINE, ACS_HLINE);
        
        // affichage du text
        mvwprintw(titleBox, 1, 1, "Jeu des paires");
        mvwprintw(titleBox, 2, 1, "Trouver les paires en un minimum de temps");
    }
    
    //L'appel de cette fonction affichera toujours le temps sur l'écran (donc pas besoin de le refaire)
    mvwprintw(chronoBox, 1, 1, "chrono : %.1fs", elapsed_time);

    wrefresh(titleBox);
    wrefresh(chronoBox);
}

double CalcElapsed_Time(struct timespec start_time)//ajouter la bordure dans les paramètres
{
    double elapsed_time = 0.0;
    struct timespec current_time;

    clock_gettime(CLOCK_REALTIME,&current_time);
    elapsed_time = (double)(current_time.tv_sec - start_time.tv_sec) + (double)(current_time.tv_nsec - start_time.tv_nsec) / 1.0e9;

    AfficheBandeau(elapsed_time);
    
    return elapsed_time;
}