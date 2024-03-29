#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <ncurses.h>

#include "../include/menu.h"


int menu(int height, int width)
{
    /*
    Fonction : menu
    Param : int height : hauteur de la fenetre utilisée
            int width : largeur de la fenetre utilisée
    Traitement : Affiche le menu du jeu et retourne le choix du joueur ou un code d'erreur
    Retour :  1 si le joueur a choisi de jouer seul
              2 si le joueur a choisi de jouer avec l'autoplayer
             -1 si le joueur a choisi de quitter
             -2 si creation de ressource ncurses echoue
    */


    // Initialisation de la fenetre
    initscr();
    WINDOW *mainContainer;

    if (mainContainer == NULL) { // Si creation de ressource echoue (gestion d'erreur)
        return -2;
    }

    // Initialisation des variables
    char choice[2];
    int mainContainerHeight;
    bool inputError = false;

    // Initialisation des couleurs
    start_color();
    init_pair(10, COLOR_WHITE, COLOR_RED); // Couleur pour les messages d'erreur

    mainContainer = subwin(stdscr, height, width, 0, 0); //déclare taille et position des boites
    box(mainContainer, ACS_VLINE, ACS_HLINE); //box(mainContainer, 0, 0); //création de la fenetre

    // Affichage de la banniere en ASCII art
    DisplayBanner(mainContainer, 3, 1);

    // Affichage du menu (Titre, choix et invite de saisie)
    mainContainerHeight = getmaxy(mainContainer); // Récupérer la hauteur de mainContainer

    wattron(mainContainer, A_BOLD);
    mvwprintw(mainContainer, mainContainerHeight/3+3, 5, "MENU");
    wattroff(mainContainer, A_BOLD);

    mvwprintw(mainContainer, mainContainerHeight/3+6, 5, "1. Un joueur");
    mvwprintw(mainContainer, mainContainerHeight/3+8, 5, "2. Autoplayer aleatoire");
    mvwprintw(mainContainer, mainContainerHeight/3+10, 5, "3. Autoplayer avec memoire");

    mvwprintw(mainContainer, mainContainerHeight/3+14, 5, "Entrez votre choix :");

    do{ // Répéter tant que le choix n'est pas valide
        wmove(mainContainer,  mainContainerHeight/3+14, 25); // Efface l'entree clavier
        wclrtoeol(mainContainer);                            // incorrecte (si erreur de saisie)


        if(inputError){ // Affichage du message d'erreur si erreur de saisie
            const char *errorText = "Erreur de saisie";

            // Calcul de la position du message d'erreur pour le centrer dans la fenetre
            int y = height / 2;
            int x = (width - strlen(errorText)) / 2;

            // Affichage du message d'erreur en gras, souligné et avec fond rouge
            wattron(mainContainer, A_BOLD | A_UNDERLINE | COLOR_PAIR(10));
            mvwprintw(mainContainer, y, x, "%s", errorText);
            wattroff(mainContainer, A_BOLD | A_UNDERLINE | COLOR_PAIR(10));

        }

        //entree du choix du joueur
        mvwgetnstr(mainContainer, mainContainerHeight/3+14, 27, choice, 1);

        switch (choice[0]) {
            case '1': // Pour jouer seul
                clear();
                wclear(mainContainer);
                endwin();
                return 1;
            case '2': // Pour jouer avec l'autoplayer
                endwin();
                return 2;
            case 'q': // Pour quitter le jeu
            case 'Q':
                endwin();
                return -1;
            case '3': // Pour jouer avec l'autoplayer2
                endwin();
                return 3;
            default: //Erreur de saisie
                inputError = true;
        }

        wrefresh(mainContainer); // Rafraichissement de la fenetre pour afficher le message d'erreur

    } while (choice[0] != '1' && choice[0] != '2'); // Tant que le choix n'est pas valide

    free(mainContainer); // Libération de la mémoire de la fenetre

    endwin(); // Fermeture de la fenetre
    return 0;
}

