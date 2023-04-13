#include <iostream>
#include <string>
#include <stdexcept>
#include "banker.h"

using namespace std;

// Function declarations
void terminal(Banker &banker);
void parseCommand(bool &active, Banker &banker, string command);

int main(int argc, char *argv[])
{
    Banker banker;
    // Create the banker.
    try
    {
        banker = Banker(argc, argv, "input.csv");
    }
    catch (invalid_argument &e)
    {
        cerr << e.what() << "\n";
        return -1;
    }

    // Start terminal program.
    terminal(banker);

    // End program.
    return 0;
}

void terminal(Banker &banker)
{
    bool active = true;

    // Print startup message.
    cout << "Banker Resource Allocation System\n"
         << "---------------------------------\n";
    cout << "Enter 'HELP' for list of commands.\n\n";

    while (active)
    {
        string command = "";
        cout << "$ ";
        cin >> command;
        parseCommand(active, banker, command);
        cout << "\n";
    }
}

void parseCommand(bool &active, Banker &banker, string command)
{
    if (command == "HELP")
    {
        cout << "\nList of commands:\n"
             << "HELP    - Displays this help message.\n"
             << "EXIT, q - Quits the program.\n"
             << "*       - Output current system information/status.\n"
             << "RQ      - Request resources.\n"
             << "\tex: RQ x 1 2 3 4, request 1, 2, 3, 4 units from the\n"
             << "\trespective resources, for customer x.\n"
             << "RL      - Release resources.\n"
             << "\tex: RL x 1 2 3 4, release 1, 2, 3, 4 units from the\n"
             << "\trespective resources, for customer x.\n";
    }
    else if (command == "EXIT" || command == "q")
    {
        cout << "Goodbye.\n";
        active = false;
    }
    else if (command == "*")
    {
        cout << "\n" << banker.getBankInfo();
    }
    else
    {
        cout << "Invalid command. Please try again.\n";
    }
}
