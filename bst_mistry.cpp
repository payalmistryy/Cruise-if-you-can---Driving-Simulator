/*
Payal Mistry
CS 302 - Programming Methodologies and
Software Implementation
Assignment 3 - Cruise If You Can
May 17, 2026
Filename: bst_mistry.cpp

This file implements the BST (binary search tree)
class used by the "Cruise If You Can" driving game
to store every Obstacle on the course. Each node
holds an Obstacle * base class pointer so the tree
can hold any mix of Cow, Detour, and Pothole objects
through dynamic binding. The tree is keyed on the
obstacle's location string. Every repetitive 
algorithm (insert, remove, display, remove_all, 
find, copy_tree) is implemented recursively per 
assignment specification, using public wrappers that
call private helpers operating on a Node *& so child
links in the parent can be modified in place. The 
BST takes ownership of any obstacle inserted and 
deletes those obstacles when nodes are removed,
which is why the destructor, copy constructor, and 
assignment operator are all defined. Duplicate
locations are not allowed; insert returns false in
that case. remove throws a runtime_error if the
target location is not found in the tree.
*/

#include <string>
#include <iostream>
#include <stdexcept>
#include "bst_mistry.h"
#include "obstacle_mistry.h"
#include "hazards_mistry.h"

using namespace std;


//BST class implementation

//Default constructor - initializes an empty tree.
BST::BST() : root(nullptr)
{
}


//Copy constructor - deep copies the source tree. 
//Each obstacle in the source has a brand new copy
//created via the derived dynamic_cast path in
//copy_tree, so the two trees do not share memory.
BST::BST(const BST & source) : root(nullptr)
{
    copy_tree(root, source.root);
}


//Destructor - releases every node and every obstacle
//the tree owns by calling remove_all on the root.
BST::~BST()
{
    remove_all(root);
}


//Assignment operator - empties this tree, then deep
//copies the source tree's contents. Self-assignment
//is guarded against first. Returns *this so chained
//assignment works.
BST & BST::operator=(const BST & rhs)
{
    if(this != &rhs)
    {
        remove_all(root);
        copy_tree(root, rhs.root);
    }
    return *this;
}


//insert (public) - inserts an obstacle into the tree
//keyed on its location. Returns true if inserted,
//false if a duplicate location is found. Throws if
//the obstacle pointer is null.
bool BST::insert(Obstacle * an_obstacle)
{
    if(an_obstacle == nullptr)
        throw invalid_argument("Cannot insert a null obstacle into the BST");
    return insert(root, an_obstacle);
}


//insert (recursive helper) - walks the tree to find
//the right spot for the obstacle. When a_root is
//null we have arrived at an empty link and a new
//node gets created here. Smaller locations go left,
//larger locations go right, duplicates return false.
bool BST::insert(Node *& a_root, Obstacle * an_obstacle)
{
    if(a_root == nullptr)
    {
        a_root = new Node;
        a_root->data = an_obstacle;
        a_root->left = nullptr;
        a_root->right = nullptr;
        return true;
    }
    if(an_obstacle->get_location() < a_root->data->get_location())
        return insert(a_root->left, an_obstacle);
    if(an_obstacle->get_location() > a_root->data->get_location())
        return insert(a_root->right, an_obstacle);
    return false;   //duplicate location, do not insert
}


//remove (public) - removes the obstacle at the given
//location from the tree. Throws if the location is
//not present. Returns true on successful removal.
bool BST::remove(const string & a_location)
{
    if(!remove(root, a_location))
        throw runtime_error("Cannot remove '" + a_location + "': location not found");
    return true;
}


//remove (recursive helper) - searches for the target
//location and removes the matching node. Three cases
//handled when the match is found: leaf (just delete),
//one child (splice the child up), two children (find
//the in-order successor in the right subtree, copy
//its data up, then recursively remove that successor
//from the right subtree).
bool BST::remove(Node *& a_root, const string & a_location)
{
    if(a_root == nullptr)
        return false;
    if(a_location < a_root->data->get_location())
        return remove(a_root->left, a_location);
    if(a_location > a_root->data->get_location())
        return remove(a_root->right, a_location);

    //Found the target node - now handle the three cases
    if(a_root->left == nullptr && a_root->right == nullptr)
    {
        delete a_root->data;
        delete a_root;
        a_root = nullptr;
    }
    else if(a_root->left == nullptr)
    {
        Node * temp = a_root;
        a_root = a_root->right;
        delete temp->data;
        delete temp;
    }
    else if(a_root->right == nullptr)
    {
        Node * temp = a_root;
        a_root = a_root->left;
        delete temp->data;
        delete temp;
    }
    else
    {
        //Two children - replace this node's obstacle 
        //with the in-order successor's obstacle, then
        //recursively remove the successor from the
        //right subtree.
        Node * successor = find_min(a_root->right);
        delete a_root->data;
        //Build a deep copy of the successor's obstacle
        //via the matching derived constructor by going
        //through dynamic_cast. This keeps polymorphism
        //intact instead of slicing to the base class.
        Obstacle * successor_data = successor->data;
        Cow * a_cow = dynamic_cast<Cow *>(successor_data);
        Detour * a_detour = dynamic_cast<Detour *>(successor_data);
        Pothole * a_pothole = dynamic_cast<Pothole *>(successor_data);
        if(a_cow != nullptr)
            a_root->data = new Cow(*a_cow);
        else if(a_detour != nullptr)
            a_root->data = new Detour(*a_detour);
        else if(a_pothole != nullptr)
            a_root->data = new Pothole(*a_pothole);
        remove(a_root->right, successor->data->get_location());
    }
    return true;
}


