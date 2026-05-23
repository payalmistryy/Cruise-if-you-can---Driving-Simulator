/*
Payal Mistry
CS 302 - Programming Methodologies and
Software Implementation
Assignment 3 - Cruise If You Can
May 5, 2026
Filename: obstacle_mistry.h

This file declares the Obstacle abstract base class
which sits at the root of the "Cruise If You Can"
driving game's core hierarchy. An Obstacle represents
something the driver might encounter on the road and
holds the data common to every type of obstacle 
(location, severity). The three derived classes
include Cow, Detour, and Pothole. Because the game
stores a collection of obstacles in a single BST. 
Obstacle declares a virtual interface (encounter,avoid, 
describe) so dynamic binding can dispatch the right behavior
through a base class pointer.
*/

#ifndef OBSTACLE_MISTRY_H
#define OBSTACLE_MISTRY_H


#include <string>
#include <iostream>

using std::string;


//Obstacle class (abstract base class)
//This class holds data common to every obstacle
//(location, severity) and declares the shared
//virtual interface that every derived obstacle
//must implement. describe() is pure virtual which
//makes Obstacle abstract. The destructor is virtual
//so derived destructors run correctly when an
//Obstacle * is deleted.
class Obstacle
{
    public:
        //Constructors and destructor
        Obstacle();                                 //Default constructor - initializes an Obstacle
        Obstacle(const string & a_location,         //Parameterized constructor - creates an Obstacle 
                 int a_severity);                   //with given location and severity (1-10)
        Obstacle(const Obstacle & source);          //Copy constructor - deep copy of source
        virtual ~Obstacle();                        //Virtual destructor - allows proper cleanup through base pointer

        //Assignment operator
        Obstacle & operator=(const Obstacle & rhs);

        //Shared virtual interface - same signature for
        //all derived classes so dynamic binding works
        virtual int encounter(int driver_speed);            //Player hits the obstacle - returns score impact
        virtual bool avoid(const string & action);          //Player tries to avoid - returns true if action worked
        virtual void describe() const = 0;                  //Pure virtual - prints obstacle to screen (different per type)

        //Accessors - read-only access to base data members
        string get_location() const;                //Returns the location string (used as BST key)
        int get_severity() const;                   //Returns the severity rating (1-10)

    protected:
        string location;        //where on the course the obstacle appears (also used as BST key)
        int severity;           //how badly the obstacle impacts the driver, scale 1-10
};


#endif


