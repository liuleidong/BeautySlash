/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DLine.cpp
\brief Implementation file for the C2DLine Class.

Implementation file for C2DLine, a class which represents a line segment i.e. a 
finite line.
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "C2DLine.h"
#include "C2DTriangle.h"
#include "C2DArc.h"
#include "C2DLineBaseSet.h"
#include "Interval.h"
#include "C2DRect.h"
#include "C2DPointSet.h"
#include "Transformation.h"


_MEMORY_POOL_IMPLEMENATION(C2DLine)

/**--------------------------------------------------------------------------<BR>
C2DLine::C2DLine
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DLine::C2DLine(void) : C2DLineBase(StraightLine)
{
	point.x = 0;
	point.y = 0;
	vector.i = 0;
	vector.j = 0;
}


/**--------------------------------------------------------------------------<BR>
C2DLine::C2DLine
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DLine::C2DLine(const C2DLine& Other) : C2DLineBase(StraightLine)
{
	point.x = Other.point.x;
	point.y = Other.point.y;
	vector.i = Other.vector.i;
	vector.j = Other.vector.j;	

}


/**--------------------------------------------------------------------------<BR>
C2DLine::~C2DLine
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DLine::~C2DLine(void)
{
}


/**--------------------------------------------------------------------------<BR>
C2DLine::ReverseDirection
\brief Reverses the direction.
<P>---------------------------------------------------------------------------*/
void C2DLine::ReverseDirection(void)
{
	point = GetPointTo();
	vector.Reverse();
}


/**--------------------------------------------------------------------------<BR>
C2DLine::C2DLine
\brief Construction from a point and vector.
<P>---------------------------------------------------------------------------*/
C2DLine::C2DLine(const C2DPoint& PointFrom, const C2DVector& VectorTo) 
: C2DLineBase(StraightLine)
{
	point = PointFrom;
	vector = VectorTo;
}


/**--------------------------------------------------------------------------<BR>
C2DLine::C2DLine
\brief Construction from 2 points
<P>---------------------------------------------------------------------------*/
C2DLine::C2DLine(const C2DPoint& PointFrom, const C2DPoint& PointTo)  
: C2DLineBase(StraightLine)
{
	point = PointFrom;
	vector.Set(PointFrom, PointTo);
}


/**--------------------------------------------------------------------------<BR>
C2DLine::Set
\brief Assignement.
<P>---------------------------------------------------------------------------*/
void C2DLine::Set(const C2DPoint& PointFrom, const C2DPoint& PointTo)
{
	point = PointFrom;
	vector.Set(PointFrom, PointTo);
}


/**--------------------------------------------------------------------------<BR>
C2DLine::Set
\brief Assignement.
<P>---------------------------------------------------------------------------*/
void C2DLine::Set(const C2DPoint& PointFrom, const C2DVector& VectorTo)
{
	point = PointFrom;
	vector = VectorTo;
}


/**--------------------------------------------------------------------------<BR>
C2DLine::SetPointTo
\brief Assigns the second point.
<P>---------------------------------------------------------------------------*/
void C2DLine::SetPointTo(const C2DPoint& PointTo)
{
	vector = PointTo - point;
}

/**--------------------------------------------------------------------------<BR>
C2DLine::SetPointFrom
\brief Sets the point that this is going from changing the vector as well.
<P>---------------------------------------------------------------------------*/
void C2DLine::SetPointFrom(const C2DPoint& PointFrom)
{
	// Get point to.
	C2DPoint PointTo(point.x + vector.i, point.y + vector.j);	
	// Set the point from 
	point = PointFrom;
	// Rest the point to.
	vector = PointTo - point;
}


/**--------------------------------------------------------------------------<BR>
C2DLine::IsOnRight
\brief True if the point in question is to the right of this.
<P>---------------------------------------------------------------------------*/
bool C2DLine::IsOnRight(const C2DPoint& OtherPoint) const
{
	return ( C2DTriangle::GetAreaSigned( point, GetPointTo(), OtherPoint) < 0);
}


