.DEFAULT_GOAL = connect4

connect4: main.c connect4.h
	gcc main.c -o connect4

test: test.c main.c connect4.h
	gcc -DTESTING test.c -o test

clean:
	rm -f *.o connect4 test
