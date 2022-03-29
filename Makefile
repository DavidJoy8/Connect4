.DEFAULT_GOAL = connect4

connect4: main.c connect4.h
	gcc main.c -o connect4

clean:
	rm -f *.o
