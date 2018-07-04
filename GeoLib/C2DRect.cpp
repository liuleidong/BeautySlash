/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DRect.cpp
\brief Declaration file for the C2DRect Class

Declaration file for the C2DRect, a class for a rectangle with horizontal and 
vertical sides. C2DPoints represent the top left and bottom right.
<P>---------------------------------------------------------------------------*/


#include "StdAfx.h"
#include "C2DRect.h"
#include "C2DLine.h"
#include "Interval.h"

_MEMORY_POOL_IMPLEMENATION(C2DRect)

/**--------------------------------------------------------------------------<BR>
C2DRect::C2DRect <BR>
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DRect::C2DRect(void): C2DBase(Rect)
{
	Clear();
}

/**--------------------------------------------------------------------------<BR>
C2DRect::~C2DRect <BR>
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DRect::~C2DRect(void)
{
}


/**--------------------------------------------------------------------------<BR>
C2DRect::C2DRect <BR>
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DRect::C2DRect(const C2DPoint& ptTopLeft, const C2DPoint& ptBottomRight): C2DBase(Rect)
{
	m_TopLeft= ptTopLeft;
	m_BottomRight = ptBottomRight;
}

/**--------------------------------------------------------------------------<BR>
C2DRect::C2DRect <BR>
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DRect::C2DRect(double dLeft, double dTop, double dRight, double dBottom): C2DBase(Rect)
{
	m_TopLeft.x = dLeft;
	m_TopLeft.y = dTop;
	m_BottomRight.x = dRight;
	m_BottomRight.y = dBottom;
}


/**--------------------------------------------------------------------------<BR>
C2DRect::C2DRect <BR>
\brief Constructor sets both the top left and bottom right to equal the rect.
<P>---------------------------------------------------------------------------*/
C2DRect::C2DRect(const C2DPoint& pt ): C2DBase(Rect)
{
	m_TopLeft = pt;
	m_BottomRight = pt;
}


/**--------------------------------------------------------------------------<BR>
C2DRect::Set <BR>
\brief Sets both the top left and bottom right to equal the rect.
<P>---------------------------------------------------------------------------*/
void C2DRect::Set(const C2DPoint& pt)
{
	m_TopLeft = pt;
	m_BottomRight = pt;
}


/**--------------------------------------------------------------------------<BR>
C2DRect::Set <BR>
\brief Sets the rect.
<P>---------------------------------------------------------------------------*/
void C2DRect::Set(const C2DPoint& ptTopLeft, const C2DPoint& ptBottomRight)
{
	m_TopLeft= ptTopLeft;
	m_BottomRight = ptBottomRight;
}



/**--------------------------------------------------------------------------<BR>
C2DRect::Set <BR>
\brief Sets the rect.
<P>---------------------------------------------------------------------------*/
void C2DRect::Set(double dLeft, double dTop, double dRight, double dBottom)
{
	m_TopLeft.x = dLeft;
	m_TopLeft.y = dTop;
	m_BottomRight.x = dRight;
	m_BottomRight.y = dBottom;
}

/**--------------------------------------------------------------------------<BR>
C2DRect::Clear <BR>
\brief Clears the rect.
<P>---------------------------------------------------------------------------*/
void C2DRect::Clear(void)
{
	Set(0, 0, 0, 0);
}


/**--------------------------------------------------------------------------<BR>
C2DRect::Clear <BR>
\brief Moves the rect.
<P>---------------------------------------------------------------------------*/
void C2DRect::Move(const C2DVector& Vector)
{
	m_TopLeft.Move(Vector);
	m_BottomRight.Move(Vector);
}


/**--------------------------------------------------------------------------<BR>
C2DRect::Clear <BR>
\brief Expands the rect if the point is in it.
<P>---------------------------------------------------------------------------*/
void C2DRect::ExpandToInclude(const C2DPoint& NewPt)
{
	if (NewPt.x > m_BottomRight.x) m_BottomRight.x = NewPt.x;
	else if (NewPt.x <  m_TopLeft.x)  m_TopLeft.x = NewPt.x;
	if (NewPt.y >  m_TopLeft.y)  m_TopLeft.y = NewPt.y;
	else if (NewPt.y < m_BottomRight.y) m_BottomRight.y = NewPt.y;
}


