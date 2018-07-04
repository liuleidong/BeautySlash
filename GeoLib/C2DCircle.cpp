/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DCircle.cpp
\brief Implementation file for the C2DCircle Class.

Implementation file for C2DCircle, a class representing a circle.
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "C2DCircle.h"
#include "Grid.h"
#include "C2DPointSet.h"
#include "C2DVector.h"
#include "C2DTriangle.h"
#include "C2DLine.h"
#include "Interval.h"
#include "C2DRect.h"


_MEMORY_POOL_IMPLEMENATION(C2DCircle)



/**--------------------------------------------------------------------------<BR>
C2DCircle::C2DCircle
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DCircle::C2DCircle(void): C2DBase(Circle)
{
}

/**--------------------------------------------------------------------------<BR>
C2DCircle::C2DCircle
\brief Copy constructor.
<P>---------------------------------------------------------------------------*/
C2DCircle::C2DCircle(const C2DCircle& Other): C2DBase(Circle)
{
	m_Centre = Other.GetCentre();
	m_dRadius = Other.GetRadius();
}

/**--------------------------------------------------------------------------<BR>
C2DCircle::C2DCircle
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DCircle::~C2DCircle(void)
{
}

/**--------------------------------------------------------------------------<BR>
C2DCircle::C2DCircle
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DCircle::C2DCircle(const C2DPoint& Centre, double dRadius): C2DBase(Circle)
{
	m_Centre = Centre;
	m_dRadius = dRadius;
}

/**--------------------------------------------------------------------------<BR>
C2DCircle::Set
\brief Assignment.
<P>---------------------------------------------------------------------------*/
void C2DCircle::Set(const C2DPoint& Centre, double dRadius)
{
	m_Centre = Centre;
	m_dRadius = dRadius;
}

/**--------------------------------------------------------------------------<BR>
C2DCircle::SetMinimum
\brief Sets the circle to be the minimum bounding circle for the 2 points.
<P>---------------------------------------------------------------------------*/
void C2DCircle::SetMinimum(const C2DPoint& Point1, const C2DPoint& Point2)
{
	C2DVector Vec(Point1, Point2);
	Vec *= 0.5;
	m_dRadius = Vec.GetLength();
	m_Centre = Point1 + Vec;

}


/**--------------------------------------------------------------------------<BR>
C2DCircle::SetMinimum
\brief Sets the circle to be the minimum bounding circle for the 3 points.
<P>---------------------------------------------------------------------------*/
void C2DCircle::SetMinimum(const C2DPoint& Point1, 
	const C2DPoint& Point2, const C2DPoint& Point3)
{
	double dDist12 = Point1.Distance(Point2);
	double dDist23 = Point2.Distance(Point3);
	double dDist31 = Point3.Distance(Point1);
	if (dDist12 >= dDist23 && dDist12 >= dDist31)
	{
		SetMinimum(Point1, Point2);
		if (this->Contains(Point3))
			return;
	}
	else if (dDist23 >= dDist31)
	{
		SetMinimum(Point2, Point3);
		if (this->Contains(Point1))
			return;
	}
	else
	{
		SetMinimum(Point3, Point1);
		if (this->Contains(Point2))
			return;
	}

	// If here, the simple minimum of any 2 doesn't incorporate the other 1 so the
	// minimum is the circumscribed circle.
	SetCircumscribed(Point1, Point2, Point3);
}


/**--------------------------------------------------------------------------<BR>
C2DCircle::SetCircumscribed
\brief Sets the circle to have all 3 points on the edge.
<P>---------------------------------------------------------------------------*/
bool C2DCircle::SetCircumscribed (const C2DPoint& Point1, 
	const C2DPoint& Point2, const C2DPoint& Point3)
{

	if ( C2DTriangle::Collinear(Point1, Point2, Point3))
		return false;

	m_Centre = C2DTriangle::GetCircumCentre(Point1, Point2, Point3);
	m_dRadius = m_Centre.Distance(Point1);

	return true;
}


