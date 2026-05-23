# Cruise If You Can

A C++ application developed for CS 302 (Programming Methodologies and Software Implementation) at Portland State University.

## Overview

A menu-driven driving game where the player builds a course of road obstacles, then attempts to drive through it. Every obstacle on the course is stored in a **binary search tree of base class pointers**, so a single tree holds a mix of object types and dispatches the correct behavior at runtime through dynamic binding. The application demonstrates true polymorphism, runtime type identification (RTTI), and a fully recursive tree that owns and manages all of its dynamic memory.

## Features

- Build a driving course from three distinct obstacle types — Cow, Detour, and Pothole — each with its own data and behavior
- Drive the course interactively: each obstacle is described, the player chooses an action to avoid it, and a running score impact is tracked
- Add, remove, search, display (in sorted order), and clear obstacles through a full menu-driven interface
- "Feed a cow" option that uses RTTI to act only on cow obstacles
- Graceful error recovery — invalid input and bad operations are caught and reported instead of crashing

## Technical Highlights

- Abstract base class (`Obstacle`) with a **pure virtual method**, making it non-instantiable and enforcing a shared interface across all derived classes
- Runtime polymorphism through a heterogeneous container: a single BST of `Obstacle *` pointers stores Cow, Detour, and Pothole objects and resolves `encounter`, `avoid`, and `describe` to the correct derived version via dynamic binding
- **RTTI with `dynamic_cast`** to safely identify a Cow at runtime and call a type-specific non-virtual method
- Binary search tree keyed on a string field, with **every repetitive operation (insert, remove, find, display, remove_all, copy) implemented recursively** using `Node *&` helpers that modify parent links in place
- Full BST removal handling all three cases — leaf, single child, and two children via in-order successor replacement
- Deep copying that preserves derived types: the copy constructor and assignment operator rebuild each obstacle through its true type rather than slicing to the base class
- Resource ownership model — the tree owns every obstacle it holds and frees them through the **Rule of Three** (destructor, copy constructor, assignment operator) and a virtual base destructor
- Exception handling with standard library exceptions (`invalid_argument`, `runtime_error`) for input validation and failed operations, wrapped in `try`/`catch` at the call sites
- Modular build with a Makefile using separate object-file compilation for incremental rebuilds
- Compiled warning-free with the `-Wall` flag

## Tech Stack

- **Language:** C++
- **Concepts:** Object-Oriented Programming, Inheritance, Polymorphism, Abstract Classes, RTTI / `dynamic_cast`, Recursion, Exception Handling, Dynamic Memory Management
- **Data Structures:** Binary Search Tree (of base class pointers)
- **Build:** Make (`g++`, separate compilation)

## How to Run

```bash
make
./cruise
```

Or build and run in one step:

```bash
make run
```

To remove build products:

```bash
make clean
```

## Author

Payal Mistry — Computer Science, Portland State University
