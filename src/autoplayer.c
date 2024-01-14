#include "../include/autoplayer.h"

double timer(double elapsed_time, WINDOW *chronoBox, struct timespec start_time, struct timespec current_time){
    clock_gettime(CLOCK_REALTIME, &current_time); // obtenir temps actuel

    // calculer le temps ecoule (en sec + nanoseconde)
    elapsed_time = (double)(current_time.tv_sec - start_time.tv_sec) +
                (double)(current_time.tv_nsec - start_time.tv_nsec) / 1.0e9;
    // affichage du temps
    mvwprintw(chronoBox, 1, 1, "chrono : %.1fs", elapsed_time);
    wrefresh(chronoBox);
    return elapsed_time;
}

void displayGameAuto (Carte *deck, Carte *compared1, Carte *compared2, int TAILLE){
    for (int i = 0; i < TAILLE_DECK; i++){
        if (&deck[i] != compared1 && (deck + i)->etat != 3 && &deck[i] != compared2)
        {
            (deck + i)->etat = 0;
            DisplayCard((deck + i), LONGUEUR, LARGEUR);
        }
        else if ((deck + i) == compared1 && (deck + i)->etat != 3)
        {
            compared1->etat = 2;
            DisplayCard(compared1, LONGUEUR, LARGEUR);
        }else if ((deck + i) == compared2 && (deck + i)->etat != 3)
        {
            compared2->etat = 2;
            DisplayCard(compared2, LONGUEUR, LARGEUR);
        }
        else if ((deck + i)->etat == 3)
        {
            (deck + i)->etat = 3;
            DisplayCard((deck + i), LONGUEUR, LARGEUR);
        }
    }
}

