#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

#include <string>
#include <vector>

using namespace std;

class Banker
{
public:
    Banker();
    Banker(int argc, char *args[], string filename);

    // Request resources function.
    int request_resources(string command);
    int release_resources(string command);
    string getBankInfo();
    enum SafetyStatus
    {
        UNSAFE = -1,
        SAFE = 0,
        SEMISAFE = 1
    };

private:
    vector<vector<string>> read_csv(string filename);
    vector<int> splitCommand(string command);
    Banker::SafetyStatus safetyCheck(vector<int> process);
    /* the available amount of each resource */
    int available[NUMBER_OF_RESOURCES];
    int MAX_AVAILABLE[NUMBER_OF_RESOURCES];
    /*the maximum demand of each customer */
    int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
    /* the amount currently allocated to each customer */
    int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
    /* the remaining need of each customer */
    int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
};