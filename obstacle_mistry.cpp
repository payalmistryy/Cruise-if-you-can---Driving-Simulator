/*
Payal Mistry
CS 302 - Programming Methodologies and
Software Implementation
Assignment 3 - Cruise If You Can
May 10, 2026
Filename: obstacle_mistry.cpp

This file implements the Obstacle abstract base class
for the "Cruise If You Can" driving game. Obstacle 
cannot be instantiated directly because describe() is
pure virtual, but every constructor, destructor, the 
assignment operator, the two regular virtual methods
(encounter, avoid), and the two accessors must still
be defined so derived classes (Cow, Detour, Pothole)
can invoke them when constructing, assigning, or
falling back to default behavior. The two virtual
methods provide a baseline implementation that derived
classes override with their own unique behavior.
*/

#include <string>
#include <iostream>
#include <stdexcept>
#include "obstacle_mistry.h"

using namespace std;


//Default constructor - initializes an Obstacle with
//empty location and severity of zero. Derived class
//default constructors call this implicitly.
Obstacle::Obstacle() : location(""), severity(0)
{
}


//Parameterized constructor - initializes an Obstacle
//with the given location string and severity rating.
//Throws invalid_argument if severity is out of the
//expected 1-10 range.
Obstacle::Obstacle(const string & a_location, int a_severity)
    : location(a_location), severity(a_severity)
{
    if(a_severity < 1 || a_severity > 10)
        throw invalid_argument("Obstacle severity must be between 1 and 10");
}


//Copy constructor - deep copies the source obstacle's
//data members. Used when an Obstacle is passed by 
//value or when derived class copy constructors call
//up to the base.
Obstacle::Obstacle(const Obstacle & source)
    : location(source.location), severity(source.severity)
{
}


//Virtual destructor - empty because Obstacle holds
//no dynamic memory of its own. Declared virtual in
//the header so derived destructors run correctly
//when delete is called on an Obstacle pointer.
Obstacle::~Obstacle()
{
}


//Assignment operator - deep copies rhs into this
//obstacle. Checks for self-assignment first. Returns
//a reference so chained assignment works.
Obstacle & Obstacle::operator=(const Obstacle & rhs)
{
    if(this != &rhs)
    {
        location = rhs.location;
        severity = rhs.severity;
    }
    return *this;
}


//encounter - baseline behavior when the driver hits
//an obstacle. Returns the severity as the default
//score impact. Derived classes override to add their
//own unique impact (speed loss, extra miles, etc).
//The driver_speed argument is unused at the base
//level but derived classes may use it.
int Obstacle::encounter(int driver_speed)
{
    (void)driver_speed;     //suppress unused parameter warning at base level
    return severity;
}


//avoid - baseline behavior when the driver tries to
//avoid an obstacle. Returns false by default since
//no specific action is recognized at the base level.
//Derived classes override to recognize their own
//valid avoidance actions ("honk", "turn_around", etc).
bool Obstacle::avoid(const string & action)
{
    (void)action;           //suppress unused parameter warning at base level
    return false;
}


//get_location - returns the obstacle's location 
//string. Used by the BST as the search key.
string Obstacle::get_location() const
{
    return location;
}


//get_severity - returns the obstacle's severity 
//rating on a 1-10 scale.
int Obstacle::get_severity() const
{
    return severity;
}

