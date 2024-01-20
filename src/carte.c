#include "../include/carte.h"

void AttributsInit(Carte *c, unsigned int etat)
{
    /**
     * Fonction : AttributsInit
     * Param :
     *      - c : la carte a modifier
     *      - etat : entier entre 0 et 3
     * Traitement : la fonction change l'etat de la carte qui lui est donnée en parametre.
     * Retour : aucune valeur de retour
     */
    c->etat = etat;
}

void DisplayCard(Carte *c, int longeur, int largeur)
{
    /**
     * Fonction : DisplayCard
     * Param :
     *      - c : la carte a afficher
     *      - longeur : la longeur de la carte
     *      - largeur : la largeur de la carte
     * Traitement : cette fonction prend en paramètre une carte, sa longeur et sa largeur
     *              elle affiche la carte en fonction de son état (état étant dans la
     *              structure carte), la longueur et largeur est un define dans le carte.h,
     *              mais cela peut-être modifié. Les cartes en fonction de leur état prendrons
     *              des couleurs ou non, un type de bordure précis ou non.
     * Retour : aucune valeur de retour
     */
    c->carte = subwin(stdscr, longeur, largeur, c->Y, c->X);
    wclear(c->carte);
    switch (c->etat)
    {
    case 0:
        wborder(c->carte, '|', '|', '-', '-', ' ', ' ', ' ', ' ');
        wbkgd(c->carte, COLOR_PAIR(3));
        //mvwprintw(c->carte,3,5,"%c",c->var);
        // mvwprintw(c->carte,4,5,"%d",c->etat);
        break;
    case 1:
        wborder(c->carte, '|', '|', '-', '-', ' ', ' ', ' ', ' ');
        wbkgd(c->carte, COLOR_PAIR(4));
        //mvwprintw(c->carte, 3, 5, "%c", c->var);
        break;
    case 2:
        wborder(c->carte, '|', '|', '-', '-', ' ', ' ', ' ', ' ');
        wbkgd(c->carte, COLOR_PAIR(5));
        mvwprintw(c->carte, 3, 5, "%c", c->var);
        break;
    case 3:
        wborder(c->carte, '|', '|', '-', '-', 'O', 'O', 'O', 'O');
        wbkgd(c->carte, COLOR_PAIR(3));
        mvwprintw(c->carte, 3, 5, "%c", c->var);
        break;
    }
    wrefresh(c->carte);
}

void DisplayCardPtr(Carte *deck, Carte *current_focus, Carte *compared, int TAILLE)
{
    /**
     * Fonction : DisplayCardPtr
     * Param :
     *      - deck : un pointeur de structure Carte qui va contenir toute les cartes du jeu
     *      - current_focus : un pointeur de structure Carte pointant vers une carte se trouvant dans le deck
     *                        sur laquelle l'utilisateur se trouve, l'état de cette carte
     *                        est 1.
     *      - compared : un pointeur de structure Carte qui peut pointer sur une carte du deck comme être égal
     *                   à NULL. Ce pointeur sert a comparer deux cartes entre elles.
     *      - TAILLE : un entier qui est égal au nombre de carte qui se trouvent dans le deck,
     *                 cet entier est un define dans le carte.h
     * Traitement : Cette fonction prend en paramètres toutes les informations qu'il lui faut
     *              pour afficher les cartes en fonction de leur pointeur. Pour traiter chaque
     *              carte dans le deck elle utilise une boucle for suivie de condition pour
     *              reconnaitre les cartes pointées qui doivent avoir un certains affichage
     *              des cartes qui ont été trouvée (et donc qui ne doivent pas changer
     *              d'affichage) et celle qui ne sont ni focus ni comparée.
     * Retour : aucune valeur de retour
     */
    for (int i = 0; i < TAILLE; i++)
    {
        if (&deck[i] != current_focus && (deck + i)->etat != 3 && &deck[i] != compared)
        {
            (deck + i)->etat = 0;
            DisplayCard((deck + i), LONGUEUR, LARGEUR);
        }
        else if ((deck + i) == current_focus && (deck + i)->etat != 3)
        { // la deuxième partie de la condition nous permet d'éviter qu'une carte trouvée
            // puisse être reséléctionnée
            current_focus->etat = 1;
            DisplayCard(current_focus, LONGUEUR, LARGEUR);
        }
        else if ((deck + i)->etat == 3)
        {
            (deck + i)->etat = 3;
            DisplayCard((deck + i), LONGUEUR, LARGEUR);
        }

        // besoin de celui la en dehors pour ne pas avoir de probleme ou il ne serait pas lu
        if (compared != NULL && (deck + i) == compared)
        {
            compared->etat = 2;
            DisplayCard(compared, LONGUEUR, LARGEUR);
        }
    }
}

