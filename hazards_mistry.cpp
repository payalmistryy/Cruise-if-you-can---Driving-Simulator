/*
Payal Mistry
CS 302 - Programming Methodologies and
Software Implementation
Assignment 3 - Cruise If You Can
May 10, 2026
Filename: hazards_mistry.cpp

This file implements the three derived classes in
the "Cruise If You Can" core hierarchy: Cow, Detour,
and Pothole. Each class overrides the shared virtual
interface (encounter, avoid, describe) with its own
unique behavior, so when the game holds these objects
through an Obstacle * in the BST, dynamic binding 
dispatches the correct version. Cow also provides a 
non-virtual feed() method used to demonstrate RTTI
with dynamic_cast from the main driver. Constructors
forward their location and severity arguments to the
Obstacle base class through an initializer list and
then set their own unique data members. Destructors
are empty because no derived class owns dynamic
memory of its own.
*/


#include <string>
#include <iostream>
#include <stdexcept>
#include "hazards_mistry.h"

using namespace std;


//Cow class implementation

//Default constructor - initializes a Cow with empty
//base data, standing posture, and a generic "moo".
Cow::Cow() : Obstacle(), is_sitting(false), sound("moo")
{
}


//Parameterized constructor - forwards location and 
//severity up to Obstacle, then sets the sitting 
//state and sound for this cow.
Cow::Cow(const string & a_location, int a_severity,
         bool a_is_sitting, const string & a_sound)
    : Obstacle(a_location, a_severity),
      is_sitting(a_is_sitting), sound(a_sound)
{
}


//Destructor - empty because Cow holds no dynamic 
//memory of its own.
Cow::~Cow()
{
}


//encounter - driver runs into a cow. Score impact is
//the base severity plus a penalty if the cow is
//sitting (harder to move). The driver_speed argument
//is not used by Cow but must remain in the signature
//for dynamic binding to work.
int Cow::encounter(int driver_speed)
{
    (void)driver_speed;
    int impact = severity;
    if(is_sitting)
        impact += 5;
    return impact;
}


//avoid - driver tries to get past the cow. Returns
//true if the player honks or swerves. Any other 
//action fails.
bool Cow::avoid(const string & action)
{
    if(action == "honk" || action == "swerve")
        return true;
    return false;
}


//describe - prints a cow specific message to screen
//including location and posture.
void Cow::describe() const
{
    cout << "A cow is " << (is_sitting ? "sitting" : "standing")
         << " in the road at " << location 
         << " going \"" << sound << "\"." << endl;
}


//feed - non-virtual method used to demonstrate RTTI.
//Returns true if the cow accepts the food offered
//(hay or grass). Other foods do not work.
bool Cow::feed(const string & food)
{
    if(food == "hay" || food == "grass")
        return true;
    return false;
}


//Detour class implementation

//Default constructor - initializes a Detour with 
//empty base data, no direction, and zero extra miles.
Detour::Detour() : Obstacle(), new_direction(""), extra_miles(0)
{
}


//Parameterized constructor - forwards location and
//severity up to Obstacle, then sets the new direction
//and extra miles for this detour. Throws if extra
//miles is negative.
Detour::Detour(const string & a_location, int a_severity,
               const string & a_new_direction, int a_extra_miles)
    : Obstacle(a_location, a_severity),
      new_direction(a_new_direction), extra_miles(a_extra_miles)
{
    if(a_extra_miles < 0)
        throw invalid_argument("Detour extra miles cannot be negative");
}


//Destructor - empty because Detour holds no dynamic
//memory of its own.
Detour::~Detour()
{
}


//encounter - driver hits the detour. Score impact is
//the base severity plus the extra miles the detour
//adds to the route.
int Detour::encounter(int driver_speed)
{
    (void)driver_speed;
    return severity + extra_miles;
}


//avoid - driver tries to skip the detour. Returns
//true only if the player turns around to take a 
//different route.
bool Detour::avoid(const string & action)
{
    if(action == "turn_around")
        return true;
    return false;
}


//describe - prints a detour specific message to 
//screen including location, new direction, and how
//many extra miles the detour adds.
void Detour::describe() const
{
    cout << "ROAD CLOSED at " << location 
         << " - detour heads " << new_direction 
         << " (" << extra_miles << " extra miles)." << endl;
}


//Pothole class implementation

//Default constructor - initializes a Pothole with
//empty base data, zero depth, and no speed penalty.
Pothole::Pothole() : Obstacle(), depth_inches(0), speed_penalty(0)
{
}


//Parameterized constructor - forwards location and
//severity up to Obstacle, then sets the depth and
//speed penalty for this pothole. Throws if depth or
//penalty is negative.
Pothole::Pothole(const string & a_location, int a_severity,
                 int a_depth_inches, int a_speed_penalty)
    : Obstacle(a_location, a_severity),
      depth_inches(a_depth_inches), speed_penalty(a_speed_penalty)
{
    if(a_depth_inches < 0 || a_speed_penalty < 0)
        throw invalid_argument("Pothole depth and speed penalty cannot be negative");
}


//Destructor - empty because Pothole holds no dynamic
//memory of its own.
Pothole::~Pothole()
{
}


//encounter - driver hits the pothole. Score impact 
//is the base severity plus the speed penalty applied
//by this pothole.
int Pothole::encounter(int driver_speed)
{
    (void)driver_speed;
    return severity + speed_penalty;
}


//avoid - driver tries to dodge the pothole. Returns
//true if the player slows down or swerves around it.
bool Pothole::avoid(const string & action)
{
    if(action == "slow_down" || action == "swerve")
        return true;
    return false;
}


//describe - prints a pothole specific message to
//screen including location, depth, and the speed
//penalty drivers suffer if they hit it.
void Pothole::describe() const
{
    cout << "Cones mark a pothole at " << location 
         << " - " << depth_inches << " inches deep, costs " 
         << speed_penalty << " mph if hit." << endl;
}

