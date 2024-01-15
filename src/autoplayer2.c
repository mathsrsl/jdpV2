#include "../include/autoplayer2.h"

int CompareCardAutoPlayer(Carte * deck,int indexCardA,int indexCardB,struct timespec start_time,double lastT)
{
    /**
     * Fonction : CompareCardAutoPlayer
     * Param : 
     *      - deck : pointeur de structure Carte contenant les 12 cartes du jeu
     *      - indexCardA, indexCardB: Cet entier permet de savoir quel carte on doit comparer avec la deuxième carte en utilisant leur index
     *      - start_time : une structure contenant le temps en seconde et nanoseconde de quand le mode de jeu a été lancé
     *      - lastT : contient le temps en seconde et nanoseconde qu'il était avant l'appel de cette fonction
     * Traitement : Cette fonction est spécifique à l'autoplayer, dans un premier temps cette fonction va utiliser la fonction CalcElapsed_Time
     *              pour savoir quand les 2 secondes de comparaison sont passé (tout en affichant le temps qui passe), dans cette boucle on vérifie
     *              si la touche q n'est pas préssée pour arrêter l'autoplayer. Enfin une fois le décompte fini on regarde si a l'index A et B
     *              du deck si ces deux cartes ont la même valeur au niveau de leur lettre.
     * Retour : Cette fonction retourne 1 si les cartes sont des paires, 0 si elles ne le sont pas et 3 si le joueur veut quitter le mode de jeu.
    */
    /*comparera les cartes jsp comment*/
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

void AutoPlayer2()
{
    clear();
    nodelay(stdscr, TRUE);
    curs_set(0); 
    start_color();

    init_pair(3, COLOR_WHITE, COLOR_BLACK); // carte normal
    init_pair(4, COLOR_GREEN, COLOR_BLACK); // carte focus
    init_pair(5, COLOR_BLUE, COLOR_BLACK);  // carte comparée

    //déclaration des varaibles utile
    int count = 0;  //permet de savoir si toute les cartes ont été trouvée
    int index_current = 0;  //permet de savoir la dernière carte qui a pu être séléctionnée
    int index_occur = -1;   //permet de savoir quel carte existe déjà en paire avec la dernière carte séléctionnée
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
            
            //Si une paire existe on va les comparers
            if(index_occur != -1)
            {
                AttributsInit((deck + index_occur),2);
                //ici pas besoin de récupérer la valeur de comparaison sachant que les cartes sont déjà connue
                estPaire = CompareCardAutoPlayer(deck,index_current,index_occur,start_time,lastT);
                index_occur = -1;
                count++;
            }else
            {
                index_current = SearchLetter(allLetterKnown,index_current,0);
                //à l'inverse si ces cettes cartes est inconnue on va simplement la comparée à sa voisine.
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

    LibereDeck(deck);
    clear();
}