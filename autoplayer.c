#include "autoplayer.h"

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
        Carte *current_focus = &deck[0]; // pointeur permettant de savoir quel carte est focus
        Carte *compared = NULL;          // pointeur qui sera initialisee seulement si une carte est selectionnee
        // la variable br permet de sortir de la boucle avec le q
        bool game = 1, br = 1; // permet de savoir la personne a trouvée toute les paires
        bool freezeInput = 1;  // permet de savoir si l'on doit bloquer tout deplacement
        int count = 0;
        int nbAlea1 = 0, nbAlea2 = 0;

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
            elapsed_time = timer(elapsed_time, chronoBox, start_time, current_time);
            
            DisplayCard(deck, LONGUEUR, LARGEUR);

            do{
                elapsed_time = timer(elapsed_time, chronoBox, start_time, current_time);

                nbAlea1 = rand() % 13;
                current_focus = &deck[nbAlea1];
                key = getch();
                if(key == 'q'){
                    br = 0;
                }
            }while(current_focus->etat != 0);

            current_focus->etat = 1;
            DisplayCard(current_focus, LONGUEUR, LARGEUR);

            DisplayCardPtr(deck, current_focus, compared, TAILLE_DECK);

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
                compared = &deck[nbAlea2];
                key = getch();
                if(key == 'q'){
                    br = 0;
                }
            }while(compared->etat != 0);

            AttributsInit(current_focus, 2);
            AttributsInit(compared, 2);

            DisplayCard(current_focus, LONGUEUR, LARGEUR);
            DisplayCard(compared, LONGUEUR, LARGEUR);
            DisplayCardPtr(deck, current_focus, compared, TAILLE_DECK);
            chronoCompare = elapsed_time;

            while (elapsed_time - chronoCompare <= 2 && elapsed_time != 0)
            {
                elapsed_time = timer(elapsed_time, chronoBox, start_time, current_time);
                
                key = getch();
                if(key == 'q'){
                    br = 0;
                }
            }
            
            if(current_focus->var == compared->var){
                AttributsInit(current_focus, 3);
                AttributsInit(compared, 3);
                DisplayCard(current_focus, LONGUEUR, LARGEUR);
                DisplayCard(compared, LONGUEUR, LARGEUR);
                count++;
                if(count==6){
                    game = 0;
                }
            }else{
                AttributsInit(current_focus, 0);
                AttributsInit(compared, 0);
            }

            DisplayCard(current_focus, LONGUEUR, LARGEUR);
            DisplayCard(compared, LONGUEUR, LARGEUR);
            DisplayCardPtr(deck, current_focus, compared, TAILLE_DECK);

            // refresh
            wrefresh(titleBox);
            wrefresh(chronoBox);

            // recupère les inputs
            key = getch();

            if(key == 'q'){
                br = 0;
            }

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

        return 0;
}