/**--------------------------------------------------------------------------<BR>
C2DCircle::SetCircumscribed
\brief Sets the circle to have all 3 points from the triangle on the edge.
<P>---------------------------------------------------------------------------*/
bool C2DCircle::SetCircumscribed (const C2DTriangle& Triangle)
{

	if ( Triangle.Collinear())
		return false;

	m_Centre = Triangle.GetCircumCentre();
	m_dRadius = m_Centre.Distance(Triangle.GetPoint1());

	return true;
}

/**--------------------------------------------------------------------------<BR>
C2DCircle::GetRadius
\brief Gets the radius.
<P>---------------------------------------------------------------------------*/
double C2DCircle::GetRadius(void) const 
{
	return m_dRadius;
}

/**--------------------------------------------------------------------------<BR>
C2DCircle::GetArea
\brief Returns the area.
<P>---------------------------------------------------------------------------*/
double C2DCircle::GetArea(void) const 
{
	return conPI* m_dRadius *m_dRadius;
}

/**--------------------------------------------------------------------------<BR>
C2DCircle::GetPerimeter
\brief Gets the perimeter.
<P>---------------------------------------------------------------------------*/
double C2DCircle::GetPerimeter(void)const 
{
	return conTWOPI*m_dRadius;
}


/**--------------------------------------------------------------------------<BR>
C2DCircle::Crosses
\brief Calculates the intersection of this with the other circle.
<P>---------------------------------------------------------------------------*/
bool C2DCircle::Crosses(const C2DCircle& Other, C2DPointSet* IntersectionPts) const
{

	double x1 = this->m_Centre.x;
	double y1 = this->m_Centre.y;
	double R1 = this->m_dRadius;

	double x2 = Other.GetCentre().x;
	double y2 = Other.GetCentre().y;
	double R2 = Other.GetRadius();

	double D = Other.GetCentre().Distance(m_Centre);

	if (D == 0)
		return false;

	if (D == (R1 + R2) )
	{
		if (IntersectionPts != 0)
		{
			C2DVector V(m_Centre, Other.GetCentre());
			V.SetLength(R1);
			C2DPoint P = m_Centre + V;
			IntersectionPts->AddCopy(P);
		}
		return true;
	}

	if ( D > (R1 + R2) || D < fabs(R1 - R2))
		return false;
	
	if (IntersectionPts != 0)
	{
		double A = (D + R1 + R2) * (D + R1 - R2) * (D - R1 + R2) * (-D + R1 + R2);
		A = sqrt(A) / 4;

		double XE1 = (x1 + x2) / 2 - (x1 - x2) * (R1 * R1 - R2 * R2) / (2 * D * D);
		double XE2 = 2 * (y1 - y2) * A / (D * D);

		double YE1 = (y1 + y2) / 2 - (y1 - y2) * (R1 * R1 - R2 * R2) / (2 * D * D);
		double YE2 = 2 * (x1 - x2) * A / (D * D);

		C2DPoint pt1( XE1 + XE2, YE1 - YE2);
		C2DPoint pt2( XE1 - XE2, YE1 + YE2);

		IntersectionPts->AddCopy(pt1);
		IntersectionPts->AddCopy(pt2);
	}

	return true;
}


/**--------------------------------------------------------------------------<BR>
C2DCircle::Move
\brief Moves this by the vector.
<P>---------------------------------------------------------------------------*/
void C2DCircle::Move(const C2DVector& Vector)
{
	m_Centre.Move(Vector);
}


/**--------------------------------------------------------------------------<BR>
C2DCircle::C2DCircle.
\brief Assignment.
<P>---------------------------------------------------------------------------*/
const C2DCircle& C2DCircle::operator=(const C2DCircle& Other)
{
	m_Centre = Other.GetCentre();
	m_dRadius = Other.GetRadius();
	return *this;
}