//display (public) - prints every obstacle in the
//tree in sorted order (in-order traversal). If the
//tree is empty a message says so.
void BST::display() const
{
    if(root == nullptr)
    {
        cout << "The course has no obstacles." << endl;
        return;
    }
    display(root);
}


//display (recursive helper) - standard in-order 
//traversal so obstacles print sorted by location.
//Calls each obstacle's describe() through the base
//pointer which dynamic binding resolves to the
//correct derived version.
void BST::display(Node * a_root) const
{
    if(a_root == nullptr)
        return;
    display(a_root->left);
    cout << "  ";
    a_root->data->describe();
    display(a_root->right);
}


//remove_all (public) - empties the tree and frees
//every node and obstacle the tree owns.
void BST::remove_all()
{
    remove_all(root);
}


//remove_all (recursive helper) - post-order 
//traversal so the children are deleted before their
//parent. Each node's obstacle is freed first, then
//the node itself, then the root pointer is set to
//null so the tree is in a clean empty state.
void BST::remove_all(Node *& a_root)
{
    if(a_root == nullptr)
        return;
    remove_all(a_root->left);
    remove_all(a_root->right);
    delete a_root->data;
    delete a_root;
    a_root = nullptr;
}


//is_empty - returns true if the tree has no nodes.
bool BST::is_empty() const
{
    return root == nullptr;
}


//find (public) - returns a base class pointer to
//the obstacle at the given location, or nullptr if
//no such obstacle exists in the tree.
Obstacle * BST::find(const string & a_location) const
{
    return find(root, a_location);
}


//find (recursive helper) - standard BST search.
//Returns the obstacle pointer when the keys match,
//or nullptr if the search runs off the bottom of
//the tree without finding anything.
Obstacle * BST::find(Node * a_root, const string & a_location) const
{
    if(a_root == nullptr)
        return nullptr;
    if(a_location < a_root->data->get_location())
        return find(a_root->left, a_location);
    if(a_location > a_root->data->get_location())
        return find(a_root->right, a_location);
    return a_root->data;
}


//copy_tree (recursive helper) - deep copies the 
//source tree into dest using pre-order traversal so
//the structure is preserved exactly. Each source
//obstacle gets a brand new derived copy via 
//dynamic_cast which keeps polymorphism intact.
void BST::copy_tree(Node *& dest, Node * source)
{
    if(source == nullptr)
    {
        dest = nullptr;
        return;
    }
    dest = new Node;
    dest->left = nullptr;
    dest->right = nullptr;

    //Build a new derived copy of source's obstacle so
    //the two trees do not share any obstacle memory.
    Obstacle * source_data = source->data;
    Cow * a_cow = dynamic_cast<Cow *>(source_data);
    Detour * a_detour = dynamic_cast<Detour *>(source_data);
    Pothole * a_pothole = dynamic_cast<Pothole *>(source_data);
    if(a_cow != nullptr)
        dest->data = new Cow(*a_cow);
    else if(a_detour != nullptr)
        dest->data = new Detour(*a_detour);
    else if(a_pothole != nullptr)
        dest->data = new Pothole(*a_pothole);
    else
        dest->data = nullptr;

    copy_tree(dest->left, source->left);
    copy_tree(dest->right, source->right);
}


//find_min (recursive helper) - returns the smallest
//node in the subtree rooted at a_root by walking 
//left as far as possible. Used by remove for the 
//two-child case to locate the in-order successor.
Node * BST::find_min(Node * a_root) const
{
    if(a_root == nullptr)
        return nullptr;
    if(a_root->left == nullptr)
        return a_root;
    return find_min(a_root->left);
}

