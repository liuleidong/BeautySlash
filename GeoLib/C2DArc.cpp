/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DArc.cpp
\brief Implementation file for the C2DArc Class.

Implementation file for C2DArc, a class which represents a curved line.
<P>---------------------------------------------------------------------------*/


#include "StdAfx.h"
#include "C2DArc.h"
#include "C2DCircle.h"
#include "C2DLineBaseSet.h"
#include "Interval.h"
#include "C2DRect.h"
#include "C2DPointSet.h"


_MEMORY_POOL_IMPLEMENATION(C2DArc)


/**--------------------------------------------------------------------------<BR>
C2DArc::C2DArc
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DArc::C2DArc(void) : C2DLineBase(ArcedLine)
{
}


/**--------------------------------------------------------------------------<BR>
C2DArc::C2DArc
\brief Copy constructor.
<P>---------------------------------------------------------------------------*/
C2DArc::C2DArc(const C2DArc& Other) : C2DLineBase(ArcedLine)
{
	m_Line = Other.GetLine();
	m_dRadius = Other.GetRadius();
	m_bCentreOnRight = Other.GetCentreOnRight();
	m_bArcOnRight = Other.GetArcOnRight();
}


/**--------------------------------------------------------------------------<BR>
C2DArc::~C2DArc
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DArc::~C2DArc(void)
{
}

/**--------------------------------------------------------------------------<BR>
C2DSegment::C2DSegment
\brief Constructor
<P>---------------------------------------------------------------------------*/
C2DArc::C2DArc(const C2DPoint& PtFrom, const C2DPoint& PtTo, double dRadius, 
					   bool bCentreOnRight, bool bArcOnRight)
: m_Line(PtFrom, PtTo),   C2DLineBase(ArcedLine)
{
	m_dRadius = dRadius;
	m_bCentreOnRight = bCentreOnRight;
	m_bArcOnRight = bArcOnRight;

}


/**--------------------------------------------------------------------------<BR>
C2DSegment::C2DSegment
\brief Constructor
<P>---------------------------------------------------------------------------*/
C2DArc::C2DArc(const C2DPoint& PtFrom, const C2DVector& Vector, double dRadius, 
					   bool bCentreOnRight, bool bArcOnRight)
: m_Line(PtFrom, Vector) , C2DLineBase(ArcedLine)
{
	m_dRadius = dRadius;
	m_bCentreOnRight = bCentreOnRight;
	m_bArcOnRight = bArcOnRight;
}

/**--------------------------------------------------------------------------<BR>
C2DSegment::C2DSegment
\brief Constructor
<P>---------------------------------------------------------------------------*/
C2DArc::C2DArc(const C2DLine& Line, double dRadius, bool bCentreOnRight
					   ,bool bArcOnRight )  : C2DLineBase(ArcedLine)
{
	m_Line = Line;
	m_dRadius = dRadius;
	m_bCentreOnRight = bCentreOnRight;
	m_bArcOnRight = bArcOnRight;
}

/**--------------------------------------------------------------------------<BR>
C2DArc::Set
\brief Assignment.
<P>---------------------------------------------------------------------------*/
void C2DArc::Set(const C2DPoint& PtFrom, const C2DPoint& PtTo, double dRadius, 
	bool bCentreOnRight, bool bArcOnRight)
{
	m_Line.Set(PtFrom, PtTo);
	m_dRadius = dRadius;
	m_bCentreOnRight = bCentreOnRight;
	m_bArcOnRight = bArcOnRight;

}

/**--------------------------------------------------------------------------<BR>
C2DArc::Set
\brief Assignment.
<P>---------------------------------------------------------------------------*/
void C2DArc::Set(const C2DPoint& PtFrom, const C2DVector& Vector, double dRadius, 
	bool bCentreOnRight, bool bArcOnRight)
{
	m_Line.Set(PtFrom, Vector);
	m_dRadius = dRadius;
	m_bCentreOnRight = bCentreOnRight;
	m_bArcOnRight = bArcOnRight;
}

/**--------------------------------------------------------------------------<BR>
C2DArc::Set
\brief Assignment.
<P>---------------------------------------------------------------------------*/
void C2DArc::Set(const C2DLine& Line, double dRadius, 
	bool bCentreOnRight, bool bArcOnRight)
{
	m_Line = Line;
	m_dRadius = dRadius;
	m_bCentreOnRight = bCentreOnRight;
	m_bArcOnRight = bArcOnRight;

}


