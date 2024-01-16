#include "../include/autoplayer.h"

void displayGameAuto (Carte *deck, Carte *compared1, Carte *compared2, int TAILLE){
    /*
    Fonction : displayGameAuto
    Param : 
        - deck : le jeu de carte
        - compared1, compared2 : les deux cartes sélectionnées et comparées
        - TAILLE : la taille du deck
    Traitement : Affiche le deck et les cartes comparées en fonction des états
    Retour :  Aucun
    */
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

void endGame (Carte *deck, float elapsed_time, bool game, int width){
    /*
    Fonction : endGame
    Param : 
        - deck : le jeu de carte
        - elapsed_time : le temps final du joueur
        - game : booléen pour savoir si la partie est remportée ou perdu
        - width : largeur de la fenêtre
    Traitement : Affiche les scores et libère la mémoire
    Retour :  Aucun
    */
    WINDOW *resultBox;
    
    nodelay(stdscr, FALSE); // permettre de bloquer le prog tant que la touche 'q' n'est pas pressee

    // affichage des meilleurs scores
    resultBox = subwin(stdscr, 7, width, 23, 0);
    box(resultBox, ACS_VLINE, ACS_HLINE);

    // affichage des scores
    results(resultBox, elapsed_time, game);
    wrefresh(resultBox);
    int key = ' ';
    while (key != 'q' && key != 'Q')
        key = getch();

    // si touche 'q' pressee : arret du jeu

    // liberation de la memoire
    delwin(resultBox);

    endwin();

    LibereDeck(deck);
    echo();
}

int compare(Carte *compared1, Carte *compared2, Carte *deck, int count){
    /*
    Fonction : compare
    Param : 
        - compared1, compared2 : les deux cartes sélectionnées et comparées
        - deck : le jeu de carte
        - count : compteur de paires trouvées
    Traitement : Compare les deux cartes sélectionnés et retourne le nb de 
                paires trouvées qui change en fct de si les deux cartes sélectionnées 
                sont les mêmes 
    Retour :  retourne le compteur de paire trouvée
    */
    if(compared1->var == compared2->var){
        AttributsInit(compared1, 3);
        AttributsInit(compared2, 3);
        displayGameAuto(deck, compared1, compared2, TAILLE_DECK);
        count++;
        return count;
    }else{
        AttributsInit(compared1, 0);
        AttributsInit(compared2, 0);
        return count;
    }
}

void autoplayer(int width){
    /*
    Fonction : autoplayer
    Param : 
        - width : largeur de la fenêtre
    Traitement : Fonction de l'autoplayer avec une boucle qui 
                joue tout seul au jeu de paires
    Retour :  Aucun
    */
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
        int count = 0, nbAlea1 = 0, nbAlea2 = 0;

        // Initialisation des variables
        int key;
        double chronoCompare = 0; // sera utile pour savoir si les cartes comparé doivent encore être affichée en mode comparaison
        struct timespec start_time;
        double elapsed_time = 0;

        // define color
        start_color();
        init_pair(3, COLOR_WHITE, COLOR_BLACK); // carte normal
        init_pair(4, COLOR_GREEN, COLOR_BLACK); // carte focus
        init_pair(5, COLOR_BLUE, COLOR_BLACK);  // carte comparée

        // obtenir le temps au début de l'exec
        clock_gettime(CLOCK_REALTIME, &start_time);
        AfficheBandeau(elapsed_time);
        
        // DisplayCard();
        while (game && br)
        {
            elapsed_time = CalcElapsed_Time(start_time);
            displayGameAuto(deck, compared1, compared2, TAILLE_DECK);
            
            do{
                elapsed_time = CalcElapsed_Time(start_time);

                nbAlea1 = rand() % (TAILLE_DECK+1);
                compared1 = &deck[nbAlea1];
                key = getch();
                if(key == 'q'){
                    br = 0;
                }
            }while(compared1->etat != 0 || compared1 == compared2);

            do{
                elapsed_time = CalcElapsed_Time(start_time);
                
                nbAlea2 = rand() % (TAILLE_DECK+1);
                compared2 = &deck[nbAlea2];
                key = getch();
                if(key == 'q'){
                    br = 0;
                }
            }while(compared2->etat != 0 || compared2 == compared1);

            displayGameAuto(deck, compared1, compared2, TAILLE_DECK);
            chronoCompare = elapsed_time;

            while (elapsed_time - chronoCompare <= 2 && elapsed_time != 0)
            {
                elapsed_time = CalcElapsed_Time(start_time);
                
                key = getch();
                if(key == 'q'){
                    br = 0;
                }
            }
            count = compare(compared1, compared2, deck, count);
            if(count==(TAILLE_DECK/2)){
                game = 0;
            }
            displayGameAuto(deck, compared1, compared2, TAILLE_DECK);
            compared1 = NULL;
            compared2 = NULL;

            // recupère les inputs
            key = getch();
            if(key == 'q'){
                br = 0;
            }
        }
    endGame (deck, (float)elapsed_time, game, width);
}
