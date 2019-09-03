#include "../include/fileReader.h"

const std::string instancePath = "./instancias_teste/";

int dimension;
int vehicle;
int capacity;
std::ifstream instanceFile;
std::vector<Demand> demands;

std::string get_file_name()
{
    int fileNumber = -1;
    std::cin >> fileNumber;

    switch (fileNumber)
    {
    case 1:
        return instancePath + "P-n19-k2.txt";
    case 2:
        return instancePath + "P-n20-k2.txt";
    case 3:
        return instancePath + "P-n23-k8.txt";
    case 4:
        return instancePath + "P-n45-k5.txt";
    case 5:
        return instancePath + "P-n50-k10.txt";
    case 6:
        return instancePath + "P-n51-k10.txt";
    case 7:
        return instancePath + "P-n55-k7.txt";
    case 0:
        exit(0);
    default:
        std::cout << "Invalid option! File 'P-n19-k2.txt' has been selected as the default option." << std::endl;
        return instancePath + "P-n19-k2.txt";
    }
}

void split_integer()
{
    std::string line;
    for (int i = 0; i < 3; i++)
    {
        getline(instanceFile, line);
        char *token = strtok((char *)line.c_str(), " ");
        token = strtok(NULL, " ");

        switch (i)
        {
        case 0:
            dimension = std::stoi(token);
            std::cout << "Dimension " << dimension << std::endl;
            break;
        case 1:
            vehicle = std::stoi(token);
            std::cout << "Vehicle " << vehicle << std::endl;
            break;
        case 2:
            capacity = std::stoi(token);
            std::cout << "capacity " << capacity << std::endl;
            break;
        default:
            return;
        }
    }
}

void read_demands()
{
    int client, client_demand;
    for (int i = 0; i < dimension; i++)
    {
        instanceFile >> client;
        instanceFile >> client_demand;
        demands.push_back(Demand(client, client_demand));
    }
}

void read_matrix(std::vector<std::vector<int>> &weight_matrix)
{
    std::string line;
    std::cout << "size: " << weight_matrix.size() << std::endl;
    int aux = -1;
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < i + 1; j++)
        {
            // instanceFile >> aux;

            // // std::cout << "Reading " << aux << std::endl;
            // weight_matrix[i].push_back(aux);

            if (i != j)
            {
                instanceFile >> aux;
                weight_matrix[i][j] = aux;
                weight_matrix[j][i] = aux;
            }
            else
            {
                getline(instanceFile, line);
                weight_matrix[i][j] = 0;
                break;
            }
        }
    }
}

void show(std::vector<std::vector<int>> &weight_matrix)
{
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            std::cout << weight_matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void readFile(std::string file)
{
    std::string line;

    instanceFile.open(file.c_str());

    if (instanceFile.is_open())
    {
        getline(instanceFile, line); //Skipping NAME
        split_integer();             // Dimension, vehicle, capacity
        getline(instanceFile, line); //Skipping DEMAND_SECTION line
        read_demands();              // Reading DEMAND_SECTION
        std::vector<std::vector<int>> weight_matrix(dimension, std::vector<int>(dimension, -1));
        getline(instanceFile, line); //empty line
        getline(instanceFile, line); //Skipping EDGE_WEIGHT_SECTION line
        getline(instanceFile, line);
        read_matrix(weight_matrix);
        show(weight_matrix);

        instanceFile.close();
    }
}