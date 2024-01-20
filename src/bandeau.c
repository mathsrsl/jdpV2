#include "../include/bandeau.h"

void AfficheBandeau(double elapsed_time)
{
    /**
     * Fonction : AfficheBandeau
     * Param : 
     *      - elapsed_time : cette varaible contient le temps écoulé en seconde et nanoseconde.
     * Traitement : Cette fonction prend le temps écoulé et l'affiche dans une boîte créée grâce à ncurses, elle affiche aussi la règle
     *              du jeu dans une autre boîte, ces deux boites seront affichée l'une à côté de l'autre.
     * Retour : Cette fonction ne renvoie pas de valeur.
    */
    //static permet de ne pas avoir a les redefinir a chaque appel
    static WINDOW * titleBox = NULL;
    static WINDOW * chronoBox = NULL;
    
    titleBox = subwin(stdscr, 4, 60, 0, 0);
    chronoBox = subwin(stdscr, 4, 19, 0, 61);
    
    box(titleBox, ACS_VLINE, ACS_HLINE);
    box(chronoBox, ACS_VLINE, ACS_HLINE);
    
    // affichage du text
    mvwprintw(titleBox, 1, 1, "Jeu des paires");
    mvwprintw(titleBox, 2, 1, "Trouver les paires en un minimum de temps");
    
    //L'appel de cette fonction affichera toujours le temps sur l'écran (donc pas besoin de le refaire)
    mvwprintw(chronoBox, 1, 1, "chrono : %.1fs", elapsed_time);

    wrefresh(titleBox);
    wrefresh(chronoBox);
}

double CalcElapsed_Time(struct timespec start_time)//ajouter la bordure dans les paramètres
{
    /**
     * Fonction : CalcElapsed_Time
     * Param :
     *      - start_time : une structure provenant de la librairie <time.h>, elle permet de stocker le début de l'execution du mode de jeu
     * Traitement : Cette fonction prend la structure start_time qui a pris les valeurs en seconde et nanoseconde le temps écoulé depuis le
     *              début du mode de jeu, elle créé une structure current_time qui prend comme valeur les secondes et nanosecondes
     *              au moment ou la fonction est executée, elle appelle ensuite la fonction AfficheBandeau pour afficher le temps écoulé qui
     *              est calculé et stocké dans la variable elapsed_time.
     * Retour : Cette fonction retourne elapsed_time, le temps écoulé depuis le début du mode de jeu.
    */
    double elapsed_time = 0.0;
    struct timespec current_time;

    clock_gettime(CLOCK_REALTIME,&current_time);
    elapsed_time = (double)(current_time.tv_sec - start_time.tv_sec) + (double)(current_time.tv_nsec - start_time.tv_nsec) / 1.0e9;

    AfficheBandeau(elapsed_time);
    
    return elapsed_time;
}
