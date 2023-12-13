output: main.o menu.o carte.o score.o 
	gcc main.o menu.o carte.o score.o -o game -lncurses

debug: main.oDebug menu.o carte.oDebug score.o
	gcc main.o menu.o carte.o score.o -o game -lncurses

main.o: main.c
	gcc main.c -c

main.oDebug: main.c
	gcc -g main.c -c

menu.o: menu.c
	gcc menu.c -c

carte.o: carte.c
	gcc carte.c -c

carte.oDebug: carte.c
	gcc -g carte.c -c

score.o: score.c
	gcc score.c -c

clean:
	rm *.o game
