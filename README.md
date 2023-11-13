# jdpV2

Compiler :
```shell
gcc -o my_program main.c menu.c -lncurses
```

Affichage des carte en couleurs :

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