/**--------------------------------------------------------------------------<BR>
C2DLine::GetPointTo
\brief Returns the second point.
<P>---------------------------------------------------------------------------*/
C2DPoint C2DLine::GetPointTo(void) const
{
	C2DPoint PointTo(point.x + vector.i, point.y + vector.j);
	return PointTo;
}

/**--------------------------------------------------------------------------<BR>
C2DLine::WouldCross
\brief True if this line would cross the other if it were infinite.
<P>---------------------------------------------------------------------------*/
bool C2DLine::WouldCross(const C2DLine& Other) const
{
	bool bPointOnRight = IsOnRight(Other.point);
	bool bPointToOnRight = IsOnRight(Other.GetPointTo());

	return (bPointOnRight^bPointToOnRight);

}

/**--------------------------------------------------------------------------<BR>
C2DLine::GrowFromCentre
\brief Grows from the centre by the amount.
<P>---------------------------------------------------------------------------*/
void C2DLine::GrowFromCentre(double dFactor)
{
	point.x -= ( vector.i * dFactor - vector.i ) / 2;
	point.y -= ( vector.j * dFactor - vector.j ) / 2;

	this->vector = this->vector * dFactor;

}


/**--------------------------------------------------------------------------<BR>
C2DLine::CrossesRay
\brief True if the ray provided (infinite line starting from the first point) 
crosses this. 
<P>---------------------------------------------------------------------------*/
bool C2DLine::CrossesRay(const C2DLine& Ray, C2DPointSet* IntersectionPts) const
{
	C2DPoint p1 = point;
	C2DPoint p2 = GetPointTo();

	C2DPoint p3 = Ray.point;
	C2DPoint p4 = Ray.GetPointTo();

	double Ua = (p4.x - p3.x)*(p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x);
	double Ub = (p2.x - p1.x)*(p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x);

	double dDenominator = (p4.y - p3.y)*(p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y);

	if (dDenominator == 0) return false;

	Ua = Ua / dDenominator;
	Ub = Ub / dDenominator;

	bool bResult = (Ua >= 0 && Ua <= 1) && (Ub >= 0);

	if (IntersectionPts !=0 && bResult)
	{
		IntersectionPts->AddCopy(C2DPoint(p1.x + Ua*(p2.x - p1.x) , p1.y + Ua*(p2.y - p1.y)));
	}

	return bResult;
}


/**--------------------------------------------------------------------------<BR>
C2DLine::GetMidPoint
\brief Gets the mid point on the line.
<P>---------------------------------------------------------------------------*/
C2DPoint C2DLine::GetMidPoint(void) const
{
	C2DPoint Result(point.x + vector.i / 2, point.y + vector.j / 2);
	return Result;
}


/**--------------------------------------------------------------------------<BR>
C2DLine::GetPointOn
\brief Gets the point on the line given by the factor. e.g. 0.5 = mid point.
<P>---------------------------------------------------------------------------*/
C2DPoint C2DLine::GetPointOn(double dFactorFromStart) const
{
	C2DVector vNew = vector * dFactorFromStart;

	C2DPoint Result(point.x + vNew.i, point.y + vNew.j );
	return Result;
}


/**--------------------------------------------------------------------------<BR>
C2DLine::Crosses
\brief  True if this line crosses the other. Returns the point is a collection is provided.
Returns whether it would cross on this or on the other. Can opt to get the point 
where the cross would occur (if not parallel) even if they don't cross.
<P>---------------------------------------------------------------------------*/
bool C2DLine::Crosses(const C2DLine& Other, C2DPointSet* IntersectionPts
					  , bool* pbOnThis, bool* pbOnOther,  bool bAddPtIfFalse) const
{
	bool bOnThis = false;
	bool bOnOther = false;

	C2DPoint p1 = point;
	C2DPoint p2 = GetPointTo();

	C2DPoint p3 = Other.point;
	C2DPoint p4 = Other.GetPointTo();

	double Ua = (p4.x - p3.x)*(p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x);
	double Ub = (p2.x - p1.x)*(p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x);

	double dDenominator = (p4.y - p3.y)*(p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y);

	if (dDenominator == 0) return false;

	Ua = Ua / dDenominator;
	Ub = Ub / dDenominator;

	bOnThis = (Ua >= 0 && Ua < 1);		// For ints we need the line to be the point set [a,b);
	bOnOther = (Ub >= 0 && Ub < 1);		// For ints we need the line to be the point set [a,b);
	bool bResult  = bOnThis && bOnOther;

	if (pbOnThis != 0) *pbOnThis = bOnThis;
	if (pbOnOther != 0) *pbOnOther = bOnOther;

	if (IntersectionPts != 0 && (bAddPtIfFalse || bResult))
	{
		IntersectionPts->AddCopy(C2DPoint(p1.x + Ua*(p2.x - p1.x) , p1.y + Ua*(p2.y - p1.y)));
	}

	return (bResult);
}

