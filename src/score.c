#include "../include/score.h"

void filFile(FILE *fichier2, char nom1[TAILLE_NOM], char score1[TAILLE_SCORE], char nom2[TAILLE_NOM], char score2[TAILLE_SCORE],char nom3[TAILLE_NOM], char score3[TAILLE_SCORE]){
    /*
    Fonction : filFile
    Param : FILE *fichier2, char nom1[TAILLE_NOM], char score1[TAILLE_SCORE], 
            char nom2[TAILLE_NOM], char score2[TAILLE_SCORE],
            char nom3[TAILLE_NOM], char score3[TAILLE_SCORE]
    Traitement : Rempli un fichier avec les scores et noms de joueurs
    Retour :  Aucun
    */
    fputs("1", fichier2);
    fputs(" ", fichier2);
    fputs(nom1, fichier2);
    fputs(" ", fichier2);
    fputs(score1, fichier2);
    fputs("\n", fichier2);
    fputs("2", fichier2);
    fputs(" ", fichier2);
    fputs(nom2, fichier2);
    fputs(" ", fichier2);
    fputs(score2, fichier2);
    fputs("\n", fichier2);
    fputs("3", fichier2);
    fputs(" ", fichier2);
    fputs(nom3, fichier2);
    fputs(" ", fichier2);
    fputs(score3, fichier2);
    fclose(fichier2);
}

void filVoidFile(FILE * fichier2, char nom[TAILLE_NOM], char score[TAILLE_SCORE]){
    /*
    Fonction : filVoidFile
    Param : FILE * fichier2, char nom[TAILLE_NOM], char score[TAILLE_SCORE]
    Traitement : Créer un fichier score s'il n'existe pas et le rempli
    Retour :  Aucun
    */
    fputs("1", fichier2);
    fputs(" ", fichier2);
    fputs(nom, fichier2);
    fputs(" ", fichier2);
    fputs(score, fichier2);

    fclose(fichier2);
}

void displayResults(WINDOW * resultBox, bool game, char nom1[TAILLE_NOM], char score1[TAILLE_SCORE], char nom2[TAILLE_NOM], char score2[TAILLE_SCORE],char nom3[TAILLE_NOM], char score3[TAILLE_SCORE]){
    /*
    Fonction : displayResults
    Param : WINDOW * resultBox, bool game, char nom1[TAILLE_NOM], char score1[TAILLE_SCORE],
            char nom2[TAILLE_NOM], char score2[TAILLE_SCORE],
            char nom3[TAILLE_NOM], char score3[TAILLE_SCORE]
    Traitement : Affiche les scores du fichier
    Retour :  Aucun
    */
    wclear(resultBox);
    box(resultBox, ACS_VLINE, ACS_HLINE);
    if(game){
        mvwprintw(resultBox, 1, 1, "DEFAITE");
    }else{
        mvwprintw(resultBox, 1, 1, "VICTOIRE");
    }
    mvwprintw(resultBox, 2, 1, "Meilleurs chrono :");
    mvwprintw(resultBox, 3, 1, "1 %s %s", nom1, score1);
    mvwprintw(resultBox, 4, 1, "2 %s %s", nom2, score2);
    mvwprintw(resultBox, 5, 1, "3 %s %s", nom3, score3);
}

void looseResults(WINDOW * resultBox, bool game){
    /*
    Fonction : looseResults
    Param : WINDOW * resultBox, bool game
    Traitement : Fonction qui affiche juste les résultats si le joueur appuie 
                sur q, sans enregistrer le score ou nom du joueur puisqu'il est considéré
                comme perdant
    Retour :  Aucun
    */
    FILE *fichier = NULL;
    fichier = fopen("jeuhighscore.txt","r");

    char c = fgetc(fichier);
    char nom1[TAILLE_NOM] = {0}, nom2[TAILLE_NOM] = {0}, nom3[TAILLE_NOM] = {0} ;
    char score1[TAILLE_SCORE] = {0}, score2[TAILLE_SCORE] = {0}, score3[TAILLE_SCORE] = {0};
    int espace = 0, i = 1, ligne = 1;

    while(c != EOF){
        switch (c) {
            case ' ' :
                espace+=1;
                break;
            case '\n' :
                i+=1;
                espace = 0;
                ligne+=1;
                break;
            default :
                break;
        }
        if (espace == 1) {
            if (c != ' '){
                if (ligne == 1){
                    strncat(nom1, &c, 1);
                }else if (ligne == 2){
                    strncat(nom2, &c, 1);
                }else if (ligne == 3){
                    strncat(nom3, &c, 1);
                }
            }
        }
        if (espace == 2) {
            if (c != ' '){
                if (ligne == 1){
                    strncat(score1, &c, 1);
                }else if (ligne == 2){
                    strncat(score2, &c, 1);
                }else if (ligne == 3){
                    strncat(score3, &c, 1);
                }
            }
        }
        c = fgetc(fichier);
    }
    displayResults(resultBox, game, nom1, score1, nom2, score2, nom3, score3);
}

