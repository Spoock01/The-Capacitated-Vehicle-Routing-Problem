#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

const string instance_path = "./instancias_teste/";

void menu()
{
    system("cls");
    cout << "The Capacited Vehicle Routing Problem" << endl;
    cout << "0 - Exit" << endl;
    cout << "1 - P-n19-k2" << endl;
    cout << "2 - P-n20-k2" << endl;
    cout << "3 - P-n23-k8" << endl;
    cout << "4 - P-n45-k5" << endl;
    cout << "5 - P-n50-k10" << endl;
    cout << "6 - P-n51-k10" << endl;
    cout << "7 - P-n55-k7" << endl;
    cout << "\nChoose one of the above: " << endl;
}

string get_file_name()
{
    int file_number = -1;
    cin >> file_number;

    switch (file_number)
    {
    case 1:
        return instance_path + "P-n19-k2.txt";
    case 2:
        return instance_path + "P-n20-k2.txt";
    case 3:
        return instance_path + "P-n23-k8.txt";
    case 4:
        return instance_path + "P-n45-k5.txt";
    case 5:
        return instance_path + "P-n50-k10.txt";
    case 6:
        return instance_path + "P-n51-k10.txt";
    case 7:
        return instance_path + "P-n55-k7.txt";
    case 0:
        exit(0);
    default:
        cout << "Invalid option! File 'P-n19-k2.txt' has been selected as the default option." << endl;
        return instance_path + "P-n19-k2.txt";
    }
}

void read_file(string file)
{
    string line;
    ifstream instance_file(file.c_str());

    if (instance_file.is_open())
    {
        while (getline(instance_file, line))
        {
            cout << line << endl;
        }
        instance_file.close();
    }
}

int main()
{
    while (true)
    {
        menu();
        read_file(get_file_name());
        system("pause");
    }

    return 0;
}