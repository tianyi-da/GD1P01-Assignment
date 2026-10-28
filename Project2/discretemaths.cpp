#include "discretemaths.h"

//  Returns the absolute value of a number
// _kfX - the value
//  Return float - the value without its sign

float Magnitude(const float _kfX)
{
	if (_kfX < 0.0f)
	{
		return -_kfX;
	}
	return _kfX;
}
// ===============================================================

// Returns the sign of a number
// _kfX - the value
// Return float - 1 if positive, -1 if negative, 0 if zero

float Signum(const float _kfX)
{
	if (_kfX > 0.0f)
	{
		return 1.0f;
	}
	if (_kfX < 0.0f)
	{
		return -1.0f;
	}
	return 0.0f;
}
// ===============================================================

// Rounds a number down to the nearest whole number
// _kfX - the value
// Return float - the largest whole number <= _kfX

float Floor(const float _kfX)
{
	float float_Whole = static_cast<float>(static_cast<long long>(_kfX));

	// Truncation rounds toward zero, which is too high for negatives
	if (_kfX < float_Whole)
	{
		return float_Whole - 1.0f;
	}
	return float_Whole;
}
// ===============================================================

//  Rounds a number up to the nearest whole number
//  _kfX - the value
//  Return float - the smallest whole number >= _kfX

float Ceiling(const float _kfX)
{
	float float_Whole = static_cast<float>(static_cast<long long>(_kfX));

	// Truncation rounds toward zero, which is too low for positives
	if (_kfX > float_Whole)
	{
		return float_Whole + 1.0f;
	}
	return float_Whole;
}
// ===============================================================


//  Returns n! (n x (n-1) x ... x 1). 0! is 1. Values of n above 20 overflow, so 0 is returned.
//  _kdn - the number
// Return unsigned long long

unsigned long long Factorial(const unsigned int _kdn)
{
	const unsigned int maxFactorial = 20; // 21! overflows unsigned long long

	if (_kdn > maxFactorial)		
	{
		return 0;
	}

	unsigned long long unsigned_Result = 1;
	// Start at 2 because multiplying by 1 changes nothing (also covers 0! and 1!)

	for (unsigned int unsigned_Count = 2; unsigned_Count <= _kdn; ++unsigned_Count)
	{
		unsigned_Result *= unsigned_Count;
	}
	return unsigned_Result;
}
// ===============================================================

// Unordered selections of r items from n: n!/(r!(n-r)!)
// _kuin - total items, _kuir - items chosen
// Return unsigned long long (0 if r > n)

unsigned long long Combination(const unsigned int _kuin, const unsigned int _kuir)
{
	if (_kuir > _kuin)
	{
		return 0;
	}

	// C(n,r) == C(n,n-r), so use the smaller one to keep numbers small
	unsigned int uiR = _kuir;
	if (_kuin - _kuir < uiR)
	{
		uiR = _kuin - _kuir;
	}

	unsigned long long unsigned_Result = 1;
	for (unsigned int unsigned_Count = 1; unsigned_Count <= uiR; ++unsigned_Count)
	{
		// Each step stays a whole number, so the division is exact
		unsigned_Result = unsigned_Result * (_kuin - uiR + unsigned_Count) / unsigned_Count;
	}
	return unsigned_Result;
}
// ===============================================================

//  Ordered arrangements of r items from n: n!/(n-r)!
// _kuin - total items, _kuir - items chosen
// Return unsigned long long (0 if r > n)

unsigned long long Permutation(const unsigned int _kuin, const unsigned int _kuir)
{
	if (_kuir > _kuin)
	{
		return 0;
	}
	// Calculates the permutation by multiplying consecutive decreasing values
	unsigned long long unsigned_Result = 1;
	for (unsigned int unsigned_Count = 0; unsigned_Count < _kuir; ++unsigned_Count)
	{
		unsigned_Result *= (_kuin - unsigned_Count);
	}
	return unsigned_Result;
}
// ===============================================================


// Returns the number of elements in a set, |A|
// _rSet - the set to count
// Return int

int Cardinality(Set& _rSet)
{
	// Counts the number of elements present in the set
	int int_Count = 0;
	for (int i_Element = 1; i_Element <= SET_SIZE_MAX; ++i_Element)
	{
		// Increments the count if the current element belongs to the set
		if (_rSet.ContainsElement(i_Element))
		{
			++int_Count;
		}
	}
	return int_Count;
}
// ===============================================================


// Returns the size of the power set, |P(A)| = 2^|A|. A set bigger than 63 elements overflows, so 0 is returned.
// _rSet - the set
// Return unsigned long long

unsigned long long Power(Set& _rSet)
{
	int int_Cardinality = Cardinality(_rSet);
	if (int_Cardinality > 63)
	{
		return 0;
	}

	// Shifting 1 left by n places gives 2^n
	return 1ULL << int_Cardinality;
}
// ===============================================================

// Returns the number of ordered pairs in A x B,  which is |A| * |B|
// _rA, _rB - the two sets
// Return unsigned long long

unsigned long long CartesianProduct(Set& _rA, Set& _rB)
{
	unsigned long long unsigned_SizeA = static_cast<unsigned long long>(Cardinality(_rA));
	unsigned long long unsigned_SizB = static_cast<unsigned long long>(Cardinality(_rB));
	return unsigned_SizeA * unsigned_SizB;
}