
floodfill: floodfill.o maze.o mouse.o list.o
	gcc -o floodfill floodfill.o maze.o mouse.o list.o

floodfill.o: maze.h list.h mouse.h common.h

maze.o: maze.h list.h mouse.h common.h

mouse.o: maze.h mouse.h common.h

list.o: list.h common.h

clean:
	rm -f *.o

real_clean: clean
	rm -f floodfill