void DisplayBanner(WINDOW *container, int posx, int posy)
{
    /*
    Fonction : DisplayBanner
    Param : WINDOW *mainContainer : fenetre dans laquelle afficher la banniere
            int posx : position horizontale de départ de la banniere dans la fenetre
            int posy : position verticale de départ de la banniere dans la fenetre
    Traitement : Affiche une banniere 'JEU DE PAIREs' ecrite en ascii art dans la fenetre et aux positions données
    Retour : void
    */

    // Initialisation des couleurs
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);

    // Stockage du texte de la banniere dans un tableau de chaine de caractere
    const char *banner_text[] = {
            " \\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\ ",
            "      _  _____ _       ____  _____ ____    ____  ____  _  ____  _____ ____",
            "     / |/  __// \\ /\\  /  _ \\/  __// ___\\  /  __\\/  _ \\/ \\/  __\\/  __// ___\\",
            "     | ||  \\  | | ||  | | \\||  \\  |    \\  |  \\/|| / \\|| ||  \\/||  \\  |    \\",
            "  /\\_| ||  /_ | \\_/|  | |_/||  /_ \\___ |  |  __/| |-||| ||    /|  /_ \\___ |",
            "  \\____/\\____\\\\____/  \\____/\\____\\\\____/  \\_/   \\_/ \\|\\_/\\_/\\_\\\\____\\\\____/",
            " \\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\ "
    };

    // Affichage de la banniere dans la fenetre a l'aide d'une boucle
    for (int i = 0; i < 7; i++) {
        if(i == 0 || i == 6){ // Si premiere ou derniere ligne : affichage sans couleur ni gras
            mvwprintw(container, posx+i, posy, "%s", banner_text[i]);
        } else { // Sinon : affichage avec couleurs et gras
            wattron(container, A_BOLD | COLOR_PAIR(i));
            mvwprintw(container, posx+i, posy, "%s", banner_text[i]);
            wattroff(container, A_BOLD | COLOR_PAIR(i));
        }
    }

    return;
}

int Rejouer(int width,int height)
{
    /**
     * Fonction : Rejouer
     * Param : 
     *      - width : permet d'avoir la largeur du jeu
     *      - height : permet d'avoir la hauteur du jeu
     * Traitement : Cette fonction marche comme le menu de base, elle sera seulement affichée après une partie et demandera au joueur
     *              si il veut refaire une partie dans ce mode de jeu ou si il veut changer de mode de jeu (donc revenir au menu) ou
     *              quitter le jeu.
     * Retour :
     *      - 0 : Pour quitter le jeu
     *      - 1 : Pour retourner au menu
     *      - 2 : Pour relancer le même mode de jeu
    */

    init_pair(11,COLOR_BLACK,COLOR_WHITE);
    init_pair(12,COLOR_WHITE,COLOR_BLACK);

    int choix = 1;
    bool inputError = 0;

    char input[2];
    
    WINDOW * choiceBox = NULL;
    WINDOW * CLEARD = NULL;

    CLEARD = subwin(stdscr,12,60,(int)(height/2)-6,(int)(width/2)-30);
    wbkgd(CLEARD,COLOR_PAIR(11));
    wclear(CLEARD);
    
    
    choiceBox = subwin(stdscr,6,30,(int)(height/2)-2,(int)(width/2)-15);
    wbkgd(choiceBox,COLOR_PAIR(11));

    mvwprintw(CLEARD,1,12,"Voulez-vous rejouer a ce mode de jeu ?");

    do
    {
        wclear(choiceBox);
        //wborder(choiceBox,'|','|','-','-',' ',' ',' ',' ');
        wmove(choiceBox,5,14);
        wclrtoeol(choiceBox);

        mvwprintw(choiceBox,1,6,"r - recommencer");
        mvwprintw(choiceBox,2,6,"q - quitter le jeu");
        mvwprintw(choiceBox,3,6,"m - retourner au menu");

        if(inputError){ // Affichage du message d'erreur si erreur de saisie
            const char *errorText = "Erreur de saisie";

            // Affichage du message d'erreur en gras, souligné et avec fond rouge
            wattron(stdscr, A_BOLD | A_UNDERLINE | COLOR_PAIR(10));
            mvwprintw(stdscr,(int) (height/2)-3,(int) (width/2)-8, "%s", errorText);
            wattroff(stdscr, A_BOLD | A_UNDERLINE | COLOR_PAIR(10));
        }
        
        refresh();

        wattron(choiceBox,COLOR_PAIR(12));
        mvwgetnstr(choiceBox, 5, 15, input, 1);
        wattroff(choiceBox,COLOR_PAIR(12));


        //permet de prendre tout type d'input, minuscule ou majuscule
        if(input[0] >= 97 && input[0] <= 122)
            input[0] -= 32;

        switch(input[0])
        {
            case 'R':
                choix = 2;
                break;
            case 'M':
                choix = 1;
                break;
            case 'Q':
                choix = 0;
                break;
            default:
                inputError = 1;
                break;
        }
    }while(input[0] != 'R' && input[0] != 'M' && input[0] != 'Q');

    clear();
    refresh();

    delwin(choiceBox);
    delwin(CLEARD);

    return choix;
}
