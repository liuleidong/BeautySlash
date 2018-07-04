/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DPoint.cpp
\brief Implementation file for the C2DPoint Class.

Implementation file for C2DPoint, a class which represents a cartesian point.
<P>---------------------------------------------------------------------------*/


#include "StdAfx.h"
#include "C2Dpoint.h"
#include "C2DVector.h"
#include "C2DLine.h"
#include "C2DPointSet.h"
#include "Grid.h"
#include "Interval.h"
#include "C2DRect.h"


//_MEMORY_POOL_IMPLEMENATION(C2DPoint)


void* C2DPoint::operator new(unsigned int)
{
	return ::new C2DPoint;
 }


void C2DPoint::operator delete(void* p)
{
	::delete p;
}


void* C2DPoint::operator new(unsigned int, const char*,int)
{
	return ::new C2DPoint;
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::C2DPoint
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DPoint::C2DPoint(void)  : C2DBase(Point)
{
}

/**--------------------------------------------------------------------------<BR>
C2DPoint::~C2DPoint
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DPoint::~C2DPoint(void)
{
}

/**--------------------------------------------------------------------------<BR>
C2DPoint::C2DPoint
\brief Constructor initialises the object.
<P>---------------------------------------------------------------------------*/
C2DPoint::C2DPoint(double dx, double dy) : C2DBase(Point)
{
	x = dx;
	y = dy;
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::C2DPoint
\brief Constructor initialises the object.
<P>---------------------------------------------------------------------------*/
C2DPoint::C2DPoint(const C2DVector& Vector) : C2DBase(Point)
{
	x = Vector.i;
	y = Vector.j;
}

/**--------------------------------------------------------------------------<BR>
C2DPoint::Move
\brief Moves the point to the new point using the vector given.
<P>---------------------------------------------------------------------------*/
void C2DPoint::Move(const C2DVector& vector)
{
	x += vector.i;
	y += vector.j;
}

/**--------------------------------------------------------------------------<BR>
C2DPoint::GetMidPoint
\brief Gets the mid point along a line given by this and a point given.
<P>---------------------------------------------------------------------------*/
C2DPoint C2DPoint::GetMidPoint(const C2DPoint& Other) const 
{
	return C2DPoint( (x+Other.x)/2, (y+Other.y)/2 );
}

/**--------------------------------------------------------------------------<BR>
C2DPoint::ReflectY
\brief Reflects the point in the Y axis.
<P>---------------------------------------------------------------------------*/
void C2DPoint::ReflectY(void)
{
	x = -x;
}

/**--------------------------------------------------------------------------<BR>
C2DPoint::ReflectX
\brief Reflects the point in the X axis.
<P>---------------------------------------------------------------------------*/
void C2DPoint::ReflectX(void)
{
	y = -y;
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::Reflect
\brief Reflects the point in the other point.
<P>---------------------------------------------------------------------------*/
void C2DPoint::Reflect(const C2DPoint& Other)
{
	// Set up a vector from this to the other
	C2DVector vec(*this, Other);
	// Now use the vector to find the reflection by continuing along it from the point given.
	*this = Other + vec;
}



/**--------------------------------------------------------------------------<BR>
C2DPoint::Reflect
\brief Reflects the point in the line.
<P>---------------------------------------------------------------------------*/
void C2DPoint::Reflect(const C2DLine& Line)
{
	// First find the point along the line that this projects onto.
	// Make a vector from the point on the line given to this point.
	C2DVector vecthis(Line.point, *this );
	// Find the length of the line given.
	double dLength = Line.vector.GetLength();
	// Now make the projection of this point on the line.
	double dProj = vecthis.Dot(Line.vector);
	dProj /= dLength;
	// Find the factor along the line that the projection is.
	double dFactor = dProj / dLength;
	// Now set up a copy of the vector of the line given.
	C2DVector vProj = Line.vector;
	// Multiply that by that factor calculated.
	vProj *= dFactor;
	// Use the vector to find the point on the line.
	C2DPoint ptOnLine = Line.point + vProj;
	// Noe simply reflect this in the point.
	this->Reflect(ptOnLine);

}


/**--------------------------------------------------------------------------<BR>
C2DPoint::Distance
\brief Calculates the distance between this and another point.
<P>---------------------------------------------------------------------------*/
double C2DPoint::Distance(const C2DPoint& Other) const 
{
	double dXD = x - Other.x;
	double dYD = y - Other.y;
	return sqrt( dXD * dXD + dYD * dYD );
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::MakeVector
\brief Returns a vector defined by this point and a move to another point given.
<P>---------------------------------------------------------------------------*/
C2DVector C2DPoint::MakeVector(const C2DPoint&  PointTo) const 
{
	return C2DVector( *this, PointTo);
}

/**--------------------------------------------------------------------------<BR>
C2DPoint::operator+
\brief Addition.
<P>---------------------------------------------------------------------------*/
const C2DPoint C2DPoint::operator+(const C2DPoint& Other) const
{
	return C2DPoint(x + Other.x, y + Other.y);
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::operator+
\brief Addition.
<P>---------------------------------------------------------------------------*/
const C2DPoint C2DPoint::operator+(const C2DVector& vector) const
{
	return C2DPoint(x + vector.i, y + vector.j);
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::operator/
\brief Divides this by the factor and returns the result.
<P>---------------------------------------------------------------------------*/
const C2DPoint C2DPoint::operator/(double dFactor) const
{
	return (C2DPoint( x/dFactor, y/dFactor));
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::operator-
\brief Subtraction.
<P>---------------------------------------------------------------------------*/
const C2DVector C2DPoint::operator-(const C2DPoint& Other) const
{
	return C2DVector(x - Other.x, y - Other.y);
}

/**--------------------------------------------------------------------------<BR>
C2DPoint::operator-=
\brief Subtraction
<P>---------------------------------------------------------------------------*/
void C2DPoint::operator-=(const C2DPoint& Other)
{
	x -= Other.x;
	y -= Other.y;
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::operator*=
\brief Mulitiplication
<P>---------------------------------------------------------------------------*/
void C2DPoint::operator*=(const C2DPoint& Other)
{
	x *= Other.x;
	y *= Other.y;
}

/**--------------------------------------------------------------------------<BR>
C2DPoint::operator*=
\brief Mulitiplication
<P>---------------------------------------------------------------------------*/
void C2DPoint::operator*=(double dFactor)
{
	x *= dFactor;
	y *= dFactor;
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::operator*=
\brief Division
<P>---------------------------------------------------------------------------*/
void C2DPoint::operator/=(double dFactor)
{
	x /= dFactor;
	y /= dFactor;
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::operator=
\brief Sets this to be equal to the point given.
<P>---------------------------------------------------------------------------*/
const C2DPoint& C2DPoint::operator=(const C2DPoint& Other)
{
	x = Other.x;
	y = Other.y;
	return *this;
}

/**--------------------------------------------------------------------------<BR>
C2DPoint::operator=
\brief Sets this to be equal to the point defined by the vector given.
<P>---------------------------------------------------------------------------*/
const C2DPoint& C2DPoint::operator=(const C2DVector& OtherVector)
{
	x = OtherVector.i;
	y = OtherVector.j;
	return *this;
}

/**--------------------------------------------------------------------------<BR>
C2DPoint::operator==
\brief Equality test which is really a relative closeness test due to use of doubles. 
Note that relative closesness is meaningless for a 0, 0 point so best not to 
use if possible. If the tolerance is 0.0000000001 this is as close as 0.0001
millimetres in 1km.
<P>---------------------------------------------------------------------------*/
bool C2DPoint::operator==(const C2DPoint& Other) const 
{
	bool bxClose;
	bool byClose;

	if ( x == 0 )
		bxClose = Other.x == 0;
	else
		bxClose = fabs((Other.x - x ) / x ) < conEqualityTolerance;

	if (!bxClose)
		return false;		// Get out early if we can.

	if ( y == 0 )
		byClose = Other.y == 0;
	else
		byClose = fabs((Other.y - y ) / y ) < conEqualityTolerance;

	return ( byClose );		// We know x is close.
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::operator!=
\brief Inverse of the equality test.
<P>---------------------------------------------------------------------------*/
bool C2DPoint::operator!=(const C2DPoint& Other) const 
{
	return !(*this == Other);
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::Project
\brief Projects the point onto the vector given.
<P>---------------------------------------------------------------------------*/
double C2DPoint::Project(const C2DVector& Vector) const
{
	C2DVector vecthis(x, y);

	return (vecthis.Dot(Vector)) / Vector.GetLength();
}

/**--------------------------------------------------------------------------<BR>
C2DPoint::Project
\brief Projects the point onto the line given.
<P>---------------------------------------------------------------------------*/
double C2DPoint::Project(const C2DLine& Line) const
{
	C2DVector vecthis(Line.point, *this );

	return (vecthis.Dot(Line.vector)) / Line.vector.GetLength();
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::Project
\brief Projects the point onto the line given. Uses interval to override virtual
function.
<P>---------------------------------------------------------------------------*/
void C2DPoint::Project(const C2DLine& Line, CInterval& Interval) const
{
	Interval.dMax = Project(Line);
	Interval.dMin = Interval.dMax;
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::Project
\brief Project this on the vector given. Uses interval to override virtual
function.
<P>---------------------------------------------------------------------------*/
void C2DPoint::Project(const C2DVector& Vector, CInterval& Interval) const
{
	Interval.dMax = Project(Vector);
	Interval.dMin = Interval.dMax;
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::ProjectsOnLine
\brief True if the point projects onto the line given and returns the point on the line.
<P>---------------------------------------------------------------------------*/
bool C2DPoint::ProjectsOnLine(const C2DLine& Line, C2DPoint* ptOnLine,
							  bool* bAbove) const
{
	
	C2DVector vecthis(Line.point, *this );
	double dProj = vecthis.Dot(Line.vector);

	if (dProj < 0)
	{
		if (bAbove)
			*bAbove = false;
		return false;
	}

	double dLength = Line.vector.GetLength();

	dProj /= dLength;

	if (dProj > dLength)
	{
		if (bAbove)
			*bAbove = true;
		return false;
	}

	if (ptOnLine != 0)
	{
		double dFactor = dProj / dLength;
		
		C2DVector vProj = Line.vector;
		vProj *= dFactor;
		*ptOnLine = Line.point + vProj;
		return true;
	}
	else
	{
		return true;
	}
}

/**--------------------------------------------------------------------------<BR>
C2DPoint::operator+=
\brief Adds the point to this.
<P>---------------------------------------------------------------------------*/
void C2DPoint::operator+=(const C2DPoint& Other)
{
	x += Other.x;
	y += Other.y;
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::operator+=
\brief Movement.
<P>---------------------------------------------------------------------------*/
void C2DPoint::operator+=(const C2DVector& Vector)
{
	x += Vector.i;
	y += Vector.j;
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::operator*
\brief Returns the multiple of this and the factor.
<P>---------------------------------------------------------------------------*/
const C2DPoint C2DPoint::operator*(double dFactor) const
{
	return C2DPoint(x * dFactor, y * dFactor);
}

/**--------------------------------------------------------------------------<BR>
C2DPoint::operator*
\brief Returns the multiple of this and the other.
<P>---------------------------------------------------------------------------*/
const C2DPoint C2DPoint::operator*(const C2DPoint& Other) const
{
	return C2DPoint(x * Other.x, y * Other.y);
}

/**--------------------------------------------------------------------------<BR>
C2DPoint::RotateToRight
\brief Rotates this to the right by the angle given relative to the origin provided.
<P>---------------------------------------------------------------------------*/
void C2DPoint::RotateToRight(double dAng, const C2DPoint& Origin)
{
	C2DVector vector(Origin, *this);
	vector.TurnRight(dAng);
	x = Origin.x + vector.i;
	y = Origin.y + vector.j;
}

/**--------------------------------------------------------------------------<BR>
C2DPoint::Grow
\brief Grows this by the factor given relative to the origin provided.
<P>---------------------------------------------------------------------------*/
void C2DPoint::Grow(double dFactor, const C2DPoint& Origin)
{
	C2DVector vector(Origin, *this);
	vector *= dFactor;
	x = Origin.x + vector.i;
	y = Origin.y + vector.j;

}


/**--------------------------------------------------------------------------<BR>
C2DPoint::GetBoundingRect
\brief Returns the bounding rectangle. (Required for the base class)
<P>---------------------------------------------------------------------------*/
void C2DPoint::GetBoundingRect(C2DRect& Rect) const
{
	Rect.Set(*this);
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::SnapToGrid
\brief Snaps this to the grid defined by the CGrid class.
<P>---------------------------------------------------------------------------*/
void C2DPoint::SnapToGrid(void)
{
	double dGrid = CGrid::GetGridSize();

	double dxMultiple = fabs(x / dGrid ) + 0.5;

	dxMultiple = floor(dxMultiple);

	x < 0 ? x = - dxMultiple * dGrid : x = dxMultiple * dGrid;

	double dyMultiple = fabs(y / dGrid ) + 0.5;

	dyMultiple = floor(dyMultiple);

	y < 0 ? y = - dyMultiple * dGrid : y = dyMultiple * dGrid;

}


/**--------------------------------------------------------------------------<BR>
C2DPoint::ProjectsOnRay
\brief True if the point projects onto the line given and returns the point on the line.
<P>---------------------------------------------------------------------------*/
void C2DPoint::ProjectOnRay(const C2DLine& Line)
{
	C2DVector vecthis(Line.point, *this );
	double dProj = vecthis.Dot(Line.vector);

	double dFactor = dProj / (Line.vector.i * Line.vector.i + Line.vector.j * Line.vector.j );
	
	C2DVector vProj = Line.vector;
	vProj *= dFactor;
	(*this) = Line.point + vProj;
}