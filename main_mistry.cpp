/*
Payal Mistry
CS 302 - Programming Methodologies and
Software Implementation
Assignment 3 - Cruise If You Can
May 6, 2026
Filename: main_mistry.cpp
//
Menu driven driver for the final "Cruise If You Can"
submission. Uses a BST keyed on obstacle location to
store every obstacle on the driving course, and lets
the user add Cow, Detour, or Pothole obstacles to the
course, remove obstacles, display them in sorted 
order, search by location, drive through the course
applying each obstacle's behavior through dynamic
binding, and feed a cow to exercise RTTI with
dynamic_cast. All BST operations and obstacle
constructors that can throw are wrapped in try/catch
blocks so the menu recovers gracefully from bad input
instead of crashing. The BST owns the obstacles it
holds, so cleanup happens automatically when the BST
destructor runs at program exit.
*/

#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include "obstacle_mistry.h"
#include "hazards_mistry.h"
#include "bst_mistry.h"

using namespace std;


//Forward declarations of menu helpers and obstacle
//builders. Each menu action gets its own function so
//main itself stays short and readable.
void show_menu();
int read_int(const string & prompt);
string read_line(const string & prompt);
void add_obstacle(BST & course);
Cow * build_cow();
Detour * build_detour();
Pothole * build_pothole();
void remove_obstacle(BST & course);
void search_obstacle(const BST & course);
void drive_course(BST & course);
void drive_one(Obstacle * an_obstacle);
void feed_a_cow(BST & course);
void clear_course(BST & course);


int main()
{
    BST course;     //the driving course - a BST of Obstacle pointers
    int choice = 0;

    cout << "Welcome to Cruise If You Can!" << endl;
    cout << "Build a driving course, then try to make it through." << endl << endl;

    do
    {
        show_menu();
        choice = read_int("Enter your choice: ");

        switch(choice)
        {
            case 1:
                add_obstacle(course);
                break;
            case 2:
                remove_obstacle(course);
                break;
            case 3:
                cout << endl << "The course:" << endl;
                course.display();
                break;
            case 4:
                search_obstacle(course);
                break;
            case 5:
                drive_course(course);
                break;
            case 6:
                feed_a_cow(course);
                break;
            case 7:
                clear_course(course);
                break;
            case 8:
                cout << "Thanks for playing!" << endl;
                break;
            default:
                cout << "Invalid choice. Please enter 1 through 8." << endl;
                break;
        }
        cout << endl;
    }
    while(choice != 8);

    return 0;
}


//show_menu - prints the main menu options.
void show_menu()
{
    cout << "=== Main Menu ===" << endl;
    cout << "1. Add an obstacle to the course" << endl;
    cout << "2. Remove an obstacle from the course" << endl;
    cout << "3. Display the entire course" << endl;
    cout << "4. Search for an obstacle by location" << endl;
    cout << "5. Drive the course" << endl;
    cout << "6. Feed a cow (RTTI demo)" << endl;
    cout << "7. Clear the entire course" << endl;
    cout << "8. Quit" << endl;
}


//read_int - prompts the user and reads an integer.
//Validates that the input was actually a number; on
//bad input clears the stream state and retries.
//Always leaves the input buffer in a clean state.
int read_int(const string & prompt)
{
    int value = 0;
    while(true)
    {
        cout << prompt;
        if(cin >> value)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a number." << endl;
    }
}


//read_line - prompts the user and reads a full line
//of text (handles multi word inputs like locations
//or sound effects).
string read_line(const string & prompt)
{
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}


//add_obstacle - prompts the user to pick which type
//of obstacle to add, builds the right derived object,
//and inserts it into the BST. Catches exceptions
//from the obstacle constructor (bad severity, etc)
//and from the BST insert (null pointer).
void add_obstacle(BST & course)
{
    cout << endl << "What kind of obstacle?" << endl;
    cout << "  1. Cow" << endl;
    cout << "  2. Detour" << endl;
    cout << "  3. Pothole" << endl;
    int kind = read_int("Choice: ");

    Obstacle * an_obstacle = nullptr;
    try
    {
        switch(kind)
        {
            case 1:
                an_obstacle = build_cow();
                break;
            case 2:
                an_obstacle = build_detour();
                break;
            case 3:
                an_obstacle = build_pothole();
                break;
            default:
                cout << "Invalid obstacle type." << endl;
                return;
        }

        if(course.insert(an_obstacle))
            cout << "Obstacle added to the course." << endl;
        else
        {
            cout << "An obstacle already exists at that location." << endl;
            delete an_obstacle;     //BST rejected it, we own the cleanup
        }
    }
    catch(const invalid_argument & e)
    {
        cout << "Could not add obstacle: " << e.what() << endl;
        delete an_obstacle;
    }
}


//build_cow - prompts for cow specific fields and
//returns a new Cow on the heap. Caller takes
//ownership.
Cow * build_cow()
{
    string location = read_line("  Location: ");
    int severity = read_int("  Severity (1-10): ");
    int sitting_choice = read_int("  Is the cow sitting? (1 = yes, 0 = no): ");
    bool is_sitting = (sitting_choice != 0);
    string sound = read_line("  Cow sound: ");
    return new Cow(location, severity, is_sitting, sound);
}