int PosCardY(int numCard)
{
    /**
     * Fonction : PosCardY
     * Param :
     *      - numCard : la place de la carte dans le deck
     * Traitement : Cette fonction regarde si la place de la carte dans le deck représentée par
     *              numCard est plus grande ou égale à 5 pour attribuer à la carte son emplacement
     *              sur l'axe des ordonnées
     * Retour : Cette fonction renvoie un entier qui définie l'endroit ou sera afficher la carte
     *          sur l'axe des ordonnées.
     */
    if (numCard <= 5)
        return 5;
    else
        return 13;
}

int PosCardX(int numCard)
{
    /**
     * Fonction : PosCardX
     * Param :
     *      - numCard : la place de la carte dans le deck
     * Traitement : Cette fonction fait un modulo de la position de la carte dans le deck
     *              et regarde le resultat pour attribuer à la carte un emplacement sur
     *              l'axe des abscisses.
     * Retour : cette fonction renvoie un entier qui définie l'endroit ou sera afficher la
     *          carte sur l'axe des abscisses.
     */
    switch (numCard % 6)
    {
    case 0:
        return 6;
        break;
    case 1:
        return 17;
        break;
    case 2:
        return 28;
        break;
    case 3:
        return 39;
        break;
    case 4:
        return 50;
        break;
    case 5:
        return 61;
        break;
    }
    
    return 0;
}

void LettreAlea(Carte *deck, char lettre[], int taille)
{
    /**
     * Fonction : LettreAlea
     * Param :
     *      - deck : un pointeur de structure Carte contenant toutes les cartes du jeu.
     *      - lettre : un tableau de char contenant les lettres pour chaque carte.
     *      - taille : la taille du tableau de lettre qui est aussi la taille du tableau
     *                 de carte.
     * Traitement : Cette fonction prend le deck de carte, le tableau de lettres (avec les lettres)
     *              et enfin la taille du tableau de lettres. Elle va ensuite rentrer dans la boucle for
     *              et faire un pseudo aléatoire lui permettant de séléctionner une lettre par une lettre
     *              qui ne sera jamais la même (sauf si c'est la paire). Une fois la lettre séléctionnée
     *              elle va donner a la carte du deck (deck + i) la lettre qu'elle a trouvé et
     *              continuer ainsi et vu que le random de 0 et 1 est impossible elle va donc
     *              prendre les deux dernières lettres restantes et les donner aux deux dernières
     *              cartes restantes.
     * Retour : aucune valeur de retour.
     */

    srand(time(NULL)); // initialise la seed pour avoir des nombres aléatoires différents a chaque appel
    int index = 0;     // index permettant de prendre une lettre au hasard dans le array
    char temp;         // permet d'echanger la place des cartes

    for (int i = taille - 1; i >= 2; i--)
    {
        index = rand() % i; // on attribut une valeur aleatoire entre 0 et i
        // et on echange cette lettre avec la dernire lettre pour n'avoir que celle
        // non touchee au debut de l'array
        (deck + taille - i - 1)->var = *(lettre + index);
        temp = lettre[i];
        lettre[i] = lettre[index];
        lettre[index] = temp;
    }

    (deck + 10)->var = *lettre;
    (deck + 11)->var = *(lettre + 1);
}