void autoplayer(int width){
    /* ------------------ Affichage du jeu ------------------ */

        // Initialisation de la fenetre
        clear();                // efface l'ecran
        curs_set(0);            // desactive le curseur
        cbreak();               // evite de d'attendre une nouvelle ligne pour getch()
        noecho();               // desactive l'echo automatique des caracteres entréees
        nodelay(stdscr, TRUE);  // eviter que getch() bloque la boucle
        keypad(stdscr, TRUE);   // pour les touches spécials (flèches)
        srand(time(NULL));

        // creation du deck avec les 12 cartes
        Carte *deck = CreationDeck();
        Carte *compared1 = NULL; // pointeur permettant de savoir quel carte est focus
        Carte *compared2 = NULL;          // pointeur qui sera initialisee seulement si une carte est selectionnee
        // la variable br permet de sortir de la boucle avec le q
        bool game = 1, br = 1; // permet de savoir la personne a trouvée toute les paires
        int count = 0;
        int nbAlea1 = 0, nbAlea2 = 0;

        WINDOW *titleBox, *chronoBox, *resultBox; // Initialisation des fenetres

        // Initialisation des variables
        int key;
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
        //tableau = subwin(stdscr, 10, 30, 0, 82);


        // création des boites
        box(titleBox, ACS_VLINE, ACS_HLINE);
        box(chronoBox, ACS_VLINE, ACS_HLINE);
        //box(tableau, ACS_VLINE, ACS_HLINE);

        // affichage du text
        mvwprintw(titleBox, 1, 1, "Jeu des paires");
        mvwprintw(titleBox, 2, 1, "Trouver les paires en un minimum de temps");
        
        // DisplayCard();
        while (game && br)
        {
            elapsed_time = timer(elapsed_time, chronoBox, start_time, current_time);
            //wrefresh(tableau);
            //DisplayCardPtr(deck, compared1, compared2, TAILLE_DECK);
            displayGameAuto(deck, compared1, compared2, TAILLE_DECK);
            
            do{
                elapsed_time = timer(elapsed_time, chronoBox, start_time, current_time);

                nbAlea1 = rand() % 13;
                compared1 = &deck[nbAlea1];
                key = getch();
                if(key == 'q'){
                    br = 0;
                }
            }while(compared1->etat != 0 || compared1 == compared2);

            //DisplayCard(compared1, LONGUEUR, LARGEUR);

            while (elapsed_time - chronoCompare <= 1 && elapsed_time != 0)
            {
                elapsed_time = timer(elapsed_time, chronoBox, start_time, current_time);
                
                key = getch();
                if(key == 'q'){
                    br = 0;
                }
            }

            do{
                elapsed_time = timer(elapsed_time, chronoBox, start_time, current_time);
                
                nbAlea2 = rand() % 13;
                compared2 = &deck[nbAlea2];
                key = getch();
                if(key == 'q'){
                    br = 0;
                }
            }while(compared2->etat != 0 || compared2 == compared1);

            //DisplayCard(compared2, LONGUEUR, LARGEUR);
            displayGameAuto(deck, compared1, compared2, TAILLE_DECK);
            /*mvwprintw(tableau, 1, 1, "1  : %c %d | 2  : %c %d | 3  : %c %d", deck[0].var, deck[0].etat, deck[1].var, deck[1].etat, deck[2].var, deck[2].etat);
            mvwprintw(tableau, 2, 1, "4  : %c %d | 5  : %c %d | 6  : %c %d", deck[3].var, deck[3].etat, deck[4].var, deck[4].etat, deck[5].var, deck[5].etat);
            mvwprintw(tableau, 3, 1, "7  : %c %d | 8  : %c %d | 9  : %c %d", deck[6].var, deck[6].etat, deck[7].var, deck[7].etat, deck[8].var, deck[8].etat);
            mvwprintw(tableau, 4, 1, "10 : %c %d | 11 : %c %d | 12 : %c %d", deck[9].var, deck[9].etat, deck[10].var, deck[10].etat, deck[11].var, deck[11].etat);
            wrefresh(tableau);*/

            chronoCompare = elapsed_time;

            while (elapsed_time - chronoCompare <= 2 && elapsed_time != 0)
            {
                elapsed_time = timer(elapsed_time, chronoBox, start_time, current_time);
                
                key = getch();
                if(key == 'q'){
                    br = 0;
                }
            }
            //mvwprintw(chronoBox, 2, 1, "%c %c %d", compared1->var, compared2->var, count);
            if(compared1->var == compared2->var){
                AttributsInit(compared1, 3);
                AttributsInit(compared2, 3);
                //DisplayCard(compared1, LONGUEUR, LARGEUR);
                //DisplayCard(compared2, LONGUEUR, LARGEUR);
                count++;
                displayGameAuto(deck, compared1, compared2, TAILLE_DECK);
                if(count==6){
                    game = 0;
                }
            }else{
                AttributsInit(compared1, 0);
                AttributsInit(compared2, 0);
            }
            //mvwprintw(chronoBox, 2, 1, "%c %c %d", compared1->var, compared2->var, count);
            //DisplayCard(compared1, LONGUEUR, LARGEUR);
            //DisplayCard(compared2, LONGUEUR, LARGEUR);
            //DisplayCardPtr(deck, compared1, compared2, TAILLE_DECK);
            displayGameAuto(deck, compared1, compared2, TAILLE_DECK);

            // refresh
            wrefresh(titleBox);
            wrefresh(chronoBox);
            //wrefresh(tableau);

            // recupère les inputs
            key = getch();

            if(key == 'q'){
                br = 0;
            }

            if (elapsed_time >= 120) // a envlever ?
                break;

            //usleep(70000); // arret de 70ms pour alléger le processeur
        }

        nodelay(stdscr, FALSE); // permettre de bloquer le prog tant que la touche 'q' n'est pas pressee

        // affichage des meilleurs scores
        resultBox = subwin(stdscr, 7, width, 23, 0);
        box(resultBox, ACS_VLINE, ACS_HLINE);

        // affichage des scores
        results(resultBox, elapsed_time, game);
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
}