/**--------------------------------------------------------------------------<BR>
C2DRect::Clear <BR>
\brief Returns the centre.
<P>---------------------------------------------------------------------------*/
C2DPoint C2DRect::GetCentre(void) const 
{
	return m_BottomRight.GetMidPoint(m_TopLeft);
}

/**--------------------------------------------------------------------------<BR>
C2DRect::Clear <BR>
\brief Grows it around the centre.
<P>---------------------------------------------------------------------------*/
void C2DRect::Grow(double dFactor)
{
	C2DPoint ptCentre = GetCentre();

	m_BottomRight.x = (m_BottomRight.x - ptCentre.x) * dFactor + ptCentre.x;
	m_BottomRight.y = (m_BottomRight.y - ptCentre.y) * dFactor + ptCentre.y;

	m_TopLeft.x = (m_TopLeft.x - ptCentre.x) * dFactor + ptCentre.x;
	m_TopLeft.y = (m_TopLeft.y - ptCentre.y) * dFactor + ptCentre.y;
}

/**--------------------------------------------------------------------------<BR>
C2DRect::GrowHeight <BR>
\brief Grows the height around the centre.
<P>---------------------------------------------------------------------------*/
void C2DRect::GrowHeight(double dFactor)
{
	C2DPoint ptCentre = GetCentre();
	m_BottomRight.y = (m_BottomRight.y - ptCentre.y) * dFactor + ptCentre.y;
	m_TopLeft.y = (m_TopLeft.y - ptCentre.y) * dFactor + ptCentre.y;
}


/**--------------------------------------------------------------------------<BR>
C2DRect::GrowWidth <BR>
\brief Grows the width around the centre.
<P>---------------------------------------------------------------------------*/
void C2DRect::GrowWidth(double dFactor)
{
	C2DPoint ptCentre = GetCentre();
	m_BottomRight.x = (m_BottomRight.x - ptCentre.x) * dFactor + ptCentre.x;
	m_TopLeft.x = (m_TopLeft.x - ptCentre.x) * dFactor + ptCentre.x;
}

/**--------------------------------------------------------------------------<BR>
C2DRect::Grow <BR>
\brief Grows it around the origin given.
<P>---------------------------------------------------------------------------*/
void C2DRect::Grow(double dFactor, const C2DPoint& Origin)
{
	m_BottomRight.Grow(dFactor, Origin);
	m_TopLeft.Grow(dFactor, Origin);
}


/**--------------------------------------------------------------------------<BR>
C2DRect::RotateToRight <BR>
\brief Rotates to the right. Note that as the horizontal/vertical line property will be
preserved. If you rotate an object and its bounding box, the box may not still
bound the object.
<P>---------------------------------------------------------------------------*/
void C2DRect::RotateToRight(double dAng, const C2DPoint& Origin)
{
	double dHalfWidth = Width() / 2;
	double dHalfHeight = Height() / 2;

	C2DPoint ptCen = GetCentre();
	ptCen.RotateToRight(dAng, Origin);

	m_TopLeft.x = ptCen.x - dHalfWidth;
	m_TopLeft.y = ptCen.y + dHalfHeight;
	m_BottomRight.x = ptCen.x + dHalfWidth;
	m_BottomRight.y = ptCen.y - dHalfHeight;
}

/**--------------------------------------------------------------------------<BR>
C2DRect::Reflect <BR>
\brief Reflects it throught th point.
<P>---------------------------------------------------------------------------*/
void C2DRect::Reflect(const C2DPoint& Point)
{
	m_TopLeft.Reflect(Point); 
	m_BottomRight.Reflect(Point);
	
	double x =  m_TopLeft.x;
	double y =  m_TopLeft.y;

	m_TopLeft = m_BottomRight;
	m_BottomRight.x = x;
	m_BottomRight.y = y;
}


/**--------------------------------------------------------------------------<BR>
C2DRect::Reflect <BR>
\brief Reflects it throught the line by simply reflecting the centre to maintain
the validity of the rect i.e. it has to be horizontal / vertical.
<P>---------------------------------------------------------------------------*/
void C2DRect::Reflect(const C2DLine& Line)
{
	C2DPoint ptCen = this->GetCentre();
	C2DPoint ptNewCen = ptCen;
	ptNewCen.Reflect(Line);
	C2DVector vec(ptCen, ptNewCen);
	Move(vec);
}


