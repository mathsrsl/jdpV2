#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> //pour usleep
#include <stdbool.h>

#include "menu.h"
#include "carte.h"
#include "score.h"

#include <ncurses.h>

int main(void) {

    initscr();

    //creation du deck avec les 12 cartes
    Carte * deck = CreationDeck();
    Carte * current_focus = &deck[0];   //pointeur permettant de savoir quel carte est focus
    Carte * compared = NULL;        //pointeur qui sera initialisee seulement si une carte est selectionnee
    int current_index = 0;          //permet de savoir ou on se trouve dans le deck
    bool game = 1;                  //permet de savoir la personne a trouvée toute les paires
    bool freezeInput = 1;           //permet de savoir si l'on doit bloquer tout deplacement
    int count = 0;

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
    curs_set(0);
    cbreak(); //evite de d'attendre une nouvelle ligne pour getch()
    noecho(); //desactive l'echo automatique des caracteres entréees
    nodelay(stdscr, TRUE); //eviter que getch() bloque la boucle
    keypad(stdscr, TRUE); // pour les touches spécials (flèches)

    WINDOW *titleBox, *chronoBox, *resultBox;

    int key;
    int cursorPos = 1;
    double chronoCompare = 0;   //sera utile pour savoir si les cartes comparé doivent encore être affichée en mode comparaison

    struct timespec start_time, current_time;
    double elapsed_time;

    //define color
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3,COLOR_WHITE,COLOR_BLACK);   //carte normal
    init_pair(4,COLOR_GREEN,COLOR_BLACK);   //carte focus
    init_pair(5,COLOR_BLUE,COLOR_BLACK);    //carte comparee

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
    while(game){
        if(count == 6)
            game = 0;
        //clear(); pas possible de faire ça
        clock_gettime(CLOCK_REALTIME, &current_time); // obtenir temps actuel
        
        // calculer le temps ecoule (en sec + nanoseconde)
        elapsed_time = (double)(current_time.tv_sec - start_time.tv_sec) +
                       (double)(current_time.tv_nsec - start_time.tv_nsec) / 1.0e9;
        // affichage du temps
        mvwprintw(chronoBox, 1, 1, "chrono : %.1fs", elapsed_time);
        mvwprintw(chronoBox, 2, 1, "KEY DETECT : %c", key);

        

        if(freezeInput)
        {
            //besoin de cette fonction ici pour que les cartes comparee ne changent pas de couleur
            //pendant les 2 secondes de delais
            AttributsPointeur(deck,current_focus,compared,TAILLE_DECK);
            if (key == 'a') {
                // cursorPos = (cursorPos>1) ? cursorPos-1 : 12;
                current_focus = rechercheCarteAccessible(deck,deck,(deck + TAILLE_DECK - 1),current_focus,'g',&current_index,TAILLE_DECK);
            } else if (key == 'z') {
                current_focus = rechercheCarteAccessible(deck,deck,(deck + TAILLE_DECK - 1),current_focus,'d',&current_index,TAILLE_DECK);
            }else if(key == 'e'){
                //a decouper en fonction
                if(compared == NULL && current_focus->etat != 3)    
                {
                    //on verifi si current_focus et bien egal a 1 pour eviter que les cartes revelee et bloquee soient debloquable
                    compared = current_focus;
                    current_focus = rechercheCarteAccessible(deck,deck,(deck + TAILLE_DECK - 1),current_focus,'d',&current_index,TAILLE_DECK);
                }else if(compared != NULL)
                {
                    //on bloque les inputs et on recupere le temps auquel tout a ete bloque pour regarder quand arreter
                    chronoCompare = elapsed_time;
                    freezeInput = 0;
                    //changer les cartes de couleur pour la comparaison
                    AttributsInit(current_focus,2);
                    DisplayCard(current_focus,LONGUEUR,LARGEUR,current_index);
                }
            }else if(key == 'q'){
                //quitter avant la fin du timer
                break;
            }
        }else if(elapsed_time - chronoCompare >= 2 && elapsed_time != 0)
        {
            chronoCompare = 0;
            freezeInput = 1;
            if(compared->var == current_focus->var)
            {
                //changes l'etat pour etre sur qu'a la prochaine iteration de la boucle elles puissent changer de forme
                AttributsInit(compared,3);
                AttributsInit(current_focus,3);
                compared = NULL;
                count++;
            }else
            {
                compared = NULL;
            }
            
        }

        //mvwprintw(stdscr,25,25,"%p, count : %d",current_focus,count);

        //refresh
        wrefresh(titleBox);
        wrefresh(chronoBox);
        
        key = getch();

        if(elapsed_time >= 120)//a envlever ?
            break;

        usleep(70000); // arret de 70ms pour alléger le processeur
    }

    nodelay(stdscr, FALSE); // permettre de bloquer le prog tant que la touche 'q' n'est pas pressee

    // affichage des meilleurs scores
    resultBox = subwin(stdscr, 7, width, 23, 0);
    box(resultBox, ACS_VLINE, ACS_HLINE);
    //mvwprintw(resultBox, 1, 1, "Meilleur chrono :");
    
    resultats(resultBox,(float)elapsed_time);
    wrefresh(resultBox);
    key = ' ';
    while(key != 'q' && key != 'Q')
        key = getch();
        

    // si touche 'q' pressee : arret du jeu
    


    delwin(titleBox);
    delwin(chronoBox);
    delwin(resultBox);

    endwin();

    LibereDeck(deck);

    printf("Au revoir !\n");

    return 0;
}