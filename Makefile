CC = g++
CFLAGS = -std=c++14
INCLUDE_PATH = include/
SRC_PATH = src/
DIST_PATH = dist/
EXE = main


all: Demand.o FileReader.o Graph.o Helper.o
	@mkdir -p $(DIST_PATH)
	@$(CC) $(CFLAGS) *.o -O3 $(SRC_PATH)main.cpp -o $(DIST_PATH)$(EXE)

Demand.o: $(INCLUDE_PATH)Demand.h $(SRC_PATH)Demand.cpp
	@$(CC) $(CFLAGS) $(SRC_PATH)Demand.cpp -c

FileReader.o: $(INCLUDE_PATH)FileReader.h $(SRC_PATH)FileReader.cpp
	@$(CC) $(CFLAGS) $(SRC_PATH)FileReader.cpp -c

Graph.o: $(INCLUDE_PATH)Graph.h $(SRC_PATH)Graph.cpp
	@$(CC) $(CFLAGS) $(SRC_PATH)Graph.cpp -c

Helper.o: $(INCLUDE_PATH)Helper.h $(SRC_PATH)Helper.cpp
	@$(CC) $(CFLAGS) $(SRC_PATH)Helper.cpp -c

clean:
	@rm -r *.o
	@rm -r dist

run:
	$(DIST_PATH)$(EXE)