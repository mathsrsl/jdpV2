#include "carte.h"

void AttributsInit(Carte *c, unsigned int etat)
{
    /**
     * Fonction : AttributsInit
     * Param :
     *      - c : la carte a modifier
     *      - etat : entier qui modifiera la carte
     * Traitement : la fonction change l'etat de la carte qui lui est donnée en parametère
     *              cela permet de savoir si si la carte est focus,comparée ou trouvée.
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
     *              structure carte), la longeur et largeur est un define dans le carte.h,
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
        // mvwprintw(c->carte,3,5,"%c",c->var);
        // mvwprintw(c->carte,4,5,"%d",c->etat);
        break;
    case 1:
        wborder(c->carte, '|', '|', '-', '-', ' ', ' ', ' ', ' ');
        wbkgd(c->carte, COLOR_PAIR(4));
        mvwprintw(c->carte, 3, 5, "%c", c->var);
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
     *                        sur laquelle l'utilisateur se trouve, l'état de cette cartes
     *                        est 1.
     *      - compared : un pointeur de structure Carte qui peut pointer sur une carte du deck comme être égal
     *                   à NULL. Ce pointeur sert a comparer deux cartes entre elle.
     *      - TAILLE : un entier qui est égal au nombre de carte qui se trouvent dans le deck,
     *                 cet entier est un define dans le carte.h
     * Traitement : Cette fonction prend en paramètres toute les informations qu'il lui faut
     *              pour afficher les cartes en fonction de leur pointeur. Pour traiter chaque
     *              carte dans le deck elle utilise une boucle for suivie de condition pour
     *              reconnaitre les cartes pointée qui doivent avoir un certains affichage
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
     * Traitement : Cette fonction regarde si la place de la carte dans le deck représenté par
     *              numCard est plus grand ou égal à 5 pour attribuer à la carte son emplacement
     *              sur l'axe des ordonnées
     * Retour : Cette fonction renvoie un entier qui défniie l'endroit ou sera afficher la carte
     *          sur l'axe des ordonnées.
     */
    if (numCard <= 5)
        return 4;
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
     *              l'axe des abscisse.
     * Retour : cette fonction renvoie un entier qui définie l'endroit ou sera afficher la
     *          carte sur l'axe des abscisse.
     */
    switch (numCard % 6)
    {
    case 0:
        return 1;
        break;
    case 1:
        return 12;
        break;
    case 2:
        return 23;
        break;
    case 3:
        return 34;
        break;
    case 4:
        return 45;
        break;
    case 5:
        return 56;
        break;
    }
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
     * Traitement : Cette fonction prend le deck de carte, le tableau de lettre (avec les lettres)
     *              et enfin la taille du tableau de lettre. Elle va en suite rentrer dans la boucle for
     *              et faire un pseudo aléatoire lui permettant de séléctionner une lettre par une lettre
     *              qui ne sera jamais la même (sauf si c'est la paire). Une fois la lettre séléctionnée
     *              elle va donner a la carte du deck (deck + i) la lettre qu'elle a trouvée et
     *              continuer ainsi et vu que le random de 0 et 1 est impossible elle va donc
     *              prendre les deux dernières lettres restantes et les donner aux deux dernières
     *              cartes restantes.
     * Retour : aucune valeur de retour.
     */

    srand(time(NULL)); // initialise la seed pour avoir des nombres aléatoire différent a chaque appel
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

void ManageInput(Carte *deck, Carte **compared, Carte **current_focus, bool *freezeInput, double *elapsed_time, double *chronoCompare, char input, bool *br)
{
    /**
     * Fonction : ManageInput
     * Param :
     *      - deck : un pointeur de structure Carte contenant toutes les cartes du jeu
     *      - compared : un pointeur de pointeur de struture Carte contenant NULL ou la carte
     *                   à comparer.
     *      - current_focus : un pointeur de pointeur de structure Carte contenant la carte
     *                        où se trouve l'utilisateur dans le deck.
     *      - freezeInput : un pointeur de booléen premettant de savoir si il faut arrêter de prendre les inputs ou continuer
     *      - elapsed_time : un pointeur de double permettant d'avoir le temps qui est passé depuis le début du jeu
     *      - chronoCompare : un pointeur de double, sa valeur est a 0 par défaut et a l'issue de cette fonction il pourra
     *                        prendre la valeur de elapsed_time ou restera à 0.
     *      - input : un char qui prend comme valeur l'input fait par l'utilisateur (key dans le main)
     *      - br : un pointeur de booléen qui est seulement utile pour faire marcher la touche 'q'
     * Traitement : La fonction va d'abord appeler la fonction DisplayCardPtr pour afficher les cartes en fonction des pointeurs
     *              et du deck, par la suite elle va regarder les inputs faits et déplacera le pointeur current_focus à droite ou 
     *              à gauche dans le deck si 'a' ou 'z' sont pressés, sinon si 'e' est pressé alors il regarde si comparede vaut NULL
     *              pour savoir si une carte est comparée pour savoir si il faut arrêter de prendre les inputs (donc modifier la valeur
     *              du pointeur freezeInput) ou si il faut simplement décaler current_focus et continuer le jeu. Si il faut comparer deux
     *              cartes elles prendront comme état 2 et le freezeInput sera égal à 0.
     * Retour : aucune valeur de retour.
     */
    // besoin de cette fonction ici pour que les cartes comparee ne changent pas de couleur
    // pendant les 2 secondes de delais
    DisplayCardPtr(deck, *current_focus, *compared, TAILLE_DECK);
    // mvwprintw(stdscr, 26, 25, "temp : %c", input);
    if (input == 'a')
    {
        // cursorPos = (cursorPos>1) ? cursorPos-1 : 12;
        *current_focus = rechercheCarteAccessible(deck, deck, (deck + TAILLE_DECK - 1), *current_focus, 'g', TAILLE_DECK);
    }
    else if (input == 'z')
    {
        *current_focus = rechercheCarteAccessible(deck, deck, (deck + TAILLE_DECK - 1), *current_focus, 'd', TAILLE_DECK);
    }
    else if (input == 'e')
    {
        // a decouper en fonction
        if (*compared == NULL && (*current_focus)->etat != 3)
        {
            // fonction avec comme paramètre deck compared current_focus return un float
            // on verifi si current_focus et bien egal a 1 pour eviter que les cartes revelee et bloquee soient debloquable
            *compared = *current_focus;
            *current_focus = rechercheCarteAccessible(deck, deck, (deck + TAILLE_DECK - 1), *current_focus, 'd', TAILLE_DECK);
        }
        else if (*compared != NULL)
        {
            // on bloque les inputs et on recupere le temps auquel tout a ete bloque pour regarder quand arreter
            *chronoCompare = *elapsed_time;
            *freezeInput = 0;
            // changer les cartes de couleur pour la comparaison
            AttributsInit(*current_focus, 2);
            DisplayCard(*current_focus, LONGUEUR, LARGEUR);
        }
    }
    else if (input == 'q')
    {
        // quitter avant la fin du timer
        *br = 0;
    }
}

void CompareCard(Carte **current_focus, Carte **compared, bool *freezeInput, double *chronoCompare, int *count)
{
    /**
     * Fonction : CompareCard
     * Param : 
     *      - current_focus : un pointeur de pointeur de structure Carte permettant de savoir ou l'on se trouve dans le deck
     *      - compared : un pointeur de pointeur de structure Carte permettant de savoir quel carte séléctionnée pour être comparée
     *      - freezeInput : un pointeur de booléen permttant de savoir si l'on doit prendre les inputs ou non
     *      - chronoCompare : un pointeur de double utilisé pour l'attente de 2 secondes pour la comparaison des cartes
     *      - count : un pointeur d'entier permetttant de savoir si toutes les paires ont été trouvées
     * Traitement : Cette fonction fait la comparaison entre current_focus et compared pour savoir si ces deux cartes ont la même lettre
     *              dans ce cas là il faudra les bloquer (état 3) et laisser current_focus a l'indice de la cartes que l'on a comparé
     *              dans le cas contraire compared sera égal à NULL l'autre current_focus quand on reviendra dans la fonction ManageInput
     *              prendra son état et couleur de base. Cette fonction est appelée seulement quand deux cartes sont comparées, donc
     *              seulement quand freezeInput est égal à 0.
     * Retour : aucune valeur de retour.
    */
    *chronoCompare = 0; // plus de carte a comparer
    *freezeInput = 1; // retour à la lecture d'input
    if ((*compared)->var == (*current_focus)->var)
    {
        // changes l'etat pour etre sur qu'a la prochaine iteration de la boucle elles puissent changer de forme
        AttributsInit(*compared, 3);
        AttributsInit(*current_focus, 3);
        *compared = NULL;
        *(count) += 1;
    }
    else
    {
        // si les cartes ne sont pas pareil comapred  reste a NULL et a la prochaine iteration
        // current_focus sera de nouveau vert
        *compared = NULL;
    }
}

Carte *CreationDeck()
{
    /**
     * Fonction : CreationDeck
     * Param : aucun paramètres
     * Traitement : Cette fonction commence par créer un tableau dynamiquement avec un calloc de taille TAILLE_DECK
     *              qui est un define dans le fichier carte.h (la taille étant de 12). Par la suite elle créé un tableau
     *              de lettre contenant les paires qui va être utilisé pour donner à chaque carte du deck une lettre grâce à
     *              la fonction LettreAlea() et enfin les positions X et Y grâce au fonction PosCardX et PosCardY des Cartes 
     *              et leur états sont initisalisés grâce à une boucle for.
     * Retour : la fonction retourne le pointeur deck pour pouvoir l'utiliser dans le main comme dans d'autre fichier.
    */
    // Creation du pointeur deck / tableau
    Carte *deck = (Carte *)calloc(TAILLE_DECK, sizeof(Carte));

    // initialisation de toute les cartes
    // creation du set de lettre utilisé pour les valeurs des cartes
    char lettre[TAILLE_DECK] = "AABBCCDDEEFF";

    if (deck != NULL)
    {
        LettreAlea(deck, lettre, TAILLE_DECK); // affecte au carte des valeurs char aleatoire (il y a forcement des paires)
        for (int i = 0; i < TAILLE_DECK; i++)
        {
            // initialisation des attributs pour ne pas avoir de mauvaise suprise de valeur trop haute
            AttributsInit((deck + i), 0);
            (deck + i)->X = PosCardX(i);
            (deck + i)->Y = PosCardY(i);
        }
    }

    return deck;
}

void LibereDeck(Carte *deck)
{
    free(deck);
}

Carte *rechercheCarteAccessible(Carte *deck, Carte *debut, Carte *fin, Carte *current_focus, char direction, int TAILLE)
{
    /**
     * Fonction : rechercheCarteAccessible
     * Param : 
     *      - deck : un pointeur de structure Carte contenant toute les cartes du jeu
     *      - debut : un pointeur de structure Carte pointant vers le début du deck
     *      - fin : un pointeur de structure Carte pointant vers la fin du deck
     *      - current_focus : un pointeur de structure Carte permettant de savoir ou se trouve le curseur
     *                        dans le deck
     *      - direction : un char qui peut prendre deux valeurs 'g' ou 'd' représentant respéctivement : gauche et droite
     *      - TAILLE :  un entier contenant la taille du deck
     * Traitement : Cette fonction va d'abord regarder dans quel sens elle doit itérer dans le tableau en fonction
     *              de la variable direction, une fois cela fait elle rentre dans une boucle while, ayant pour condition 
     *              que le pointeur de structure Carte val_return soit égal à NULL, pour trouver la 
     *              carte la plus proche où le curseur pourra être posé. Si la fonction arrive a la fin/début du deck elle
     *              utilisera les pointeurs début/fin pour continuer son itération, enfin si aucune carte n'est séléctionnable
     *              (cela veut dire que presque toutes les paires ont été trouvées) alors une variable count qui est incrémentée
     *              à chaque mouvement dans le deck vérifira la condition count > 12 et  val_return prendra la valeur de current_focus
     *              ce qui nous fait sortir de la boucle while. Sinon si une carte est trouvée val_return prendra son adresse en valeur
     *              et aura une valeur différente de NULL.
     * Retour : Cette fonction renvoie un pointeur de structure Carte étant la carte dans le deck la plus proche où le curseur
     *          peut être placé.
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
