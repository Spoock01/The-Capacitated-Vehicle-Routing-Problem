CC = g++
CFLAGS = -std=c++14 -Wall -Werror -Wextra -pedantic -Wpedantic -Wunused -Wshadow -Ofast
INCLUDE_PATH = include/
SRC_PATH = src/
DIST_PATH = dist/
EXE = main


all: Demand.o FileReader.o Graph.o Helper.o ConstructiveHeuristic.o MovementHeuristic.o
	@mkdir -p $(DIST_PATH)
	@$(CC) $(CFLAGS) *.o $(SRC_PATH)main.cpp -o $(DIST_PATH)$(EXE)

Demand.o: $(INCLUDE_PATH)Demand.h $(SRC_PATH)Demand.cpp
	@$(CC) $(CFLAGS) $(SRC_PATH)Demand.cpp -c

FileReader.o: $(INCLUDE_PATH)FileReader.h $(SRC_PATH)FileReader.cpp
	@$(CC) $(CFLAGS) $(SRC_PATH)FileReader.cpp -c

Graph.o: $(INCLUDE_PATH)Graph.h

Helper.o: $(INCLUDE_PATH)Helper.h $(SRC_PATH)Helper.cpp
	@$(CC) $(CFLAGS) $(SRC_PATH)Helper.cpp -c

ConstructiveHeuristic.o: $(INCLUDE_PATH)ConstructiveHeuristic.h $(SRC_PATH)ConstructiveHeuristic.cpp
	@$(CC) $(CFLAGS) $(SRC_PATH)ConstructiveHeuristic.cpp -c

MovementHeuristic.o: $(INCLUDE_PATH)MovementHeuristic.h $(SRC_PATH)MovementHeuristic.cpp
	@$(CC) $(CFLAGS) $(SRC_PATH)MovementHeuristic.cpp -c

clean:
	@rm -r *.o
	@rm -r dist

run:
	$(DIST_PATH)$(EXE)