//build_detour - prompts for detour specific fields
//and returns a new Detour on the heap. Caller takes
//ownership.
Detour * build_detour()
{
    string location = read_line("  Location: ");
    int severity = read_int("  Severity (1-10): ");
    string direction = read_line("  Detour direction: ");
    int extra_miles = read_int("  Extra miles added: ");
    return new Detour(location, severity, direction, extra_miles);
}


//build_pothole - prompts for pothole specific fields
//and returns a new Pothole on the heap. Caller takes
//ownership.
Pothole * build_pothole()
{
    string location = read_line("  Location: ");
    int severity = read_int("  Severity (1-10): ");
    int depth = read_int("  Depth in inches: ");
    int penalty = read_int("  Speed penalty (mph): ");
    return new Pothole(location, severity, depth, penalty);
}


//remove_obstacle - prompts for a location and tries
//to remove the obstacle there from the BST. Catches
//runtime_error if no obstacle exists at that 
//location.
void remove_obstacle(BST & course)
{
    if(course.is_empty())
    {
        cout << "The course is empty - nothing to remove." << endl;
        return;
    }
    string location = read_line("Location of obstacle to remove: ");
    try
    {
        course.remove(location);
        cout << "Obstacle at '" << location << "' removed." << endl;
    }
    catch(const runtime_error & e)
    {
        cout << "Could not remove: " << e.what() << endl;
    }
}


//search_obstacle - prompts for a location and looks
//it up in the BST. If found, calls describe() on it
//through dynamic binding so the right derived 
//message prints.
void search_obstacle(const BST & course)
{
    if(course.is_empty())
    {
        cout << "The course is empty - nothing to search." << endl;
        return;
    }
    string location = read_line("Location to search for: ");
    Obstacle * found = course.find(location);
    if(found != nullptr)
    {
        cout << "Found: ";
        found->describe();
    }
    else
    {
        cout << "No obstacle at that location." << endl;
    }
}


//drive_course - walks every obstacle in the BST in
//sorted order and lets the user try to avoid each
//one. Tracks the total score impact along the way.
//Dynamic binding dispatches each obstacle's correct
//describe/avoid/encounter methods. Because the BST
//does not expose iteration directly, the entire 
//course is displayed first to show what the driver
//will face, then for each obstacle the user provides
//an action and a speed which drive_one processes.
void drive_course(BST & course)
{
    if(course.is_empty())
    {
        cout << "The course is empty - add some obstacles first." << endl;
        return;
    }

    cout << endl << "Starting drive! Here is what is on the road:" << endl;
    course.display();
    cout << endl;

    //For each obstacle that the user previously added
    //we let them search by location and respond. This
    //keeps the dispatch through the base pointer 
    //while staying within the BST's public interface.
    int total_impact = 0;
    int how_many = read_int("How many obstacles do you want to drive past? ");
    for(int i = 0; i < how_many; ++i)
    {
        cout << endl << "Obstacle " << (i + 1) << ":" << endl;
        string location = read_line("  Which location? ");
        Obstacle * an_obstacle = course.find(location);
        if(an_obstacle == nullptr)
        {
            cout << "  No obstacle at '" << location << "' - skipping." << endl;
            continue;
        }
        string action = read_line("  Your action (honk/swerve/turn_around/slow_down): ");
        int speed = read_int("  Your speed (mph): ");

        cout << "  Coming up: ";
        an_obstacle->describe();
        if(an_obstacle->avoid(action))
        {
            cout << "  Success! No impact." << endl;
        }
        else
        {
            int impact = an_obstacle->encounter(speed);
            cout << "  Failed! Score impact: " << impact << endl;
            total_impact += impact;
        }
    }
    cout << endl << "Drive complete. Total score impact: " << total_impact << endl;
}


//feed_a_cow - demonstrates RTTI. Asks the user for a
//location, finds the obstacle there, attempts a 
//dynamic_cast to Cow * and if the cast succeeds 
//calls Cow's non-virtual feed() method with the food
//the user provides.
void feed_a_cow(BST & course)
{
    if(course.is_empty())
    {
        cout << "The course is empty - no cows to feed." << endl;
        return;
    }
    string location = read_line("Location of the cow to feed: ");
    Obstacle * found = course.find(location);
    if(found == nullptr)
    {
        cout << "No obstacle at that location." << endl;
        return;
    }
    Cow * a_cow = dynamic_cast<Cow *>(found);
    if(a_cow == nullptr)
    {
        cout << "The obstacle at '" << location << "' is not a cow." << endl;
        return;
    }
    string food = read_line("What food are you offering? ");
    if(a_cow->feed(food))
        cout << "The cow accepts the " << food << " and wanders off!" << endl;
    else
        cout << "The cow turns away from the " << food << "." << endl;
}


//clear_course - empties the entire BST and frees 
//every obstacle in it.
void clear_course(BST & course)
{
    if(course.is_empty())
    {
        cout << "The course is already empty." << endl;
        return;
    }
    course.remove_all();
    cout << "Course cleared." << endl;
}


