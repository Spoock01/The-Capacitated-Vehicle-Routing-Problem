#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "../include/Helper.h"
#include "../include/FileReader.h"

using namespace std;

int main()
{
    while (true)
    {
        menu(); // helper.h
        readFile(getFileName());
        std::cin.get();
        std::cout << "Press enter to continue ...";
        std::cin.get();
    }

    return 0;
}