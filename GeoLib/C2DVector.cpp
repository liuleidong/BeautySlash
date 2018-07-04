/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DVector.cpp
\brief Implexmentation files for C2DVector, a class for a 2D vector from the 
origin with doubles for i and j.
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "C2DVector.h"
#include "C2DPoint.h"


_MEMORY_POOL_IMPLEMENATION(C2DVector)

/**--------------------------------------------------------------------------<BR>
C2DVector::C2DVector <BR>
\brief Constructor initialises the object.
<P>---------------------------------------------------------------------------*/
C2DVector::C2DVector(void)
{
	i = 0;
	j = 0;
}

/**--------------------------------------------------------------------------<BR>
C2DVector::~C2DVector <BR>
\brief Destructor does nothing.
<P>---------------------------------------------------------------------------*/
C2DVector::~C2DVector(void)
{
}


/**--------------------------------------------------------------------------<BR>
C2DVector::C2DVector <BR>
\brief Constructor initialises the object directly.
<P>---------------------------------------------------------------------------*/
C2DVector::C2DVector(double di, double dj)
{
	i = di;
	j = dj;
}

/**--------------------------------------------------------------------------<BR>
C2DVector::C2DVector <BR>
\brief Constructor initialises the object to another.
<P>---------------------------------------------------------------------------*/
C2DVector::C2DVector(const C2DVector& Other)
{
	i = Other.i;
	j = Other.j;
}

/**--------------------------------------------------------------------------<BR>
C2DVector::C2DVector <BR>
\brief Constructor initialises the object by passing 2 points, this representing the
movement from the first to the second.
<P>---------------------------------------------------------------------------*/
C2DVector::C2DVector(const C2DPoint& PointFrom, const C2DPoint& PointTo)
{
	i = PointTo.x - PointFrom.x;
	j = PointTo.y - PointFrom.y;
}


/**--------------------------------------------------------------------------<BR>
C2DVector::C2DVector <BR>
\brief 	Constructor converts a point to the vector. 
(A point can be interpreted as a point and vice versa)
<P>---------------------------------------------------------------------------*/
C2DVector::C2DVector(const C2DPoint& Point)
{
	i = Point.x;
	j = Point.y;
}


/**--------------------------------------------------------------------------<BR>
C2DVector::Set <BR>
\brief Sets to the values given.
<P>---------------------------------------------------------------------------*/
void C2DVector::Set(double di, double dj)
{
	i = di;
	j = dj;
}


/**--------------------------------------------------------------------------<BR>
C2DVector::Set <BR>
\brief Sets it to be the vector from the 1st to the second.
<P>---------------------------------------------------------------------------*/
void C2DVector::Set(const C2DPoint& PointFrom, const C2DPoint& PointTo)
{
	i = PointTo.x - PointFrom.x;
	j = PointTo.y - PointFrom.y;
}


/**--------------------------------------------------------------------------<BR>
C2DVector::Reverse <BR>
\brief This reverses the direction of the vector.
<P>---------------------------------------------------------------------------*/
void C2DVector::Reverse(void)
{
	i = -i;
	j = -j;
}


/**--------------------------------------------------------------------------<BR>
C2DVector::TurnRight <BR>
\brief Turns right through a 90 degree angle.
<P>---------------------------------------------------------------------------*/
void C2DVector::TurnRight(void)
{
	double tempi = i;
	i = j;
	j = -tempi;
}


/**--------------------------------------------------------------------------<BR>
C2DVector::TurnLeft <BR>
\brief Turns left through a 90 degree angle.
<P>---------------------------------------------------------------------------*/
void C2DVector::TurnLeft(void)
{
	double tempi = i;
	i = -j;
	j = tempi;
}


/**--------------------------------------------------------------------------<BR>
C2DVector::GetDistance <BR>
\brief Returns the distance of the vector.
<P>---------------------------------------------------------------------------*/
double C2DVector::GetLength(void) const
{
	return sqrt( i * i + j * j);
}


/**--------------------------------------------------------------------------<BR>
C2DVector::SetDistance <BR>
\brief Sets the distance of the vector.
<P>---------------------------------------------------------------------------*/
void C2DVector::SetLength(double dDistance)
{
	MakeUnit();
	i = i * dDistance;
	j = j * dDistance;
}


/**--------------------------------------------------------------------------<BR>
C2DVector::MakeUnit <BR>
\brief Makes the vector a unit vector.
<P>---------------------------------------------------------------------------*/
void C2DVector::MakeUnit(void)
{
	double dDistance = GetLength();
	if (dDistance == 0 ) return;
	i = i / dDistance;
	j = j / dDistance;
}


/**--------------------------------------------------------------------------<BR>
C2DVector::operator+ <BR>
\brief Addition.
<P>---------------------------------------------------------------------------*/
const C2DVector C2DVector::operator+(const C2DVector& Other) const
{
	return C2DVector(i + Other.i, j + Other.j);
}


/**--------------------------------------------------------------------------<BR>
C2DVector::operator- <BR>
\brief Subtraction.
<P>---------------------------------------------------------------------------*/
const C2DVector C2DVector::operator-(const C2DVector& Other) const
{
	return C2DVector(i - Other.i, j - Other.j);
}


/**--------------------------------------------------------------------------<BR>
C2DVector::operator+= <BR>
\brief Addition.
<P>---------------------------------------------------------------------------*/
void C2DVector::operator+=(const C2DVector& Other)
{
	i += Other.i;
	j += Other.j;
}