/**--------------------------------------------------------------------------<BR>
C2DLine::GetBoundingRect
\brief  Returns the bounding rectangle.
<P>---------------------------------------------------------------------------*/
void C2DLine::GetBoundingRect(C2DRect& Rect) const
{
	Rect.Set(point);
	Rect.ExpandToInclude(GetPointTo());
}


/**--------------------------------------------------------------------------<BR>
C2DLine::Distance
\brief  Distance to a point.
<P>---------------------------------------------------------------------------*/
double C2DLine::Distance(const C2DPoint& TestPoint) const
{
	return Distance(TestPoint, 0);
}


/**--------------------------------------------------------------------------<BR>
C2DLine::Distance
\brief Returns the distance from this to the point.
<P>---------------------------------------------------------------------------*/
double C2DLine::Distance(const C2DPoint& TestPoint,  C2DPoint* ptOnThis) const
{
	C2DVector vP1ToPoint(point, TestPoint);
	double dLength = GetLength();
	double dProjLength = vP1ToPoint.Dot(vector);

	if (dProjLength < 0)
	{
		if (ptOnThis != 0)
			*ptOnThis = point;

		return TestPoint.Distance(point);
	}
	else
	{
		dProjLength = dProjLength / dLength;
		if (dProjLength < dLength)
		{
			// The projection is on the line
			double dFactorOnLine = dProjLength / dLength;
			C2DPoint PtOnLine(point.x + vector.i * dFactorOnLine, 
							  point.y + vector.j * dFactorOnLine);
			if (ptOnThis != 0)
				*ptOnThis = PtOnLine;
			return TestPoint.Distance(PtOnLine);
		}
		else
		{
			if (ptOnThis != 0)
				*ptOnThis = GetPointTo();

			return TestPoint.Distance( GetPointTo() );
		}
	}
}


