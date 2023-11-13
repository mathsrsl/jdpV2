# jdpV2

## Utilisation

**Compiler :**
```shell
gcc -o game main.c menu.c -lncurses -Wall
```

**Executer :**
```shell
./game
```

## Note

**Affichage des carte en couleurs :**

```C
//define color
start_color();
init_pair(1, COLOR_BLUE, COLOR_BLACK);
init_pair(2, COLOR_GREEN, COLOR_BLACK);
```

```C
if(key == 'a'){
    wattron(caseOne, COLOR_PAIR(1));
    wborder(caseOne, '|', '|', '-', '-', ' ', ' ', ' ', ' ');
    wattroff(caseOne, COLOR_PAIR(1));
} else  if(key == 'z'){
    wattron(caseOne, COLOR_PAIR(2));
    wborder(caseOne, '|', '|', '-', '-', ' ', ' ', ' ', ' ');
    wattroff(caseOne, COLOR_PAIR(2));
} else {
    wborder(caseOne, '|', '|', '-', '-', 'O', 'O', 'O', 'O');
}
```

**Autre techinique pour afficher un cadre**

```C
//affichage d'un cadre blanc autour de mainContainer
chtype ulcorner = ACS_ULCORNER;
chtype urcorner = ACS_URCORNER;
chtype llcorner = ACS_LLCORNER;
chtype lrcorner = ACS_LRCORNER;
chtype hline = ACS_HLINE;
chtype vline = ACS_VLINE;
 
mvwaddch(mainContainer, 0, 0, ulcorner);
mvwaddch(mainContainer, 0, width - 1, urcorner);
mvwaddch(mainContainer, height - 1, 0, llcorner);
mvwaddch(mainContainer, height - 1, width - 1, lrcorner);
mvwhline(mainContainer, 0, 1, hline, width - 2);
mvwhline(mainContainer, height - 1, 1, hline, width - 2);
mvwvline(mainContainer, 1, 0, vline, height - 2);
mvwvline(mainContainer, 1, width - 1, vline, height - 2);
```

**Entrée clavier qui n'accepte que 3 caractères**

```C
mvwgetnstr(mainContainer, positionX, positionY, input, 3);
```