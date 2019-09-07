#ifndef FILE_READER_H
#define FILE_READER_H

#include "../include/Demand.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

std::string getFileName();

int getDimension();
int getVehicle();
int getCapacity();
void readFile(std::string);

#endif