/**--------------------------------------------------------------------------<BR>
C2DLine::Distance
\brief Returns the distance from this to the other line.
<P>---------------------------------------------------------------------------*/
double C2DLine::Distance(const C2DLine& Other, 
			C2DPoint* ptOnThis,  C2DPoint* ptOnOther) const
{

	// First, project the other line onto this and if it falls entirely below it or
	// above it then 1. There is no intersection, 2. This is closest to one end on this line.
	C2DPoint ptOtherP2 = Other.GetPointTo();
	C2DVector vThisP1OtherP1(point, Other.point);
	C2DVector vThisP1OtherP2(point, ptOtherP2);
	C2DPoint ptThisP2 = GetPointTo();

	double dOtherP1Proj = vThisP1OtherP1.Dot(vector);
	double dOtherP2Proj = vThisP1OtherP2.Dot(vector);
	// If they are both less than 0 then the projection falls below the line.
	if (dOtherP1Proj <= 0 && dOtherP2Proj <= 0)
	{
		if (ptOnThis != 0)
			*ptOnThis = point;
		return Other.Distance(point, ptOnOther);
	}
	// Now modify the projection so it is the length along this line.
	double dThisLength = GetLength();
	dOtherP1Proj = dOtherP1Proj / dThisLength;
	dOtherP2Proj = dOtherP2Proj / dThisLength;
	// If the projections are both above the line then the second point is closest
	if (dOtherP1Proj >= dThisLength && dOtherP2Proj >= dThisLength)
	{
		if (ptOnThis != 0)
			*ptOnThis = ptThisP2;
		return Other.Distance( ptThisP2,  ptOnOther);
	}

	// This hasn't worked so try the same on the other line.
	C2DVector vOtherP1ThisP1(Other.point, point);
	C2DVector vOtherP1ThisP2(Other.point, ptThisP2);

	double dThisP1Proj = vOtherP1ThisP1.Dot(Other.vector);
	double dThisP2Proj = vOtherP1ThisP2.Dot(Other.vector);
	// If they are both less than 0 then the projection falls below the line.
	if (dThisP1Proj <= 0 && dThisP2Proj <= 0)
	{
		if (ptOnOther != 0)
			*ptOnOther = Other.point;
		return Distance(Other.point, ptOnThis);
	}
	// Now modify the projection so it is the length along this line.
	double dOtherLength = Other.GetLength();
	dThisP1Proj = dThisP1Proj / dOtherLength;
	dThisP2Proj = dThisP2Proj / dOtherLength;
	// If the projections are both above the line then the second point is closest
	if (dThisP1Proj >= dOtherLength && dThisP2Proj >= dOtherLength)
	{
		if (ptOnOther != 0)
			*ptOnOther = ptOtherP2;
		return Distance( ptOtherP2,  ptOnThis);
	}
	// Now test for an intersection.
	C2DPointSet IntPoint;
	if (this->Crosses(Other, &IntPoint))
	{
		if (ptOnOther != 0)
			*ptOnOther = IntPoint[0];
		if (ptOnThis != 0)
			*ptOnThis = IntPoint[0];
		return 0;
	}
	// Otherwise, there MUST be a point projection on one of the lines otherwise both
	// lines project on either side of each other which is impossible. 
	// So find the distances to all these projections and take the minimum.
	double dDist = 0;
	double dMinDist = 0;
	bool bSet = false;

	// Is the other lines first point projected on this?
	C2DPoint ptOnThisTemp;
	C2DPoint ptOnOtherTemp; 
	if (dOtherP1Proj >= 0 && dOtherP1Proj <= dThisLength)
	{
		// If so find the point on this line and get distance to it.
		double dFactor = dOtherP1Proj / dThisLength;
		ptOnThisTemp = C2DPoint(point.x + vector.i * dFactor,
								   point.y + vector.j * dFactor);

		dMinDist = Other.point.Distance( ptOnThisTemp );
		bSet = true;

		if (ptOnOther != 0)
			*ptOnOther = Other.point;
		if (ptOnThis != 0)
			*ptOnThis = ptOnThisTemp;

	}
	// Is the other lines second point projected onto this?
	if (dOtherP2Proj >= 0 && dOtherP2Proj <= dThisLength)
	{
		// If so find the point on this and then the distance. Is it less?
		double dFactor = dOtherP2Proj / dThisLength;
		ptOnThisTemp = C2DPoint(point.x + vector.i * dFactor,
											   point.y + vector.j * dFactor);

		dDist = ptOtherP2.Distance( ptOnThisTemp );
		if (!bSet || dDist < dMinDist)
		{
			if (ptOnOther != 0)
				*ptOnOther = ptOtherP2;
			if (ptOnThis != 0)
				*ptOnThis = ptOnThisTemp;
			dMinDist = dDist;

			bSet = true;
		}
	}
	// Is the first point on this projected onto the other line?
	if (dThisP1Proj >= 0 && dThisP1Proj <= dOtherLength)
	{
		// If so find the point and the distance. Is it less?
		double dFactor = dThisP1Proj / dOtherLength;
		ptOnOtherTemp = C2DPoint(Other.point.x + Other.vector.i * dFactor,
								 Other.point.y + Other.vector.j * dFactor);

		dDist = point.Distance( ptOnOtherTemp );
		if (!bSet || dDist < dMinDist)
		{
			if (ptOnOther != 0)
				*ptOnOther = ptOnOtherTemp;
			if (ptOnThis != 0)
				*ptOnThis = point;
			dMinDist = dDist;

			bSet = true;
		}

	}

	// Is the second point on this projected onto the other line?
	if (dThisP2Proj >= 0 && dThisP2Proj <= dOtherLength)
	{
		// If so find the point and the distance. Is it less?
		double dFactor = dThisP2Proj / dOtherLength;

		ptOnOtherTemp = C2DPoint(Other.point.x + Other.vector.i * dFactor,
											 Other.point.y + Other.vector.j * dFactor);

		dDist = ptThisP2.Distance(  ptOnOtherTemp );
		if (!bSet || dDist < dMinDist)
		{
			if (ptOnOther != 0)
				*ptOnOther = ptOnOtherTemp;
			if (ptOnThis != 0)
				*ptOnThis = ptThisP2;

			dMinDist = dDist;

			bSet = true;
		}
	}

	assert(	bSet );
	// Now return the minimum distance
	return dMinDist;

}

