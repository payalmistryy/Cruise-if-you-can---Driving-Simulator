/*
Payal Mistry
CS 302 - Programming Methodologies and
Software Implementation
Assignment 3 - Cruise If You Can
May 6, 2026
Filename: hazards_mistry.h

This file declares the three derived classes in the
"Cruise If You Can" core hierarchy: Cow, Detour, and
Pothole. All three inherit publicly from Obstacle and
override the same self-similar interface (encounter,
avoid, describe) so the game can store them together
in one BST and dispatch the right behavior through a
base class pointer. Each derived class adds its own
unique data members that simulates what happens when the
driver runs into one. Cow also provides a non-virtual
feed() method which is used to demonstrate RTTI 
from the application.
*/

#ifndef HAZARDS_MISTRY_H
#define HAZARDS_MISTRY_H


#include <string>
#include <iostream>
#include "obstacle_mistry.h"

using std::string;


//Class: Cow (derived)
//This class represents a cow standing in the road.
//Inherits location and severity from Obstacle and
//adds its own sitting state and sound. Cow also
//provides a non-virtual feed() method used to
//demonstrate RTTI with dynamic_cast.
class Cow : public Obstacle
{
    public:
        Cow();                                      //Default constructor - initializes a Cow w/default values
        Cow(const string & a_location,              //Parameterized constructor - creates a Cow w/given
            int a_severity,                         //location, severity, sitting state, and sound
            bool a_is_sitting,
            const string & a_sound);
        ~Cow();

        //Overrides of the shared virtual interface
        int encounter(int driver_speed);            //Driver runs into a cow - returns score impact
        bool avoid(const string & action);          //Tries to avoid the cow - "honk" or "swerve" succeed
        void describe() const;                      //Prints a cow specific description to screen

        //RTTI specific method - non-virtual, only on Cow
        bool feed(const string & food);             //Feeds the cow to make it move; true if food works

    private:
        bool is_sitting;        //true if the cow is sitting in the road, false if standing
        string sound;           //the cow's sound ("moo" or "mooo")
};


//Class: Detour (derived)
//This class represents a road closure that forces
//the driver to change direction. Inherits location
//and severity from Obstacle and adds its own new
//direction and extra miles added to the route.
class Detour : public Obstacle
{
    public:
        Detour();                                   //Default constructor - initializes a Detour w/default values
        Detour(const string & a_location,           //Parameterized constructor - creates a Detour w/given
               int a_severity,                      //location, severity, new direction, and extra miles
               const string & a_new_direction,
               int a_extra_miles);
        ~Detour();

        //Overrides of the shared virtual interface
        int encounter(int driver_speed);            //Driver hits the detour - returns score impact
        bool avoid(const string & action);          //Tries to avoid the detour - "turn_around" succeeds
        void describe() const;                      //Prints a detour specific description to screen

    private:
        string new_direction;   //direction the detour forces the driver to go (north, east, etc)
        int extra_miles;        //extra miles the detour adds to the route
};


//Class: Pothole (derived)
//This class represents a pothole in the road, marked
//by cones. Inherits location and severity from 
//Obstacle and adds its own depth in inches and the
//speed penalty applied if the driver hits it.
class Pothole : public Obstacle
{
    public:
        Pothole();                                  //Default constructor - initializes a Pothole w/default values
        Pothole(const string & a_location,          //Parameterized constructor - creates a Pothole w/given
                int a_severity,                     //location, severity, depth, and speed penalty
                int a_depth_inches,
                int a_speed_penalty);
        ~Pothole();

        //Overrides of the shared virtual interface
        int encounter(int driver_speed);            //Driver hits the pothole - return score impact
        bool avoid(const string & action);          //Tries to avoid the pothole - "slow_down" or "swerve" succeed
        void describe() const;                      //Prints a pothole specific description to screen

    private:
        int depth_inches;       //depth of the pothole in inches
        int speed_penalty;      //mph the driver loses if they hit the pothole
};


#endif