/**--------------------------------------------------------------------------<BR>
C2DRect::Expand <BR>
\brief Expands by a set amount.
<P>---------------------------------------------------------------------------*/
void C2DRect::Expand(double dRange)
{
	m_BottomRight.x += dRange;
	m_BottomRight.y -= dRange;

	m_TopLeft.x -= dRange;
	m_TopLeft.y += dRange;
}


/**--------------------------------------------------------------------------<BR>
C2DRect::ExpandToInclude <BR>
\brief Expands to include the other rectangle.
<P>---------------------------------------------------------------------------*/
void C2DRect::ExpandToInclude(const C2DRect& Other)
{
	ExpandToInclude(Other.GetTopLeft());
	ExpandToInclude(Other.GetBottomRight());
}


/**--------------------------------------------------------------------------<BR>
C2DRect::IsValid <BR>
\brief Returns whether the area is valid.
<P>---------------------------------------------------------------------------*/
bool C2DRect::IsValid(void) const 
{
	return ( (m_TopLeft.x < m_BottomRight.x ) && (m_TopLeft.y > m_BottomRight.y ) );
}


/**--------------------------------------------------------------------------<BR>
C2DRect::Contains <BR>
\brief True if the point is in the rectangle.
<P>---------------------------------------------------------------------------*/
bool C2DRect::Contains(const C2DPoint& Pt) const
{
	return ( Pt.x >=  m_TopLeft.x && Pt.x <= m_BottomRight.x &&
			 Pt.y <=  m_TopLeft.y && Pt.y >= m_BottomRight.y);
}


/**--------------------------------------------------------------------------<BR>
C2DRect::GetArea <BR>
\brief Returns the area.
<P>---------------------------------------------------------------------------*/
double C2DRect::GetArea(void) const 
{
	return ( (m_TopLeft.y - m_BottomRight.y) * (m_BottomRight.x -  m_TopLeft.x));
}

/**--------------------------------------------------------------------------<BR>
C2DRect::Width <BR>
\brief Returns the width.
<P>---------------------------------------------------------------------------*/
double C2DRect::Width(void) const 
{
	return (m_BottomRight.x -  m_TopLeft.x);
}

/**--------------------------------------------------------------------------<BR>
C2DRect::Height <BR>
\brief Returns the height.
<P>---------------------------------------------------------------------------*/
double C2DRect::Height(void) const 
{
	return (m_TopLeft.y - m_BottomRight.y);

}


/**--------------------------------------------------------------------------<BR>
C2DRect::operator = <BR>
\brief Assignment.
<P>---------------------------------------------------------------------------*/
const C2DRect& C2DRect::operator=(const C2DRect& Other)
{
	m_TopLeft = Other.GetTopLeft();
	m_BottomRight = Other.GetBottomRight();
	return *this;
}



/**--------------------------------------------------------------------------<BR>
C2DRect::Overlaps <BR>
\brief True if there is an overlap.
<P>---------------------------------------------------------------------------*/
bool C2DRect::Overlaps(const C2DRect& Other) const
{
	bool bOvX = !(Other.GetLeft() >= m_BottomRight.x ||
				  Other.GetRight() <=  m_TopLeft.x);
	
	bool bOvY = !(Other.GetBottom() >=  m_TopLeft.y ||
				  Other.GetTop() <= m_BottomRight.y);

	return bOvX && bOvY;

}

/**--------------------------------------------------------------------------<BR>
C2DRect::Overlaps <BR>
\brief True if there is an overlap. Returns the overlap.
<P>---------------------------------------------------------------------------*/
bool C2DRect::Overlaps(const C2DRect& Other, C2DRect& Overlap) const 
{

	C2DPoint ptOvTL;
	C2DPoint ptOvBR;

	ptOvTL.y = min(m_TopLeft.y, Other.GetTopLeft().y);
	ptOvBR.y = max(m_BottomRight.y, Other.GetBottomRight().y);

	ptOvTL.x = max(m_TopLeft.x, Other.GetTopLeft().x);
	ptOvBR.x = min(m_BottomRight.x, Other.GetBottomRight().x);

	Overlap.Set(ptOvTL, ptOvBR);

	return Overlap.IsValid();
}

