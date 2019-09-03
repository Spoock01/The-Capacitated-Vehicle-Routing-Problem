#ifndef HELPER_H
#define HELPER_H

#include <iostream>

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

#endif