/**--------------------------------------------------------------------------<BR>
C2DLine::RotateToRight
\brief Rotation.
<P>---------------------------------------------------------------------------*/
void C2DLine::RotateToRight(double dAng, const C2DPoint& Origin)
{
	point.RotateToRight(dAng, Origin);
	vector.TurnRight(dAng);
}

/**--------------------------------------------------------------------------<BR>
C2DLine::Grow
\brief Grows the line in size.
<P>---------------------------------------------------------------------------*/
void C2DLine::Grow(double dFactor, const C2DPoint& Origin)
{
	C2DPoint pointTo = GetPointTo();
	point.Grow(dFactor, Origin);
	pointTo.Grow(dFactor, Origin);
	SetPointTo(pointTo);
}

/**--------------------------------------------------------------------------<BR>
C2DLine::Reflect
\brief Point reflection.
<P>---------------------------------------------------------------------------*/
void C2DLine::Reflect(const C2DPoint& Point)
{
	C2DPoint pointTo = GetPointTo();
	point.Reflect(Point);
	pointTo.Reflect(Point);
	SetPointTo(pointTo);

}


/**--------------------------------------------------------------------------<BR>
C2DLine::Reflect
\brief Reflects the in the line given.
<P>---------------------------------------------------------------------------*/
void C2DLine::Reflect(const C2DLine& Line)
{
	C2DPoint pointTo = GetPointTo();
	point.Reflect(Line);
	pointTo.Reflect(Line);
	SetPointTo(pointTo);
}


/**--------------------------------------------------------------------------<BR>
C2DLine::Crosses
\brief Crosses function for a line base. Just calls the appropriate function.
<P>---------------------------------------------------------------------------*/
bool C2DLine::Crosses(const C2DLineBase& Other, C2DPointSet* IntersectionPts)const
{
	switch (Other.GetType())
	{
	case StraightLine:
		return this->Crosses( dynamic_cast<const C2DLine&>(Other), IntersectionPts);
		break;
	case ArcedLine:
		{
		const C2DArc& Arc = dynamic_cast<const C2DArc&>(Other);
		return Arc.Crosses( *this, IntersectionPts);
		break;
		}
	default:
		return false;
		break;
	}
}


/**--------------------------------------------------------------------------<BR>
C2DLine::Distance
\brief Distance function for a line base. Just calls the appropriate function.
<P>---------------------------------------------------------------------------*/
double C2DLine::Distance(const C2DLineBase& Other, C2DPoint* ptOnThis, 
		C2DPoint* ptOnOther) const
{
	switch (Other.GetType())
	{
	case StraightLine:
		return this->Distance( dynamic_cast<const C2DLine&>(Other), ptOnThis, ptOnOther);
		break;
	case ArcedLine:
		{
		const C2DArc& Arc = dynamic_cast<const C2DArc&>(Other);
		return Arc.Distance( *this, ptOnOther, ptOnThis);
		break;
		}
	default:
		return 0;
		break;
	}

}

/**--------------------------------------------------------------------------<BR>
C2DLine::Project
\brief Projects the line onto the other line.
<P>---------------------------------------------------------------------------*/
void C2DLine::Project(const C2DLine& Line, CInterval& Interval) const
{
	double dP1 = point.Project(Line);
	Interval.dMax = dP1;
	Interval.dMin = dP1;
	Interval.ExpandToInclude( GetPointTo().Project( Line) );

}

