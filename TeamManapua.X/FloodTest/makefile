#
#
#	Container makefile (Linked List)

CCC = gcc
CCFLAGS = -ansi 

floodtester: FloodTester.o FloodFill.o stackA.o stack.o
	$(CCC) -o floodtester FloodTester.o FloodFill.o stackA.o stack.o -lncurses 

FloodFill.o: stackA.o stack.o
StackA.o: stack.h

links:
	ln -fs Stack/stack.h .
	ln -fs Stack/stack.c .
	ln -fs Stack/stackA.h .
	ln -fs Stack/stackA.c .
clean:
	rm -rf *.o

real_clean: clean
	rm -rf floodtester