/**--------------------------------------------------------------------------<BR>
C2DArc::Set
\brief Assignment. Sets the curve to be to and from the points on the line but with the
point given on the curve.
<P>---------------------------------------------------------------------------*/
void C2DArc::Set(const C2DLine& Line, const C2DPoint ptOnArc)
{
	m_Line = Line;
	C2DPoint ptTo = m_Line.GetPointTo();

	C2DCircle Circle;
	Circle.SetCircumscribed( m_Line.point , ptTo,  ptOnArc) ;
	m_dRadius = m_Line.point.Distance( Circle.GetCentre() );
	m_bArcOnRight = m_Line.IsOnRight(ptOnArc);
	m_bCentreOnRight = m_Line.IsOnRight(Circle.GetCentre());

}


/**--------------------------------------------------------------------------<BR>
C2DArc::C2DArc
\brief Grow by a factor around the origin.
<P>---------------------------------------------------------------------------*/
void C2DArc::Grow(double dFactor, const C2DPoint& Origin)
{
	m_Line.Grow(dFactor,  Origin);
	m_dRadius *= dFactor;
	
}

/**--------------------------------------------------------------------------<BR>
C2DArc::C2DArc
\brief Reflection through a point.
<P>---------------------------------------------------------------------------*/
void C2DArc::Reflect(const C2DPoint& point)
{
	m_Line.Reflect( point);
}

/**--------------------------------------------------------------------------<BR>
C2DArc::C2DArc
\brief Reflection in a line.
<P>---------------------------------------------------------------------------*/
void C2DArc::Reflect(const C2DLine& Line)
{
	m_Line.Reflect( Line);
	m_bArcOnRight = !m_bArcOnRight;
	m_bCentreOnRight = !m_bCentreOnRight;
}

/**--------------------------------------------------------------------------<BR>
C2DSegment::IsValid
\brief Invalid if the circle radius is too small to join the 2 points on the line.
<P>---------------------------------------------------------------------------*/
bool C2DArc::IsValid(void) const
{
	return (m_Line.vector.GetLength() <= (2 * m_dRadius));
}


bool C2DArc::MakeValid(void)
{
	double dLength = m_Line.vector.GetLength();

	if (dLength > (2 * m_dRadius))
	{
		m_dRadius = dLength / (2.0 - conEqualityTolerance);
		return true;
	}

	return false;
}


/**--------------------------------------------------------------------------<BR>
C2DSegment::GetCircleCentre
\brief Finds the centre of the corresponding circle.
<P>---------------------------------------------------------------------------*/
C2DPoint C2DArc::GetCircleCentre(void) const
{
	if (!IsValid() ) 
		return C2DPoint(0, 0);

	C2DPoint MidPoint = m_Line.GetMidPoint();
	double dMinToStart = MidPoint.Distance( m_Line.point);

	double dMidToCentre = sqrt( m_dRadius * m_dRadius - dMinToStart * dMinToStart);

	C2DVector MidToCentre = m_Line.vector;
	if (m_bCentreOnRight) MidToCentre.TurnRight();
	else MidToCentre.TurnLeft();

	MidToCentre.SetLength(dMidToCentre);

	return (MidPoint + MidToCentre);
}

/**--------------------------------------------------------------------------<BR>
C2DSegment::GetArcLength
\brief Gets the length of the arc.
<P>---------------------------------------------------------------------------*/
double C2DArc::GetLength(void) const
{
	if (m_bCentreOnRight ^ m_bArcOnRight)
	{
		return( m_dRadius * GetSegmentAngle());
	}
	else
	{
		return( m_dRadius * (conTWOPI - GetSegmentAngle()) );
	}
}


/**--------------------------------------------------------------------------<BR>
C2DSegment::GetSegmentAngle
\brief Gets the angle of the segment. Always +ve and less than PI. In radians.
<P>---------------------------------------------------------------------------*/
double C2DArc::GetSegmentAngle(void) const
{
	if (!IsValid()) return 0;

	return ( 2 * asin( (m_Line.vector.GetLength() / 2) / m_dRadius));
}


