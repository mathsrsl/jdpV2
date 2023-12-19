#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> //pour usleep
#include <stdbool.h>

#include "menu.h"
#include "carte.h"
#include "score.h"
#include "autoplayer.h"

#include <ncurses.h>

int main(void)
{
/* ------------------ Initialisation du jeu et affichage de menu ------------------ */
    initscr(); // Initialisation de la fenetre

    // Initialisation des variables
    int height = 30;
    int width = 80; // 100
    int rows, cols;

    int menuChoice;

    // Erreur terminal trop petit
    getmaxyx(stdscr, rows, cols); // taille du terminal
    if (rows < height || cols < width)
    {
        endwin();
        printf("Erreur, terminal trop petit\n");
        return -1;
    }

    // Afficher le menu du jeu
    menuChoice = menu(height, width);

    if (menuChoice == -2) {
        endwin();
        printf("Erreur, creation de ressource ncurses echoue\n");
        return -1;
    } else if (menuChoice == -1) { // Si touche q pressee
        endwin();
        printf("Au revoir\n");
        return 0;
    }else if(menuChoice == 1){

        /* ------------------ Affichage du jeu ------------------ */
    
        // Initialisation de la fenetre
        clear();                                    // efface l'ecran
        curs_set(0);                                // desactive le curseur
        cbreak();                                   // evite de d'attendre une nouvelle ligne pour getch()
        noecho();                                   // desactive l'echo automatique des caracteres entréees
        nodelay(stdscr, TRUE);                      // eviter que getch() bloque la boucle
        keypad(stdscr, TRUE);                       // pour les touches spécials (flèches)
    
        // creation du deck avec les 12 cartes
        Carte *deck = CreationDeck();
        Carte *current_focus = &deck[0];            // pointeur permettant de savoir quel carte est focus
        Carte *compared = NULL;                     // pointeur qui sera initialisee seulement si une carte est selectionnee

        bool game = 1, br = 1;                      // permet de savoir la personne a trouvée toute les paires, br permet de sortir de la boucle avec le la touche 'q'
        bool freezeInput = 1;                       // permet de savoir si l'on doit bloquer tout deplacement
        int count = 0;
    
        WINDOW *titleBox, *chronoBox, *resultBox; // Initialisation des fenetres
    
        // Initialisation des variables
        int key;
        int cursorPos = 1;
        double chronoCompare = 0; // sera utile pour savoir si les cartes comparé doivent encore être affichée en mode comparaison
    
        struct timespec start_time, current_time;
        double elapsed_time;
    
        // define color
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_BLACK); // carte normal
        init_pair(4, COLOR_GREEN, COLOR_BLACK); // carte focus
        init_pair(5, COLOR_BLUE, COLOR_BLACK);  // carte comparée
    
        // obtenir le temps au début de l'exec
        clock_gettime(CLOCK_REALTIME, &start_time);
    
        // déclare taille et position des boites
        titleBox = subwin(stdscr, 4, 60, 0, 0);
        chronoBox = subwin(stdscr, 4, 19, 0, 61);
    
        // création des boites
        box(titleBox, ACS_VLINE, ACS_HLINE);
        box(chronoBox, ACS_VLINE, ACS_HLINE);
    
        // affichage du text
        mvwprintw(titleBox, 1, 1, "Jeu des paires");
        mvwprintw(titleBox, 2, 1, "Trouver les paires en un minimum de temps");
    
        // DisplayCard();
        while (game && br)
        {
            if (count == 6)
                game = 0;
            // clear(); pas possible de faire ça
            clock_gettime(CLOCK_REALTIME, &current_time); // obtenir temps actuel
    
            // calculer le temps ecoule (en sec + nanoseconde)
            elapsed_time = (double)(current_time.tv_sec - start_time.tv_sec) +
                           (double)(current_time.tv_nsec - start_time.tv_nsec) / 1.0e9;
            // affichage du temps
            mvwprintw(chronoBox, 1, 1, "chrono : %.1fs", elapsed_time);
            mvwprintw(chronoBox, 2, 1, "KEY DETECT : %c", key);
    
            // savoir si il faut bloquer la récuperation d'input ou s'il faut continuer à les lires
            if (freezeInput)
            {
                // la fonction ne permet pas de se déplacer, l'index est trouvée mais le pointeur n'est pas modifé
                ManageInput(deck, &compared, &current_focus, &freezeInput, &elapsed_time, &chronoCompare, key, &br);
            }
            else if (elapsed_time - chronoCompare >= 2 && elapsed_time != 0)
            {
                CompareCard(&current_focus, &compared, &freezeInput, &chronoCompare, &count);
            }
    
            // mvwprintw(stdscr, 25, 25, "current focus : %p", current_focus);
    
            // refresh
            wrefresh(titleBox);
            wrefresh(chronoBox);
    
            // recupère les inputs
            key = getch();
    
            if (elapsed_time >= 120) // a envlever ?
                break;
    
            usleep(70000); // arret de 70ms pour alléger le processeur
        }
    
        nodelay(stdscr, FALSE); // permettre de bloquer le prog tant que la touche 'q' n'est pas pressee
    
        // affichage des meilleurs scores
        resultBox = subwin(stdscr, 7, width, 23, 0);
        box(resultBox, ACS_VLINE, ACS_HLINE);
    
        // affichage des scores
        results(resultBox, (float)elapsed_time, game);
        wrefresh(resultBox);
        key = ' ';
        while (key != 'q' && key != 'Q')
            key = getch();
    
        // si touche 'q' pressee : arret du jeu
    
        // liberation de la memoire
        delwin(titleBox);
        delwin(chronoBox);
        delwin(resultBox);
    
        endwin();
    
        LibereDeck(deck);
    
        printf("Au revoir !\n");

    }else if (menuChoice == 2){
        clear();
        curs_set(0);
        //autoplayer(width);
    }
    return 0;
}
