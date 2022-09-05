#some name types for readable make file
OB1 = lib/auxiliarry.o lib/heapNode.o lib/memoryFragment.o lib/memoryFragmentsList.o lib/minHeap.o lib/processEntry.o lib/generatorMain.o lib/buddyNode.o lib/buddyTree.o
OB2 = lib/auxiliarry.o lib/heapNode.o lib/memoryFragment.o lib/memoryFragmentsList.o lib/minHeap.o lib/processEntry.o lib/memoryManagerMain.o lib/buddyNode.o lib/buddyTree.o
OB = lib/auxiliarry.o lib/heapNode.o lib/memoryFragment.o lib/memoryFragmentsList.o lib/minHeap.o lib/processEntry.o lib/generatorMain.o lib/memoryManagerMain.o lib/buddyNode.o lib/buddyTree.o
HEADERS = headers/auxiliarry.h headers/heapNode.h headers/memoryFragment.h headers/memoryFragmentsList.h headers/minHeap.h headers/processEntry.h headers/buddyNode.h headers/buddyTree.h
EXEC1 = generator
EXEC2 = manager
COMP = g++
FLAGS = -g -Wall -c -ggdb3
#executable
all :
	make ./generator
	make ./manager
lib/buddyNode.o: source/buddyNode.cpp $(HEADERS)
	$(COMP) $(FLAGS) source/buddyNode.cpp
	mv buddyNode.o lib/buddyNode.o

lib/buddyTree.o: source/buddyTree.cpp $(HEADERS)
	$(COMP) $(FLAGS) source/buddyTree.cpp
	mv buddyTree.o lib/buddyTree.o

lib/auxiliarry.o: source/auxiliarry.cpp $(HEADERS)
	$(COMP) $(FLAGS) source/auxiliarry.cpp
	mv auxiliarry.o lib/auxiliarry.o

lib/heapNode.o: source/heapNode.cpp $(HEADERS)
	$(COMP) $(FLAGS) source/heapNode.cpp
	mv heapNode.o lib/heapNode.o 

lib/minHeap.o: source/minHeap.cpp $(HEADERS)
	$(COMP) $(FLAGS) source/minHeap.cpp
	mv minHeap.o lib/minHeap.o

lib/memoryFragment.o: source/memoryFragment.cpp $(HEADERS)
	$(COMP) $(FLAGS) source/memoryFragment.cpp
	mv memoryFragment.o lib/memoryFragment.o

lib/memoryFragmentsList.o: source/memoryFragmentsList.cpp $(HEADERS)
	$(COMP) $(FLAGS) source/memoryFragmentsList.cpp
	mv memoryFragmentsList.o lib/memoryFragmentsList.o

lib/processEntry.o: source/processEntry.cpp $(HEADERS)
	$(COMP) $(FLAGS) source/processEntry.cpp
	mv processEntry.o lib/processEntry.o

lib/generatorMain.o: source/generatorMain.cpp $(HEADERS)
	$(COMP) $(FLAGS) source/generatorMain.cpp
	mv generatorMain.o lib/generatorMain.o

lib/memoryManagerMain.o: source/memoryManagerMain.cpp $(HEADERS)
	$(COMP) $(FLAGS) source/memoryManagerMain.cpp
	mv memoryManagerMain.o lib/memoryManagerMain.o

generator: $(OB1)
	$(COMP) -g $(OB1) -o $(EXEC1) -lrt -pthread

manager: $(OB2)
	$(COMP) -g $(OB2) -o $(EXEC2) -lrt -pthread

clean :
	rm -f $(OB) $(EXEC1) $(EXEC2)