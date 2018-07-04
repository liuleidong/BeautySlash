/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DSegment.cpp
\brief Implementation file for C2DSegment.

Implementation file for C2DSegment which is a segment of a circle.
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "C2DSegment.h"
#include "C2DCircle.h"
#include "Interval.h"


_MEMORY_POOL_IMPLEMENATION(C2DSegment)

/**--------------------------------------------------------------------------<BR>
C2DSegment::C2DSegment
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DSegment::C2DSegment(void) : C2DBase(Segment)
{
}

/**--------------------------------------------------------------------------<BR>
C2DSegment::~C2DSegment
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DSegment::~C2DSegment(void)
{
}

/**--------------------------------------------------------------------------<BR>
C2DSegment::C2DSegment
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DSegment::C2DSegment(const C2DArc& Arc) : C2DBase(Segment)
{
	m_Arc = Arc;

}

/**--------------------------------------------------------------------------<BR>
C2DSegment::C2DSegment
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DSegment::C2DSegment(const C2DPoint& PtFrom, const C2DPoint& PtTo, double dRadius, 
					   bool bCentreOnRight, bool bArcOnRight)
: m_Arc(PtFrom, PtTo, dRadius, bCentreOnRight, bArcOnRight), C2DBase(Segment)
{


}

/**--------------------------------------------------------------------------<BR>
C2DSegment::C2DSegment
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DSegment::C2DSegment(const C2DPoint& PtFrom, const C2DVector& Vector, double dRadius, 
					   bool bCentreOnRight, bool bArcOnRight)
: m_Arc(PtFrom, Vector, dRadius, bCentreOnRight, bArcOnRight), C2DBase(Segment)
{
	
}

/**--------------------------------------------------------------------------<BR>
C2DSegment::C2DSegment
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DSegment::C2DSegment(const C2DLine& Line, double dRadius, bool bCentreOnRight
					   ,bool bArcOnRight )
: m_Arc(Line, dRadius, bCentreOnRight, bArcOnRight), C2DBase(Segment)
{

}

/**--------------------------------------------------------------------------<BR>
C2DSegment::Set
\brief Assignment.
<P>---------------------------------------------------------------------------*/
void C2DSegment::Set(const C2DPoint& PtFrom, const C2DPoint& PtTo, double dRadius, 
	bool bCentreOnRight, bool bArcOnRight)
{
	m_Arc.Set(PtFrom, PtTo, dRadius, bCentreOnRight, bArcOnRight);
}

/**--------------------------------------------------------------------------<BR>
C2DSegment::Set
\brief Assignment.
<P>---------------------------------------------------------------------------*/
void C2DSegment::Set(const C2DPoint& PtFrom, const C2DVector& Vector, double dRadius, 
	bool bCentreOnRight, bool bArcOnRight)
{
	m_Arc.Set(PtFrom, Vector, dRadius, bCentreOnRight, bArcOnRight);
}

/**--------------------------------------------------------------------------<BR>
C2DSegment::Set
\brief Assignment.
<P>---------------------------------------------------------------------------*/
void C2DSegment::Set(const C2DLine& Line, double dRadius, 
	bool bCentreOnRight, bool bArcOnRight)
{
	m_Arc.Set(Line, dRadius, bCentreOnRight, bArcOnRight);

}


/**--------------------------------------------------------------------------<BR>
C2DSegment::IsValid
\brief Invalid if the circle radius is too small to join the 2 points on the line.
<P>---------------------------------------------------------------------------*/
bool C2DSegment::IsValid(void) const
{
	return (m_Arc.GetLine().vector.GetLength() < 2 * m_Arc.GetRadius());
}

/**--------------------------------------------------------------------------<BR>
C2DSegment::GetCircleCentre
\brief Finds the centre of the corresponding circle.
<P>---------------------------------------------------------------------------*/
C2DPoint C2DSegment::GetCircleCentre(void) const
{
	return m_Arc.GetCircleCentre();

}

/**--------------------------------------------------------------------------<BR>
C2DSegment::GetPerimeter
\brief Find the permimeter.
<P>---------------------------------------------------------------------------*/
double C2DSegment::GetPerimeter(void) const
{
	return (m_Arc.GetLine().vector.GetLength() + m_Arc.GetLength());
}

/**--------------------------------------------------------------------------<BR>
C2DSegment::GetArcLength
\brief Gets the length of the arc.
<P>---------------------------------------------------------------------------*/
double C2DSegment::GetArcLength(void) const
{
	return m_Arc.GetLength();
}


/**--------------------------------------------------------------------------<BR>
C2DSegment::GetSegmentAngle
\brief Gets the angle of the segment. Always +ve and less than PI. In radians.
<P>---------------------------------------------------------------------------*/
double C2DSegment::GetSegmentAngle(void) const
{
	if (!IsValid()) return 0;

	return m_Arc.GetSegmentAngle();
}

/**--------------------------------------------------------------------------<BR>
C2DSegment::GetArea
\brief Calculates the area.
<P>---------------------------------------------------------------------------*/
double C2DSegment::GetArea(void) const
{
	double dSegAng = m_Arc.GetSegmentAngle();
	double dRadius = m_Arc.GetRadius();
	if (m_Arc.GetCentreOnRight() ^ m_Arc.GetArcOnRight())
	{
		return (   dRadius * dRadius * ( dSegAng - sin(dSegAng)) / 2);

	}
	else
	{
		// if the curve is the big bit.
		return   (   dRadius * dRadius * (conPI - ( dSegAng - sin(dSegAng)) / 2));
	}
}

