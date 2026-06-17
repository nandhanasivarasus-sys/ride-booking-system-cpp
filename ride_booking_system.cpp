/*
------------------------------------------------------------
PROJECT TITLE: Ride Booking System Simulation (C++)

PROBLEM STATEMENT:
To design and implement a console-based ride booking system that simulates
real-world cab services by allocating drivers based on user preferences,
calculating fares, and managing ride lifecycle.

------------------------------------------------------------
APPROACH / AUTOMATION LOGIC:

1. USER INPUT:
   - Accept user details: name, pickup location, vehicle type.
   - Convert input to uniform format (case handling).
   - Map location to predefined zones (A–E).

2. DRIVER MANAGEMENT:
   - Drivers are preloaded with zone and vehicle type.
   - Stored using STL vector (dynamic storage).
   - Allocation logic prioritizes:
        1. Same zone + same vehicle
        2. Same zone + different vehicle
        3. Nearby zone + same vehicle
        4. Nearby zone + different vehicle

3. DISTANCE CALCULATION:
   - Uses STL map with pair keys for zone-to-zone distance.
   - Handles symmetric lookup (A→B = B→A).
   - Returns -1 for invalid routes.

4. OBJECT-ORIENTED DESIGN:
   - Inheritance: Vehicle → bike, Auto, sedan, suv
   - Polymorphism: Virtual fare() overridden in SUV
   - Composition: Ride HAS-A User, Driver, Vehicle
   - Encapsulation: Private members with controlled access

5. MEMORY MANAGEMENT:
   - Dynamic allocation using pointers (new).
   - Drivers and Vehicles stored via pointers.

6. FARE CALCULATION:
   - Base fare = distance × rate/km
   - SUV adds extra charge
   - Discount applied using operator overloading ( -= )

7. ERROR HANDLING:
   - Namespace used for exception handling
   - Handles:
        - Invalid routes
        - Same pickup & drop
   - try-catch prevents program crash

8. ADDITIONAL FEATURES:
   - Template function to track maximum fare
   - Random user ID generation using rand()
   - Rating system using function overloading
   - Typecasting using static_cast

9. PROGRAM FLOW:
   - Loop runs for multiple bookings
   - Driver marked unavailable during ride
   - Reset after ride completion
   - User can rate driver
   - Loop continues until user exits

------------------------------------------------------------
KEY CONCEPTS USED:
- Classes & Objects
- Inheritance and Polymorphism
- Function & Operator Overloading
- Templates
- STL (vector, map, pair)
- Exception Handling
- Dynamic Memory Allocation
- Pointers and Object Linking

------------------------------------------------------------
LIMITATIONS:
- Single-user (no concurrency)
- No persistent storage (data lost after execution)
- Memory not freed explicitly (can be improved)

------------------------------------------------------------
*/


#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
using namespace std;
int C = 0;

string zoneA[] = {"annanagar", "mogappair", "ambattur"};
string zoneB[] = {"tnagar", "adyar", "velachery"};
string zoneC[] = {"guindy", "saidapet", "nandanam"};
string zoneD[] = {"tambaram", "chrompet", "pallavaram"};
string zoneE[] = {"porur", "poonamallee", "avadi"};

template <typename T>       //template function to get max fare of the day
T getMax(T a, T b) {
    return (a > b) ? a : b;
}
namespace ErrorHandling {               //namespace for error handling of same pickup-drop location and invalid distance

    void checksame(bool a) {
        if(a)
            throw "Pickup and drop area are same !";
    }

    void checkdist(int d) {
        if(d == -1)
            throw "Invalid route / distance not found";
    }
}

map<pair<string,string>, int> distMap = {                   //stl map to store distances between zones
    {{"A","B"},10}, {{"A","C"},15}, {{"A","D"},20},
    {{"B","C"},12}, {{"B","D"},18},
    {{"C","D"},8},  {{"C","E"},25},
    {{"D","E"},14},
    {{"B","E"},30}, {{"A","E"},35}
};
int calcdistance(string start, string end) {
    if(start == end)
        return 5;
    else{
         if (distMap.count({start, end}))
            return distMap[{start, end}];
        else if (distMap.count({end, start}))
            return distMap[{end, start}];
        else
            return -1;
    }   
}
    
