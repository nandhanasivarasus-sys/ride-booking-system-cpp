# Ride Booking System

This is a console-based ride booking system developed in C++ as a learning project to apply Object-Oriented Programming concepts in a real-world scenario.

The program simulates how cab services work by assigning drivers based on user location and vehicle preference, calculating fares, and managing ride completion and ratings.

## Features

- User ride booking
- Driver allocation based on zone and vehicle type
- Distance calculation between zones
- Fare calculation for different vehicle categories
- Driver availability tracking
- Discount on eligible rides
- Driver rating system
- Error handling for invalid bookings

## Concepts Used

### Object-Oriented Programming
- Classes and Objects
- Inheritance
- Polymorphism
- Encapsulation
- Composition

### C++ Features
- Function Overloading
- Operator Overloading
- Templates
- Friend Functions
- Exception Handling

### STL
- Vector
- Map
- Pair

## Vehicle Types

- Bike
- Auto
- Sedan
- SUV

Each vehicle has its own fare calculation logic.

## Driver Allocation Logic

The system searches for drivers in the following order:

1. Same zone and same vehicle type
2. Same zone and different vehicle type
3. Nearby zone and same vehicle type
4. Nearby zone and different vehicle type

## How to Run

Compile:

```bash
g++ project.cpp -o ridebooking
```

Run:

```bash
./ridebooking
```

## Limitations

- Data is not stored permanently
- Supports only one booking session at a time
- No graphical interface
- Uses predefined zones and drivers

## Future Improvements

- Database integration
- Ride history storage
- GUI or web interface
- Better route optimisation
- Multiple users

## Author

Nandhana Sivarasu  
BE Computer Science and Engineering  
College of Engineering Guindy, Anna University
