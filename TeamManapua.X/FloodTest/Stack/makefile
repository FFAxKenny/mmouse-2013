#
#
#	Container makefile (Linked List)

CCC = gcc
CCFLAGS = -ansi

stackAtester: stackA.o stack.o mainA.o
	$(CCC) -o stackAtester stackA.o stack.o mainA.o -lncurses 
stacktester: stack.o main.o
	$(CCC) -o stacktester stack.o main.o -lncurses 


clean:
	rm -rf *.o

real_clean: clean
	rm -rf floodtester