/**--------------------------------------------------------------------------<BR>
C2DCircle::C2DCircle.
\brief GetBoundingRect.
<P>---------------------------------------------------------------------------*/
void C2DCircle::GetBoundingRect(C2DRect& Rect) const 
{
	Rect.Set( m_Centre.x - m_dRadius, m_Centre.y + m_dRadius,
			  m_Centre.x + m_dRadius, m_Centre.y - m_dRadius);
}

/**--------------------------------------------------------------------------<BR>
C2DCircle::Crosses.
\brief Line intersection. 
<P>---------------------------------------------------------------------------*/
bool C2DCircle::Crosses(const C2DLine& Line, 
	C2DPointSet* IntersectionPts) const
{
	double x1 = Line.point.x;
	double x2 = Line.point.x + Line.vector.i;
	double x3 = m_Centre.x;

	double y1 = Line.point.y;
	double y2 = Line.point.y + Line.vector.j;
	double y3 = m_Centre.y;

	double r = m_dRadius;

	double a = (x2 - x1)* (x2 - x1) + (y2 - y1)* (y2 - y1);

	double b = 2 * (    (x2 - x1)* (x1 - x3) + (y2 - y1) *(y1 - y3)   );

	double c = x3* x3 + y3 * y3 + x1 * x1 + y1* y1 - 2 * (x3 * x1 + y3 * y1 ) - r * r;

	double u = - b/ (2 * a);


	C2DPoint ptClosestToCen;

	if (u < 0)
	{
		ptClosestToCen = Line.point;
	}
	else if ( u > 1)
	{
		ptClosestToCen = Line.GetPointTo();
	}
	else
	{
		ptClosestToCen = Line.point + (Line.vector * u);
	}

	double dDist = ptClosestToCen.Distance(m_Centre);

	if (dDist > m_dRadius)
	{
		return false;
	}
	else if (IntersectionPts == 0)
	{
		return true;
	}
	else
	{
		// Calculate the points.
		double d1 = b* b - 4* a* c;
		assert (d1 >= 0);

		if (d1 < 0)
			return false;
		else if (d1 == 0)
		{
			double p1 = - b / (2* a);
			IntersectionPts->AddCopy(Line.GetPointOn(p1));
			return true;

		}
		else
		{
			d1 = sqrt(d1);
			double p1 = (- b + d1)/ (2* a);
			double p2 = (- b - d1)/ (2* a);

			bool bResult = false;
			if (p2 >= 0 && p2 <= 1)
			{
				bResult = true;
				IntersectionPts->AddCopy(Line.GetPointOn( p2));
			}

			if (p1 >= 0 && p1 <= 1)
			{
				bResult = true;
				IntersectionPts->AddCopy(Line.GetPointOn( p1));
			}

			return bResult;
		}
	}
}


/**--------------------------------------------------------------------------<BR>
C2DCircle::CrossesRay.
\brief True if it crosses the ray represented by the line.
<P>---------------------------------------------------------------------------*/
bool C2DCircle::CrossesRay(const C2DLine& Ray, C2DPointSet* IntersectionPts) const
{
	double dDist = Ray.point.Distance(m_Centre);
	C2DLine RayCopy = Ray;
	// Ensure the copy line will go through the circle if the ray would.
	RayCopy.vector.SetLength((dDist + m_dRadius) * 2);

	return Crosses(RayCopy, IntersectionPts);
}


/**--------------------------------------------------------------------------<BR>
C2DCircle::Contains.
\brief Point containment.
<P>---------------------------------------------------------------------------*/
bool C2DCircle::Contains(const C2DPoint& pt) const 
{
	return m_Centre.Distance(pt) < m_dRadius;
}


/**--------------------------------------------------------------------------<BR>
C2DCircle::IsWithinDistance.
\brief Proximity.
<P>---------------------------------------------------------------------------*/
bool C2DCircle::IsWithinDistance(const C2DPoint& pt, double dRange) const
{
	return m_Centre.Distance(pt) < m_dRadius + dRange;
}


