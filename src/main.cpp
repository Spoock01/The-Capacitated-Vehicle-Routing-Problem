#include "../include/FileReader.h"
#include "../include/Graph.h"
#include "../include/Helper.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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