/**--------------------------------------------------------------------------<BR>
C2DLine::Project
\brief Projects the line onto the vector.
<P>---------------------------------------------------------------------------*/
void C2DLine::Project(const C2DVector& Vector, CInterval& Interval) const
{
	double dP1 = point.Project(Vector);
	Interval.dMax = dP1;
	Interval.dMin = dP1;
	Interval.ExpandToInclude( GetPointTo().Project( Vector) );

}


/**--------------------------------------------------------------------------<BR>
C2DLine::GetSubLines
\brief Splits the line up into sublines dependant upon the points provided which are
points on this line. Adds the sub lines, in order, to the collection.
<P>---------------------------------------------------------------------------*/
void C2DLine::GetSubLines(const C2DPointSet& PtsOnLine, C2DLineBaseSet& LineSet) const 
{
	// if there are no points on the line to split on then add a copy of this and return.
	unsigned int usPointsCount = PtsOnLine.size();
	if (usPointsCount == 0 )
	{
		LineSet.AddCopy(*this);
	}
	else
	{
		C2DPointSet TempPts;
		TempPts.AddCopy(PtsOnLine);

		if (usPointsCount > 1) // They need sorting
		{
			// Now sort the points according to the order in which they will be encountered
			TempPts.SortByDistance(point);
		}

		// Add the line from the start of this to the first.
		LineSet.Add(new C2DLine(point, TempPts[0]  ));

		// Add all the sub lines.
		for (unsigned int i = 1; i < usPointsCount; i++)
			LineSet.Add(new C2DLine(TempPts[i - 1], TempPts[i]));

		// Add the line from the last point on this to the end of this.
		LineSet.Add(new C2DLine( *TempPts.GetLast() , GetPointTo() ));
	}

	assert (LineSet.size() == (PtsOnLine.size() + 1) );
}


/**--------------------------------------------------------------------------<BR>
C2DLine::SnapToGrid
\brief Snaps to the Grid - see CGrid
<P>---------------------------------------------------------------------------*/
void C2DLine::SnapToGrid(void)
{
	C2DPoint pt = GetPointTo();

	pt.SnapToGrid();

	point.SnapToGrid();
	SetPointTo(pt);
}

/**--------------------------------------------------------------------------<BR>
C2DLine::GetY
\brief Given x, this returns y assuming the line is infinite.
<P>---------------------------------------------------------------------------*/
double C2DLine::GetY(double dx) const
{
	if ( vector.i == 0)
	{
		assert(false);
		return 0;
	}
	double m = vector.j / vector.i;
	return m * dx + point.y - m * point.x;
}


/**--------------------------------------------------------------------------<BR>
C2DLine::OverlapsVertically
\brief True if part of this line is above or below the other. Returns the point 
on this and on the other.
<P>---------------------------------------------------------------------------*/
bool C2DLine::OverlapsVertically(const C2DLine& Other, double& dVerticalDistance, 
									C2DPoint& ptOnThis, C2DPoint& ptOnOther) const
{
	// Get the 2 points for both lines
	C2DPoint OtherTo(Other.point.x + Other.vector.i, Other.point.y + Other.vector.j);
	C2DPoint ThisTo(point.x + vector.i, point.y + vector.j);
	// Make an interval for both in the x plane
	CInterval iThis( point.x, point.x);
	iThis.ExpandToInclude( ThisTo.x );

	CInterval iOther( Other.point.x, Other.point.x);
	iOther.ExpandToInclude( OtherTo.x );
	// This is an interval for the overlap between the 2
	CInterval iOverlap;
	// If there is an overlap...
	if (iThis.Overlaps(iOther, iOverlap))
	{
		double dThisYMin;
		double dThisYMax;

		double dOtherYMin;
		double dOtherYMax;
		// If the line is vertical then y at the x min / max can be set to the ends of the line.
		if (vector.i == 0)
		{
			dThisYMin = point.y;
			dThisYMax = ThisTo.y;
		}
		else	// otherwise, caluclate the y values at the interval ends
		{
			dThisYMin = GetY(iOverlap.dMin);
			dThisYMax = GetY(iOverlap.dMax);
		}
		// Now do the same for the other line
		if (Other.vector.i == 0)
		{
			dOtherYMin = Other.point.y;
			dOtherYMax = OtherTo.y;
		}
		else
		{
			dOtherYMin = Other.GetY(iOverlap.dMin);
			dOtherYMax = Other.GetY(iOverlap.dMax);
		}
		// Now find the distance between the 2 at the ends
		double dDistMin = dOtherYMin - dThisYMin;
		double dDistMax = dOtherYMax - dThisYMax;
		// If they are both same sign then there is no intersection
		if ( (dDistMin * dDistMax) > 0)
		{
			dDistMin = fabs( dDistMin);
			dDistMax = fabs( dDistMax);
			// find which one is smallest
			if ( dDistMin > dDistMax)
			{
				dVerticalDistance = dDistMax;// distance at the max is smallest
				ptOnThis.x = iOverlap.dMax;
				ptOnThis.y = dThisYMax;
				ptOnOther.x = iOverlap.dMax;
				ptOnOther.y = dOtherYMax;
			}
			else
			{
				dVerticalDistance = dDistMin;// distance at the min is smallest
				ptOnThis.x = iOverlap.dMin;
				ptOnThis.y = dThisYMin;
				ptOnOther.x = iOverlap.dMin;
				ptOnOther.y = dOtherYMin;
			}
			return true;
		}
		else
		{
			// find the intersection.
			dVerticalDistance = 0;
			C2DPointSet pts;
			if(this->Crosses(Other, &pts))
			{
				ptOnThis = pts[0];
				ptOnOther = ptOnThis;
			}
			else
			{
				assert(false);
			}
		}
		
		return true;
	}
	else
	{
		return false;
	}

}

