/*
Payal Mistry
CS 302 - Programming Methodologies and
Software Implementation
Assignment 3 - Cruise If You Can
May 5, 2026
Filename: bst_mistry.h

This file declares the BST (binary search tree)
class used by the "Cruise If You Can" driving game
to store every Obstacle on the course. Each node
holds an Obstacle * (base class pointer) so a single
tree can hold any mix of Cow, Detour, and Pothole
objects through dynamic binding. The tree is keyed
on the obstacle's location string. Every repetitive
algorithm (insert, remove, display, remove_all) is
implemented recursively per assignment specification.
The BST owns its nodes and the obstacles those nodes
point to, so it manages dynamic memory and follows
the rule of three (copy constructor, assignment 
operator, destructor).
*/

#ifndef BST_MISTRY_H
#define BST_MISTRY_H


#include <string>
#include <iostream>
#include "obstacle_mistry.h"

using std::string;


//Node struct - internal building block of the BST
//Each node holds a base class pointer to an Obstacle
//so the tree can store any derived type. Left and
//right point to child nodes or nullptr at the leaves.
struct Node
{
    Obstacle * data;        //base class pointer - allows upcasting from any derived obstacle
    Node * left;            //pointer to left subtree (smaller location strings)
    Node * right;           //pointer to right subtree (larger location strings)
};


//BST class
//Binary search tree keyed on the obstacle's location
//string. Stores Obstacle * in each node so the tree
//holds collection of derived types.
//All repetitive algorithms are recursive. The BST
//takes ownership of any obstacle inserted into it
//and deletes them in remove_all and the destructor.
class BST
{
    public:
        //Constructors and destructor
        BST();                              //Default constructor - empty tree
        BST(const BST & source);            //Copy constructor - deep copy of source tree
        ~BST();                             //Destructor - removes all nodes and obstacles

        //Assignment operator
        BST & operator=(const BST & rhs);

        //Public interface - calls the recursive helpers
        bool insert(Obstacle * an_obstacle);                //Inserts an obstacle into the tree by its location
        bool remove(const string & a_location);             //Removes the obstacle with matching location
        void display() const;                               //Prints every obstacle in sorted order
        void remove_all();                                  //Empties the tree and frees all dynamic memory
        bool is_empty() const;                              //Returns true if the tree has no nodes
        Obstacle * find(const string & a_location) const;   //Returns pointer to obstacle at location, or nullptr

    private:
        Node * root;        //top of the tree, nullptr when the tree is empty

        //Recursive helpers - operate on a Node *& so they can
        //modify links in the parent without extra bookkeeping
        bool insert(Node *& a_root, Obstacle * an_obstacle);
        bool remove(Node *& a_root, const string & a_location);
        void display(Node * a_root) const;
        void remove_all(Node *& a_root);
        Obstacle * find(Node * a_root, const string & a_location) const;
        void copy_tree(Node *& dest, Node * source);            //recursive deep copy used by copy constructor and op=
        Node * find_min(Node * a_root) const;                   //finds smallest node in a subtree (used by remove)
};


#endif