int generateUserId() {                              //random user id generator using rand() function and time seeding to ensure different ids on each run
    static bool seeded = false;
    if (!seeded) {
        srand(time(0));
        seeded = true;
    }
    return 1000 + rand() % 9000; 
}

string getZone(string a) {

    for (int i = 0; i < 3; i++) {
        if (a == zoneA[i]) return "A";
        if (a == zoneB[i]) return "B";
        if (a == zoneC[i]) return "C";
        if (a == zoneD[i]) return "D";
        if (a == zoneE[i]) return "E";
    }

    return "Pickup point not included";
}
class User;                 //forward declaration
class Driver;
class Ride;

class Vehicle {
protected:
    string type;
    float ratekm;

public:
    Vehicle(string t, float r) : type(t), ratekm(r) {}

    virtual float fare(float distance) {            //virtual function for fare calculation
        return distance * ratekm;
    }

    string getType() {
        return type;
    }
    virtual ~Vehicle() {}                           
};
//derived classes for different vehicle types with specific fare rates

class bike : public Vehicle {
    public:
        bike() : Vehicle("BIKE", 10) {}
};

class Auto : public Vehicle {
    public:
        Auto() : Vehicle("AUTO", 15) {}
};

class sedan : public Vehicle {
    public:
        sedan() : Vehicle("SEDAN", 20) {}
};

class suv : public Vehicle {
public:
    suv() : Vehicle("SUV", 30) {}

    float fare(float distance) override {                       //overridden virtual fare function for SUV to include extra charge
        return distance * ratekm + 50; // extra charge
    }
};



class Driver {
    int driverId;
    string name;
    string zone;
    bool available;
    Vehicle* vehicle;
    float totalRating = 0;
    int ratingCount = 0;
    float rate;

public:
    Driver() {          //default constructor with default values and incrementing driver count
        driverId = 0;
        name = "Unknown";
        zone = "None";
        vehicle = nullptr;
        available = true;
        rate = 0;
        C++;
    }

    Driver(int id, string n, string z, Vehicle* v)                  //parameterized constructor to initialize driver details and increment driver count
        : driverId(id), name(n), zone(z), vehicle(v), available(true), rate(0) {
        C++;
    }

    Driver(const Driver &d) {                           //copy constructor to create a new driver object from an existing one and increment driver count
        driverId = d.driverId;
        name = d.name;
        zone = d.zone;
        vehicle = d.vehicle;   
        available = d.available;
        rate = d.rate;
        C++;
    }

    bool isAvailable() {
        return available;
    }

    void setAvailability(bool status) {
        available = status;
    }

    string getZone() {
        return zone;
    }
    
    string getName() {
        return name;
    }

    int getId() {
        return driverId;
    }

    Vehicle* getVehicle() {
        return vehicle;
    }

    void display() {
        cout << "Driver: " << name << " (ID: " << driverId << ") - Vehicle: " << vehicle->getType() << endl;
    }

    ~Driver() {
        C--;
    }
    friend void rating(Driver &d, int r);
    friend void rating(Driver &d, double r);
};

//function overloading of a friend function 

void rating(Driver &d, int r){
    d.totalRating += r;
    d.ratingCount++;

    float avg = d.totalRating / d.ratingCount;
    cout<<endl;
    cout << d.getName() << " has an average rating of " << avg << " stars\n";
}

void rating(Driver &d, double r){
    d.totalRating += r;
    d.ratingCount++;

    float avg = d.totalRating / d.ratingCount;
    cout<<endl;
    cout << d.getName() << " has an average rating of " << avg << " stars\n";
}

class User {
    int id;
    string name;
    string zone;

public:
    User(int i, string n, string z) : id(i), name(n), zone(z) {}

    string getZone() {
        return zone;
    }
    
};


