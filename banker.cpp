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
            MAX_AVAILABLE[i] = stoi(args[i + 1]);
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
        s += to_string(available[i]) + "/" + to_string(MAX_AVAILABLE[i]) + "\n";
    }

    s += "Maximum resources per customer:\n";
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        s += "Customer #" + to_string(i) + ": ";
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
        {
            s += to_string(maximum[i][j]) + " ";
        }
        s += "\n";
    }

    s += "Current allocated resources by customer:\n";
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        s += "Customer #" + to_string(i) + ": ";
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
        {
            s += to_string(allocation[i][j]) + " ";
        }
        s += "\n";
    }

    s += "Resources still needed by customer:\n";
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        s += "Customer #" + to_string(i) + ": ";
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
        {
            s += to_string(need[i][j]) + " ";
        }
        s += "\n";
    }

    return s;
}

int Banker::request_resources(string command)
{
    // Format and check request.
    vector<int> process;
    try
    {
        process = splitCommand(command);
    }
    catch (invalid_argument &e)
    {
        throw e;
        return -1;
    }
    // Handle request, check for validity.
    try
    {
        if (process.size() >= NUMBER_OF_RESOURCES + 1)
        {
            int customer = process[0];
            switch (safetyCheck(process))
            {
            case SAFE:
                // The request has been deemed safe. Process by adding to the currently allotted array.
                process.erase(process.begin());
                for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
                {
                    allocation[customer][i] += process[i];
                    available[i] -= process[i];
                }
                cout << "Customer allocation processed succesfully.\n";
                break;
            case SEMISAFE:
                // The request has been deemed semisafe. Process by adding to the currently allotted array.
                process.erase(process.begin());
                for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
                {
                    need[customer][i] += process[i];
                }
                cout << "Customer allocation greater than maximum. Process added to needed resource queue.\n";
                break;
            default:
                throw invalid_argument("An error has occured.");
                break;
            }
        }
        else
        {
            throw invalid_argument("Invalid number of arguments for request/release.");
        }
    }
    catch (invalid_argument &e)
    {
        throw e;
        return -1;
    }

    return 0;
}

int Banker::release_resources(string command)
{
    // Format and check request.
    vector<int> process;
    try
    {
        process = splitCommand(command);
    }
    catch (invalid_argument &e)
    {
        throw e;
        return -1;
    }
    // Handle request, check for validity.
    try
    {
        if (process.size() >= NUMBER_OF_RESOURCES + 1)
        {
            int customer = process[0];
            // Change the resource values to be negative.
            for (int i = 1; i < NUMBER_OF_RESOURCES + 1; i++)
            {
                process[i] = process[i] * -1;
            }
            switch (safetyCheck(process))
            {
            case SAFE:
            case SEMISAFE:
                // The request has been deemed safe. Process by adding to the currently allotted array.
                process.erase(process.begin());
                for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
                {
                    allocation[customer][i] += process[i];
                    available[i] -= process[i];
                    // Check if the resource is negative. If so, set to 0.
                    if (allocation[customer][i] < 0)
                    {
                        allocation[customer][i] = 0;
                    }
                    // Check if the available resources is greater than their max. If so, set to their max.
                    if (available[i] > MAX_AVAILABLE[i])
                    {
                        available[i] = MAX_AVAILABLE[i];
                    }
                }
                cout << "Customer deallocation processed succesfully.\n";
                break;
            default:
                throw invalid_argument("An error has occured.");
                break;
            }
        }
        else
        {
            throw invalid_argument("Invalid number of arguments for request/release.");
        }
    }
    catch (invalid_argument &e)
    {
        throw e;
        return -1;
    }

    return 0;
}

vector<int> Banker::splitCommand(string command)
{
    istringstream iss(command);
    vector<int> vecCommand;

    char c;
    iss >> c >> c;

    int num;
    while (iss >> num)
    {
        vecCommand.push_back(num);
    }

    // Check if this is a valid customer id.
    if (vecCommand.size() != 0 && vecCommand[0] > NUMBER_OF_CUSTOMERS - 1)
    {
        throw invalid_argument("Invalid customer ID.");
    }

    // Check if this is a valid number of resources.
    if (vecCommand.size() != 0 && vecCommand.size() != (NUMBER_OF_RESOURCES + 1))
    {
        throw invalid_argument("Invalid number of arguments for request/release.");
    }

    return vecCommand;
}

Banker::SafetyStatus Banker::safetyCheck(vector<int> process)
{
    Banker::SafetyStatus safe = SAFE;

    int customer = process[0];
    process.erase(process.begin());

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        // Check if the request is less than the maximum amount of the resource for the customer,
        // and if the request is less than the maximum amount of the resource available total.
        if (process[i] > maximum[customer][i] || process[i] > MAX_AVAILABLE[i])
        {
            // This sort of request will never be safe.
            safe = UNSAFE;
            break;
        }
        // Check if the request in addition to the current allocation
        // is less than the maximum demand from the customer, and that it is
        // less than the total available of that resource.
        if (process[i] + allocation[customer][i] > maximum[customer][i] || process[i] + allocation[customer][i] > available[i])
        {
            // This sort of request could be safe, if the current allocation
            // to the customer(s) were to be freed. We'll mark this as "semisafe"
            // and let the caller determine what should be done with the request
            // (adding to the need array).
            safe = SEMISAFE;
        }
    }

    if (safe == UNSAFE)
    {
        throw invalid_argument("Operation canceled: the specified request/release will never pass the safety check.");
    }
    return safe;
}