/**--------------------------------------------------------------------------<BR>
C2DRect::Contains <BR>
\brief True if the other is entirely contained within this.
<P>---------------------------------------------------------------------------*/
bool C2DRect::Contains(const C2DRect& Other) const
{
	return (Other.GetLeft() >  m_TopLeft.x &&
			  Other.GetRight() < m_BottomRight.x &&
			  Other.GetBottom() > m_BottomRight.y &&
			  Other.GetTop() <  m_TopLeft.y);
}

/**--------------------------------------------------------------------------<BR>
C2DRect::GetPointClosestToOrigin <BR>
\brief Returns the point which is closest to the origin (0,0).
<P>---------------------------------------------------------------------------*/
C2DPoint C2DRect::GetPointClosestToOrigin(void) const
{
	C2DPoint ptResult;
	if ( fabs(m_TopLeft.x) < fabs(m_BottomRight.x))
	{
		// Left is closest to the origin.
		ptResult.x =  m_TopLeft.x;
	}
	else
	{
		// Right is closest to the origin
		ptResult.x = m_BottomRight.x;
	}

	if ( fabs(m_TopLeft.y) < fabs(m_BottomRight.y))
	{
		// Top is closest to the origin.
		ptResult.y =  m_TopLeft.y;
	}
	else
	{
		// Bottom is closest to the origin
		ptResult.y = m_BottomRight.y;
	}

	return ptResult;
}

/**--------------------------------------------------------------------------<BR>
C2DRect::GetPointFurthestFromOrigin <BR>
\brief Returns the point which is furthest from the origin (0,0).
<P>---------------------------------------------------------------------------*/
C2DPoint C2DRect::GetPointFurthestFromOrigin(void) const
{
	C2DPoint ptResult;
	if ( fabs(m_TopLeft.x) > fabs(m_BottomRight.x))
	{
		// Left is furthest to the origin.
		ptResult.x =  m_TopLeft.x;
	}
	else
	{
		// Right is furthest to the origin
		ptResult.x = m_BottomRight.x;
	}

	if ( fabs(m_TopLeft.y) > fabs(m_BottomRight.y))
	{
		// Top is furthest to the origin.
		ptResult.y =  m_TopLeft.y;
	}
	else
	{
		// Bottom is furthest to the origin
		ptResult.y = m_BottomRight.y;
	}

	return ptResult;
}

/**--------------------------------------------------------------------------<BR>
C2DRect::Distance
\brief Returns the distance from the rectangle to the point, 0 if the points inside.
<P>---------------------------------------------------------------------------*/
double C2DRect::Distance(const C2DPoint& TestPoint) const
{

	if(TestPoint.x > m_BottomRight.x) // To the east half
	{
		if (TestPoint.y >  m_TopLeft.y)			// To the north east
			return TestPoint.Distance(C2DPoint( m_BottomRight.x,  m_TopLeft.y)); 
		else if (TestPoint.y < m_BottomRight.y)		// To the south east
			return TestPoint.Distance(m_BottomRight);
		else
			return (TestPoint.x - m_BottomRight.x);	// To the east
	}
	else if (TestPoint.x <  m_TopLeft.x)	// To the west half
	{
		if (TestPoint.y >  m_TopLeft.y)			// To the north west
			return TestPoint.Distance(m_TopLeft); 
		else if (TestPoint.y < m_BottomRight.y)		// To the south west
			return TestPoint.Distance(  C2DPoint(  m_TopLeft.x,  m_BottomRight.y));
		else
			return (m_TopLeft.x  - TestPoint.x );	// To the west
	}
	else
	{
		if (TestPoint.y >  m_TopLeft.y)		//To the north
			return (TestPoint.y -  m_TopLeft.y);
		else if (TestPoint.y < m_BottomRight.y)	// To the south
			return (m_BottomRight.y - TestPoint.y );
	}

	assert(Contains(TestPoint));
	return 0;	// Inside
}