/**--------------------------------------------------------------------------<BR>
C2DVector::operator-= <BR>
\brief Subtraction.
<P>---------------------------------------------------------------------------*/
void C2DVector::operator-=(const C2DVector& Other)
{
	i -= Other.i;
	j -= Other.j;
}


/**--------------------------------------------------------------------------<BR>
C2DVector::operator* <BR>
\brief Factor multiplication.
<P>---------------------------------------------------------------------------*/
const C2DVector C2DVector::operator*(const double dFactor) const
{
	return C2DVector(i * dFactor, j * dFactor);
}


/**--------------------------------------------------------------------------<BR>
C2DVector::operator*= <BR>
\brief Factor multiplication.
<P>---------------------------------------------------------------------------*/
void C2DVector::operator*=(const double dFactor)
{
	i = i * dFactor;
	j = j * dFactor;
}

/**--------------------------------------------------------------------------<BR>
C2DVector::Dot <BR>
\brief Dot product of another vector.
a.b = |a||b|cos(theta) where theta is the angle between the 2.
|a|cos(theta) is the scalar projection of a onto b.
In other words - if you take the dot product of a and b and then divide that by
the length of b you get the length of a projected onto b. See diagram.
<P>---------------------------------------------------------------------------*/
const double C2DVector::Dot(const C2DVector& Other) const
{
	return i * Other.i + j * Other.j;
}

/**--------------------------------------------------------------------------<BR>
C2DVector::Cross <BR>
\brief Cross product of another vector.
<P>---------------------------------------------------------------------------*/
const double C2DVector::Cross(const C2DVector& Other) const
{
	return i * Other.i - j * Other.j;
}



/**--------------------------------------------------------------------------<BR>
C2DVector::operator= <BR>
\brief Sets to another.
<P>---------------------------------------------------------------------------*/
const C2DVector& C2DVector::operator=(const C2DVector& Other)
{
	i = Other.i;
	j = Other.j;
	return *this;

}


/**--------------------------------------------------------------------------<BR>
C2DVector::operator== <BR>
\brief Equality test. Note that due to use of doubles, a tolerance is used.
<P>---------------------------------------------------------------------------*/
bool C2DVector::operator==(const C2DVector& Other) const
{
	bool biClose;
	bool bjClose;

	if ( i == 0 )
		biClose = Other.i == 0;
	else
		biClose = fabs((Other.i - i ) / i ) < conEqualityTolerance;
	if ( j == 0 )
		bjClose = Other.j == 0;
	else
		bjClose = fabs((Other.j - j ) / j ) < conEqualityTolerance;

	return ( biClose &&  bjClose );
}


/**--------------------------------------------------------------------------<BR>
C2DVector::operator!= <BR>
\brief Inequality test.
<P>---------------------------------------------------------------------------*/
bool C2DVector::operator!=(const C2DVector& Other) const
{
	return ! this->operator ==(Other);
}


/**--------------------------------------------------------------------------<BR>
C2DVector::AngleFromNorth <BR>
\brief Returns the positive angle from the the positive y axis (north) in radians.
<P>---------------------------------------------------------------------------*/
double C2DVector::AngleFromNorth(void) const
{
	if ( j == 0)
	{
		if(i > 0)
			return conHALFPI;
		else
			return 3 * conHALFPI;
	}
	if ( i == 0)
	{
		if(j > 0)
			return 0;
		else
			return conPI;
	}

	double ang = atan(i/j);

	if(  j < 0)	ang += conPI;
	else if (i < 0 )ang += 2*conPI;

	return ang;
}


/**--------------------------------------------------------------------------<BR>
C2DVector::AngleToRight <BR>
\brief Returns the positive angle from this to the given vector.
<P>---------------------------------------------------------------------------*/
double C2DVector::AngleToRight(const C2DVector& Other) const
{
	double Result = Other.AngleFromNorth() - AngleFromNorth();
	if (Result < 0) Result += conTWOPI;	

	return Result;
}

/**--------------------------------------------------------------------------<BR>
C2DVector::AngleToLeft <BR>
\brief Returns the positive angle from this to the given vector.
<P>---------------------------------------------------------------------------*/
double C2DVector::AngleToLeft(const C2DVector& Other) const
{
	return (conTWOPI - AngleToRight(Other));
}

/**--------------------------------------------------------------------------<BR>
C2DVector::TurnRight <BR>
\brief Turns the vector right by the angle given in radians.
<P>---------------------------------------------------------------------------*/
void C2DVector::TurnRight(double dAng)
{
	TurnLeft(conTWOPI - dAng);
}


/**--------------------------------------------------------------------------<BR>
C2DVector::AngleBetween <BR>
\brief Returns the shortest angle between 2 vectors i.e. the dot product of the 
norms.
<P>---------------------------------------------------------------------------*/
double C2DVector::AngleBetween(const C2DVector& Other) const
{	
	double dDot = this->Dot(Other);
	dDot /= ( this->GetLength() * Other.GetLength());
	return acos(dDot);
}


/**--------------------------------------------------------------------------<BR>
C2DVector::TurnLeft <BR>
\brief Turns the vector left by the angle given in radians.
<P>---------------------------------------------------------------------------*/
void C2DVector::TurnLeft(double dAng)
{
	double temp = i;

	i = cos(dAng)* i - sin(dAng)* j ;
	j = sin(dAng)* temp + cos(dAng)* j ;
}

/**--------------------------------------------------------------------------<BR>
C2DVector::operator = <BR>
\brief Assignment to a point.
<P>---------------------------------------------------------------------------*/
const C2DVector& C2DVector::operator=(const C2DPoint& Other)
{
	i = Other.x;
	j = Other.y;

	return *this;
}