class Ride {
    User* user;
    Driver* driver;
    Vehicle* vehicle;
    float distance;
    float fare;

public:
    Ride(User* u, Driver* d, float dist)
        : user(u), driver(d), distance(dist) {
        vehicle = d->getVehicle();
        fare = vehicle->fare(distance);
    }
    float getFare() {
        return fare;
    }   
    void operator -=(double discount) {             //operator overloading for discount
        fare -= discount; 
    }
    void display() {
        cout << "Ride: " << vehicle->getType()<<endl<< "Fare: " << fare << endl;
    }
};


class RideManager {
    vector<Driver*> drivers;                //stl vector to store driver objects and manage them

public:
    void addDriver(Driver* d) {
        drivers.push_back(d);                   
    }
    Driver* findDriver(string zone, string vehicleType) {

    // 1. Exact match
    for (int i = 0; i < drivers.size(); i++) {
        if (drivers[i]->isAvailable() &&
            drivers[i]->getZone() == zone &&
            drivers[i]->getVehicle()->getType() == vehicleType) {
            return drivers[i];
        }
    }

    // 2. Same zone, other vehicles
    for (int i = 0; i < drivers.size(); i++) {
        if (drivers[i]->isAvailable() &&
            drivers[i]->getZone() == zone &&
            drivers[i]->getVehicle()->getType() != vehicleType) {
            
            cout << "\nOther vehicle options available in your zone are:\n";
            cout << "Assigning nearest available driver...\n";
            return drivers[i];
        }
    }

    // 3. Nearby zones, same vehicle
    for (int i = 0; i < drivers.size(); i++) {
        if (drivers[i]->isAvailable() &&
            drivers[i]->getZone() != zone &&
            drivers[i]->getVehicle()->getType() == vehicleType) {
            
            cout << "\nYour preferred vehicle available in nearby zones:\n";
            cout << "Assigning nearest available driver...\n";
            return drivers[i];
        }
    }

    // 4. Nearby zones, other vehicles
    for (int i = 0; i < drivers.size(); i++) {
        if (drivers[i]->isAvailable() &&
            drivers[i]->getZone() != zone &&
            drivers[i]->getVehicle()->getType() != vehicleType) {
            
            cout << "\nOther vehicle options in nearby zones:\n";
            cout << "Assigning nearest available driver...\n";
            return drivers[i];
        }
    }

    return nullptr;
}

    ~RideManager() {
    for (auto d : drivers) {
        delete d->getVehicle(); // delete vehicle first
        delete d;               // then delete driver
    }
}
};

