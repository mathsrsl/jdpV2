#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <ncurses.h>

#include "menu.h"

int menu(int height, int width)
{
    /*
    Fonction : menu
    Param : int height, int width
    Traitement : Affiche le menu du jeu
    Retour :  1 si le joueur a choisi de jouer seul
              2 si le joueur a choisi de jouer avec l'autoplayer
             -1 si le joueur a choisi de quitter
    */


    // Initialisation de la fenetre
    initscr();
    WINDOW *mainContainer;

    // Initialisation des variables
    char choice[2];
    bool inputError = false;

    // Initialisation des couleurs
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(8, COLOR_WHITE, COLOR_RED);

    mainContainer = subwin(stdscr, height, width, 0, 0); //déclare taille et position des boites
    box(mainContainer, ACS_VLINE, ACS_HLINE); //box(mainContainer, 0, 0); //création de la fenetre

    //affichage de la banniere
    const char *banner_text[] = {
            "============================================================================",
            "  JJJJ  EEEEE  U   U     DDD   EEEEE  SSS     PPPP   AAAAA  III  RRRR   SSS",
            "    J    E     U   U     D  D  E     S        P   P  A   A   I   R   R  S",
            "    J    EEE   U   U     D   D EEE    SSS     PPPP   AAAAA   I   RRRR    SSS",
            "J   J    E     U   U     D  D  E         S    P      A   A   I   R  R      S",
            " JJJ     EEEEE   UUU      DDD   EEEEE  SSS    P      A   A  III  R   R  SSS",
            "============================================================================"
    };

    for (int i = 0; i < 7; i++) {
        if(i == 0 || i == 6){
            mvwprintw(mainContainer, 3+i, 2, "%s", banner_text[i]);
        } else {
            wattron(mainContainer, A_BOLD | COLOR_PAIR(i));
            mvwprintw(mainContainer, 3+i, 2, "%s", banner_text[i]);
            wattroff(mainContainer, A_BOLD | COLOR_PAIR(i));
        }
    }

    // Affichage du menu
    wattron(mainContainer, A_BOLD);
    mvwprintw(mainContainer, LINES/3+3, 5, "MENU");
    wattroff(mainContainer, A_BOLD);

    mvwprintw(mainContainer, LINES/3+6, 5, "1. Un joueur");
    mvwprintw(mainContainer, LINES/3+8, 5, "2. Autoplayer");

    mvwprintw(mainContainer, LINES/3+12, 5, "Entrez votre choix :");

    do{
        wmove(mainContainer,  LINES/3+12, 25); // Efface l'entree clavier incorrecte
        wclrtoeol(mainContainer);


        if(inputError){ // Message d'erreur en gras, souligné et en rouge
            const char *errorText = "Erreur de saisie";

            int y = height / 2;
            int x = (width - strlen(errorText)) / 2;

            wattron(mainContainer, A_BOLD | A_UNDERLINE | COLOR_PAIR(8));
            mvwprintw(mainContainer, y, x, "%s", errorText);
            wattroff(mainContainer, A_BOLD | A_UNDERLINE | COLOR_PAIR(8));

        }

        mvwgetnstr(mainContainer, LINES/3+12, 27, choice, 1); //entree du choix du joueur

        switch (choice[0]) {
            case '1':
                clear();
                wclear(mainContainer);
                endwin();
                return 1;
            case '2':
                endwin();
                return 2;
            case 'q':
            case 'Q':
                endwin();
                return -1;
            default: //Erreur de saisie
                inputError = true;
        }

        wrefresh(mainContainer);

    } while (choice[0] != '1' && choice[0] != '2');

    free(mainContainer); // Libération de la mémoire de la fenetre

    endwin();
    return 0;
}