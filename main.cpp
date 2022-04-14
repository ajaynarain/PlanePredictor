#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

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

        vector<Flight> flights;

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
                flights.push_back(currentFlight);
            }

            else
            {
                Flight currentFlight = Flight(year, month, day, origin, destination, airline, false, true);

                // INSERT INTO DATA STRUCTURE
                flights.push_back(currentFlight);
            }
        }
    }

    // THIS IS COLE


    // Possible data structures:
    // Red-black tree
    // 

    return 0;
}
