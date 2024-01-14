output: main.o menu.o autoplayer.o bandeau.o carte.o score.o autoplayer2.o
	gcc ../obj/main.o ../obj/menu.o ../obj/carte.o ../obj/score.o ../obj/autoplayer.o ../obj/bandeau.o ../obj/autoplayer2.o -o ../bin/game -lncurses -Wall

debug: main.oDebug menu.o carte.oDebug score.o autoplayer.o bandeau.o autoplayer2.o
	gcc ../obj/main.o ../obj/menu.o ../obj/carte.o ../obj/score.o ../obj/autoplayer.o ../obj/bandeau.o ../obj/autoplayer2.o -o ../bin/game -lncurses -Wall

main.o: main.c
	gcc -c main.c -o ../obj/main.o

main.oDebug: main.c
	gcc -g -c main.c -o ../obj/main.o

menu.o: menu.c
	gcc -c menu.c -o ../obj/menu.o

carte.o: carte.c
	gcc -c carte.c -o ../obj/carte.o

carte.oDebug: carte.c
	gcc -g -c carte.c -o ../obj/carte.o

score.o: score.c
	gcc -c score.c -o ../obj/score.o

autoplayer.o: autoplayer.c
	gcc -c autoplayer.c -o ../obj/autoplayer.o

bandeau.o: bandeau.c
	gcc -c bandeau.c -o ../obj/bandeau.o

autoplayer2.o: autoplayer2.c
	gcc -c autoplayer2.c -o ../obj/autoplayer2.o

clean:
	rm ../obj/*.o ../bin/game