/**--------------------------------------------------------------------------<BR>
C2DArc::C2DArc
\brief Returns the bounding rectangle.
<P>---------------------------------------------------------------------------*/
void C2DArc::GetBoundingRect(C2DRect& Rect) const
{
	if (!IsValid()) return;

	C2DPoint CentrePoint = GetCircleCentre();
	C2DPoint EndPoint = m_Line.GetPointTo();

	// First set up the rect that bounds the 2 points then check for if the arc expands this.
	m_Line.GetBoundingRect(Rect);
	
	// If the arc crosses the y axis..
	if (  ( (m_Line.point.x - CentrePoint.x) * (EndPoint.x - CentrePoint.x) ) < 0 )
	{
		// if the +ve y axis..
		if ( m_Line.GetMidPoint().y > CentrePoint.y)
		{
			if (m_bCentreOnRight ^ m_bArcOnRight) 
			{
				Rect.SetTop(CentrePoint.y + m_dRadius);

			}
			else
			{
				// If the segment is the "Big" bit....
				Rect.SetBottom(CentrePoint.y - m_dRadius);	
			}
		}
		else // if the -ve y axis...
		{
			if (m_bCentreOnRight ^ m_bArcOnRight)
			{
				Rect.SetBottom(CentrePoint.y - m_dRadius);
			}
			else
			{
				// If the segment is th "Big" bit then...
				Rect.SetTop(CentrePoint.y + m_dRadius);
			}
		}
	}
	else if (!(m_bCentreOnRight ^ m_bArcOnRight))
	{
		Rect.SetBottom(CentrePoint.y - m_dRadius);	
		Rect.SetTop(CentrePoint.y + m_dRadius);
	}

	// If the arc crosses the x axis..
	if (  ( (m_Line.point.y - CentrePoint.y) * (EndPoint.y - CentrePoint.y) ) < 0 )
	{
		// if the +ve x axis..
		if ( m_Line.GetMidPoint().x > CentrePoint.x)
		{
			if (m_bCentreOnRight ^ m_bArcOnRight)
			{
				Rect.SetRight(CentrePoint.x + m_dRadius);
			}
			else
			{
				// If the segment is th "Big" bit then...
				Rect.SetLeft (CentrePoint.x - m_dRadius);
			}
		}
		else // if the -ve x axis...
		{
			if (m_bCentreOnRight ^ m_bArcOnRight)
			{
				Rect.SetLeft(CentrePoint.x - m_dRadius);
			}
			else
			{
				// If the segment is th "Big" bit then...
				Rect.SetRight(CentrePoint.x + m_dRadius);
			}
		}
	}
	else if (!(m_bCentreOnRight ^ m_bArcOnRight))
	{
		Rect.SetLeft(CentrePoint.x - m_dRadius);
		Rect.SetRight(CentrePoint.x + m_dRadius);
	}
}



/**--------------------------------------------------------------------------<BR>
C2DArc::C2DArc
\brief True if this crosses the line.
<P>---------------------------------------------------------------------------*/
bool C2DArc::Crosses(const C2DLine& Line, C2DPointSet* IntersectionPts) const
{
	C2DPointSet IntPtsTemp;
	C2DCircle TestCircle(GetCircleCentre(), m_dRadius);
	if (TestCircle.Crosses(Line, &IntPtsTemp))
	{
		for (int i = IntPtsTemp.size() - 1 ; i >= 0  ; i--)
		{
			if (m_Line.IsOnRight(IntPtsTemp[i]) ^ m_bArcOnRight ||
				IntPtsTemp[i] == m_Line.point ||
				IntPtsTemp[i] == m_Line.GetPointTo())
			{
				IntPtsTemp.DeleteAt(i);
			}

		}

		if (IntPtsTemp.size() == 0)
			return false;
		else
		{
			if (IntersectionPts != 0)
				(*IntersectionPts) << IntPtsTemp;
			return true;
		}
	}
	else
	{
		return false;
	}
}


/**--------------------------------------------------------------------------<BR>
C2DArc::C2DArc
\brief True if this crosses the other arc.
<P>---------------------------------------------------------------------------*/
bool C2DArc::Crosses(const C2DArc& Other, C2DPointSet* IntersectionPts) const
{
	C2DCircle TestCircleThis(GetCircleCentre(), m_dRadius);
	C2DCircle TestCircleOther(Other.GetCircleCentre(), Other.GetRadius());

	C2DPointSet IntPtsTemp;

	if (TestCircleThis.Crosses(TestCircleOther, &IntPtsTemp))
	{

		for (int i = IntPtsTemp.size() - 1; i >= 0 ; i--)
		{
			if ((m_Line.IsOnRight(IntPtsTemp[i]) ^ m_bArcOnRight) ||
				Other.GetLine().IsOnRight(IntPtsTemp[i]) ^ Other.GetArcOnRight() ||
				IntPtsTemp[i] == m_Line.point ||
				IntPtsTemp[i] == m_Line.GetPointTo() ||
				IntPtsTemp[i] == Other.GetPointFrom() ||
				IntPtsTemp[i] == Other.GetPointTo())
			{
				IntPtsTemp.DeleteAt(i);
			}
		}

		if (IntPtsTemp.size() == 0)
			return false;
		else
		{
			if (IntersectionPts != 0)
				(*IntersectionPts) << IntPtsTemp;
			return true;
		}
	}
	else
	{
		return false;
	}

}

