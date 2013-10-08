
floodfill: floodfill.o maze.o display.o mouse.o queue.o heap.o
	gcc -o floodfill floodfill.o maze.o display.o mouse.o queue.o heap.o

floodfill.o: maze.h display.h queue.h heap.h mouse.h

maze.o: maze.h queue.h heap.h

mouse.o: maze.h mouse.h

display.o: maze.h display.h

queue.o: queue.h

heap.o: heap.h

clean:
	rm -f *.o

real_clean: clean
	rm -f floodfill

