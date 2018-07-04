/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 3DPoint.cpp
\brief Implementation file for a 3D point class.

Implementation file for a simple 3D point class.
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "C3DPoint.h"

_MEMORY_POOL_IMPLEMENATION(C3DPoint)

/**--------------------------------------------------------------------------<BR>
C3DPoint::C3DPoint <BR>
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C3DPoint::C3DPoint(void)
{
}

/**--------------------------------------------------------------------------<BR>
C3DPoint::~C3DPoint <BR>
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C3DPoint::~C3DPoint(void)
{
}

/**--------------------------------------------------------------------------<BR>
C3DPoint::Distance <BR>
\brief Distance to another.
<P>---------------------------------------------------------------------------*/
double C3DPoint::Distance(const C3DPoint& Other) const
{
	double dx = x - Other.x;
	double dy = y - Other.y;
	double dz = z - Other.z;
	return sqrt(  dx*dx + dy * dy + dz * dz);

}