/**--------------------------------------------------------------------------<BR>
C2DArc::C2DArc
\brief True if this crosses the Ray (infininte line) defined by the line.
<P>---------------------------------------------------------------------------*/
bool C2DArc::CrossesRay(const C2DLine& Ray, C2DPointSet* IntersectionPts) const
{
	double dDist = Ray.point.Distance(GetCircleCentre());
	C2DLine RayCopy = Ray;
	// Ensure the copy line will go through the circle if the ray would.
	RayCopy.vector.SetLength((dDist + m_dRadius) * 2);

	return Crosses(RayCopy, IntersectionPts);
}


/**--------------------------------------------------------------------------<BR>
C2DArc::C2DArc
\brief Distance between this and the test point.
<P>---------------------------------------------------------------------------*/
double C2DArc::Distance(const C2DPoint& TestPoint) const
{
	return Distance(TestPoint, 0);
}


/**--------------------------------------------------------------------------<BR>
C2DArc::Distance
\brief Distance between this and the test point.
<P>---------------------------------------------------------------------------*/
double C2DArc::Distance(const C2DPoint& TestPoint, C2DPoint* ptOnThis) const
{
	C2DPoint ptCen = GetCircleCentre();
	C2DCircle Circle(ptCen, m_dRadius);
	C2DPoint ptOnCircle;
	double dCircleDist = Circle.Distance(TestPoint, &ptOnCircle);

	if (m_bArcOnRight ^ m_Line.IsOnRight(ptOnCircle))
	{
		// The closest point on the circle isn't on the curve
		double d1 = TestPoint.Distance(m_Line.point);
		double d2 = TestPoint.Distance(m_Line.GetPointTo());
		
		if (d1 < d2)
		{
			if(ptOnThis != 0)
				*ptOnThis = m_Line.point;
			return d1;
		}
		else
		{
			if(ptOnThis != 0)
				*ptOnThis = m_Line.GetPointTo();
			return d2;

		}
	}
	else
	{
		// The closest point on the circle IS on the curve
		if(ptOnThis != 0)
			*ptOnThis = ptOnCircle;
		return fabs(dCircleDist);
	}

}



