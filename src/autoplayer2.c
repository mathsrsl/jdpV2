#include "../include/autoplayer2.h"

int CompareCardAutoPlayer(Carte * deck,int indexCardA,int indexCardB,struct timespec start_time,double lastT)
{
    /**
     * Fonction : CompareCardAutoPlayer
     * Param : 
     *      - deck : pointeur de structure Carte contenant les 12 cartes du jeu
     *      - indexCardA, indexCardB: Cet entier permet de savoir quelle carte on doit comparer avec la deuxième carte en utilisant leur index
     *      - start_time : une structure contenant le temps en seconde et nanoseconde de quand le mode de jeu a été lancé
     *      - lastT : contient le temps en seconde et nanoseconde qu'il était avant l'appel de cette fonction
     * Traitement : Cette fonction est spécifique à l'autoplayer, dans un premier temps cette fonction va utiliser la fonction CalcElapsed_Time
     *              pour savoir quand les 2 secondes de comparaison sont passées (tout en affichant le temps qui passe), dans cette boucle on vérifie
     *              si la touche q n'est pas pressée pour arrêter l'autoplayer. Enfin une fois le décompte fini on regarde si a l'index A et B
     *              du deck si ces deux cartes ont la même valeur au niveau de leur lettre.
     * Retour :
     *      - 1 : si les cartes sont des paires
     *      - 0 : si les cartes ne sont pas des paires
     *      - 3 : si le joueur veut quitter le jeu
    */
    bool estPaire = 0;
    double elapsed_time = CalcElapsed_Time(start_time);
    char ch;
    //boucle pour le décompte des 2.0 secondes (elle permet d'afficher le temps avec l'appel de la fonction CalcElapsed_Time)

    DisplayCard((deck + indexCardA),LONGUEUR,LARGEUR);
    DisplayCard((deck + indexCardB),LONGUEUR,LARGEUR);

    refresh();

    //wprintw(stdscr,"%c,%c",(deck + indexCardA)->var,(deck + indexCardB)->var);
    while(elapsed_time - lastT < 2.0)
    {
        ch = getch();
        elapsed_time = CalcElapsed_Time(start_time);

        //on verifie les inputs pour savoir si le joueur ne tente pas de quitter le jeu.
        if(ch == 'q' || ch == 'Q') 
            return 3;
    }

    if((deck + indexCardA)->var ==  (deck+indexCardB)->var)
    {
        (deck + indexCardA)->etat = 3;
        (deck + indexCardB)->etat = 3;

        estPaire = 1;
    }else
    {
        (deck + indexCardA)->etat = 0;
        (deck + indexCardB)->etat = 0;

        estPaire = 0;
    }

    DisplayCard((deck + indexCardA),LONGUEUR,LARGEUR);
    DisplayCard((deck+indexCardB),LONGUEUR,LARGEUR);

    return estPaire;

}

int SearchLetter(char letterFound[],int index_current,bool mode)
{
    /**
     * Fonction : SearchLetter
     * Param : 
     *      - letterFound : un tableau de la même taille que le deck, ce tableau contient toute les cartes connues
     *                      ou si inconnue un simple espace pour indiquer une carte inconnue.
     *      - index_current : permet de savoir où on se trouve dans le deck.
     *      - mode : permet d'utiliser une des deux modes de la fonction.
     * Traitement : Cette fonction a deux modes que l'on peut acceder en mettant 0 ou 1 comme dernier paramètre.
     *              Si le mode est 1 alors la fonction va regarder si la lettre que l'on a à l'index_current a
     *              une paire connue, si elle trouve une paire connue alors elle retourne i (l'index de la paire)
     *              sinon elle retourne -1. Si la fonction est en mode 0 alors elle va chercher la prochaine carte 
     *              inconnue (la carte dans letterFound où il y a un espace au lieu d'une lettre) et retourner son
     *              index.
     * Retour : 
     *      - en mode 1 :
     *              - -1 si la lettre à l'index_current dans letterFound n'a pas de paire connue dans ce même tableau
     *              - sinon l'index de la carte étant la paire connue de la lettre à l'index_current dans letterFound
     *      - en mode 0 : l'index de la carte inconnue dans letterFound la plus proche
    */
    if(mode)
    {
        for(int i = 0;i < TAILLE_DECK;i++)
        {
            if(letterFound[i] == letterFound[index_current] && i != index_current)
                return i;
        }
        return -1;
    }else{
        while(letterFound[index_current] != ' ' && letterFound[index_current] != '\0')
        {
            index_current+=1;
        }
        return index_current;
    } 
}

