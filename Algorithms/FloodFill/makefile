#
#
#	Container makefile (Linked List)
#
#

CCC = g++
CCFLAGS = -ansi

mazetest: driver.o RawMaze.o Maze.o
	$(CCC) -o mazetest driver.o RawMaze.o Maze.o

mazetest.o: RawMaze.h Maze.h
RawMaze.o: RawMaze.h
Maze.o: RawMaze.h Maze.h

clean:
	rm -f *.o

real_clean: clean
	rm -f maze
