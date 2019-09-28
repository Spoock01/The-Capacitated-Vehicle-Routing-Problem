#include <time.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../include/FileReader.h"
#include "../include/Graph.h"
#include "../include/Helper.h"

int main() {
    std::srand(time(NULL));

    while (true) {
        // auto graph = Graph<int>();
        // graph.createGraph(5, 5, 3);
        // std::cout << graph;
        menu(); // helper.h
        readFile(getFileName());
        std::cin.get();
        std::cout << "Press enter to continue ...";
        std::cin.get();
    }

    return 0;
}