/**--------------------------------------------------------------------------<BR>
C2DLine::OverlapsAbove
\brief True if part of this line is above the other. Returns the point 
on this and on the other.
<P>---------------------------------------------------------------------------*/
bool C2DLine::OverlapsAbove(const C2DLine& Other, double& dVerticalDistance, 
									C2DPoint& ptOnThis, C2DPoint& ptOnOther) const
{
	// Get the 2 points for both lines
	C2DPoint OtherTo(Other.point.x + Other.vector.i, Other.point.y + Other.vector.j);
	C2DPoint ThisTo(point.x + vector.i, point.y + vector.j);
	// Make an interval for both in the x plane
	CInterval iThis( point.x, point.x);
	iThis.ExpandToInclude( ThisTo.x );

	CInterval iOther( Other.point.x, Other.point.x);
	iOther.ExpandToInclude( OtherTo.x );
	// This is an interval for the overlap between the 2
	CInterval iOverlap;
	// If there is an overlap...
	if (iThis.Overlaps(iOther, iOverlap))
	{
		double dThisYMin;
		double dThisYMax;

		double dOtherYMin;
		double dOtherYMax;
		// If the line is vertical then y at the x min / max can be set to the ends of the line.
		if (vector.i == 0)
		{
			dThisYMin = point.y;
			dThisYMax = ThisTo.y;
		}
		else	// otherwise, caluclate the y values at the interval ends
		{
			dThisYMin = GetY(iOverlap.dMin);
			dThisYMax = GetY(iOverlap.dMax);
		}
		// Now do the same for the other line
		if (Other.vector.i == 0)
		{
			dOtherYMin = Other.point.y;
			dOtherYMax = OtherTo.y;
		}
		else
		{
			dOtherYMin = Other.GetY(iOverlap.dMin);
			dOtherYMax = Other.GetY(iOverlap.dMax);
		}
		
		// Now find the distance between the 2 at the ends
		double dDistMin = dThisYMin - dOtherYMin;
		double dDistMax = dThisYMax - dOtherYMax;
		// If they are both > 0 then no intersection
		if ( (dDistMin > 0) && (dDistMax > 0))
		{
			dDistMin = fabs( dDistMin);
			dDistMax = fabs( dDistMax);
			// find which one is smallest
			if ( dDistMin > dDistMax)
			{	
				dVerticalDistance = dDistMax;	// distance at the max is smallest
				ptOnThis.x = iOverlap.dMax;
				ptOnThis.y = dThisYMax;
				ptOnOther.x = iOverlap.dMax;
				ptOnOther.y = dOtherYMax;
			}
			else
			{
				dVerticalDistance = dDistMin;  // distance at the min is smallest
				ptOnThis.x = iOverlap.dMin;
				ptOnThis.y = dThisYMin;
				ptOnOther.x = iOverlap.dMin;
				ptOnOther.y = dOtherYMin;
			}

			return true;
		}	
		else if ( (dDistMin < 0) && (dDistMax < 0)) // This is below.
		{
			return false;
		}
		else
		{
			// find the intersection.
			dVerticalDistance = 0;
			C2DPointSet pts;
			if(this->Crosses(Other, &pts))
			{
				ptOnThis = pts[0];
				ptOnOther = ptOnThis;
			}
			else
			{
				assert(false);
			}
		}
	
		return true;
	}
	else
	{
		return false;
	}

}


