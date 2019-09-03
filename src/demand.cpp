#include "../include/demand.h"

Demand::Demand(int client, int client_demand)
{
    this->_client = client;
    this->_client_demand = client_demand;
}

Demand::~Demand()
{
    int i = 0;
}