/**--------------------------------------------------------------------------<BR>
C2DRect::Distance
\brief Returns the distance from the rectangle to the other rect, 0 if overlap.
<P>---------------------------------------------------------------------------*/
double C2DRect::Distance(const C2DRect& Other) const
{
	if (this->Overlaps(Other))
		return 0;

	if (Other.GetLeft() > this->m_BottomRight.x)
	{
		// Other is to the right
		if (Other.GetBottom() > this->m_TopLeft.y)
		{
			// Other is to the top right
			C2DPoint ptTopRight(m_BottomRight.x,  m_TopLeft.y);
			return ptTopRight.Distance(C2DPoint(Other.GetLeft(), Other.GetBottom()));
		}
		else if (Other.GetTop() < this->m_BottomRight.y)
		{
			// Other to the bottom right
			return m_BottomRight.Distance( Other.GetTopLeft() );
		}
		else
		{
			// to the right
			return Other.GetLeft() - this->m_BottomRight.x;
		}
	}
	else if ( Other.GetRight() < this->m_TopLeft.x)
	{
		// Other to the left
		if (Other.GetBottom() > this->m_TopLeft.y)
		{
			// Other is to the top left
			return  m_TopLeft.Distance(Other.GetBottomRight());
		}
		else if (Other.GetTop() < this->m_BottomRight.y)
		{
			// Other to the bottom left
			C2DPoint ptBottomLeft(m_TopLeft.x, m_BottomRight.y);
			return ptBottomLeft.Distance ( C2DPoint( Other.GetRight(), Other.GetTop()));
		}
		else
		{
			//Just to the left
			return (this->m_TopLeft.x - Other.GetRight());
		}
	}
	else
	{
		// There is horizontal overlap;
		if (Other.GetBottom() >  m_TopLeft.y)
			return Other.GetBottom() -  m_TopLeft.y;
		else
			return m_BottomRight.y - Other.GetTop();
	}		

}



/**--------------------------------------------------------------------------<BR>
C2DRect::Distance
\brief Projection onto the line.
<P>---------------------------------------------------------------------------*/
void C2DRect::Project(const C2DLine& Line, CInterval& Interval) const
{
	this->m_TopLeft.Project( Line, Interval);
	Interval.ExpandToInclude( m_BottomRight.Project( Line));
	C2DPoint TR( m_BottomRight.x,   m_TopLeft.y);
	C2DPoint BL(m_TopLeft.x ,  m_BottomRight.y);
	Interval.ExpandToInclude( TR.Project( Line));
	Interval.ExpandToInclude( BL.Project( Line));
}


/**--------------------------------------------------------------------------<BR>
C2DRect::Distance
\brief Projection onto the vector.
<P>---------------------------------------------------------------------------*/
void C2DRect::Project(const C2DVector& Vector, CInterval& Interval) const
{
	this->m_TopLeft.Project( Vector, Interval);
	Interval.ExpandToInclude( m_BottomRight.Project( Vector));
	C2DPoint TR( m_BottomRight.x,   m_TopLeft.y);
	C2DPoint BL(m_TopLeft.x ,  m_BottomRight.y);
	Interval.ExpandToInclude( TR.Project( Vector));
	Interval.ExpandToInclude( BL.Project( Vector));

}

/**--------------------------------------------------------------------------<BR>
C2DRect::SnapToGrid
\brief Snaps to the grid - see CGrid.
<P>---------------------------------------------------------------------------*/
void C2DRect::SnapToGrid(void)
{
	m_TopLeft.SnapToGrid();
	m_BottomRight.SnapToGrid();

}



/**--------------------------------------------------------------------------<BR>
C2DRect::OverlapsVertically
\brief True if this is above or below the other.
<P>---------------------------------------------------------------------------*/
bool C2DRect::OverlapsVertically( const C2DRect& Other) const
{
	return !(Other.GetLeft() >= m_BottomRight.x ||
				  Other.GetRight() <=  m_TopLeft.x);
}


/**--------------------------------------------------------------------------<BR>
C2DRect::OverlapsAbove
\brief True if this is above the other.
<P>---------------------------------------------------------------------------*/
bool C2DRect::OverlapsAbove( const C2DRect& Other) const
{
	if (Other.GetLeft() >= m_BottomRight.x ||
				  Other.GetRight() <=  m_TopLeft.x)
	{
		return false;
	}
	else 
	{
		return m_TopLeft.y > Other.GetBottom();
	}
}


/**--------------------------------------------------------------------------<BR>
C2DRect::OverlapsAbove
\brief True if this is below the other.
<P>---------------------------------------------------------------------------*/
bool C2DRect::OverlapsBelow( const C2DRect& Other) const
{
	if (Other.GetLeft() >= m_BottomRight.x ||
				  Other.GetRight() <=  m_TopLeft.x)
	{
		return false;
	}
	else 
	{
		return m_BottomRight.y < Other.GetTop();
	}
}