/**--------------------------------------------------------------------------<BR>
C2DArc::Distance
\brief Distance between this and another curve.
<P>---------------------------------------------------------------------------*/
double C2DArc::Distance(const C2DArc& Other, 
			C2DPoint* ptOnThis, C2DPoint* ptOnOther) const
{
	C2DPointSet IntPts1;
	C2DPointSet IntPts2;

	C2DPoint ptThisCen = this->GetCircleCentre();
	C2DPoint ptOtherCen = Other.GetCircleCentre();

	C2DCircle CircleThis( ptThisCen, m_dRadius);
	C2DCircle CircleOther( ptOtherCen, Other.GetRadius() );

	if (CircleThis.Crosses(  CircleOther , &IntPts1 ) )
	{
		for (unsigned int i = 0; i < IntPts1.size(); i++)
		{
			if (  (m_Line.IsOnRight( IntPts1[i] ) == m_bArcOnRight ) &&
				  (Other.GetLine().IsOnRight( IntPts1[i] ) == Other.GetArcOnRight() )     )
			{
				if (ptOnThis)
					*ptOnThis = IntPts1[i];
				if (ptOnOther)
					*ptOnOther = IntPts1[i];
				return 0;
			}
		}

		IntPts1.DeleteAll();
	}


	C2DLine LineCenToOther;
	LineCenToOther.point = ptThisCen;
	LineCenToOther.vector = C2DVector(ptThisCen, ptOtherCen);
	LineCenToOther.GrowFromCentre( max(m_dRadius, Other.GetRadius()) * 10);

	double dMinDist = 1.7E308;
	double dDist;

	if (this->Crosses(LineCenToOther, &IntPts1) && Other.Crosses(LineCenToOther, &IntPts2))
	{
		for (unsigned int i = 0 ; i < IntPts1.size(); i++)
		{
			for (unsigned int j = 0 ; j < IntPts2.size(); j++)
			{
				dDist = IntPts1[i].Distance(IntPts2[j]);
				if (dDist < dMinDist)
				{
					if (ptOnThis != 0)
						*ptOnThis = IntPts1[i];
					if (ptOnOther != 0)
						*ptOnOther = IntPts2[j];

					dMinDist = dDist;
				}
			}
		}
	}

	C2DPoint ptOnThisTemp;
	dDist = Distance(Other.GetPointFrom(), &ptOnThisTemp);
	if (dDist < dMinDist)
	{
		if (ptOnThis != 0)
			*ptOnThis = ptOnThisTemp;
		if (ptOnOther != 0)
			*ptOnOther = Other.GetPointFrom();

		dMinDist = dDist;
	}

	dDist = Distance(Other.GetPointTo(), &ptOnThisTemp);
	if (dDist < dMinDist)
	{
		if (ptOnThis != 0)
			*ptOnThis = ptOnThisTemp;
		if (ptOnOther != 0)
			*ptOnOther = Other.GetPointTo();

		dMinDist = dDist;
	}

	C2DPoint ptOnOtherTemp;
	dDist = Other.Distance(GetPointFrom(), &ptOnOtherTemp);
	if (dDist < dMinDist)
	{
		if (ptOnThis != 0)
			*ptOnThis = GetPointFrom();
		if (ptOnOther != 0)
			*ptOnOther = ptOnOtherTemp;
		dMinDist = dDist;
	}

	dDist = Other.Distance(GetPointTo(), &ptOnOtherTemp);
	if (dDist < dMinDist)
	{
		if (ptOnThis != 0)
			*ptOnThis = GetPointTo();
		if (ptOnOther != 0)
			*ptOnOther = ptOnOtherTemp;
		dMinDist = dDist;
	}

	return dMinDist;
}

/**--------------------------------------------------------------------------<BR>
C2DArc::Distance
\brief Distance between this and a straight line.
<P>---------------------------------------------------------------------------*/
double C2DArc::Distance(const C2DLine& Line, 
			C2DPoint* ptOnThis , C2DPoint* ptOnOther ) const
{
	C2DCircle Circle( GetCircleCentre(), m_dRadius);

	C2DPoint ptThis;
	C2DPoint ptOther;

	double dCircDist = Circle.Distance( Line, &ptThis, &ptOther);
	double dDist = 0;

	if (m_Line.IsOnRight( ptThis ) ^ this->m_bArcOnRight)
	{
		// The point found isn't on this. 
		// This means the 2 closest points cannot be ON both lines, we must have a end point as one.

		ptThis = m_Line.point;
		dDist = Line.Distance(ptThis, &ptOther);

		C2DPoint ptThisTemp = m_Line.GetPointTo();
		C2DPoint ptOtherTemp;
		double d2 = Line.Distance(ptThisTemp, &ptOtherTemp);
		if (d2 < dDist)
		{
			dDist = d2;
			ptThis = ptThisTemp;
			ptOther = ptOtherTemp;
		}
		// If the line was outside the circle then stop here as no need to go any further.
		// This is because the closest point on this must be one of the end points.
		if (dCircDist < 0)
		{
			double d3 = Distance( Line.point, &ptThisTemp);
			if (d3 < dDist)
			{
				dDist = d3;
				ptThis = ptThisTemp;
				ptOther = Line.point;
			}	
			double d4 = Distance( Line.GetPointTo(), &ptThisTemp);
			if (d4 < dDist)
			{
				dDist = d4;
				ptThis = ptThisTemp;
				ptOther = Line.GetPointTo();
			}	
		}
	}
	else
	{
		dDist = fabs(dCircDist);
	}

	if (ptOnThis)
		*ptOnThis = ptThis;
	if (ptOnOther)
		*ptOnOther = ptOther;

	return dDist;

}