/**--------------------------------------------------------------------------<BR>
C2DSegment::GetAreaSigned
\brief Returns the area which is positive if anti-clockwise -ve if clockwise
<P>---------------------------------------------------------------------------*/
double C2DSegment::GetAreaSigned(void) const
{

	if (m_Arc.GetArcOnRight())
		return GetArea();
	else
		return -GetArea();

}


/**--------------------------------------------------------------------------<BR>
C2DSegment::GetInverse
\brief Returns the inverse of this i.e. the other part of the circle to this.
<P>---------------------------------------------------------------------------*/
void C2DSegment::GetInverse(C2DSegment& Other) const
{
	Other.Set(m_Arc.GetLine(), m_Arc.GetRadius(), 
				m_Arc.GetCentreOnRight(), !m_Arc.GetArcOnRight() );
}


/**--------------------------------------------------------------------------<BR>
C2DSegment::GetCentroid
\brief Returns the centroid.
<P>---------------------------------------------------------------------------*/
C2DPoint C2DSegment::GetCentroid(void) const
{
	// Find the area first. Do it explicitly as we may need bits of the calc later.
	double dSegAng = m_Arc.GetSegmentAngle();
	bool bBig = m_Arc.GetArcOnRight() == m_Arc.GetCentreOnRight();

	double dRadius = m_Arc.GetRadius();
	double dRadiusSquare = dRadius * dRadius;
	double dCircleArea = dRadiusSquare * conPI;
	double dArea = dRadiusSquare * ( (dSegAng - sin(dSegAng)) / 2);

	// Find the maximum length of the small segment along the direction of the line.
	double dLength = m_Arc.GetLine().GetLength();
	// Now find the average height of the segment over that line
	double dHeight = dArea / dLength;

	// Find the centre point on the line and the centre of the circle
	C2DPoint ptLineCen = m_Arc.GetLine().GetMidPoint();
	C2DPoint ptCircleCen = m_Arc.GetCircleCentre();

	// Set up a line from the mid point on the line to the circle centre
	// then set the length of it to the average height divided by 2. The end
	// point of the line is then the centroid. If we are using the small bit, 
	// The line needs to be reversed.
	C2DLine Line( ptLineCen, ptCircleCen);

	Line.vector.Reverse();

	Line.vector.SetLength(  dHeight / 2 );

	if (bBig)
	{
		C2DPoint ptSmallCen = Line.GetPointTo();
		// Return the weighted average of the 2 centroids.
		return ( C2DPoint(ptCircleCen * dCircleArea - ptSmallCen * dArea) ) / ( dCircleArea - dArea);
	}
	else
		return Line.GetPointTo();
}




/**--------------------------------------------------------------------------<BR>
C2DSegment::GetBoundingRect
\brief Returns the bounding rectangle.
<P>---------------------------------------------------------------------------*/
void C2DSegment::GetBoundingRect(C2DRect& Rect) const
{
	m_Arc.GetBoundingRect(Rect);
}


/**--------------------------------------------------------------------------<BR>
C2DSegment::Contains
\brief True if the point is inside the shape.
<P>---------------------------------------------------------------------------*/
bool C2DSegment::Contains( const C2DPoint& TestPoint) const
{
	C2DPoint ptCentre = GetCircleCentre();
	
	if (TestPoint.Distance(ptCentre) > m_Arc.GetRadius()) 
		return false;

	else 
	{
		if (m_Arc.GetLine().IsOnRight(TestPoint))
		{
			return m_Arc.GetArcOnRight();
		}
		else
		{
			return !m_Arc.GetArcOnRight();
		}
	}
}

/**--------------------------------------------------------------------------<BR>
C2DSegment::Distance
\brief Returns the distance to the point given, 0 if the points inside.
<P>---------------------------------------------------------------------------*/
double C2DSegment::Distance(const C2DPoint& TestPoint) const
{
	if (Contains(TestPoint))
		return 0;

	return min(m_Arc.Distance(TestPoint), m_Arc.GetLine().Distance(TestPoint));

}


/**--------------------------------------------------------------------------<BR>
C2DSegment::Distance
\brief Projects this onto the line given.
<P>---------------------------------------------------------------------------*/
void C2DSegment::Project(const C2DLine& Line, CInterval& Interval) const
{
	m_Arc.Project(Line, Interval);
	CInterval LineInterval;
	m_Arc.GetLine().Project(Line, LineInterval);
	Interval.ExpandToInclude( LineInterval );


}


/**--------------------------------------------------------------------------<BR>
C2DSegment::Distance
\brief Projects this onto the vector given.
<P>---------------------------------------------------------------------------*/
void C2DSegment::Project(const C2DVector& Vector, CInterval& Interval) const
{
	m_Arc.Project(Vector, Interval);
	CInterval LineInterval;
	m_Arc.GetLine().Project(Vector, LineInterval);
	Interval.ExpandToInclude( LineInterval );
}


/**--------------------------------------------------------------------------<BR>
C2DSegment::SnapToGrid
\brief See CGrid.
<P>---------------------------------------------------------------------------*/
void C2DSegment::SnapToGrid(void)
{
	m_Arc.SnapToGrid();

}