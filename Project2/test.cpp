// MAT101 – Fundamental Maths & Engineering Principles
// Definitions for the unit tests of the library

#include "test.h"

static int TestsRun = 0;
static int TestsPassed = 0;

// Records and prints the result of one test
static void Check(const char* _kpcName, bool _bPassed)
{
	TestsRun++;
	if (_bPassed)
	{
		TestsPassed++;
	}
	std::cout << (_bPassed ? "[PASS] " : "[FAIL] ") << _kpcName << std::endl;
}

static void TestCaseMagnitude()
{
	std::cout << "\n==== Magnitude ====" << std::endl;
	Check("Magnitude(3) == 3", Magnitude(3.0f) == 3.0f);
	Check("Magnitude(-6) == 6", Magnitude(-6.0f) == 6.0f);
	Check("Magnitude(0) == 0", Magnitude(0.0f) == 0.0f);
	Check("Magnitude(-7.5) == 7.5", Magnitude(-7.5f) == 7.5f);
}

static void TestCaseSignum()
{
	std::cout << "\n==== Signum ====" << std::endl;
	Check("Signum(7) == 1", Signum(7.0f) == 1.0f);
	Check("Signum(-8) == -1", Signum(-8.0f) == -1.0f);
	Check("Signum(0) == 0", Signum(0.0f) == 0.0f);
	Check("Signum(0.001) == 1", Signum(0.001f) == 1.0f);
}

static void TestCaseFloor()
{
	std::cout << "\n==== Floor ====" << std::endl;
	Check("Floor(2.5) == 2", Floor(2.7f) == 2.0f);
	Check("Floor(-2.3) == -3", Floor(-2.3f) == -3.0f);
	Check("Floor(4) == 4", Floor(4.0f) == 4.0f);
	Check("Floor(-4) == -4", Floor(-4.0f) == -4.0f);
	Check("Floor(0.5) == 0", Floor(0.5f) == 0.0f);
}

static void TestCaseCeiling()
{
	std::cout << "\n==== Ceiling ====" << std::endl;
	Check("Ceiling(3.3) == 4", Ceiling(3.3f) == 4.0f);
	Check("Ceiling(-2.7) == -2", Ceiling(-2.7f) == -2.0f);
	Check("Ceiling(6) == 6", Ceiling(6.0f) == 6.0f);
	Check("Ceiling(-4) == -4", Ceiling(-4.0f) == -4.0f);
	Check("Ceiling(0.7) == 1", Ceiling(0.7f) == 1.0f);
}

static void TestCaseFactorial()
{
	std::cout << "\n==== Factorial ====" << std::endl;
	Check("Factorial(0) == 1", Factorial(0) == 1);
	Check("Factorial(3) == 6", Factorial(3) == 6);
	Check("Factorial(8) == 40320", Factorial(8) == 40320);
	Check("Factorial(10) == 3628800", Factorial(10) == 3628800);
	Check("Factorial(20) == 2432902008176640000", Factorial(20) == 2432902008176640000ULL);
}

static void TestCasePermutation()
{
	std::cout << "\n==== Permutation ====" << std::endl;
	Check("Permutation(5,2) == 20", Permutation(5, 2) == 20);
	Check("Permutation(5,0) == 1", Permutation(5, 0) == 1);
	Check("Permutation(5,5) == 120", Permutation(5, 5) == 120);
	Check("Permutation(10,3) == 720", Permutation(10, 3) == 720);
	Check("Permutation(3,5) == 0 (r > n)", Permutation(3, 5) == 0);
}

static void TestCaseCombination()
{
	std::cout << "\n==== Combination ====" << std::endl;
	Check("Combination(5,2) == 10", Combination(5, 2) == 10);
	Check("Combination(5,0) == 1", Combination(5, 0) == 1);
	Check("Combination(5,5) == 1", Combination(5, 5) == 1);
	Check("Combination(10,3) == 120", Combination(10, 3) == 120);
	Check("Combination(52,5) == 2598960", Combination(52, 5) == 2598960);
	Check("Combination(3,5) == 0 (r > n)", Combination(3, 5) == 0);
}

static void TestCaseCardinality()
{
	std::cout << "\n==== Cardinality ====" << std::endl;

	Set setEmpty;
	Check("Cardinality(empty) == 0", Cardinality(setEmpty) == 0);

	Set setA;
	setA.AddElement(1);
	setA.AddElement(2);
	setA.AddElement(3);
	Check("Cardinality({1,2,3}) == 3", Cardinality(setA) == 3);

	setA.AddElement(2);
	Check("Adding a duplicate does not change cardinality", Cardinality(setA) == 3);

	setA.RemoveElement(1);
	Check("Cardinality after removing 1 == 2", Cardinality(setA) == 2);

	Set setUniversal;
	setUniversal.SetUniversal();
	Check("Cardinality(universal) == 100", Cardinality(setUniversal) == 100);
}

static void TestCasePower()
{
	std::cout << "\n==== Power ====" << std::endl;

	Set setEmpty;
	Check("Power(empty) == 1", Power(setEmpty) == 1);

	Set setOne;
	setOne.AddElement(7);
	Check("Power({7}) == 2", Power(setOne) == 2);

	Set setThree;
	setThree.AddElement(1);
	setThree.AddElement(2);
	setThree.AddElement(3);
	Check("Power({1,2,3}) == 8", Power(setThree) == 8);

	Set setTen;
	for (int iElement = 1; iElement <= 10; ++iElement)
	{
		setTen.AddElement(iElement);
	}
	Check("Power(10 elements) == 1024", Power(setTen) == 1024);

	Set setUniversal;
	setUniversal.SetUniversal();
	Check("Power(universal) == 0 (overflow guard)", Power(setUniversal) == 0);
}

static void TestCaseCartesianProduct()
{
	std::cout << "\n==== CartesianProduct ====" << std::endl;

	Set setA;
	setA.AddElement(1);
	setA.AddElement(2);

	Set setB;
	setB.AddElement(3);
	setB.AddElement(4);
	setB.AddElement(5);

	Set setEmpty;

	Check("{1,2} x {3,4,5} == 6", CartesianProduct(setA, setB) == 6);
	Check("{3,4,5} x {1,2} == 6 (order does not change size)", CartesianProduct(setB, setA) == 6);
	Check("A x empty == 0", CartesianProduct(setA, setEmpty) == 0);
	Check("empty x B == 0", CartesianProduct(setEmpty, setB) == 0);
	Check("A x A == 4", CartesianProduct(setA, setA) == 4);
}

void AllTests()
{
	TestCaseMagnitude();
	TestCaseSignum();
	TestCaseFloor();
	TestCaseCeiling();
	TestCaseFactorial();
	TestCasePermutation();
	TestCaseCombination();
	TestCaseCardinality();
	TestCasePower();
	TestCaseCartesianProduct();

	std::cout << "\n==============================" << std::endl;
	std::cout << "All Tests  run   : " << TestsRun << std::endl;
	std::cout << "Only Tests passed: " << TestsPassed << std::endl;
	std::cout << "Only Tests failed: " << (TestsRun - TestsPassed) << std::endl;
}