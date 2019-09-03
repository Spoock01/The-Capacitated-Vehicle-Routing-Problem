#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "../include/helper.h"
#include "../include/fileReader.h"

using namespace std;

int main()
{
    while (true)
    {
        menu(); // helper.h
        readFile(get_file_name());
        system("pause");
    }

    return 0;
}