/**--------------------------------------------------------------------------<BR>
C2DArc::Project
\brief Projection of this onto a line.
<P>---------------------------------------------------------------------------*/
void C2DArc::Project(const C2DLine& Line, CInterval& Interval) const
{
	C2DArc ThisCopy(*this);
	C2DLine LineCopy(Line);

	double dAng = LineCopy.vector.AngleFromNorth();

	LineCopy.vector.TurnLeft(dAng);
	ThisCopy.RotateToRight( -dAng, LineCopy.point);

	C2DRect rect;
	ThisCopy.GetBoundingRect(rect);

	Interval.dMax = rect.GetTop() - LineCopy.point.y;
	Interval.dMin = Interval.dMax;

	Interval.ExpandToInclude( rect.GetBottom() - LineCopy.point.y );
}

/**--------------------------------------------------------------------------<BR>
C2DArc::Project
\brief Projection of this onto a vector.
<P>---------------------------------------------------------------------------*/
void C2DArc::Project(const C2DVector& Vector, CInterval& Interval) const
{
	C2DArc ThisCopy(*this);
	C2DVector VecCopy(Vector);

	double dAng = VecCopy.AngleFromNorth();

	VecCopy.TurnLeft(dAng);
	ThisCopy.RotateToRight( -dAng, C2DPoint(0, 0));

	C2DRect rect;
	ThisCopy.GetBoundingRect(rect);

	Interval.dMax = rect.GetTop() - VecCopy.j;
	Interval.dMin = Interval.dMax;

	Interval.ExpandToInclude( rect.GetBottom() - VecCopy.j );

}


/**--------------------------------------------------------------------------<BR>
C2DArc::GetMidPoint
\brief Gets the point half way along the curve.
<P>---------------------------------------------------------------------------*/
C2DPoint C2DArc::GetMidPoint(void) const
{
	assert(IsValid());
	// Make a line from the circle centre to the middle of the line
	C2DPoint ptCentre = GetCircleCentre();

	C2DPoint ptLineCentre = m_Line.GetMidPoint();

	C2DLine CenToMid(ptCentre, ptLineCentre);

	if ( CenToMid.vector.i == 0 && CenToMid.vector.j == 0)
	{
		// The centre of the line is the same as the centre of the circle
		// i.e. this arc is 180 degrees or half a circle.
		CenToMid = m_Line;
		CenToMid.SetPointFrom( ptLineCentre );
		if ( m_bArcOnRight )
			CenToMid.vector.TurnRight();
		else
			CenToMid.vector.TurnLeft();
	}
	else
	{
		// extend it to the edge of the arc
		CenToMid.SetLength( m_dRadius );
		// if the arc on the opposite side to the centre then reverse the line.
		if (this->m_bArcOnRight == this->m_bCentreOnRight)
		{
			CenToMid.vector.Reverse();
		}
	}

	return CenToMid.GetPointTo();
}

/**--------------------------------------------------------------------------<BR>
C2DArc::GetPointOn
\brief Gets the point on the curve determined by the factor.
<P>---------------------------------------------------------------------------*/
C2DPoint C2DArc::GetPointOn(double dFactorFromStart) const
{
	assert(IsValid());
	// make 2 lines from the centre to the ends of the line
	C2DPoint ptCentre = GetCircleCentre();

	C2DLine CenToStart(ptCentre, m_Line.point);

	C2DLine CenToEnd(ptCentre, m_Line.GetPointTo());

	if ( !this->m_bArcOnRight)	// clockwise
	{
		// Find the angle from one to the other and muliply it by the factor
		// before turning the line's vector by the result.
		double dAngleToRight = CenToStart.vector.AngleToRight( CenToEnd.vector );
		double dNewAngle = dAngleToRight* dFactorFromStart;
		CenToStart.vector.TurnRight( dNewAngle );
		return CenToStart.GetPointTo();
	}
	else	// anticlockwise
	{
		double dAngleToLeft = CenToStart.vector.AngleToLeft( CenToEnd.vector );
		double dNewAngle = dAngleToLeft* dFactorFromStart;
		CenToStart.vector.TurnLeft( dNewAngle );
		return CenToStart.GetPointTo();
	}

}




/**--------------------------------------------------------------------------<BR>
C2DArc::ReverseDirection
\brief Reverses the direction.
<P>---------------------------------------------------------------------------*/
void C2DArc::ReverseDirection(void)
{
	m_Line.ReverseDirection();

	m_bCentreOnRight = !m_bCentreOnRight;
	m_bArcOnRight = !m_bArcOnRight;

}