int ManageInput(Carte *deck, Carte **compared, Carte **current_focus, struct timespec start_time, char input)
{
    /**
     * Fonction : ManageInput
     * Param :
     *      - deck : un pointeur de structure Carte contenant toutes les cartes du jeu
     *      - compared : un pointeur de pointeur de struture Carte contenant NULL ou la carte
     *                   à comparer.
     *      - current_focus : un pointeur de pointeur de structure Carte contenant la carte
     *                        où se trouve l'utilisateur dans le deck.
     *      - start_time : une structure contenant le temps en seconde et nanoseconde de quand le mode de jeu à été lancé.
     *      - input : permet de savoir quelle touche a été appuyé par l'utilisateur.
     * Traitement : La fonction va d'abord appeler la fonction DisplayCardPtr pour afficher les cartes en fonction des pointeurs
     *              et du deck, par la suite elle va regarder les inputs faits et déplacera le pointeur current_focus à droite ou 
     *              à gauche dans le deck si 'a' ou 'z' sont pressés. Si 'e' est pressé alors elle regarde si compared est égal à NULL
     *              pour savoir si une carte est comparée pour savoir si il faut comparer ces deux cartes ou si il faut simplement
     *              décaler current_focus et sélectionner la carte ou était current_focus avec compared.
     * Retour : 
     *      - 0 si aucun input est fait
     *      - valeur de count si une comparaison est faite 
     *      - -1 si le joueur veut quitter le jeu
     */
    // besoin de cette fonction ici pour que les cartes comparees ne changent pas de couleur
    // pendant les 2 secondes de delais
    DisplayCardPtr(deck, *current_focus, *compared, TAILLE_DECK);
    // mvwprintw(stdscr, 26, 25, "temp : %c", input);
    if (input == 'a' || input == 'A')
    {
        // cursorPos = (cursorPos>1) ? cursorPos-1 : 12;
        *current_focus = rechercheCarteAccessible(deck, deck, (deck + TAILLE_DECK - 1), *current_focus, 'g', TAILLE_DECK);
    }
    else if (input == 'z' || input == 'Z')
    {
        *current_focus = rechercheCarteAccessible(deck, deck, (deck + TAILLE_DECK - 1), *current_focus, 'd', TAILLE_DECK);
    }
    else if (input == 'e' || input == 'E')
    {
        // on regarde si le pointeur de comparaison est vide ou si l'état de la carte séléctionnée n'est pas 3 (donc problème)
        if (*compared == NULL && (*current_focus)->etat != 3)
        {
            // fonction avec comme paramètre deck compared current_focus return un float
            // on verifie si current_focus et bien egal a 1 pour eviter que les cartes revelees et bloquees soient debloquables
            *compared = *current_focus;
            *current_focus = rechercheCarteAccessible(deck, deck, (deck + TAILLE_DECK - 1), *current_focus, 'd', TAILLE_DECK);
        }
        else if (*compared != NULL)
        {
            AttributsInit(*current_focus, 2);
            DisplayCard(*current_focus, LONGUEUR, LARGEUR);
            //sinon comparaison
            int count = CompareCard(*current_focus,*compared,start_time,CalcElapsed_Time(start_time));
            *compared = NULL;
            return count;
        }
    }
    else if (input == 'q' || input == 'Q')
    {
        // quitter avant la fin du timer
        return -1;
    }

    return 0;
}

int CompareCard(Carte *current_focus, Carte *compared,struct timespec start_time, double chronoCompare)
{
    /**
     * Fonction : CompareCard
     * Param : 
     *      - current_focus : un pointeur de pointeur de structure Carte permettant de savoir ou l'on se trouve dans le deck
     *      - compared : un pointeur de pointeur de structure Carte permettant de savoir quelle carte séléctionnée pour être comparée
     *      - start_time : une structure contenant le temps en seconde et en nanoseconde de quand le jeu a été lancé.
     *      - chronoCompare : un double permettant de savoir quelle seconde nous devons retirer au temps actuel pour avoir le stop de 2 secondes.
     * Traitement : Cette fonction va d'abord récuperer le temps écoulé dans elapsed_time grâce à la fonction CalcElapsed_Time, grâce à cela elle
     *              va pouvoir faire le temps d'attente avant de valider ou non si les cartes sont des paires. Dans la boucle d'attente la fonction 
     *              peut être quittée si la touche q et appuyé.
     * Retour : 
     *      - 1 si les cartes sont des paires
     *      - 0 si les cartes ne sont pas des paires
     *      - -1 si le joueur veut quitter le jeu
    */

    double elapsed_time = CalcElapsed_Time(start_time);
    char ch;
    while(elapsed_time - chronoCompare < 2.0)
    {
        ch = getch();
        elapsed_time = CalcElapsed_Time(start_time);
        if(ch == 'q' || ch == 'Q')
            return -1;
            //-1 est le jeu se ferme
    }

    if (compared->var == current_focus->var)
    {
        // changes l'etat pour etre sur qu'a la prochaine iteration de la boucle elles puissent changer de forme
        AttributsInit(compared, 3);
        AttributsInit(current_focus, 3);
        return 1;
    }
    
    return 0;
}

