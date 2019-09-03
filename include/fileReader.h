#ifndef FILE_READER_H
#define FILE_READER_H

#include <iostream>
#include <string>
#include <fstream>
#include <string.h>
#include <vector>
#include "../include/demand.h"

std::string get_file_name();

int getDimension();
int getVehicle();
int getCapacity();
void readFile(std::string);

#endif