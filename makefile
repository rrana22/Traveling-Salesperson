CC = g++
LFLAGS = -std=c++11
CFLAGS = -c

# EXECUTABLE_NAME is the name of the program to run
EXECUTABLE_NAME1 = greedytsp
SOURCES1 = greedyMain.cpp CountryClass.cpp GreedyTSP.cpp

EXECUTABLE_NAME2 = nearinsertion
SOURCES2 = nearInsertionMain.cpp CountryClass.cpp NearestInsertion.cpp

EXECUTABLE_NAME3 = nearneighbor
SOURCES3 = nearNeighborMain.cpp CountryClass.cpp NearestNeighbor.cpp

OBJECTS1 = $(SOURCES1:.cpp=.o)
OBJECTS2 = $(SOURCES2:.cpp=.o)
OBJECTS3 = $(SOURCES3:.cpp=.o)

all: $(EXECUTABLE_NAME1) $(EXECUTABLE_NAME2) $(EXECUTABLE_NAME3)

$(EXECUTABLE_NAME1): $(OBJECTS1)
	$(CC) -o $(EXECUTABLE_NAME1) $(OBJECTS1)

$(EXECUTABLE_NAME2): $(OBJECTS2)
	$(CC) -o $(EXECUTABLE_NAME2) $(OBJECTS2)

$(EXECUTABLE_NAME3): $(OBJECTS3)
	$(CC) -o $(EXECUTABLE_NAME3) $(OBJECTS3)

.cpp.o:
	$(CC) $(CFLAGS) $(LFLAGS) $< -o $@

clean:
	$(RM) *.o *~ $(EXECUTABLE_NAME1)
	$(RM) *.o *~ $(EXECUTABLE_NAME2)
	$(RM) *.o *~ $(EXECUTABLE_NAME3)


