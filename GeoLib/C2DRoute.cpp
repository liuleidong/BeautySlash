/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DRoute.cpp
\brief Declaration file for a class representing a set of connected points.

Implementation file for a class representing a set of connected points which
do not necassarily join.
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "C2DRoute.h"
#include "Interval.h"
#include "C2DLine.h"

_MEMORY_POOL_IMPLEMENATION(C2DRoute)


/**--------------------------------------------------------------------------<BR>
C2DRoute::C2DRoute
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DRoute::C2DRoute(void): C2DBase(Route)
{
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::~C2DRoute
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DRoute::~C2DRoute(void)
{
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::AddPoint
\brief Adds a point to the collection.
<P>---------------------------------------------------------------------------*/
void C2DRoute::AddPoint(const C2DPoint& pt)
{
	m_Points.AddCopy(pt);
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::AddPointDirect
\brief Adds a point to the end of the route. Pointer now responsibility of the Route.
<P>---------------------------------------------------------------------------*/
void C2DRoute::AddPointDirect( C2DPoint* pt)
{
	m_Points.Add(pt);
	
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::InsertPointDirect
\brief Inserts a point in the route. Pointer now responsibility of the Route.
<P>---------------------------------------------------------------------------*/
void C2DRoute::InsertPointDirect(unsigned int nIndx, C2DPoint* pt)
{
	m_Points.InsertAt(nIndx, pt);
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::RemoveLast
\brief Removes the last point from the collection.
<P>---------------------------------------------------------------------------*/
void C2DRoute::RemoveLast(void)
{
	assert(m_Points.size() >= 1);

	if (m_Points.size() > 0)
		m_Points.DeleteLast();
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::Clear
\brief Removes all the points
<P>---------------------------------------------------------------------------*/
void C2DRoute::Clear(void)
{
	m_Points.DeleteAll();
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::GetPoint
\brief Returns a reference to the point at the given index.
<P>---------------------------------------------------------------------------*/
const C2DPoint& C2DRoute::GetPoint(int nPointIndex) const
{
	return m_Points[nPointIndex];
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::operator=
\brief Assignment.
<P>---------------------------------------------------------------------------*/
const C2DRoute& C2DRoute::operator=(const C2DRoute& Other)
{
	m_Points.DeleteAll();

	for (int i = 0; i < Other.GetPointsCount(); i++)
	{
		m_Points.AddCopy(Other.GetPoint(i));
	}
	return *this;
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::Crosses
\brief True if the given lines crosses any line in the route. Records the intersection
points.
<P>---------------------------------------------------------------------------*/
bool C2DRoute::Crosses(const C2DLine& Line, C2DPointSet* IntersectionPts)
{
	bool bResult = false;

	IntersectionPts->DeleteAll();

	for (unsigned int i = 0; i < m_Points.size() - 1; i ++)
	{
		C2DLine Test(m_Points[i], m_Points[i+1]);
		if (Test.Crosses( Line , IntersectionPts ))
		{
			bResult = true;
		}
	}	
	return bResult;
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::AddIfCommonEnd
\brief Adds the points of another to this if they have a common end which is either
the first or last point. Returns true is the was a match.
<P>---------------------------------------------------------------------------*/
bool C2DRoute::AddIfCommonEnd(C2DRoute& Other)
{
	assert(!IsClosed());
	assert(!Other.IsClosed());

	int nThisCount = m_Points.size() ;
	if (nThisCount < 1) return false;

	int nOtherCount = Other.GetPointsCount();
	if (nOtherCount < 1) return false;

	if ( m_Points[0] == Other.GetPoint(0))
	{
		delete Other.ExtractPoint(0);
		while (Other.GetPointsCount() > 0)
			m_Points.InsertAt(0, Other.ExtractPoint(0));

		return true;
	}
	else if(m_Points[0] == Other.GetPoint(nOtherCount - 1))
	{
		delete Other.ExtractLast();
		while (Other.GetPointsCount() > 0)
			m_Points.InsertAt(0, Other.ExtractLast());

		return true;
	}
	else if (m_Points[nThisCount - 1] == Other.GetPoint(0))
	{
		m_Points.DeleteLast();	
		while (Other.GetPointsCount() > 0)
			m_Points.Add(Other.ExtractPoint(0));

		return true;
	}	
	else if (m_Points[nThisCount - 1] == Other.GetPoint(nOtherCount - 1))
	{
		m_Points.DeleteLast();
		while (Other.GetPointsCount() > 0)
			m_Points.Add(Other.ExtractLast());

		return true;
	}	

	return false;
}

/**--------------------------------------------------------------------------<BR>
C2DRoute::HasRepeatedPoints
\brief Checks for repeated points.
<P>---------------------------------------------------------------------------*/
bool C2DRoute::HasRepeatedPoints(void) const
{
	for (unsigned int i = 0 ; i < m_Points.size(); i++)
	{
		for (unsigned int r = i + 1; r < m_Points.size(); r++)
		{
			if (m_Points[i] == m_Points[r]) 
				return true;
		}
	}
	return false;
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::IsClosed
\brief True if the last point matches the first.
<P>---------------------------------------------------------------------------*/
bool C2DRoute::IsClosed(void) const
{
	int nCount = m_Points.size();

	if ( nCount == 0)
		return false;

	return (m_Points[0] == m_Points[nCount - 1]);

}



/**--------------------------------------------------------------------------<BR>
C2DRoute::PurgeRepeatedAdjacentPoints
\brief Removes repeated points that are next to one another. Used when cleaning 
up a route after integer based intersections which can result in repeated points.
<P>---------------------------------------------------------------------------*/
void C2DRoute::PurgeRepeatedAdjacentPoints(void)
{
	unsigned int nIndx = 1;
	while (nIndx < m_Points.size())
	{
		if (m_Points[nIndx - 1] == m_Points[nIndx])
		{
			if (nIndx < m_Points.size() / 2)
			{
				// Start of the array.
				m_Points.DeleteAt(nIndx);
			}
			else
			{
				// Nearer the end.
				m_Points.DeleteAt(nIndx - 1);
			}
		}
		else
			nIndx ++;
	}

}

/**--------------------------------------------------------------------------<BR>
C2DRoute::Move
\brief Moves the points by the vector given.
<P>---------------------------------------------------------------------------*/
void C2DRoute::Move(const C2DVector& Vector)
{
	m_Points.Move(Vector);
}

/**--------------------------------------------------------------------------<BR>
C2DRoute::RotateToRight
\brief Rotates the points by the vector given.
<P>---------------------------------------------------------------------------*/
void C2DRoute::RotateToRight(double dAng, const C2DPoint& Origin)
{
	m_Points.RotateToRight(dAng,  Origin);
}

/**--------------------------------------------------------------------------<BR>
C2DRoute::Grow
\brief Grow around an origin.
<P>---------------------------------------------------------------------------*/
void C2DRoute::Grow(double dFactor, const C2DPoint& Origin)
{
	m_Points.Grow(dFactor,  Origin);
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::Reflect
\brief Reflection in a point.
<P>---------------------------------------------------------------------------*/
void C2DRoute::Reflect(const C2DPoint& Point)
{
	m_Points.Reflect( Point);
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::Reflect
\brief Reflection in a point.
<P>---------------------------------------------------------------------------*/
void C2DRoute::Reflect(const C2DLine& Line)
{
	m_Points.Reflect( Line);
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::Distance
\brief Distance to a point.
<P>---------------------------------------------------------------------------*/
double C2DRoute::Distance(const C2DPoint& TestPoint) const
{
	if (m_Points.size() == 0)
		return 0;
	if (m_Points.size() == 1)
		return m_Points[0].Distance(TestPoint);

	double dDist = 0;
	C2DLine TestLine(m_Points[0], m_Points[1]);
	double dResult = TestLine.Distance(TestPoint);

	for (unsigned int i = 1 ; i < m_Points.size() - 1; i++)
	{
		TestLine.Set(m_Points[i], m_Points[i + 1]);
		dDist = TestLine.Distance(TestPoint);
		if (dDist < dResult)
			dResult = dDist;
	}

	return dResult;
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::GetLinesCount
\brief Line count.
<P>---------------------------------------------------------------------------*/
unsigned int  C2DRoute::GetLinesCount() const 
{ 
	if (m_Points.size() == 0)
		return 0;
	else 
		return m_Points.size() - 1;

}


/**--------------------------------------------------------------------------<BR>
C2DRoute::InsertOptimally
\brief Inserts the pointer to the C2DPoint in the line such that the perimiter is minimised.
<P>---------------------------------------------------------------------------*/
void C2DRoute::InsertOptimally( C2DPoint* pt)
{
	C2DLine Line( m_Points[0] , m_Points[1]);

	double dResult = Line.Distance(*pt);
	unsigned int nClosest = 0 ;

	for (unsigned int i = 1; i < GetLinesCount(); i++)
	{
		Line.Set( m_Points[i] , m_Points[i + 1]);
		double dDist = Line.Distance(*pt);
		if (dDist < dResult)
		{
			dResult = dDist;
			nClosest = i;
		}
	}

	this->m_Points.InsertAt(nClosest + 1, pt);
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::Distance
\brief Distance to a point.
<P>---------------------------------------------------------------------------*/
double C2DRoute::Distance(const C2DPoint& TestPoint, unsigned int& nClosestLine) const
{
	C2DLine Line( m_Points[0] , m_Points[1]);

	double dResult = Line.Distance(TestPoint);
	nClosestLine = 0 ;

	for (unsigned int i = 1; i < GetLinesCount(); i++)
	{
		Line.Set( m_Points[i] , m_Points[i + 1]);
		double dDist = Line.Distance(TestPoint);
		if (dDist < dResult)
		{
			dResult = dDist;
			nClosestLine = i;
		}
	}

	
	return dResult;
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::GetBoundingRect
\brief Returns the bounding rectangle of the route.
<P>---------------------------------------------------------------------------*/
void C2DRoute::GetBoundingRect(C2DRect& Rect) const
{
	m_Points.GetBoundingRect(Rect);
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::Project
\brief Projects this onto the line given.
<P>---------------------------------------------------------------------------*/
void C2DRoute::Project(const C2DLine& Line, CInterval& Interval) const
{
	if (m_Points.size() == 0)
		return;

	m_Points[0].Project(  Line,  Interval);

	for (unsigned int i = 1; i < m_Points.size(); i++)
	{
		Interval.ExpandToInclude( m_Points[i].Project( Line)  );
	}
}


/**--------------------------------------------------------------------------<BR>
C2DRoute::Project
\brief Projects this onto the vector given.
<P>---------------------------------------------------------------------------*/
void C2DRoute::Project(const C2DVector& Vector, CInterval& Interval) const
{
	if (m_Points.size() == 0)
		return;

	m_Points[0].Project(  Vector,  Interval);

	for (unsigned int i = 1; i < m_Points.size(); i++)
	{
		Interval.ExpandToInclude( m_Points[i].Project( Vector)  );
	}
}

/**--------------------------------------------------------------------------<BR>
C2DRoute::SnapToGrid
\brief .
<P>---------------------------------------------------------------------------*/
void C2DRoute::SnapToGrid(void)
{
	m_Points.SnapToGrid();
}