/**--------------------------------------------------------------------------<BR>
C2DLine::DistanceAsRay
\brief 	Returns the distance from this to the point with this as a ray.
<P>---------------------------------------------------------------------------*/
double C2DLine::DistanceAsRay(const C2DPoint& TestPoint) const
{
	return DistanceAsRay(TestPoint, 0);
}


/**--------------------------------------------------------------------------<BR>
C2DLine::DistanceAsRay
\brief Returns the distance from this to the point with this as a ray.
<P>---------------------------------------------------------------------------*/
double C2DLine::DistanceAsRay(const C2DPoint& TestPoint,  C2DPoint* ptOnThis) const
{
	C2DVector vP1ToPoint(point, TestPoint);

	// The projection is on the line
	double dFactorOnLine = vP1ToPoint.Dot(vector) / (vector.i * vector.i + vector.j * vector.j);
	C2DPoint PtOnLine(point.x + vector.i * dFactorOnLine, 
					  point.y + vector.j * dFactorOnLine);
	if (ptOnThis != 0)
		*ptOnThis = PtOnLine;
	return TestPoint.Distance(PtOnLine);

}


/**--------------------------------------------------------------------------<BR>
C2DLine::Join
\brief Function to join the 2 lines at the point where they do / would intersect. If they do then
the lines are clipped to remove the smallest part of the line. Returns false if they
cannot be joined.
<P>---------------------------------------------------------------------------*/
bool C2DLine::Join(C2DLine& Other)
{
    C2DPoint p1 = point;
    C2DPoint p2 = GetPointTo();

    C2DPoint p3 = Other.point;
    C2DPoint p4 = Other.GetPointTo();

    double Ua = (p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x);
    double Ub = (p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x);

    double dDenominator = (p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y);

    if (dDenominator == 0)
        return false;

    Ua = Ua / dDenominator;
    Ub = Ub / dDenominator;

    C2DPoint IntPt(p1.x + Ua * (p2.x - p1.x), p1.y + Ua * (p2.y - p1.y));
    if ( Ua >=0.5)
        SetPointTo( IntPt );
    else
        SetPointFrom( IntPt );

    if ( Ub >=0.5)
        Other.SetPointTo( IntPt );
    else
        Other.SetPointFrom( IntPt );

    return true;
}


/**--------------------------------------------------------------------------<BR>
C2DLine::Transform
\brief Function to transform the line by the user defined transformation.
<P>---------------------------------------------------------------------------*/
void C2DLine::Transform(CTransformation* pProject)
{
	C2DPoint pt2(point.x + vector.i, point.y + vector.j);

	pProject->Transform(point.x, point.y);
	pProject->Transform(pt2.x, pt2.y);

	vector.i = pt2.x - point.x;
	vector.j = pt2.y - point.y;
}

/**--------------------------------------------------------------------------<BR>
C2DLine::InverseTransform
\brief Function to transform the line by the user defined transformation.
<P>---------------------------------------------------------------------------*/
void C2DLine::InverseTransform(CTransformation* pProject)
{
	C2DPoint pt2(point.x + vector.i, point.y + vector.j);

	pProject->InverseTransform(point.x, point.y);
	pProject->InverseTransform(pt2.x, pt2.y);

	vector.i = pt2.x - point.x;
	vector.j = pt2.y - point.y;
}