Carte *CreationDeck()
{
    /**
     * Fonction : CreationDeck
     * Param : aucun paramètres
     * Traitement : Cette fonction commence par créer un tableau dynamiquement avec un calloc de taille TAILLE_DECK
     *              qui est un define dans le fichier carte.h (la taille étant de 12). Par la suite elle créé un tableau
     *              de lettres contenant les paires qui vont être utilisées pour donner à chaque carte du deck une lettre aléatoire grâce à
     *              la fonction LettreAlea() et enfin les positions X et Y grâce au fonction PosCardX et PosCardY des Cartes 
     *              et leur états sont initisalisés grâce à une boucle for.
     * Retour : la fonction retourne le pointeur deck pour pouvoir l'utiliser dans le main comme dans d'autre fichier.
    */
    // Creation du pointeur deck / tableau
    Carte *deck = (Carte *)calloc(TAILLE_DECK, sizeof(Carte));

    // initialisation de toute les cartes
    // creation du set de lettres utilisé pour les valeurs des cartes
    char lettre[TAILLE_DECK] = "AABBCCDDEEFF";

    if (deck != NULL)
    {
        LettreAlea(deck, lettre, TAILLE_DECK); // affecte au carte des valeurs char aleatoire
        for (int i = 0; i < TAILLE_DECK; i++)
        {
            // initialisation des attributs pour ne pas avoir de mauvaise suprise de valeur trop haute
            AttributsInit((deck + i), 0);
            (deck + i)->X = PosCardX(i);
            (deck + i)->Y = PosCardY(i);
        }
    }else{
        endwin();
        fprintf(stderr,"Erreur : la creation du tableau n'a pas pu etre faites.");
        exit(-4);
    }

    return deck;
}

void LibereDeck(Carte *deck)
{
    /**
     * Fonction : LibereDeck
     * Param : 
     *      - deck : un pointeur de structure Carte contenant toutes les cartes du jeu
     * Traitement : Cette fonction libère l'espace mémoire alloué pour le deck de cartes
     * Retour : aucune valeur de retour.
    */
    free(deck);
}

Carte *rechercheCarteAccessible(Carte *deck, Carte *debut, Carte *fin, Carte *current_focus, char direction, int TAILLE)
{
    /**
     * Fonction : rechercheCarteAccessible
     * Param : 
     *      - deck : un pointeur de structure Carte contenant toutes les cartes du jeu
     *      - debut : un pointeur de structure Carte pointant vers le début du deck
     *      - fin : un pointeur de structure Carte pointant vers la fin du deck
     *      - current_focus : un pointeur de structure Carte permettant de savoir ou se trouve le curseur
     *                        dans le deck
     *      - direction : un char qui peut prendre deux valeurs 'g' ou 'd' représentant respéctivement : gauche et droite
     *      - TAILLE :  un entier contenant la taille du deck
     * Traitement : Cette fonction va d'abord regarder de quel côté elle doit parcourir le tableau en fonction de la variable direction,
     *              si la direction est égale à g alors elle regardera à sa gauche, sinon elle regarde à sa droite. Elle va en suite regarder
     *              toutes les cartes jusqu'à trouver une carte ou elle peut poser le curseur (une carte avec pour état 0), si elle en trouve une
     *              elle renvoie l'adresse de cette carte, sinon si elle ne trouve aucune carte (donc la varaible count et plus grand que 12)
     *              alors elle renvoie l'adresse du pointeur current_focus, donc ou se trouvait le joueur avant d'appuyer sur 'a' ou 'z'.
     * Retour : Cette fonction renvoie l'adresse de la carte la plus proche où l'on peut mettre le curseur
    */
    Carte *val_return = NULL;
    Carte *current_val = current_focus;

    int count = 0; // on a besoin de current_index pour eviter de perdre l'original pour l'affichage des cartes

    if (direction == 'g')
    {
        while (val_return == NULL)
        {
            if (current_val->etat == 0)
                val_return = current_val;
            else
            {
                if (current_val == debut)
                    // va a la fin du deck si l'utilisateur appuie sur 'a' tout en étant au début
                    current_val = fin;
                else
                    // sinon on se déplace simplement en arrière
                    current_val--;
            }
            count++;
            if (count > 12)
                // si on essaye de se deplacer d'une carte a une autre quand il nous reste qu'une carte
                // cette condition nous permet de sortir de la boucle
                val_return = current_focus;
        }
    }
    else
    {
        // même boucle dans le sens inverse de celle précédente
        while (val_return == NULL)
        {
            if (current_val->etat == 0)
                val_return = current_val;
            else
            {
                if (current_val == fin)
                    current_val = debut;
                else
                    current_val++;
            }
            count++;
            if (count > 12)
                val_return = current_focus;
        }
    }
    if (current_focus->etat != 3)
        current_focus->etat = 0;
    /// mvwprintw(stdscr, 27, 25, "current_val : %p", val_return);
    return val_return;
}

