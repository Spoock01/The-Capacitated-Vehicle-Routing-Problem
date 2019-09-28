#ifndef FILE_READER_H
#define FILE_READER_H

#include <string.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../include/Demand.h"

std::string getFileName();

int getDimension();
int getVehicle();
int getCapacity();
void readFile(std::string);

#endif