/**--------------------------------------------------------------------------<BR>
C2DCircle::RotateToRight.
\brief RotateToRight.
<P>---------------------------------------------------------------------------*/
void C2DCircle::RotateToRight(double dAng, const C2DPoint& Origin)
{
	m_Centre.RotateToRight(dAng, Origin);

}


/**--------------------------------------------------------------------------<BR>
C2DCircle::Grow.
\brief Grow.
<P>---------------------------------------------------------------------------*/
void C2DCircle::Grow(double dFactor, const C2DPoint& Origin)
{
	m_Centre.Grow(dFactor,  Origin);
	m_dRadius *= dFactor;
}


/**--------------------------------------------------------------------------<BR>
C2DCircle::Reflect.
\brief Reflect.
<P>---------------------------------------------------------------------------*/
void C2DCircle::Reflect(const C2DPoint& Point)
{
	m_Centre.Reflect(Point);
}

/**--------------------------------------------------------------------------<BR>
C2DCircle::Reflect.
\brief Reflects throught the line provided.
<P>---------------------------------------------------------------------------*/
void C2DCircle::Reflect(const C2DLine& Line)
{
	m_Centre.Reflect(Line);

}


/**--------------------------------------------------------------------------<BR>
C2DCircle::Distance.
\brief Distance to a point.
<P>---------------------------------------------------------------------------*/
double C2DCircle::Distance(const C2DPoint& TestPoint) const
{
	double dDist = m_Centre.Distance(TestPoint);
	return (dDist - m_dRadius);  // -ve indicates inside.
}


/**--------------------------------------------------------------------------<BR>
C2DCircle::Distance.
\brief Distance to a point provides the closest point on this.
<P>---------------------------------------------------------------------------*/
double C2DCircle::Distance(const C2DPoint& TestPoint, C2DPoint* ptOnThis) const
{
	double dDist = m_Centre.Distance(TestPoint);

	if (ptOnThis != 0)
	{
		if (dDist == 0)
		{
			// point is the centre so just arbitrary point to the circle
			*ptOnThis = m_Centre + C2DVector( m_dRadius, 0);
		}
		else
		{
			// find the point on the circle.
			C2DLine LineCenToPt(m_Centre, TestPoint);
			LineCenToPt.vector.SetLength(m_dRadius);
			*ptOnThis = LineCenToPt.GetPointTo();
		}
	}

	return (dDist - m_dRadius);  // -ve indicates inside.

}