/**--------------------------------------------------------------------------<BR>
C2DArc::Crosses
\brief True if this crosses the other line. Calls appropriate function.
<P>---------------------------------------------------------------------------*/
bool C2DArc::Crosses(const C2DLineBase& Other, 
		C2DPointSet* IntersectionPts) const
{
	switch (Other.GetType())
	{
	case StraightLine:
		return this->Crosses( dynamic_cast<const C2DLine&>(Other), IntersectionPts);
		break;
	case ArcedLine:
		return this->Crosses( dynamic_cast<const C2DArc&>(Other), IntersectionPts);
		break;
	default:
		return false;
		break;
	}
}

/**--------------------------------------------------------------------------<BR>
C2DArc::Distance
\brief Distance to another line. Calls appropriate function.
<P>---------------------------------------------------------------------------*/
double C2DArc::Distance(const C2DLineBase& Other, C2DPoint* ptOnThis, 
		C2DPoint* ptOnOther) const
{
	switch (Other.GetType())
	{
	case StraightLine:
		return this->Distance( dynamic_cast<const C2DLine&>(Other), ptOnThis, ptOnOther);	
		break;
	case ArcedLine:
		{
		return this->Distance( dynamic_cast<const C2DArc&>(Other), ptOnThis, ptOnOther);
		break;
		}
	default:
		return 0;
		break;
	}

}

/**--------------------------------------------------------------------------<BR>
C2DArc::GetSubLines
\brief Splits the line up into sublines dependant upon the points provided which are
points on this line. Adds the sub lines, in order, to the collection.
<P>---------------------------------------------------------------------------*/
void C2DArc::GetSubLines(const C2DPointSet& PtsOnLine, C2DLineBaseSet& LineSet) const
{
	// if there are no points on the line to split on then add a copy of this and return.
	unsigned int usPointsCount = PtsOnLine.size();
	if (usPointsCount == 0 )
	{
		LineSet.AddCopy(*this);
		return;
	}
	else
	{
		// Make a copy of the points for sorting.
		C2DPointSet TempPts;
		TempPts.AddCopy(PtsOnLine);

		if (usPointsCount > 1) // They need sorting
		{
			// Make a line from the mid point of my line to the start
			C2DLine CenToStart( m_Line.GetMidPoint(), m_Line.point );
			// Now sort the points according to the order in which they will be encountered
			if (m_bArcOnRight)
				TempPts.SortByAngleToLeft( CenToStart );
			else
				TempPts.SortByAngleToRight( CenToStart );
		}

		C2DPoint ptCentre = this->GetCircleCentre();

		// Add the line from the start of this to the first.
		C2DLine Line( m_Line.point, TempPts[0] );
		LineSet.Add(new C2DArc( Line,  m_dRadius, 
								  Line.IsOnRight(ptCentre) , m_bArcOnRight)  );

		// Add all the sub lines.
		for (unsigned int i = 1; i < usPointsCount; i++)
		{
			Line.Set( TempPts[i - 1], TempPts[i]);
			LineSet.Add(new C2DArc( Line,  m_dRadius, 
								    Line.IsOnRight(ptCentre), m_bArcOnRight)    );
		}
		// Add the line from the last point on this to the end of this.
		Line.Set( *TempPts.GetLast(), m_Line.GetPointTo() );
		LineSet.Add(new C2DArc( Line,  m_dRadius, 
								   Line.IsOnRight(ptCentre), m_bArcOnRight)  );
	}
}

/**--------------------------------------------------------------------------<BR>
C2DArc::SnapToGrid
\brief Snaps to the grid defined using CGrid.
<P>---------------------------------------------------------------------------*/
void C2DArc::SnapToGrid(void)
{
	m_Line.SnapToGrid();

	double dLength = m_Line.vector.GetLength();
	
	if (dLength > ( 2 * m_dRadius ) )
	{
		m_dRadius = dLength / 1.999999999999;	// To ensure errors in the right way.
	}
}




/**--------------------------------------------------------------------------<BR>
C2DLine::Transform
\brief Function to transform the line by the user defined transformation.
<P>---------------------------------------------------------------------------*/
void C2DArc::Transform(CTransformation* pProject)
{
	m_Line.Transform(pProject);
}


/**--------------------------------------------------------------------------<BR>
C2DLine::Transform
\brief Function to transform the line by the user defined transformation.
<P>---------------------------------------------------------------------------*/
void C2DArc::InverseTransform(CTransformation* pProject)
{
	m_Line.InverseTransform(pProject);
}