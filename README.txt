MAT101 Project 2 - Discrete Maths Library

Minghan Lyu
Bachelor of Software Engineering - Game Programming
=====================================================================================
Overview
A C++ library of discrete maths functions, with a unit test program that
checks each function and prints the results in the terminal.

Part A
The library, implemented in discretemaths.cpp against the provided discretemaths.h.
Part B 
Unit tests in test.h / test.cpp, run from main.cpp.
=====================================================================================

Functions implemented

Number functions
Magnitude:	absolute value
Signum:		sign of a number (1, -1 or 0)
Floor:		round down to a whole number
Ceiling:	round up to a whole number

Counting functions
Factorial:	n! (returns 0 above 20, where 64 bits overflow)
Permutation:	nPr, ordered arrangements
Combination:	nCr, unordered selections

Set functions in the assignment
Cardinality:	number of elements in a set
Power:	size of the power set, 2^n (returns 0 above 63 elements)
CartesianProduct:	size of A x B
===============================================================================================

Files used in this project

discretemaths.h:	Provided header with the `Set` class and declarations (unmodified) 
discretemaths.cpp:	Library implementation                                             
test.h:			Unit test declarations                                             
test.cpp:		Unit test definitions and test counting                            
main.cpp:           	Entry point, runs all tests     
===============================================================================================                                   

Building and running
1. Open Project2.sln in Visual Studio 2022.
2. Select the Debug or Release configuration, platform x64.
3. Build with Build > Rebuild Solution.
4. Run with Ctrl+F5.

To run the release build on its own, double-click `Project2.exe` from the
build zip. No other dependencies are needed.
===============================================================================================

Test output
The program prints each test as PASS or FAIL, grouped by function, followed by a summary:

    Tests run   : [49]
    Tests passed: [49]
    Tests failed: [0]

