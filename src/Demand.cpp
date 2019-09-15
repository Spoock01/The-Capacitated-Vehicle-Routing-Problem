#include "../include/Demand.h"

Demand::Demand(int client, int client_demand)
{
    this->_client = client;
    this->_client_demand = client_demand;
}

int Demand::getClientDemand() { return this->_client_demand; }

int Demand::getClient() { return this->_client; }