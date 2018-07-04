/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DTriangle.cpp
\brief Implementation file for the C2DTriangle Class.

Implementation file for C2DTriangle, a class which represents a triangle.
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "C2DTriangle.h"
#include "C2DLine.h"
#include "C2DPointSet.h"
#include "Interval.h"
#include "C2DRect.h"


_MEMORY_POOL_IMPLEMENATION(C2DTriangle)

/**--------------------------------------------------------------------------<BR>
C2DTriangle::C2DTriangle <BR>
\brief Constructor. 
<P>---------------------------------------------------------------------------*/
C2DTriangle::C2DTriangle(void) : C2DBase(Triangle), m_pt1(0,0), m_pt2(0,0), m_pt3(0,0)
{

}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::~C2DTriangle <BR>
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DTriangle::~C2DTriangle(void)
{
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::C2DTriangle <BR>
\brief Constructor. 
<P>---------------------------------------------------------------------------*/
C2DTriangle::C2DTriangle(const C2DPoint& pt1, const C2DPoint& pt2, const C2DPoint& pt3) : C2DBase(Triangle)
{
	m_pt1 = pt1;
	m_pt2 = pt2;
	m_pt3 = pt3;
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::Set <BR>
\brief Assignement. 
<P>---------------------------------------------------------------------------*/
void C2DTriangle::Set(const C2DPoint& pt1, const C2DPoint& pt2, const C2DPoint& pt3)
{
	m_pt1 = pt1;
	m_pt2 = pt2;
	m_pt3 = pt3;
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::GetFermatPoint <BR>
\brief Returns the Fermat point. 
<P>---------------------------------------------------------------------------*/
C2DPoint C2DTriangle::GetFermatPoint(void) const
{
	return C2DTriangle::GetFermatPoint(m_pt1, m_pt2, m_pt3);

}

/**--------------------------------------------------------------------------<BR>
C2DTriangle::InCentre. <BR>
\brief Returns the InCentre
<P>---------------------------------------------------------------------------*/
C2DPoint C2DTriangle::GetInCentre(void) const
{
	return C2DTriangle::GetInCentre(m_pt1, m_pt2, m_pt3);
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::GetFermatPoint <BR>
\brief Returns the Fermat point. 

To locate the Fermat point:

1. Construct three regular triangles out of the three sides of the given triangle. 
2. For each new vertex of the regular triangle, draw a line from it to the opposite 
triangle's vertex. These three lines intersect at the Fermat point. 
3. For the case that the largest angle of the triangle exceeds 120°, the solution 
is a point on the vertex of that angle.

<P>---------------------------------------------------------------------------*/
C2DPoint C2DTriangle::GetFermatPoint(const C2DPoint& pt1, const C2DPoint& pt2, const C2DPoint& pt3)
{
	C2DLine Line12(pt1, pt2);
	C2DLine Line23(pt2, pt3);
	C2DLine Line31(pt3, pt1);

	double dAng2 = conPI - Line12.vector.AngleBetween(Line23.vector);
	if (dAng2 >= conTWOTHIRDPI) // greater than 120 degrees
	{
		return pt2;
	}
	else if (dAng2 < conTHIRDPI)  // if less than 60 then 1 of the other 2 could be greater than 120
	{
		double dAng3 = conPI - Line23.vector.AngleBetween(Line31.vector);
		if (dAng3 >= conTWOTHIRDPI) // greater than 120 degrees
		{
			return pt3;
		}
		else if (  (conPI - dAng2 - dAng3) >= conTWOTHIRDPI) // if least angle is greater than 120
		{
			return pt1;
		}
	}

	bool bClockwise = Line12.IsOnRight(pt3);

	if (bClockwise)
	{
		Line12.vector.TurnLeft(conTHIRDPI);		// 60 degrees
		Line23.vector.TurnLeft(conTHIRDPI);		// 60 degrees
	}
	else
	{
		Line12.vector.TurnRight(conTHIRDPI);	// 60 degrees
		Line23.vector.TurnRight(conTHIRDPI);	// 60 degrees
	}
	
	Line12.SetPointFrom(pt3);
	Line23.SetPointFrom(pt1);	
	
	C2DPointSet IntPt;
	if (Line12.Crosses(Line23, &IntPt))
	{
		return IntPt[0];
	}
	else
	{
		assert(0);
	}	

	return 	C2DPoint(0, 0);
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::GetCircumCentre <BR>
\brief Static version of InCentre function.

The center of the incircle can be found as the intersection of the three internal 
angle bisectors. The center of an excircle is the intersection of the internal
bisector of one angle and the external bisectors of the other two.
<P>---------------------------------------------------------------------------*/
C2DPoint C2DTriangle::GetInCentre(const C2DPoint& pt1, const C2DPoint& pt2, const C2DPoint& pt3)
{
	/// Set up a line to bisect the lines from 1 to 2 and 1 to 3
	C2DLine Line1(pt1, pt2);
	C2DLine Line2(pt1, pt3);
	Line1.SetLength( Line2.GetLength() );
	C2DLine Line12Bisect(  pt1, pt3.GetMidPoint( Line1.GetPointTo()));

	/// Set up a line to bisect the lines from 2 to 1 and 2 to 3
	C2DLine Line3(pt2, pt1);
	C2DLine Line4(pt2, pt3);
	Line3.SetLength( Line4.GetLength() );
	C2DLine Line34Bisect(  pt2, pt3.GetMidPoint( Line3.GetPointTo()));

	// Now intersect the 2 lines and find the point.
	C2DPointSet Int;

	// Add the intersection even if there isn't one (i.e. infinite lines)
	Line12Bisect.Crosses(Line34Bisect, &Int, 0, 0, true);

	assert (Int.size() == 1);

	return Int[0];
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::GetCircumCentre <BR>
\brief Returns the circumcentre. 
<P>---------------------------------------------------------------------------*/
C2DPoint C2DTriangle::GetCircumCentre(void) const
{
	return C2DTriangle::GetCircumCentre(m_pt1, m_pt2, m_pt3);

}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::GetCircumCentre <BR>
\brief Returns the circumcentre. 

The circumcenter of a triangle can be found as the intersection of the three 
perpendicular bisectors. (A perpendicular bisector is a line that forms a right 
angle with one of the triangle's sides and intersects that side at its midpoint.) 
This is because the circumcenter is equidistant from any pair of the triangle's 
points, and all points on the perpendicular bisectors are equidistant from those 
points of the triangle.

The circumcentre's position depends on the type of triangle:

If and only if a triangle is acute (all angles smaller than a right angle), the 
circumcenter lies inside the triangle.
If and only if it is obtuse (has one angle bigger than a right angle), the 
circumcenter lies outside.
If and only if it is a right triangle, the circumcenter lies on one of its sides 
(namely, the hypotenuse). This is one form of Thales' theorem.
<P>---------------------------------------------------------------------------*/
C2DPoint C2DTriangle::GetCircumCentre(const C2DPoint& pt1, const C2DPoint& pt2, const C2DPoint& pt3)
{
	C2DLine Line12(pt1, pt2);
	C2DLine Line23(pt2, pt3);
	
	// Move the lines to start from the midpoint on them
	Line12.point = Line12.GetMidPoint();
	Line23.point = Line23.GetMidPoint();
	// Turn them right (left would work as well)
	Line12.vector.TurnRight();
	Line23.vector.TurnRight();
	// Find the intersection between them taking the intersect point even if they don't 
	// intersect directly (i.e. where they would intersect because we may have turned them
	// the wrong way).
	C2DPointSet IntPt;
	Line12.Crosses(Line23, &IntPt, 0, 0, true);

	C2DPoint ptResult(0, 0);

	if (IntPt.size() == 1)
	{
		ptResult = IntPt[0];
	}
	else
	{
		// co-linear so fail.
		assert(0);
	}

	return ptResult;
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::GetAreaSigned <BR>
\brief Returns the area signed. 
<P>---------------------------------------------------------------------------*/
double C2DTriangle::GetAreaSigned(void) const
{
	return C2DTriangle::GetAreaSigned(m_pt1, m_pt2, m_pt3);
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::GetArea <BR>
\brief returns the area. 
<P>---------------------------------------------------------------------------*/
double C2DTriangle::GetArea(void) const
{
	return fabs(C2DTriangle::GetAreaSigned(m_pt1, m_pt2, m_pt3));
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::GetAreaSigned <BR>
\brief Returns the area signed. 
<P>---------------------------------------------------------------------------*/
double C2DTriangle::GetAreaSigned(const C2DPoint& pt1, const C2DPoint& pt2, const C2DPoint& pt3)
{
	double dArea = pt1.x * pt2.y - pt2.x * pt1.y +
				   pt2.x * pt3.y - pt3.x * pt2.y +
				   pt3.x * pt1.y - pt1.x * pt3.y;

	dArea /= 2.0;

	return dArea;
}

/**--------------------------------------------------------------------------<BR>
C2DTriangle::IsClockwise <BR>
\brief Returns whether the triangle is clockwise.
<P>---------------------------------------------------------------------------*/
bool C2DTriangle::IsClockwise(const C2DPoint& pt1, const C2DPoint& pt2, const C2DPoint& pt3)
{
	return GetAreaSigned(pt1, pt2, pt3) < 0;
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::Collinear <BR>
\brief True if collinear. 
<P>---------------------------------------------------------------------------*/
bool C2DTriangle::Collinear(void) const
{
	return (GetAreaSigned() == 0);
	
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::Collinear <BR>
\brief True if collinear.  
<P>---------------------------------------------------------------------------*/
bool C2DTriangle::Collinear(const C2DPoint& pt1, const C2DPoint& pt2, const C2DPoint& pt3)
{
	return ( C2DTriangle::GetAreaSigned(pt1, pt2, pt3) == 0);
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::GetPerimeter <BR>
\brief Returns the perimeter. 
<P>---------------------------------------------------------------------------*/
double C2DTriangle::GetPerimeter(void) const
{
	return m_pt1.Distance(m_pt2) + m_pt2.Distance(m_pt3) + m_pt3.Distance(m_pt1);
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::Move <BR>
\brief Moves the triangle. 
<P>---------------------------------------------------------------------------*/

void C2DTriangle::Move(const C2DVector& Vector)
{
	m_pt1.Move(Vector);
	m_pt2.Move(Vector);
	m_pt3.Move(Vector);
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::RotateToRight <BR>
\brief Rotation. 
<P>---------------------------------------------------------------------------*/
void C2DTriangle::RotateToRight(double dAng, const C2DPoint& Origin)
{
	m_pt1.RotateToRight(dAng, Origin);
	m_pt2.RotateToRight(dAng, Origin);
	m_pt3.RotateToRight(dAng, Origin);
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::Grow <BR>
\brief Grows the shape around the origin. 
<P>---------------------------------------------------------------------------*/
void C2DTriangle::Grow(double dFactor, const C2DPoint& Origin)
{
	m_pt1.Grow(dFactor, Origin);
	m_pt2.Grow(dFactor, Origin);
	m_pt3.Grow(dFactor, Origin);
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::Reflect <BR>
\brief Reflects the triangle. 
<P>---------------------------------------------------------------------------*/
void C2DTriangle::Reflect(const C2DPoint& Point)
{
	m_pt1.Reflect(Point);
	m_pt2.Reflect(Point);
	m_pt3.Reflect(Point);
}

/**--------------------------------------------------------------------------<BR>
C2DTriangle::Reflect <BR>
\brief Reflects the triangle in the line. 
<P>---------------------------------------------------------------------------*/
void C2DTriangle::Reflect(const C2DLine& Line)
{
	m_pt1.Reflect(Line);
	m_pt2.Reflect(Line);
	m_pt3.Reflect(Line);
}

/**--------------------------------------------------------------------------<BR>
C2DTriangle::Distance <BR>
\brief Distance to a point. 
<P>---------------------------------------------------------------------------*/
double C2DTriangle::Distance(const C2DPoint& ptTest) const
{
	return Distance(ptTest, 0);
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::Distance <BR>
\brief Distance to a point. 
<P>---------------------------------------------------------------------------*/
double C2DTriangle::Distance(const C2DPoint& ptTest, C2DPoint* ptOnThis) const
{
	double dArea = GetAreaSigned();

	// Construct the lines.
	C2DLine Line12(m_pt1, m_pt2);
	C2DLine Line23(m_pt2, m_pt3);
	C2DLine Line31(m_pt3, m_pt1);

	if (dArea == 0)
	{
		// Colinear so find the biggest line and return the distance from that
		double d1 = Line12.GetLength();
		double d2 = Line23.GetLength();
		double d3 = Line31.GetLength();
		if (d1 > d2 && d1 > d3)
			return Line12.Distance(ptTest, ptOnThis);
		else if (d2 > d3)
			return Line23.Distance(ptTest, ptOnThis);
		else
			return Line31.Distance(ptTest, ptOnThis);
	}
	// Find out it the triangle is clockwise or not.
	bool bClockwise = dArea < 0;

	// Set up some pointers to record the lines that the point is "above", "above" meaning that the
	// point is on the opposite side of the line to the rest of the triangle
	C2DLine* LineAbove1 = 0;
	C2DLine* LineAbove2 = 0;

	// Find out which Lines have the point above.
	if (  Line12.IsOnRight( ptTest ) ^ bClockwise  )  // if the pt is on the opposite side to the triangle
		LineAbove1 = &Line12;
	if ( Line23.IsOnRight( ptTest ) ^ bClockwise)
	{
		if (LineAbove1)
			LineAbove2 = &Line23;
		else
			LineAbove1 = &Line23;
	}
	if ( Line31.IsOnRight( ptTest ) ^ bClockwise)
	{
		if (LineAbove1)
		{
			// We can't have all the lines with the point above.
			assert(!LineAbove2);
			LineAbove2 = &Line31;
		}
		else
			LineAbove1 = &Line31;
	}

	// Check for containment (if there isn't a single line that its above then it must be inside)
	if (!LineAbove1)
	{
		// Pt inside so project onto all the lines and find the closest projection (there must be one).
	
		// Set up a record of the point projection on the lines.
		C2DPoint ptOnLine;
		bool bSet = false;
		double dMinDist = 0;
		if (ptTest.ProjectsOnLine( Line12, &ptOnLine))
		{
			dMinDist = ptTest.Distance(ptOnLine);
			if (ptOnThis)
				*ptOnThis = ptOnLine;
			bSet = true;
		}
		if (ptTest.ProjectsOnLine( Line23, &ptOnLine))
		{
			double dDist = ptTest.Distance(ptOnLine);
			if (!bSet || dDist < dMinDist)
			{
				dMinDist = dDist;
				if (ptOnThis)
					*ptOnThis = ptOnLine;
				bSet = true;
			}
		}
		if ( ptTest.ProjectsOnLine( Line31, &ptOnLine))
		{
			double dDist = ptTest.Distance(ptOnLine);
			if (!bSet || dDist < dMinDist)
			{
				dMinDist = dDist;
				if (ptOnThis)
					*ptOnThis = ptOnLine;
				bSet = true;
			}
		}
		assert(bSet);
		return -dMinDist; //-ve if inside
	}
	else if (!LineAbove2)
	{
		// it is only above 1 of the lines so simply return the distance to that line
		return LineAbove1->Distance(ptTest, ptOnThis);
	}
	else
	{
		// It's above 2 lines so first check them both for projection. Can only be projected on 1.
		// If the point can be projected onto the line then that's the closest point.
		C2DPoint ptOnLine;
		if (ptTest.ProjectsOnLine( * LineAbove1, &ptOnLine))
		{
			if (ptOnThis)
				*ptOnThis = ptOnLine;
			return ptOnLine.Distance(ptTest);
		}
		else if (ptTest.ProjectsOnLine( * LineAbove2, &ptOnLine))
		{
			if (ptOnThis)
				*ptOnThis = ptOnLine;
			return ptOnLine.Distance(ptTest);
		}
		else
		{
			// The point doesn't project onto either line so find the closest point
			if (LineAbove1 == &Line12)
			{
				if (LineAbove2 == &Line23)
				{
					if (ptOnThis)
						*ptOnThis = m_pt2;
					return ptTest.Distance(m_pt2);
				}
				else
				{
					if (ptOnThis)
						*ptOnThis = m_pt1;
					return ptTest.Distance(m_pt1);
				}
			}
			else
			{
				if (ptOnThis)
					*ptOnThis = m_pt3;
				return ptTest.Distance(m_pt3);
			}
		}
	}
	
	// shouldn't get to here.
	assert(0);
	return 0;
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::Distance <BR>
\brief Distance to another. 
<P>---------------------------------------------------------------------------*/
double C2DTriangle::Distance(const C2DTriangle& Other, 
			C2DPoint* ptOnThis , C2DPoint* ptOnOther ) const
{

	C2DPoint ptTemp;
	double dMinDist = Distance(Other.GetPoint1(), ptOnThis);
	if (ptOnOther)
		*ptOnOther = Other.GetPoint1();

	double dDist =  Distance(Other.GetPoint2(), &ptTemp);
	if (dDist < dMinDist)
	{
		if (ptOnOther)
			*ptOnOther = Other.GetPoint2();
		if (ptOnThis)
			*ptOnThis = ptTemp;

		dMinDist = dDist; 
	}

	dDist =  Distance(Other.GetPoint3(), &ptTemp);
	if (dDist < dMinDist)
	{
		if (ptOnOther)
			*ptOnOther = Other.GetPoint3();
		if (ptOnThis)
			*ptOnThis = ptTemp;
		dMinDist = dDist; 
	}

	dDist =  Other.Distance( m_pt1, &ptTemp);
	if (dDist < dMinDist)
	{
		if (ptOnOther)
			*ptOnOther = ptTemp;
		if (ptOnThis)
			*ptOnThis = m_pt1;
		dMinDist = dDist; 
	}

	dDist =  Other.Distance( m_pt2, &ptTemp);
	if (dDist < dMinDist)
	{
		if (ptOnOther)
			*ptOnOther = ptTemp;
		if (ptOnThis)
			*ptOnThis = m_pt2;
		dMinDist = dDist; 
	}

	dDist =  Other.Distance( m_pt3, &ptTemp);
	if (dDist < dMinDist)
	{
		if (ptOnOther)
			*ptOnOther = ptTemp;
		if (ptOnThis)
			*ptOnThis = m_pt3;
		dMinDist = dDist; 
	}

	return dMinDist;
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::Contains <BR>
\brief Containment. 
<P>---------------------------------------------------------------------------*/
bool C2DTriangle::Contains(const C2DPoint& ptTest)
{
	bool bClockwise = GetAreaSigned() < 0;

	if ( (GetAreaSigned(m_pt1, m_pt2, ptTest) < 0) ^ bClockwise)
		return false;

	if ( (GetAreaSigned(m_pt2, m_pt3, ptTest) < 0) ^ bClockwise)
		return false;
	
	if ( (GetAreaSigned(m_pt3, m_pt1, ptTest) < 0) ^ bClockwise)
		return false;

	return true;
}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::GetBoundingRect <BR>
\brief Returns the bounding rectangle. 
<P>---------------------------------------------------------------------------*/
void C2DTriangle::GetBoundingRect(C2DRect& Rect) const
{
	Rect.Set(m_pt1);
	Rect.ExpandToInclude(m_pt2);
	Rect.ExpandToInclude(m_pt3);

}


/**--------------------------------------------------------------------------<BR>
C2DTriangle::Project <BR>
\brief Projects this onto the line given.
<P>---------------------------------------------------------------------------*/
void C2DTriangle::Project(const C2DLine& Line, CInterval& Interval) const
{
	m_pt1.Project( Line, Interval);
	Interval.ExpandToInclude( m_pt2.Project(Line));
	Interval.ExpandToInclude( m_pt3.Project(Line));

}

/**--------------------------------------------------------------------------<BR>
C2DTriangle::Project <BR>
\brief Projects this onto the vector given.
<P>---------------------------------------------------------------------------*/
void C2DTriangle::Project(const C2DVector& Vector, CInterval& Interval) const
{
	m_pt1.Project( Vector, Interval);
	Interval.ExpandToInclude( m_pt2.Project(Vector));
	Interval.ExpandToInclude( m_pt3.Project(Vector));
}

/**--------------------------------------------------------------------------<BR>
C2DTriangle::SnapToGrid <BR>
\brief See CGrid.
<P>---------------------------------------------------------------------------*/
void C2DTriangle::SnapToGrid(void)
{
	m_pt1.SnapToGrid();
	m_pt2.SnapToGrid();
	m_pt3.SnapToGrid();
}