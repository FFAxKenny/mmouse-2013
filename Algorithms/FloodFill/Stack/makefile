#
#
#	Container makefile (Linked List)
#
#

CCC = g++
CCFLAGS = -ansi

teststack: main.o stack.o 
	$(CCC) -o teststack main.o stack.o

stack.o: stack.h

clean:
	rm -f *.o

real_clean: clean
	rm -f teststack
