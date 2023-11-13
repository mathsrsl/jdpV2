#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> //pour usleep

#include "menu.h"
//#include "carte.h"

#include <ncurses.h>

int main(void) {

    initscr();

    int height = 30;
    int width = 80; //100
    int rows, cols;

    int menuChoice;

    //Erreur terminal trop petit
    getmaxyx(stdscr, rows, cols); //taille du terminal
    if(rows < height || cols < width){
        endwin();
        printf("Erreur, terminal trop petit\n");
        return -1;
    }

    //Afficher le menu du jeu
    menuChoice = menu(height, width);

    if(menuChoice == -1){ //Si touche q pressee
        endwin();
        printf("Au revoir\n");
        return 0;
    }

    /* ------------------ Affichage du jeu ------------------ */

    clear();
    cbreak(); //evite de d'attendre une nouvelle ligne pour getch()
    noecho(); //desactive l'echo automatique des caracteres entréees
    nodelay(stdscr, TRUE); //eviter que getch() bloque la boucle
    keypad(stdscr, TRUE); // pour les touches spécials (flèches)

    WINDOW *titleBox, *chronoBox, *resultBox;

    int key;
    int cursorPos = 1;

    struct timespec start_time, current_time;
    double elapsed_time;

    //define color
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    //obtenir le temps au début de l'exec
    clock_gettime(CLOCK_REALTIME, &start_time);


    //déclare taille et position des boites
    titleBox = subwin(stdscr, 4, 60, 0, 0);
    chronoBox = subwin(stdscr, 4, 19, 0, 61);

    //création des boites
    box(titleBox, ACS_VLINE, ACS_HLINE);
    box(chronoBox, ACS_VLINE, ACS_HLINE);

    //affichage du text
    mvwprintw(titleBox, 1, 1, "Jeu des paires");
    mvwprintw(titleBox, 2, 1, "Trouver les paires en un minimum de temps");

    //DisplayCard();

    while(key != 'q' && key != 'Q' && elapsed_time < 120){
        clock_gettime(CLOCK_REALTIME, &current_time); // obtenir temps actuel

        // calculer le temps ecoule (en sec + nanoseconde)
        elapsed_time = (double)(current_time.tv_sec - start_time.tv_sec) +
                       (double)(current_time.tv_nsec - start_time.tv_nsec) / 1.0e9;
        // affichage du temps
        mvwprintw(chronoBox, 1, 1, "chrono : %.1fs", elapsed_time);
        mvwprintw(chronoBox, 2, 1, "KEY DETECT : %c", key);

        if (key == 'a') {
            cursorPos = (cursorPos>1) ? cursorPos-1 : 12;
        } else if (key == 'z') {
            cursorPos = (cursorPos<12) ? cursorPos+1 : 1;
        }

        //refresh
        wrefresh(chronoBox);

        key = getch();

        usleep(100000); // arret de 100ms pour alléger le processeur
    }

    nodelay(stdscr, FALSE); // permettre de bloquer le prog tant que la touche 'q' n'est pas pressee

    // affichage des meilleurs scores
    resultBox = subwin(stdscr, 7, width, 23, 0);
    box(resultBox, ACS_VLINE, ACS_HLINE);
    mvwprintw(resultBox, 1, 1, "Meilleur chrono :");
    wrefresh(resultBox);

    // si touche 'q' pressee : arret du jeu
    key = ' ';
    while(key != 'q' && key != 'Q')
        key = getch();


    delwin(titleBox);
    delwin(chronoBox);
    delwin(resultBox);

    endwin();
    return 0;
}