#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <unordered_map>

using namespace std;

// Object used to hold flight data
class Flight
{
    private:
        string year, month, day;
        string departure, arrival, airline;
        bool delayed;
        bool cancelled;

    public:
        Flight();
        Flight(string _year, string _month, string _day, string _departure, string _arrival, string _airline, bool _delayed, bool _cancelled);

        void printFlight();
};

// Default constructor
Flight::Flight()
{
    year = month = day = departure = arrival = airline = "NULL";
    delayed = cancelled = false;
}

//Flight Constructor
Flight::Flight(string _year, string _month, string _day, string _departure, string _arrival, string _airline, bool _delayed, bool _cancelled){
    year = _year;
    month = _month;
    day = _day;
    departure = _departure;
    arrival = _arrival;
    airline = _airline;
    delayed = _delayed;
    cancelled = _cancelled;
}

void Flight::printFlight()
{
    cout << year << " " << month << " " << day << " " << departure << " " << arrival;
    cout << " " << airline << " " << delayed << " " << cancelled << endl;
}

// Object used to hold airline data
class Airline
{
    private:
        string name;
        int numFlights, numFlightsDelayed, numFlightsCancelled;
        double pctDelayed, pctCancelled;

    public:
        Airline();
        Airline(string _name);
        // REMINDER: only consider positive flight delays
};

//Airline Constructor
Airline::Airline(string _name){
    name = _name;
    numFlights= 0;
    numFlightsDelayed = 0;
    numFlightsCancelled = 0;
    pctDelayed = 0.0;
    pctCancelled = 0.0;
}

struct HashNode
{   
        string key;
        Flight val;

        // Constructor
        HashNode(string _key, Flight _val) : key(_key), val(_val) {};
};


/*  REFERENCE:
    My code (Cole Hamilton) from Stepik 10.2.1 and following webpage:
    https://www.geeksforgeeks.org/implementing-hash-table-open-addressing-linear-probing-cpp/
    This webpage was used to help with the structure of the HashNode and Unordered Map classes
    as well as linear probing upon insertion and searching
*/
class UnorderedMap
{
    private:
        HashNode** map;
        int numEntries, numBuckets;
        float currentLoad;
        const float MAX_LOAD = .6;

        int hash(string str);
        void rehash();

    public:
        UnorderedMap()
        {
            numEntries = 0;
            numBuckets = 10;
            currentLoad = numEntries / numBuckets;

            // '()' is used to initialize to null
            map = new HashNode*[numBuckets]();
        }

        void insert(string key, Flight val);
        Flight getFlight(string key);
        int getEntries();
        int getSize();

        // Print map
        void printMap();
};

int UnorderedMap::hash(string str)
{
    /*
    Input string will be of the following form:
    First two chars: airline
    Remaining digits: unique ID identifying flights
    EX: AA289 -> the 289th American Airlines flight from data

    - hash() will thus use the powers of 26 method for the first two chars and then add flight number
    - to prevent all flights from clustering, we multiply the airline code by 10^order of number of flights - 1
    */

    int result = 0;

    string digits = str.substr(2, str.size() - 2);
    int temp = (str[0] * 26 + str[1]) * pow(10, digits.length() - 1);

    return result + stoi(digits);
}

void UnorderedMap::rehash()
{
    // Resize by doubling
    HashNode** temp = new HashNode*[numBuckets * 2]();

    // Rehash all values into new container
    int oldNumBuckets = numBuckets;
    numBuckets *= 2;
    for (int i = 0; i < oldNumBuckets; i++)
    {
        if (map[i] != nullptr)
        {
            // Find the next open bucket in the new container
            int index = hash(map[i]->key) % numBuckets;
            while (temp[index] != nullptr)
            {
                index++;

                // Reduce if needed
                index %= numBuckets;
            }

            // Insert
            if (temp[index] == nullptr)
                temp[index] = map[i];
        }
    }

    currentLoad = numEntries / numBuckets;

    // Copy into map variable
    delete[] map;
    map = temp;
}