/**--------------------------------------------------------------------------<BR>
C2DCircle::Distance.
\brief Distance to a line provides the closest point on both.
<P>---------------------------------------------------------------------------*/
double C2DCircle::Distance(const C2DLine& Line, 
			C2DPoint* ptOnThis, C2DPoint* ptOnOther) const
{
	CInterval ProjInt;
	Project(Line, ProjInt);

	if (ProjInt.dMax < 0)
	{
		// This means that the circle projects entirely "below" the line so the nearest point
		// To this is the first point on the line and there are no interections.
		if (ptOnOther !=0)
			*ptOnOther = Line.point;

		return Distance(Line.point, ptOnThis);
	}
	
	double dLength = Line.GetLength();

	if (ProjInt.dMin > dLength)
	{
		// This means that the circle projects entirely "above" the line so the nearest point
		// To this is the second point on the line and there are no interections.
		C2DPoint ptClosest = Line.GetPointTo();
		if (ptOnOther !=0)
			*ptOnOther = ptClosest;
		return Distance(ptClosest, ptOnThis);
	}

	// Now find out if there's an intersection.
	C2DPointSet IntPts;
	if ( Crosses(Line, &IntPts) )
	{
		if (ptOnThis != 0)
			*ptOnThis = IntPts[0];
		if (ptOnOther != 0)
			*ptOnOther = IntPts[0];

		return 0;
	}

	// Now find out if the line is entirely inside
	if (ProjInt.dMin > 0 && ProjInt.dMax < dLength && this->Contains( Line.point) )
	{
		double d1 = Distance(Line.point, ptOnThis);
		C2DPoint ptThisTemp;
		double d2 = Distance(Line.GetPointTo(),  &ptThisTemp);
		assert( d1 < 0 && d2 < 0);
		if (d2 > d1) // NOTE USE OF > AS d2 and d1 are -ve.
		{
			if (ptOnThis)
				*ptOnThis = ptThisTemp;
			if (ptOnOther)
				*ptOnOther = Line.GetPointTo();
			return d2;
		}
		else
		{
			if (ptOnOther)
				*ptOnOther = Line.point;
			return d1;
		}
	}

	// We now know the line is entirely outside.
	// Now find out if this is closest to a point on the line. 
	double dCenOnLine = ( ProjInt.dMax + ProjInt.dMin ) / 2.0;

	if ( dCenOnLine > 0 )
	{	
		if (dCenOnLine < dLength)
		{
			// The centre is projected on the line
			double dFactor = dCenOnLine / dLength;
			
			C2DVector vProj = Line.vector;
			vProj *= dFactor;
			C2DPoint ptOnLine = Line.point + vProj;


			if (ptOnOther != 0)
				*ptOnOther = ptOnLine;

			return Distance(ptOnLine, ptOnThis);
		}
		else
		{
			// The centre is projected above the line.
			C2DPoint ptClosest = Line.GetPointTo();
			if (ptOnOther !=0)
				*ptOnOther = ptClosest;
			return Distance(ptClosest, ptOnThis);	
		}
	}
	else
	{
		// This means that the circle projects entirely "below" the line.
		if (ptOnOther !=0)
			*ptOnOther = Line.point;
		return Distance(Line.point, ptOnThis);

	}
}


/**--------------------------------------------------------------------------<BR>
C2DCircle::Distance.
\brief Distance to a line provides the closest point on both.
<P>---------------------------------------------------------------------------*/
double C2DCircle::Distance(const C2DCircle& Other, 
			C2DPoint* ptOnThis , C2DPoint* ptOnOther ) const
{
	double dCenCenDist = m_Centre.Distance(Other.GetCentre());
	double dOtherRadius = Other.GetRadius();

	C2DPoint ptThis;
	C2DPoint ptOther;
	double dDist = dCenCenDist - m_dRadius - dOtherRadius;

	if (dDist > 0 )
	{
		// they do not interect and they are outside each other.
		if (ptOnThis != 0 || ptOnOther != 0)
		{
			C2DLine Line(m_Centre, Other.GetCentre());
			Line.vector.SetLength(m_dRadius);
			ptThis = Line.GetPointTo();

			Line.vector.Reverse();
			Line.SetPointFrom(Other.GetCentre());
			Line.vector.SetLength(Other.GetRadius() );
			ptOther = Line.GetPointTo();
		}
	}
	else
	{
		if ( (dCenCenDist + m_dRadius) < dOtherRadius)
		{
			// This is inside the other
			dDist =  dCenCenDist + m_dRadius  - dOtherRadius ; // -ve if inside
			if (ptOnThis != 0 || ptOnOther != 0)
			{
				C2DVector vec( Other.GetCentre(), m_Centre);
				vec *= (   m_dRadius   /dCenCenDist  ); // set the vector to be the length of my radius.
				ptThis = m_Centre + vec;
				vec *= (   dDist   /m_dRadius  ); // set the vector to be the distance.
				ptOther = ptThis + vec;
			}
		}
		else if ( (dCenCenDist + dOtherRadius) < m_dRadius)
		{
			// The other is inside this.
			dDist = dCenCenDist + dOtherRadius -  m_dRadius; // -ve if inside
			if (ptOnThis != 0 || ptOnOther != 0)
			{
				C2DVector vec( m_Centre, Other.GetCentre());
				vec *= (   dOtherRadius   /dCenCenDist  ); // set the vector to be the length of my radius.
				C2DPoint ptOther = Other.GetCentre() + vec;
				vec *= (   dDist   /  dOtherRadius  ); // set the vector to be the distance.
				C2DPoint ptThis = ptOther + vec;
			}
		}
		else
		{
			// there is an intersection
			dDist = 0;
			if (ptOnThis != 0 || ptOnOther != 0)
			{
				C2DPointSet Ints;
				if (Crosses(Other, &Ints))
				{
					ptThis = Ints[0];
					ptOther = ptThis;
				}
				else
				{
					assert(0);
					return 0;
				}
			}
		}
	}

	if (ptOnThis)
		*ptOnThis = ptThis;
	if (ptOnOther)
		*ptOnOther = ptOther;

	return dDist;
}