void winResults(WINDOW * resultBox, float temps, bool game){
    /*
    Fonction : winResults
    Param : WINDOW * resultBox, float temps, bool game
    Traitement : Fonction qui enregistre le score du joueur comme il a gagné,
                 modifie les scores du fichier si besoin et affiche les scores
    Retour :  Aucun
    */
    FILE *fichier = NULL;
    fichier = fopen("jeuhighscore.txt","r");

    char nomJoueur[TAILLE_NOM];
    char tempsJoueur[TAILLE_SCORE];

    //transforme le float en chaine de caracteres
    snprintf(tempsJoueur, sizeof(tempsJoueur), "%.1f", temps);
    if (fichier == NULL){
        mvwprintw(resultBox, 1, 1, "Entrer votre nom en 4 lettres :\n");
        echo();
        mvwgetnstr(resultBox, 2, 1, nomJoueur, 4);
        fichier = fopen("jeuhighscore.txt","w+");
        filVoidFile(fichier, nomJoueur, tempsJoueur);
    } else {
        char c = fgetc(fichier);
        char nom1[TAILLE_NOM] = {0}, nom2[TAILLE_NOM] = {0}, nom3[TAILLE_NOM] = {0} ;
        char score1[TAILLE_SCORE] = {0}, score2[TAILLE_SCORE] = {0}, score3[TAILLE_SCORE] = {0};
        int espace = 0, i = 1, ligne = 1;
        while(c != EOF){
            switch (c) {
                case ' ' :
                    espace+=1;
                    break;
                case '\n' :
                    i+=1;
                    espace = 0;
                    ligne+=1;
                    break;
                default :
                    break;
            }
            if (espace == 1) {
                if (c != ' '){
                    if (ligne == 1){
                        strncat(nom1, &c, 1);
                    }else if (ligne == 2){
                        strncat(nom2, &c, 1);
                    }else if (ligne == 3){
                        strncat(nom3, &c, 1);
                    }
                }
            }
            if (espace == 2) {
                if (c != ' '){
                    if (ligne == 1){
                        strncat(score1, &c, 1);
                    }else if (ligne == 2){
                        strncat(score2, &c, 1);
                    }else if (ligne == 3){
                        strncat(score3, &c, 1);
                    }
                }
            }
            c = fgetc(fichier);
        }
        if (atoi(score3)>temps){
            mvwprintw(resultBox, 1, 1, "Entrer votre nom en 4 lettres :\n");
            echo();
            mvwgetnstr(resultBox, 2, 1, nomJoueur, 4);
            if ((atoi(score1))>temps){
                strcpy(score3, score2);
                strcpy(nom3, nom2);
                strcpy(score2, score1);
                strcpy(nom2, nom1);
                strcpy(score1, tempsJoueur);
                strcpy(nom1, nomJoueur);
            }else if((atoi(score1))<temps && (atoi(score2))>temps){
                strcpy(score3, score2);
                strcpy(nom3, nom2);
                strcpy(score2, tempsJoueur);
                strcpy(nom2, nomJoueur);
            }else {
                strcpy(score3, tempsJoueur);
                strcpy(nom3, nomJoueur);
            }
        }
        fichier = fopen("jeuhighscore.txt","w+");
        filFile(fichier, nom1, score1, nom2, score2, nom3, score3);
        displayResults(resultBox, game, nom1, score1, nom2, score2, nom3, score3);
    }
}

void results(WINDOW * resultBox, float temps, bool game){
    /*
    Fonction : results
    Param : WINDOW * resultBox, float temps, bool gam
    Traitement : Fonction permet de diviser en fonction de si la partie est
                 gagnée ou non
    Retour :  Aucun
    */
    if(game){
        looseResults(resultBox, game);
    }else{
        winResults(resultBox, temps, game);
    }
}