int main() {
    RideManager manager;                    

    int did = 100;
    int rounded;
    char ch,c,res;
    float rate;

    // Adding drivers to the system with different zones and vehicle types

    manager.addDriver(new Driver(did++, "Ravi_Bike_A", "A", new bike()));
    manager.addDriver(new Driver(did++, "Arun_Auto_A", "A", new Auto()));
    manager.addDriver(new Driver(did++, "Karthik_Sedan_A", "A", new sedan()));
    manager.addDriver(new Driver(did++, "Suresh_SUV_A", "A", new suv()));

    manager.addDriver(new Driver(did++, "Mani_Bike_B", "B", new bike()));
    manager.addDriver(new Driver(did++, "Vignesh_Auto_B", "B", new Auto()));
    manager.addDriver(new Driver(did++, "Prakash_Sedan_B", "B", new sedan()));
    manager.addDriver(new Driver(did++, "Dinesh_SUV_B", "B", new suv()));

    manager.addDriver(new Driver(did++, "Ajay_Bike_C", "C", new bike()));
    manager.addDriver(new Driver(did++, "Santhosh_Auto_C", "C", new Auto()));
    manager.addDriver(new Driver(did++, "Ramesh_Sedan_C", "C", new sedan()));
    manager.addDriver(new Driver(did++, "Gokul_SUV_C", "C", new suv()));

    manager.addDriver(new Driver(did++, "Hari_Bike_D", "D", new bike()));
    manager.addDriver(new Driver(did++, "Bala_Auto_D", "D", new Auto()));
    manager.addDriver(new Driver(did++, "Kumar_Sedan_D", "D", new sedan()));
    manager.addDriver(new Driver(did++, "Vijay_SUV_D", "D", new suv()));

    manager.addDriver(new Driver(did++, "Naveen_Bike_E", "E", new bike()));
    manager.addDriver(new Driver(did++, "Deepak_Auto_E", "E", new Auto()));
    manager.addDriver(new Driver(did++, "Mohan_Sedan_E", "E", new sedan()));
    manager.addDriver(new Driver(did++, "Ashok_SUV_E", "E", new suv()));

    float maxToday = 0;

    do{
        string name,area,drop,v;
        cout<<"----WELCOME----"<<endl;
        cout<<endl;
        cout<<"Please enter name:  ";
        cin.ignore();
        getline(cin, name);

        int id=generateUserId();
        cout<<endl;
        cout<<"Enter your pickup area: ";
        cin>>area;
        for (char &i : area)                    //converting user input to lowercase for case-insensitive comparison
            i = tolower(i);

        cout<<endl;
        cout<<"Enter your choice of commute (bike/auto/sedan/suv) : ";
        cin>>v;
        for (char &i : v)
            i = toupper(i);           //converting user input to uppercase for case-insensitive comparison with vehicle types stored in uppercase       

        string zone=getZone(area);
        User u(id,name,zone);
        Driver *d=manager.findDriver(zone,v);

        cout<<endl;

        if(d==nullptr){
            cout<<"Sorry! Currently no drivers are available in your zone. "<<endl;
            cout<<"\nDo you want to retry booking other options - (y/n) ? ";
            cin>>ch;
            switch(ch){
                case 'y':
                    continue;
                case 'n':
                    cout<<"Thank you for visiting! "<<endl;
                    c='n';
                    break;
                default:
                    cout<<"Invalid choice ! Try again "<<endl;            
            }
        }    
        else{
            cout<<endl;

            cout<<"Driver Found!"<<endl;
            cout<<"Name: "<<d->getName()<<"\nDriver ID: "<<d->getId()<<"\nVehicle type: "<< d->getVehicle()->getType() << endl;
            d->setAvailability(false);

            cout<<endl;
            cout<<"Enter your drop area: ";
            cin>>drop;
            for (char &i : drop)                            //converting user input to lowercase for case-insensitive comparison with zones stored in lowercase
                i = tolower(i);
            string dropzone=getZone(drop);

            try{                                                 //try-catch block to handle errors related to invalid distance and same pickup-drop location
                float distance=calcdistance(zone,dropzone);

                if(distance==-1)
                    ErrorHandling::checkdist(distance);         
                else if(area==drop){
                    bool flag=true;
                    ErrorHandling::checksame(flag);   
            }    
                Ride r(&u,d,distance);
                r.display();

                if(r.getFare()>100){
                    cout<<"\nYou are eligible to avail a discount of 10%! "<<endl;
                    r-=r.getFare()*0.1;                         //operator overloading for discount
                    r.display();
                }
                maxToday = getMax(maxToday, r.getFare());       //template function to get max fare of the day
                cout << "\nHighest fare so far today: " << maxToday << endl;

                cout << "\nRide in progress...\n";
                cout << "\nPress any key to complete ride...";
                cin >> ch;

                cout << "\nRide completed!\n";
                d->setAvailability(true);                           //setting driver availability back to true after ride completion
                
                cout<<"\nWould you like to rate your ride (y/n) ? "<<endl;
                cin>>res;

                switch(res){

                    case 'y':
                        cout<<"\nPlease leave a rating below for your driver - "<<d->getName()<<endl;
                        cout<<"\nRating (/5): ";
                        cin>>rate;
                        
                        rounded = static_cast<int>(rate);                       //typecasting to int (static_cast)

                        cout << "\nRounded rating: " << rounded << endl;

                        rating(*d,rate);
                        cout<<"\nThank you for your feedback! "<<endl;
                        break;

                    case 'n':
                        cout<<"\nRate us some other time! \nThank you for your booking!"<<endl;
                        break;
                    default:

                        cout<<"\nInvalid choice ! Try again "<<endl;        
                        
                }

            }
            catch(const char* msg) {                    //catch block to handle exceptions
                cout << "\nError: " << msg << endl;
            }
            
        }
        cout<<"\nDo you want to book another ride? (y/n) : ";
        cin>>c;
    }while(c=='y' || c=='Y'); 

    return 0;
}