void UnorderedMap::insert(string key, Flight val)
{
    // Implemented with linear probing

    // Node to be inserted
    HashNode* node = new HashNode(key, val);

    // Find the next open bucket
    int index = hash(key) % numBuckets;
    while (map[index] != nullptr && map[index]->key != key)
    {
        index++;
        
        // Reduce if needed
        index %= numBuckets;
    }

    // Insert
    if (map[index] == nullptr)
    {
        map[index] = node;
        numEntries++;
    }

    // Check if resizing and rehashing needs to occur
    HashNode** temp;
    currentLoad = numEntries / numBuckets;

    if (currentLoad >= MAX_LOAD) rehash();
}

Flight UnorderedMap::getFlight(string key)
{
    // Implemented with linear probing
    int index = hash(key) % numEntries;
    int finiteCounter = 0;

    while (map[index] != nullptr)
    {
        // Returns empty flight if the flight is not found
        if (finiteCounter++ > numBuckets)
            return Flight();

        // Return value when node is found
        if (map[index]->key == key)
            return map[index]->val;

        // Linear probing
        index++;
        index %= numBuckets;
    }

    // Return empty if not found
    return Flight();
}

int UnorderedMap::getEntries() { return numEntries; }

int UnorderedMap::getSize() { return numBuckets; }

void UnorderedMap::printMap()


{
    for (int i = 0; i < numBuckets; i++)
    {
        if (map[i] != nullptr)
        {
            cout << map[i]->key << endl;
            map[i]->val.printFlight();
        }
    }
}

int main() {

    // Read in Excel sheets to gather flight data and place in containers
    // Loop will create Flight objects to be inserted into Red-Black tree/other container
    ifstream data;

    data.open("Project_3_Dataset_Origin (1).csv");

    if (data.is_open())
    {
        // Read in first row which is column headers
        string firstLine;
        data >> firstLine;

        // Read in flight data
        string trash1, trash2;
        string year, month, day, airline, origin, destination;

        // Transfer to booleans
        string departDelay, arriveDelay, cancellation;

        // TEST DATA STRUCTURE
        // vector<Flight> flights;

        // Unordered map instantation
        UnorderedMap hashMap;
        unordered_map<string, int> flightCounters;

        while (!data.eof())
        {
            getline(data, year, ',');
            getline(data, month, ',');
            getline(data, day, ',');
            getline(data, airline, ',');
            getline(data, origin, ',');
            getline(data, destination, ',');
            getline(data, departDelay, ',');
            getline(data, arriveDelay, ',');
            getline(data, trash1, ',');
            getline(data, cancellation, ',');
            getline(data, trash2);
            
            // Create flight object based on strings
            if (flightCounters.find(airline) != flightCounters.end())
                flightCounters[airline]++;

            else
                flightCounters.insert(make_pair(airline, 0));

            string uniqueID;
            uniqueID = airline + to_string(flightCounters[airline]);

            // Check if flight is cancelled before adding delays
            if (cancellation == "0")
            {
                bool delayed, cancelled;
                cancelled = false;

                if (stoi(departDelay) > 0 && origin == "ORD") 
                    delayed = true;
                else if (stoi(departDelay) <= 0 && origin == "ORD")
                    delayed = false;
                else if (stoi(arriveDelay) > 0 && destination == "ORD")
                    delayed = true;
                else if (stoi(arriveDelay) <= 0 && destination == "ORD")
                    delayed = false;

                Flight currentFlight = Flight(year, month, day, origin, destination, airline, delayed, cancelled);
                
                // INSERT INTO DATA STRUCTURE
                hashMap.insert(uniqueID, currentFlight);
            }

            else
            {
                Flight currentFlight = Flight(year, month, day, origin, destination, airline, false, true);

                // INSERT INTO DATA STRUCTURE
                hashMap.insert(uniqueID, currentFlight);
            }
        }

        // Test
        hashMap.getFlight("AA1").printFlight();
    }

    return 0;
}