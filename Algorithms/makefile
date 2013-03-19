#
#
#	Container makefile (Linked List)
#
#

CCC = g++
CCFLAGS = -ansi

maze: maze.o rawMazeUtils.o MazeUtils.o
	$(CCC) -o maze maze.o rawMazeUtils.o MazeUtils.o

maze.o: rawMazeUtils.h MazeUtils.h
rawMazeUtils.o: rawMazeUtils.h
MazeUtils.o: rawMazeUtils.h MazeUtils.h

clean:
	rm -f *.o

real_clean: clean
	rm -f maze
