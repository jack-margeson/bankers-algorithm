#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include "banker.h"

Banker::Banker()
{
}

Banker::Banker(int argc, char *args[], string filename)
{
    if (argc - 1 == NUMBER_OF_RESOURCES)
    {
        // Process and assign command line argument values to the "available " array
        for (int i = 0; i < argc - 1; i++)
        {
            available[i] = stoi(args[i + 1]);
        }
    }
    else
    {
        throw invalid_argument("Invalid number of resources.");
    }

    // Process and assign CSV values to the "maximum" array.
    // Additionally, init. the allocated and need arrays to 0.
    vector<vector<string>> data = this->read_csv(filename);
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
        {
            maximum[i][j] = stoi(data[i][j]);
            allocation[i][j] = 0;
            need[i][j] = 0;
        }
    }

}

vector<vector<string>> Banker::read_csv(string filename)
{
    ifstream file(filename);
    vector<vector<string>> data;

    if (!file.is_open())
    {
        cerr << "Unable to open file '" << filename << "'.\n";
    }
    else
    {
        string line;
        while (getline(file, line))
        {
            vector<string> row;
            stringstream line_stream(line);
            string cell;

            while (std::getline(line_stream, cell, ','))
            {
                row.push_back(cell);
            }

            data.push_back(row);
        }

        file.close();
    }
    return data;
}

string Banker::getBankInfo()
{
    string s = "";

    s += "Resources available:\n";
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        s += "Resource #" + to_string(i + 1) + ": ";
        s += to_string(available[i]) + "\n";
    }

    s += "Maximum resources per customer:\n";
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        s += "Customer #" + to_string(i + 1) + ": ";
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
        {
            s += to_string(maximum[i][j]) + " ";
        }
        s += "\n";
    }

    s += "Current allocated resources by customer:\n";
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        s += "Customer #" + to_string(i + 1) + ": ";
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
        {
            s += to_string(allocation[i][j]) + " ";
        }
        s += "\n";
    }

    s += "Resources still needed by customer:\n";
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        s += "Customer #" + to_string(i + 1) + ": ";
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
        {
            s += to_string(need[i][j]) + " ";
        }
        s += "\n";
    }

    return s;
}