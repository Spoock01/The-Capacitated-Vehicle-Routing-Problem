#ifndef HELPER_H
#define HELPER_H

#include "../include/Graph.h"
#include<vector>

void menu();
void print2dVector(std::vector<std::vector<int>> &);
void printVector(std::vector<int>, bool);
void printRouteAndDistance(std::vector<int>, int);
int getDistance(std::vector<int> route, Graph<int>& graph);
int rand_int(int range);
std::vector<std::vector<int>> splitRoutes(std::vector<int> &route);
std::vector<int> mountRoute(std::vector<int> route, std::vector<int> routeToAppend);

#endif