void AutoPlayer2(int width)
{
    /**
     * Fonction : AutoPlayer2
     * Param : 
     *      - width : un entier représentant la largeur du jeu
     * Traitement : Cette fonction, comme le jeu de base, contient toute les initialisations dont l'algorithme a besoin
     *              pour jouer au jeu, la logique de l'algorithme est la suivante : on utilise un tableau (allLetterKnown)
     *              pour se rappeler de toute les lettres que l'on selectionne, l'algorithme commence à l'index 0 du deck séléctionne cette carte
     *              puis séléctionne une inconnue dans le tableau (où il y a un espace au lieu d'une lettre), généralement c'est la carte qui suit
     *              tant que la fonction SearchLetter ne renvoie pas un index de carte connue l'algorithme continue a séléctionner des cartes inconnues
     *              et à les stocker dans le talbeau. Si une carte a une paire connue, donc si index_occur n'est pas égal à -1, alors l'algorithme va
     *              prioriser la comparaison de ces deux cartes, pour se faire il regarde toujours après la première séléction d'une carte et après la 
     *              comparaison de deux cartes pour savoir si l'une des deux n'aurait pas de paire connue (si elles ne sont pas des paires). De cette manière
     *              on s'assure qu'au bout de la 12 ème cartes que le jeu est fini. 
     *              L'autoplayer 2 ne modifie pas les fichiers score pour éviter d'avoir un fichier score rempli d'autoplayer.
     * Retour : Aucune valeur de retour.
    */

    clear();
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(0);

    init_pair(3, COLOR_WHITE, COLOR_BLACK); // carte normal
    init_pair(4, COLOR_GREEN, COLOR_BLACK); // carte focus
    init_pair(5, COLOR_BLUE, COLOR_BLACK);  // carte comparée

    //déclaration des variables utiles
    int count = 0;  //permet de savoir si toutes les cartes ont été trouvée
    int index_current = 0;  //permet de savoir la dernière carte qui a pu être séléctionnée
    int index_occur = -1;   //permet de savoir quelle carte existe déjà en paire avec la dernière carte séléctionnée
    int estPaire = 0;
    double lastT= 0.0;
    char ch;
        
    //sera utile pour afficher le temps en dehors de cette boucle pour les cmprs
    struct timespec start_time;
    clock_gettime(CLOCK_REALTIME,&start_time);
    
    //déclaration des tableaux
    Carte * deck = CreationDeck();
    char allLetterKnown[TAILLE_DECK] = " ";

    while(count < 6)
    {
        ch = getch();
        //On a toujours besoin de savoir quel est le temps au début de cette itération de la boucle
        for(int k = 0;k<TAILLE_DECK;k++)
            DisplayCard((deck+k),LONGUEUR,LARGEUR);

        lastT = CalcElapsed_Time(start_time);

        //on regarde si une paire a été trouvée
        if(index_occur == -1)
        {
            //on cherche la carte la plus proche qui peut être séléctionnée (car elle n'est pas connue)
            index_current = SearchLetter(allLetterKnown,index_current,0);
            //Si aucune paire n'est connue on séléctionne la carte suivante a l'index i
            AttributsInit((deck + index_current),2);
            allLetterKnown[index_current] = (deck + index_current)->var;
            //printf("%c\n",allLetterKnown[index_current]);
            //Puis on cherche si cette carte est déjà connue, le résultat est enregistré dans la variable index_occur
            index_occur = SearchLetter(allLetterKnown,index_current,1);
            //mvwprintw(stdscr,20,5,"index :%d,count : %d,estPaire : %d",index_current,count,estPaire);
            
            //Si une paire existe on va les comparer
            if(index_occur != -1)
            {
                AttributsInit((deck + index_occur),2);
                //ici pas besoin de récupérer la valeur de comparaison sachant que les cartes sont déjà connues
                estPaire = CompareCardAutoPlayer(deck,index_current,index_occur,start_time,lastT);
                index_occur = -1;
                count++;
            }else
            {
                index_current = SearchLetter(allLetterKnown,index_current,0);
                //à l'inverse si cette carte est inconnue on va simplement la comparée à sa voisine.
                AttributsInit((deck + index_current),2);
                allLetterKnown[index_current] = (deck + index_current)->var;
                //ici on récupère la valeur de la fonction compare pour savoir si la seconde carte doit être recherchée ou non
                estPaire = CompareCardAutoPlayer(deck,index_current - 1,index_current,start_time,lastT);
                if(estPaire == 1)
                //on regarde si la lettre aurait sa paire connue
                    count++;
                else if(estPaire == 0)
                    index_occur = SearchLetter(allLetterKnown,index_current,1);
                //quoi qu'il arrive on avance dans le deck
            }
        }else 
        {
            //ici c'est le cas ou la dernière carte révélée a une paire, dans quel cas on saute une étape pour comparer ces deux cartes
            AttributsInit((deck + index_occur),2);
            AttributsInit((deck + index_current),2);
            
            estPaire = CompareCardAutoPlayer(deck,index_occur,index_current,start_time,lastT);
            count++;
            //on remet index_occur à -1 pour qu'on ne reste pas ici à l'infini
            index_occur = -1;
        }
        if(estPaire == 3 || ch == 'q' || ch == 'Q')
            break;
        refresh();
    }

    AfficheScore(width,estPaire);

    LibereDeck(deck);
}

void AfficheScore(int width,int quitter)
{
    WINDOW *resultBox = NULL;
    char key= ' ';

    // affichage des meilleurs scores
    resultBox = subwin(stdscr, 7, width, 23, 0);
    box(resultBox, ACS_VLINE, ACS_HLINE);

    // affichage des scores
    if(quitter != 3)
        results(resultBox,0.0,1,3);
    else
        results(resultBox,0.0,1,0);

    wrefresh(resultBox);

    while (key != 'q' && key != 'Q') // si touche 'q' pressee : arret du jeu
        key = getch();

    delwin(resultBox);

}
