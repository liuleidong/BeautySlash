/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DPoint.h
\brief Declaration file for the C2DPoint Class.

Declaration file for C2DPoint, a class which represents a cartesian point.

\class C2DPoint.
\brief Class which represents a cartesian point.

Class which represents a point with doubles for x and y.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DPOINT_H 
#define _GEOLIB_C2DPOINT_H

#include "C2DBase.h"
#include "MemoryPool.h"


class C2DLine;
class C2DPointSet;

#ifdef _EXPORTING
	#define CLASS_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define CLASS_DECLSPEC
	#else
		#define CLASS_DECLSPEC		__declspec(dllimport)
	#endif
#endif


class CLASS_DECLSPEC C2DPoint : public C2DBase
{
public:
	_MEMORY_POOL_DECLARATION

	/// Constructor.
	C2DPoint(void);
	/// Destructor.
	~C2DPoint(void);
	/// Constructor assigns the data.
	C2DPoint(double dx, double dy);

	/// Assignment
	void Set(double dx, double dy) { x = dx; y = dy;}

	/// Constuct from vector which can be thought of as a point (and vice versa).
	C2DPoint(const C2DVector& Vector);
	/// Returns the mid point between this and the other.
	C2DPoint GetMidPoint(const C2DPoint& Other) const;
	/// Project this on the vector given returning a distance along the vector.
	double Project(const C2DVector& Vector) const;
	/// Project this on the line given returning a distance along the line.
	double Project(const C2DLine& Line) const;
	/// Project this on the vector given returning a distance along the line.
	void Project(const C2DVector& Vector, CInterval& Interval) const;
	/// Project this on the line given returning a distance along the line.
	void Project(const C2DLine& Line, CInterval& Interval) const;
	/// True if the point projects onto the line given and returns the point on the line.
	bool ProjectsOnLine(const C2DLine& Line, C2DPoint* ptOnLine = 0, 
		bool* bAbove = 0) const;

	/// Returns the distance between this and the other.
	double Distance(const C2DPoint& Other) const;
	/// Returns a vector from this to the other.
	C2DVector MakeVector(const C2DPoint& PointTo) const;

	/// Adds this to the other.
	const C2DPoint operator+(const C2DPoint& Other) const;
	/// Addition to this.
	void operator+=(const C2DPoint& Other);
	/// Movement.
	const C2DPoint operator+(const C2DVector& Other) const;
	/// Movement.
	void operator+=(const C2DVector& Other);
	/// Subtracts this from the other.
	const C2DVector operator-(const C2DPoint& Other) const;
	/// Subtraction
	void operator-=(const C2DPoint& Other);

	/// Multiplies this by the factor and returns the result.
	const C2DPoint operator*(double dFactor) const;
	/// Mulitiplication
	void operator*=(double dFactor);
	/// Mulitiplication
	void operator*=(const C2DPoint& Other);
	/// Multiplies this by the point and returns the result.
	const C2DPoint operator*(const C2DPoint& Other) const;

	/// Divides this by the factor and returns the result.
	const C2DPoint operator/(double dFactor) const;
	/// Divides this by the factor.
	void operator/=(double dFactor);

	/// Assignment to another.
	const C2DPoint& operator=(const C2DPoint& Other);
	/// Assignement to a vector.
	const C2DPoint& operator=(const C2DVector& Vector);
	/// Equality test which uses a tolerance level.
	bool operator==(const C2DPoint& Other) const;
	/// Inequality test, inverse of equality test.
	bool operator!=(const C2DPoint& Other) const;

	/// Moves this by the vector given.
	void Move(const C2DVector& vector);
	/// Rotates this to the right about the origin provided by the angle given.
	void RotateToRight(double dAng, const C2DPoint& Origin);
	/// Grows this from the origin by the amount (1 = no change).
	void Grow(double dFactor, const C2DPoint& Origin);
	/// Reflects this through the point given.
	void Reflect(const C2DPoint& Other);
	/// Reflects the point in the line given.
	void Reflect(const C2DLine& Line);
	/// Reflects in the y axis.
	void ReflectY(void);
	/// Reflects in the x axis.
	void ReflectX(void);

	/// Returns the bounding rectangle. (Required for the base class).
	void GetBoundingRect(C2DRect& Rect) const;
	/// Snaps to the conceptual grid.
	void SnapToGrid(void);
	/// Projects the point onto the line as a ray.
	void ProjectOnRay(const C2DLine& Line);
	/// The x value.
	double x;
	/// The y value.
	double y;
};

#endif