/**--------------------------------------------------------------------------<BR>
C2DCircle::Project.
\brief Projection onto a line.
<P>---------------------------------------------------------------------------*/
void C2DCircle::Project(const C2DLine& Line, CInterval& Interval) const
{
	// Create a line that goes through the circle from edge to edge and with the same vector as the
	// Line to project on.
	C2DLine LineCopy(m_Centre, Line.vector);
	
	LineCopy.vector.SetLength( m_dRadius * 2 );

	LineCopy.Move ( LineCopy.vector * -0.5 );

	// Now just project the line onto the interval.
	LineCopy.Project(Line, Interval);
}


/**--------------------------------------------------------------------------<BR>
C2DCircle::Project.
\brief Projection onto a vector.
<P>---------------------------------------------------------------------------*/
void C2DCircle::Project(const C2DVector& Vector, CInterval& Interval) const
{
	// Create a line that goes through the circle from edge to edge and with the same vector.
	C2DLine Line(m_Centre, Vector);
	
	Line.vector.SetLength( m_dRadius * 2 );

	Line.Move ( Vector * -0.5 );

	// Now just project the line onto the interval.
	Line.Project(Vector, Interval);

}


/**--------------------------------------------------------------------------<BR>
C2DCircle::SetInscribed.
\brief Sets the circle to be the maximum circle inside the triangle defined by the 
points given.
<P>---------------------------------------------------------------------------*/
void C2DCircle::SetInscribed(const C2DPoint& Point1, 
	const C2DPoint& Point2, const C2DPoint& Point3)
{
	SetInscribed(  C2DTriangle(Point1, Point2, Point3));
}


/**--------------------------------------------------------------------------<BR>
C2DCircle::SetInscribed.
\brief Sets the circle to be the maximum circle inside the triangle.
<P>---------------------------------------------------------------------------*/
void C2DCircle::SetInscribed(const C2DTriangle& Triangle)
{
	C2DPoint InCen = Triangle.GetInCentre();

	C2DLine Line( Triangle.GetPoint1(), Triangle.GetPoint2()  );
	
	C2DVector vec(Line.point, InCen );
	double dProj = vec.Dot(Line.vector);
	double dLength = Line.vector.GetLength();
	dProj /= dLength;

	double dFactor = dProj / dLength;
		
	C2DVector vProj = Line.vector;
	vProj *= dFactor;
	C2DPoint ptOnLine = Line.point + vProj;

	Set(InCen, InCen.Distance( ptOnLine));
	
}


/**--------------------------------------------------------------------------<BR>
C2DPoint::SnapToGrid
\brief Snaps this to the grid defined by the CGrid class.
<P>---------------------------------------------------------------------------*/
void C2DCircle::SnapToGrid(void)
{
	m_Centre.SnapToGrid();

	double dGrid = CGrid::GetGridSize();

	double dMultiple = fabs(m_dRadius / dGrid ) + 0.5;

	dMultiple = floor(dMultiple);

	m_dRadius < 0 ? m_dRadius = - dMultiple * dGrid : m_dRadius = dMultiple * dGrid;

}