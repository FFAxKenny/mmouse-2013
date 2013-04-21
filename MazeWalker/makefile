#
#
#	Container makefile (Linked List)
#
#

CCC = g++
CCFLAGS = -ansi

testdisplay: testdisplay.o display.o 
	$(CCC) -o testdisplay testdisplay.o display.o

mazewalkwer: driver.o display.o 
	$(CCC) -o testdisplay driver.o display.o

driver.o: display.h


clean:
	rm -f *.o

real_clean: clean
	rm -f maze
