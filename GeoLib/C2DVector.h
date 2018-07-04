/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DVector.h
\brief Declaration file for a class representing a 2D vector.

Declaration file for a class representing a 2D vector from the origin (0, 0)
with doubles for i and j.

\class C2DVector
\brief Class represents a 2D vector from the origin.

Class represents a 2D vector from the origin. includes a variety of functions for
simple manipulation.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2VECTOR_H 
#define _GEOLIB_C2VECTOR_H

#include "MemoryPool.h"


class C2DPoint;

#ifdef _EXPORTING
	#define CLASS_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define CLASS_DECLSPEC
	#else
		#define CLASS_DECLSPEC		__declspec(dllimport)
	#endif
#endif



class CLASS_DECLSPEC C2DVector
{
public:
	_MEMORY_POOL_DECLARATION

	/// Constructor.
	C2DVector(void);
	/// Destructor.
	~C2DVector(void);
	/// Constructor with assignment.
	C2DVector(double di, double dj);
	/// Constructor with assignment.
	C2DVector(const C2DVector& Other);
	/// Constructor provides 2 points, this being the vector from the first to the second.
	C2DVector(const C2DPoint& PointFrom, const C2DPoint& PointTo);
	/// Constructor converts a point to the vector (a point can be interpreted as a point and vice versa)
	C2DVector(const C2DPoint& Point);

	/// Sets to the values given.
	void Set(double di, double dj);
	/// Sets it to be the vector from the 1st to the second.
	void Set(const C2DPoint& PointFrom, const C2DPoint& PointTo);

	/// Reverses the direction of the vector.
	void Reverse(void);
	/// Turns right 90 degrees.
	void TurnRight(void);
	/// Turns right by the angle given in radians.
	void TurnRight(double dAng);
	/// Turns left by 90 degrees.
	void TurnLeft(void);
	/// Turns left by the angle given in radians.
	void TurnLeft(double dAng);
	// Returns the length of the vector.
	double GetLength(void) const;
	/// Sets the length of the vector.
	void SetLength(double dDistance);
	/// Makes the vector unit.
	void MakeUnit(void);

	/// Addition.
	const C2DVector operator+(const C2DVector& Other) const;
	/// Subtraction.
	const C2DVector operator-(const C2DVector& Other) const;
	/// Addition.
	void operator+=(const C2DVector& Other);
	/// Subtraction.
	void operator-=(const C2DVector& Other);

	/// Multiplication.
	const C2DVector operator*(const double dFactor) const;
	/// Multiplication.
	void operator*=(const double dFactor);

	/// Dot product.
	const double Dot(const C2DVector& Other) const;
	/// Cross product.
	const double Cross(const C2DVector& Other) const;

	/// Assignment.
	const C2DVector& operator=(const C2DVector& Other);
	/// Assignment to a point.
	const C2DVector& operator=(const C2DPoint& Other);

	/// Equality test.
	bool operator==(const C2DVector& Other) const;
	/// Inequality test.
	bool operator!=(const C2DVector& Other) const;

	/// Returns the angle from north in radians.
	double AngleFromNorth(void) const;
	/// Returns the angle to the right to another vector.
	double AngleToRight(const C2DVector& Other) const;
	/// Returns the angle to the left to another vector.
	double AngleToLeft(const C2DVector& Other) const;
	/// Returns the shortest angle between 2 vectors i.e. the dot product of the norms
	double AngleBetween(const C2DVector& Other) const;
	/// The value in the x axis.
	double i;
	/// The value in the y axis.
	double j;
};

#endif