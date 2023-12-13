#include "carte.h"

void AttributsInit(Carte *c, unsigned int etat)
{
    c->etat = etat; // change en fonction de si oui ou non la carte est focus
}

void DisplayCard(Carte *c, int longeur, int largeur)
{
    /*
        Fonction permettant de creer les bordures de cartes en fonction de leur etat
    */
    c->carte = subwin(stdscr, longeur, largeur, c->X, c->Y);
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
    /*
        Cette fonction prend un deck de carte, la carte focus au moment ou elle est appelee, une eventuelle carte a comparer et la taille du deck
        elle affiche et change les valeurs de chaque carte en fonctions de leur etat
    */
    for (int i = 0; i < TAILLE; i++)
    {
        if (&deck[i] != current_focus && (deck + i)->etat != 3 && &deck[i] != compared)
        {
            (deck + i)->etat = 0;
            DisplayCard((deck + i), LONGUEUR, LARGEUR);
        }
        else if ((deck + i) == current_focus && (deck + i)->etat != 3)
        {
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

int PosCardX(int numCard)
{
    /*
        Cette fonciton renvoie simplement l'emplacement de la carte sur l'axe x
        en fonction de sa place dans le deck, place que l'on donne en paramètre
    */
    if (numCard <= 5)
        return 4;
    else
        return 13;
}

int PosCardY(int numCard)
{
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
    srand(time(NULL));
    int index = 0; // index permettant de prendre une lettre au hasard dans le array
    char temp;     // permet d'echanger la place des cartes

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
    *chronoCompare = 0;
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
    /*
    cette fonction permet de trouver la carte la plus proche pouvante etre utilisee par le pointeur de focus
    les cartes pouvants etre focus sont les cartes ayant comme numero d'etat 0 (sans compter la cartes)
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
