#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> //pour usleep
#include <stdbool.h>

#include "../include/menu.h"
#include "../include/carte.h"
#include "../include/score.h"
#include "../include/autoplayer.h"
#include "../include/bandeau.h"
#include "../include/autoplayer2.h"

#include <ncurses.h>

int main(void)
{
/* ------------------ Initialisation du jeu et affichage de menu ------------------ */
    initscr(); // Initialisation de la fenetre

    // Initialisation des variables
    int height = 30;
    int width = 80; // 100
    int rows, cols;

    bool continuer = 1;

    int menuChoice;

    // Erreur terminal trop petit
    getmaxyx(stdscr, rows, cols); // taille du terminal
    if (rows < height || cols < width)
    {
        endwin();
        printf("Erreur, terminal trop petit\n");
        return -1;
    }


    while(continuer)
    {
        // Afficher le menu du jeu
        menuChoice = menu(height, width);

        if (menuChoice == -2) {
            endwin();
            printf("Erreur, creation de ressource ncurses echoue\n");
            return -2;
        } else if (menuChoice == -1) { // Si touche q pressee
            endwin();
            printf("Au revoir\n");
            return 0;
        }else if(menuChoice == 1){
            Jeu(width);
        }else if (menuChoice == 2){
            clear();
            curs_set(0);
            autoplayer(width);
        }else if (menuChoice == 3)
        {
            AutoPlayer2();
        }

        continuer = Rejouer();
    }
    
    endwin();
    printf("Au revoir !\n");

    return 0;
}
