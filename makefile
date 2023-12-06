output: 
	gcc main.c menu.c carte.c score.c -o game -lncurses

clean:
	rm game
