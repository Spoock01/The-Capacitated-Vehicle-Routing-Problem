#include "../include/FileReader.h"
#include "../include/Graph.h"
#include "../include/Helper.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>

int main()
{
	std::srand(time(NULL));

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