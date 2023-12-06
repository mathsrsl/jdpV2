#include "score.h"

void filFile(FILE *fichier2, char nom1[TAILLE_NOM], char score1[TAILLE_SCORE], char nom2[TAILLE_NOM], char score2[TAILLE_SCORE],char nom3[TAILLE_NOM], char score3[TAILLE_SCORE]){
        //remplissage avec les score de base
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
    //Créé le fichier et le rempli si il n'y a aucun score dedans
    fputs("1", fichier2);
    fputs(" ", fichier2);
    fputs(nom, fichier2);
    fputs(" ", fichier2);
    fputs(score, fichier2);

    fclose(fichier2);
}

void resultats(WINDOW * resultBox, float temps){
    //à decouper
    FILE *fichier = NULL;
    char n[100];
    fichier = fopen("jeuhighscore.txt","r");
    char nomJoueur[TAILLE_NOM];
    mvwprintw(resultBox, 1, 1, "Entrer votre nom en 4 lettres :\n");
    //scanf("%s", nomJoueur);
    mvwgetnstr(resultBox,3,2, nomJoueur, 4);
    char tempsJoueur[TAILLE_SCORE];
    //transforme le float en chaine de caracteres
    snprintf(tempsJoueur, sizeof(tempsJoueur), "%.1f", temps);
    if (fichier == NULL){
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
            if ((atoi(score1))>temps){
                for (i=0;i<5;i++){
                    score3[i]=score2[i];
                }
                for (i=0;i<5;i++){
                    nom3[i]=nom2[i];
                }
                for (i=0;i<5;i++){
                    score2[i]=score1[i];
                }
                for (i=0;i<5;i++){
                    nom2[i]=nom1[i];
                }
                for (i=0;i<5;i++){
                    score1[i]=tempsJoueur[i];
                }
                for (i=0;i<5;i++){
                    nom1[i]=nomJoueur[i];
                }
            }else if((atoi(score1))<temps && (atoi(score2))>temps){
                for (i=0;i<5;i++){
                    score3[i]=score2[i];
                }
                for (i=0;i<5;i++){
                    nom3[i]=nom2[i];
                }
                for (i=0;i<5;i++){
                    score2[i]=tempsJoueur[i];
                }
                for (i=0;i<5;i++){
                    nom2[i]=nomJoueur[i];
                }
            }else {
                for (i=0;i<5;i++){
                    score3[i]=tempsJoueur[i];
                }
                for (i=0;i<5;i++){
                    nom3[i]=nomJoueur[i];
                }
            }
        }
        fichier = fopen("jeuhighscore.txt","w+");
        filFile(fichier, nom1, score1, nom2, score2, nom3, score3);
        wclear(resultBox);
        mvwprintw(resultBox, 1, 1, "Victoire \nMeilleur chrono :\n1 %s %s\n2 %s %s\n3 %s %s", nom1, score1, nom2, score2, nom3, score3);
        //saut a la ligne buger
    }
}