void Jeu(int width)
{
    /**
     * Fonction : Jeu
     * Param : 
     *      - width : la largeur minimal du terminal
     * Traitement : Cette fonction met en place le jeu de base, elle initialise les variables dont on aura besoin pour jouer au jeu 
     *              comme les pointeurs (deck, current_focus pour savoir ou l'on se trouve dans le jeu et compared). Elle contient aussi
     *              la boucle de jeu et l'appel des fonctions de déplacement, calcule de temps et écriture du score.
     * Retour : Cette fonction ne retourne pas de valeur
    */
    
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
        Carte *current_focus = &deck[0];            // pointeur permettant de savoir quelle carte est focus
        Carte *compared = NULL;                     // pointeur qui sera initialise seulement si une carte est selectionnee

        bool game = 1;
        int count = 0;
        int sortir = 1;                      // permet de savoir la personne a trouvée toute les paires, br permet de sortir de la boucle avec le la touche 'q'
        int buffer = 0;
    
        WINDOW *resultBox; // Initialisation des fenetres
    
        // Initialisation des variables
        char key = ' ';
        double elasped_time = 0; 
    
        struct timespec start_time;
    
        // define color
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_WHITE, COLOR_BLACK); // carte normal
        init_pair(4, COLOR_GREEN, COLOR_BLACK); // carte focus
        init_pair(5, COLOR_BLUE, COLOR_BLACK);  // carte comparée
    
        // obtenir le temps au début de l'exec
        clock_gettime(CLOCK_REALTIME, &start_time);
    
        // DisplayCard();
        while(sortir)
        {
            if (count == 6)
            {
                game = 0;
                sortir = 0;
            }

            elasped_time = CalcElapsed_Time(start_time);
            //if ( elasped_time >= 120) // a envlever ?
                //break;
    
            // savoir si il faut bloquer la récuperation d'input ou s'il faut continuer à les lires
            // la fonction ne permet pas de se déplacer, l'index est trouvée mais le pointeur n'est pas modifié
            buffer = ManageInput(deck, &compared, &current_focus,start_time, key);
            if(buffer == -1)
                sortir = 0;
            else
                count+=buffer;
            //mvwprintw(stdscr, 25, 25, "current focus : %p,count :%d", compared,count);
            
            // recupère les inputs
            key = getch();
            //mvwprintw(stdscr,20,0,"key : %c,sortir : %d,count : %d,game : %d,buffer : %d",key,sortir,count,game,buffer);
            refresh();
            usleep(65000); // arret de 70ms pour alléger le processeur
        }

        nodelay(stdscr, FALSE); // permettre de bloquer le prog tant que la touche 'q' n'est pas pressee
    
        // affichage des meilleurs scores
        resultBox = subwin(stdscr, 7, width, 23, 0);
        box(resultBox, ACS_VLINE, ACS_HLINE);
    
        results(resultBox, (float)elasped_time-2.0, game,0);
        wrefresh(resultBox);
        key = ' ';
        while (key != 'q' && key != 'Q') // si touche 'q' pressee : arret du jeu
            key = getch();
        // liberation de la memoire
        
        delwin(resultBox);
        LibereDeck(deck);
}
