#include "../include/Helper.h"
#include <iostream>
#include <vector>

const std::string INSTANCE_PATH = "./instancias_teste/";

void menu()
{
#ifdef _WIN32
    system("cls");
#endif
#ifdef linux
    system("clear");
#endif
    std::cout << "The Capacited Vehicle Routing Problem" << std::endl;
    std::cout << "0 - Exit" << std::endl;
    std::cout << "1 - P-n19-k2" << std::endl;
    std::cout << "2 - P-n20-k2" << std::endl;
    std::cout << "3 - P-n23-k8" << std::endl;
    std::cout << "4 - P-n45-k5" << std::endl;
    std::cout << "5 - P-n50-k10" << std::endl;
    std::cout << "6 - P-n51-k10" << std::endl;
    std::cout << "7 - P-n55-k7" << std::endl;
    std::cout << "\nChoose one of the above: " << std::endl;
}

std::string getFileName()
{
    int fileNumber = -1;
    std::cin >> fileNumber;

    switch (fileNumber)
    {
    case 1:
        return INSTANCE_PATH + "P-n19-k2.txt";
    case 2:
        return INSTANCE_PATH + "P-n20-k2.txt";
    case 3:
        return INSTANCE_PATH + "P-n23-k8.txt";
    case 4:
        return INSTANCE_PATH + "P-n45-k5.txt";
    case 5:
        return INSTANCE_PATH + "P-n50-k10.txt";
    case 6:
        return INSTANCE_PATH + "P-n51-k10.txt";
    case 7:
        return INSTANCE_PATH + "P-n55-k7.txt";
    case 0:
        exit(0);
    default:
        std::cout << "Invalid option! File 'P-n19-k2.txt' has been selected as the "
                     "default option."
                  << std::endl;
        return INSTANCE_PATH + "P-n19-k2.txt";
    }
}

void print2dVector(std::vector<std::vector<int>> &vector2D)
{
    for (auto i = 0; i < (int)vector2D.size(); i++)
    {
        for (auto j = 0; j < (int)vector2D[i].size(); j++)
        {
            std::cout << vector2D[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void printVector(std::vector<int> route, bool endLine = true)
{
    for (auto i : route)
    {
        std::cout << i << " ";
    }
    if (endLine)
        std::cout << "\n";
}

void printRouteAndDistance(std::vector<int> route, int distance)
{
    std::cout << "ROUTE: { ";
    printVector(route, false);
    std::cout << "}\nDistance = " << distance << "\n\n";
}