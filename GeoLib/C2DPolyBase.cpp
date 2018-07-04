/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DPolyBase.cpp
\brief Implementation file for the C2DPolyBase Class.

Implementation file for C2DPolyBase, a class representing a generic area which
is a closed set of lines of some sort.
<P>---------------------------------------------------------------------------*/


#include "StdAfx.h"
#include "C2DPolyBase.h"
#include "C2DArc.h"
#include "C2DHoledPolyBase.h"
#include "C2DHoledPolyBaseSet.h"
#include "RandomNumber.h"
#include "Interval.h"
#include "C2DCircle.h"
#include "C2DLineBaseSetSet.h"
#include "IndexSet.h"
#include "C2DPointSet.h"
#include "C2DSegment.h"
#include "Sort.h"


_MEMORY_POOL_IMPLEMENATION(C2DPolyBase)


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::C2DPolyBase <BR>
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DPolyBase::C2DPolyBase(void) : C2DBase(PolyBase)
{

}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::C2DPolyBase <BR>
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DPolyBase::C2DPolyBase(const C2DPolyBase& Other): C2DBase(PolyBase)
{
	Set(Other);	
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::~C2DPolyBase <BR>
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DPolyBase::~C2DPolyBase(void)
{

}




/**--------------------------------------------------------------------------<BR>
C2DPolyBase::GetLine <BR>
\brief Returns a reference to the line. Cyclic past the last line.
<P>---------------------------------------------------------------------------*/
const C2DLineBase* C2DPolyBase::GetLine(unsigned int i) const
{
	unsigned int nLines = m_Lines.size();

	if (nLines == 0)
		return 0;

	return &m_Lines[i % nLines];
}



/**--------------------------------------------------------------------------<BR>
C2DPolyBase::GetLineRect <BR>
\brief Returns the line rectangle.
<P>---------------------------------------------------------------------------*/
const C2DRect* C2DPolyBase::GetLineRect(unsigned int i) const
{
	unsigned int nLinesRects = m_LineRects.size();

	if (nLinesRects == 0)
		return 0;

	return &m_LineRects[i % nLinesRects];
}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Contains <BR>
\brief True if the point is contained.
<P>---------------------------------------------------------------------------*/
bool C2DPolyBase::Contains(const C2DPoint& pt) const
{
	if (!m_BoundingRect.Contains(pt))
		return false;

	C2DPointSet IntersectedPts;

	C2DLine Ray(pt, C2DVector(m_BoundingRect.Width(), 0.000001)); // Make sure to leave

	if (!this->Crosses(Ray, &IntersectedPts))
		return false;
	else
	{
		IntersectedPts.SortByDistance(Ray.point);
		if ( IntersectedPts[0] == pt)
		{
			// For integers, the pt can start On a line, meaning it's INSIDE, but the ray could cross again
			// so just return true. Because the equality test is really a test for proximity, this leads to the
			// possibility that a point could lie just outside the shape but be considered to be inside. This would
			// only be a problem with very small shapes that are a very long way from the origin. E.g. a 1m2 object
			// 1 million metres from the origin and a point 0.1mm away from the edge would give rise to a relative 
			// difference of 0.0001 / 1000000 = 0.000000001 which would just be consider to be inside.
			return true;
		}
		else
		{
			// Return true if the ray 
			return (IntersectedPts.size() & (unsigned int)1);
		}
	}
}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::HasCrossingLines <BR>
\brief True if there are crossing lines.
<P>---------------------------------------------------------------------------*/
bool C2DPolyBase::HasCrossingLines(void) const
{
	assert (m_Lines.size() == m_LineRects.size());
	return m_Lines.HasCrossingLines();

}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Distance <BR>
\brief The distance from the shape to the point. 0 if inside.
<P>---------------------------------------------------------------------------*/
double C2DPolyBase::Distance(const C2DLineBase& Line) const
{
	if (m_Lines.size() == 0)
		return 0;

	double dMin = m_Lines[0].Distance(Line);
	double dDist;

	for (unsigned int i = 1 ; i < m_Lines.size(); i++)
	{
		dDist = m_Lines[i].Distance(Line);
		if (dDist == 0 )
			return 0;
		if (dDist < dMin)
			dMin = dDist;
	}

	if ( Contains(Line.GetPointFrom()))
		return -dMin;
	else
		return dMin;
}



/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Distance <BR>
\brief The distance from the edge of the area.
<P>---------------------------------------------------------------------------*/
double C2DPolyBase::Distance(const C2DPolyBase& Other, 
		C2DPoint* ptOnThis , C2DPoint* ptOnOther ) const
{
	if (m_Lines.size() == 0)
		return 0;

	if (Other.GetLineCount() == 0)
		return 0;

	if (Other.GetLineRectCount() != Other.GetLineCount())
		return 0;

	if (m_Lines.size() != m_LineRects.size())
		return 0;

	// First we find the closest line rect to the other's bounding rectangle.
	unsigned int usThisClosestLineGuess = 0;
	const C2DRect& OtherBoundingRect = Other.GetBoundingRect();
	double dClosestDist = m_LineRects[0].Distance(OtherBoundingRect);
	for (unsigned int i = 1; i < m_LineRects.size(); i++)
	{
		double dDist = m_LineRects[i].Distance(OtherBoundingRect);
		if (dDist < dClosestDist)
		{
			dClosestDist = dDist;
			usThisClosestLineGuess = i;
		}
	}
	// Now cycle through all the other poly's line rects to find the closest to the
	// guessed at closest line on this.
	unsigned int usOtherClosestLineGuess = 0;
	dClosestDist = Other.GetLineRect(0)->Distance(m_LineRects[usThisClosestLineGuess]);
	for (unsigned int j = 1; j < Other.GetLineRectCount(); j++)
	{
		double dDist = Other.GetLineRect(j)->Distance(m_LineRects[usThisClosestLineGuess]);
		if (dDist < dClosestDist)
		{
			dClosestDist = dDist;
			usOtherClosestLineGuess = j;
		}
	}

	// Now we have a guess at the 2 closest lines.
	double dMinDistGuess = m_Lines[usThisClosestLineGuess].Distance(
							*Other.GetLine(usOtherClosestLineGuess),
							ptOnThis,
							ptOnOther);
	// If its 0 then return 0.
	if (dMinDistGuess == 0)
		return 0;

	C2DPoint ptOnThisTemp;
	C2DPoint ptOnOtherTemp;

	// Now go through all of our line rects and only check further if they are closer
	// to the other's bounding rect than the min guess.
	for (unsigned int i = 0; i < m_Lines.size(); i++)
	{
		if (m_LineRects[i].Distance( OtherBoundingRect ) <  dMinDistGuess)
		{
			for ( unsigned int j = 0 ; j < Other.GetLineCount() ; j++)
			{
				double dDist = m_Lines[i].Distance(*Other.GetLine(j),
													&ptOnThisTemp,
													&ptOnOtherTemp);
				
				if (dDist < dMinDistGuess)
				{	
					if (ptOnThis != 0)
						*ptOnThis = ptOnThisTemp;
					if (ptOnOther != 0)
						*ptOnOther = ptOnOtherTemp;

					if (dDist == 0)
						return 0;

					dMinDistGuess = dDist; 
				}
			}
		}
	}

	// if we are here, there is no intersection but the other could be inside this or vice-versa
	if ( m_BoundingRect.Contains(Other.GetBoundingRect())
		&& Contains(ptOnOtherTemp)  )
	{
		dMinDistGuess *= -1.0;
	}
	else if ( Other.GetBoundingRect().Contains(m_BoundingRect)
		&& Other.Contains( ptOnThisTemp ))
	{
		dMinDistGuess *= -1.0;
	}

	return dMinDistGuess;
}



/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Distance <BR>
\brief The distance from the edge.
<P>---------------------------------------------------------------------------*/
double C2DPolyBase::Distance(const C2DPoint& pt) const
{
	if (m_Lines.size() == 0)
		return 0;

	double dResult = m_Lines[0].Distance(pt);
	for (unsigned int i = 1; i < m_Lines.size(); i++)
	{
		double dDist = m_Lines[i].Distance(pt);
		if (dDist < dResult)
			dResult = dDist;
	}

	if (Contains(pt))
		return -dResult;
	else
		return dResult;
}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::MakeBoundingRect <BR>
\brief Makes the bounding rectangle.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::MakeBoundingRect(void)
{
	if ( m_LineRects.size() == 0)
	{
		m_BoundingRect.Clear();
		return;
	}
	else
	{
		m_BoundingRect = m_LineRects[0];

		for (unsigned int i = 1 ; i  < m_LineRects.size(); i++)
		{
			m_BoundingRect.ExpandToInclude(m_LineRects[i]);
		}
	}
}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::MakeLineRects <BR>
\brief Makes the bounding rectangles for the lines.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::MakeLineRects(void)
{
	m_LineRects.DeleteAll();

	unsigned int nCount = m_Lines.size();

	for (unsigned int i = 0; i < nCount; i++)
	{
		C2DRect* pRect = new C2DRect;
		m_Lines[i].GetBoundingRect(*pRect);
		m_LineRects.Add( pRect );
	}
}




/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Clear <BR>
\brief Clears all.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::Clear(void)
{
	m_BoundingRect.Clear();
	m_Lines.DeleteAll();
	m_LineRects.DeleteAll();
}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::IsWithinDistance <BR>
\brief True if the point is within the distance given or inside.
<P>---------------------------------------------------------------------------*/
bool C2DPolyBase::IsWithinDistance(const C2DPoint& pt, double dRange) const
{
	C2DRect RectTemp = m_BoundingRect;
	RectTemp.Expand(dRange);

	if (!RectTemp.Contains(pt))
		return false;

	if (m_Lines.size() == 0)
		return false;

	if (m_Lines[0].GetPointFrom().Distance(pt) < dRange)
		return true;

	if (this->Contains(pt))
		return true;

	for (unsigned int i = 1; i < m_Lines.size(); i++)
	{
		if(m_Lines[i].Distance(pt) < dRange)
			return true;
	}

	return false;
}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::GetPerimeter <BR>
\brief Returns the perimeter.
<P>---------------------------------------------------------------------------*/
double C2DPolyBase::GetPerimeter(void) const
{
	double dResult = 0;

	for (unsigned int i = 0; i < this->m_Lines.size(); i++)
	{
		dResult += m_Lines[i].GetLength();
	}

	return dResult;
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Move <BR>
\brief Moves the shape.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::Move(const C2DVector& vector)
{

	assert(m_Lines.size() == m_LineRects.size());

	if(m_Lines.size() != m_LineRects.size())
		return;

	for (unsigned int i = 0; i < this->m_Lines.size(); i++)
	{
		m_Lines[i].Move(vector);
		m_LineRects[i].Move(vector);
	}

	m_BoundingRect.Move(vector);

}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::RotateToRight <BR>
\brief Moves the shape.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::RotateToRight(double dAng, const C2DPoint& Origin)
{
	assert(m_Lines.size() == m_LineRects.size());

	if(m_Lines.size() != m_LineRects.size())
		return;

	for (unsigned int i = 0; i < m_Lines.size(); i++)
	{
		m_Lines[i].RotateToRight(dAng, Origin);
		m_Lines[i].GetBoundingRect(m_LineRects[i]);		
	}

	MakeBoundingRect();
}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Grow <BR>
\brief Grows the shape.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::Grow(double dFactor, const C2DPoint& Origin)
{
	assert(m_Lines.size() == m_LineRects.size());

	if(m_Lines.size() != m_LineRects.size())
		return;

	for (unsigned int i = 0; i < m_Lines.size(); i++)
	{
		m_Lines[i].Grow(dFactor, Origin);
		m_Lines[i].GetBoundingRect(m_LineRects[i]);		
	}

	m_BoundingRect.Grow(dFactor, Origin);

}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Reflect <BR>
\brief Reflect
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::Reflect(const C2DPoint& point)
{
	assert(m_Lines.size() == m_LineRects.size());

	if(m_Lines.size() != m_LineRects.size())
		return;

	for (unsigned int i = 0; i < m_Lines.size(); i++)
	{
		m_Lines[i].Reflect(point);
	}
	ReverseDirection(); // ALSO MAKES THE LINES AGAIN.

	m_BoundingRect.Reflect(point);
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Reflect <BR>
\brief Reflect
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::Reflect(const C2DLine& Line)
{
	assert(m_Lines.size() == m_LineRects.size());

	if(m_Lines.size() != m_LineRects.size())
		return;

	for (unsigned int i = 0; i < m_Lines.size(); i++)
	{
		m_Lines[i].Reflect(Line);
	}
	ReverseDirection(); // ALSO MAKES THE LINES AGAIN.

	m_BoundingRect.Reflect(Line);
}



/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Project <BR>
\brief Projection onto the line
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::Project(const C2DLine& Line, CInterval& Interval) const
{
	if(m_Lines.size() == 0)
		return;

	m_Lines[0].Project(Line, Interval);

	for (unsigned int i = 1; i < m_Lines.size(); i++)
	{
		CInterval LineInt;
		m_Lines[i].Project(Line, LineInt);
		Interval.ExpandToInclude( LineInt );
	}
}



/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Project <BR>
\brief Projection onto the vector
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::Project(const C2DVector& Vector, CInterval& Interval) const
{
	if(m_Lines.size() == 0)
		return;

	m_Lines[0].Project(Vector, Interval);

	for (unsigned int i = 1; i < m_Lines.size(); i++)
	{
		CInterval LineInt;
		m_Lines[i].Project(Vector, LineInt);
		Interval.ExpandToInclude( LineInt );
	}
}



/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Crosses <BR>
\brief True if the line crosses the shape. Returns the points.
<P>---------------------------------------------------------------------------*/
bool C2DPolyBase::Crosses(const C2DLineBase& Line, C2DPointSet* IntersectionPts) const
{
	C2DRect LineRect;
	Line.GetBoundingRect(LineRect);

	if (!m_BoundingRect.Overlaps(LineRect))
		return false;

	assert(m_Lines.size() == m_LineRects.size());

	if(m_Lines.size() != m_LineRects.size())
		return false;

	C2DPointSet IntersectionTemp;

	bool bResult = false;

	for (unsigned int i = 0; i < this->m_Lines.size(); i++)
	{
		if (m_LineRects[i].Overlaps(LineRect) &&
			m_Lines[i].Crosses(Line, &IntersectionTemp))
		{
			bResult = true;
		}
	}

	(*IntersectionPts) << IntersectionTemp;

	return bResult;

}

bool C2DPolyBase::Crosses(const C2DLineBase &Line, C2DPointSet *IntersectionPts, C2DLineBaseSet *IntersectionLines) const
{
    C2DRect LineRect;
    Line.GetBoundingRect(LineRect);

    if (!m_BoundingRect.Overlaps(LineRect))
        return false;

    assert(m_Lines.size() == m_LineRects.size());

    if(m_Lines.size() != m_LineRects.size())
        return false;

    C2DPointSet IntersectionTemp;
    C2DLineBaseSet IntersectionLinesTemp;

    bool bResult = false;

    for (unsigned int i = 0; i < this->m_Lines.size(); i++)
    {
        if (m_LineRects[i].Overlaps(LineRect) &&
            m_Lines[i].Crosses(Line, &IntersectionTemp))
        {
            bResult = true;
            IntersectionLinesTemp.AddCopy(m_Lines[i]);
        }
    }

    (*IntersectionPts) << IntersectionTemp;
    (*IntersectionLines) << IntersectionLinesTemp;

    return bResult;
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::CrossesRay <BR>
\brief True if the ray crosses the shape. Returns the points.
<P>---------------------------------------------------------------------------*/
bool C2DPolyBase::CrossesRay(const C2DLine& Line, C2DPointSet* IntersectionPts) const
{
	double dDist = Line.point.Distance(m_BoundingRect.GetCentre());

	C2DLine LineTemp = Line;

	LineTemp.vector.SetLength(dDist + m_BoundingRect.Width() + m_BoundingRect.Height());

	return Crosses(LineTemp, IntersectionPts);
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Contains <BR>
\brief True if the line is contained within the shape.
<P>---------------------------------------------------------------------------*/
bool C2DPolyBase::Contains(const C2DLineBase& Line) const
{

	if (!Contains(Line.GetPointFrom())) 
		return false;

	return !Crosses(Line);
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Contains <BR>
\brief True if the other is contained within this.
<P>---------------------------------------------------------------------------*/
bool C2DPolyBase::Contains(const C2DPolyBase& Other) const
{
	if (Other.GetLineCount() == 0) 
		return false;
	
	if (!m_BoundingRect.Contains(Other.GetBoundingRect()))
		return false;

	if (!Contains(Other.GetLine(0)->GetPointFrom())) 
		return false;

	return !this->Crosses(Other);
}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Contains <BR>
\brief True if the other is contained within this.
<P>---------------------------------------------------------------------------*/
bool C2DPolyBase::Contains(const C2DHoledPolyBase& Other) const
{
	if (Other.GetRim() == 0)
		return false;

	return Contains( *Other.GetRim());
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::operator= <BR>
\brief Assignment.
<P>---------------------------------------------------------------------------*/
const C2DPolyBase& C2DPolyBase::operator=(const C2DPolyBase& Other)
{
	Set(Other);

	return *this;
}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Set <BR>
\brief Assignment.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::Set(const C2DPolyBase& Other)
{
	Clear();

	for (unsigned int i = 0; i < Other.GetLineCount() ; i++)
	{
		m_Lines.AddCopy(*Other.GetLine(i));
	}

	m_BoundingRect = Other.GetBoundingRect();

	for (unsigned int i = 0 ; i < Other.GetLineRectCount() ; i ++)
	{
		m_LineRects.Add(new C2DRect(*Other.GetLineRect(i)));
	}
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Crosses <BR>
\brief True if the line crosses this.
<P>---------------------------------------------------------------------------*/
bool C2DPolyBase::Crosses(const C2DLineBase& Line) const
{
	C2DRect LineRect;
	Line.GetBoundingRect(LineRect);

	for (unsigned int i = 0; i < this->m_Lines.size(); i++)
	{
		if (m_LineRects[i].Overlaps( LineRect ) &&  m_Lines[i].Crosses(Line))
			return true;
	}
	return false;
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Crosses <BR>
\brief True if the other crosses this.
<P>---------------------------------------------------------------------------*/
bool C2DPolyBase::Crosses(const C2DPolyBase& Other) const
{
	if (!m_BoundingRect.Overlaps(Other.GetBoundingRect()))
		return false;

	for (unsigned int i = 0; i < this->m_Lines.size(); i++)
	{
		if (Other.Crosses(m_Lines[i]))
			return true;
	}
	return false;
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Crosses <BR>
\brief True if the other crosses this and returns the intersection points.
<P>---------------------------------------------------------------------------*/
bool C2DPolyBase::Crosses(const C2DPolyBase& Other, C2DPointSet* IntersectionPts) const
{
	if (!m_BoundingRect.Overlaps(Other.GetBoundingRect()))
		return false;

	C2DPointSet IntPtsTemp;

	m_Lines.GetIntersections( Other.GetLines(), &IntPtsTemp, 0, 0);

	bool bResult = IntPtsTemp.size() > 0;

	*IntersectionPts << IntPtsTemp;

	return bResult;

}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Overlaps <BR>
\brief True if the other area overlaps this.
<P>---------------------------------------------------------------------------*/
bool C2DPolyBase::Overlaps(const C2DPolyBase& Other) const 
{
	if (m_Lines.size() == 0 || Other.GetLineCount() == 0)
		return false;

	if (Other.Contains(m_Lines[0].GetPointTo()))
		return true;

	if (Crosses(Other))
		return true;

	return (this->Contains(Other.GetLine(0)->GetPointTo() ));
}


bool C2DPolyBase::Overlaps(const C2DHoledPolyBase& Other) const
{
	return Other.Overlaps(*this);


}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::ReverseDirection <BR>
\brief Reverses the direction of the lines.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::ReverseDirection(void)
{
	m_Lines.ReverseOrder();

	for (unsigned int i = 0; i < m_Lines.size() ; i++)
	{
		m_Lines[i].ReverseDirection();
	}	

	MakeLineRects();
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::IsClosed <BR>
\brief True if the lines form a closed shape.
<P>---------------------------------------------------------------------------*/
bool C2DPolyBase::IsClosed(void) const
{
	return m_Lines.IsClosed(false);
}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::CreateDirect <BR>
\brief Creates directly from a set of lines through taking them from the set given.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::CreateDirect(C2DLineBaseSet& Lines)
{
	m_Lines.DeleteAll();
	m_Lines << Lines;

	this->MakeLineRects();
	this->MakeBoundingRect();

}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::Create <BR>
\brief Creates from a set of lines through copying them from the set given.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::Create(const C2DLineBaseSet& Lines)
{
	m_Lines.DeleteAll();
	m_Lines.AddCopy(Lines);

	this->MakeLineRects();
	this->MakeBoundingRect();

}




/**--------------------------------------------------------------------------<BR>
C2DPolyBase::RandomPerturb <BR>
\brief Perturbs the shape by a very small random amount so as to avoid degeneracies
with another polygon caused by coincident lines or points.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::RandomPerturb(void)
{
	C2DPoint pt = m_BoundingRect.GetPointFurthestFromOrigin();
	double dMinEq = max(pt.x, pt.y) * conEqualityTolerance;
	CRandomNumber rn(dMinEq * 10, dMinEq * 100);

	C2DVector cVector( rn.Get(), rn.Get() );
	if (CRandomNumber::GetBool())
		cVector.i = - cVector.i ;
	if (CRandomNumber::GetBool())
		cVector.j = - cVector.j ;
	Move(cVector);
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::GetOverlaps <BR>
\brief Gets the overlaps.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::GetOverlaps(const C2DPolyBase& Other, C2DHoledPolyBaseSet& Polygons,
							  CGrid::eDegenerateHandling eDegen) const 
{
	GetBoolean( Other, Polygons, true, true, eDegen);

}



/**--------------------------------------------------------------------------<BR>
C2DPolyBase::GetNonOverlaps <BR>
\brief Returns the set of complex polygons forming the difference between this and the
other. i.e. the parts of this which are not inside the other. If there is no 
interaction, nothing is done.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::GetNonOverlaps(const C2DPolyBase& Other, 
				C2DHoledPolyBaseSet& Polygons, CGrid::eDegenerateHandling eDegen)const 
{
	GetBoolean( Other, Polygons, false, true, eDegen);
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::GetUnion <BR>
\brief Returns the combined areas of this and the other polygon. If there is no 
interaction, nothing is done.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::GetUnion(const C2DPolyBase& Other, 
			C2DHoledPolyBaseSet& Polygons, CGrid::eDegenerateHandling eDegen) const 
{
	GetBoolean( Other, Polygons, false, false, eDegen);
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::GetBoolean <BR>
\brief Gets the union / intersect etc depending on the inside / outside flags.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::GetBoolean(const C2DPolyBase& Other, C2DHoledPolyBaseSet& HoledPolys,
						bool bThisInside, bool bOtherInside, 
						CGrid::eDegenerateHandling eDegen) const
{
	if (m_BoundingRect.Overlaps(Other.GetBoundingRect() ))
	{
		switch (eDegen)
		{
		case CGrid::None:
			{
				C2DLineBaseSetSet Routes1, Routes2;
				C2DPolyBase::GetRoutes( *this, bThisInside, Other, bOtherInside, Routes1, Routes2);
				Routes1 << Routes2;

				if (Routes1.size() > 0)
				{
					// Add all the joining routes together to form closed routes
					Routes1.MergeJoining();
					// Set up some temporary polygons.
					C2DPolyBaseSet Polygons;
					// Turn the routes into polygons.
					for (int i = Routes1.size() - 1; i >= 0; i--)
					{
						C2DLineBaseSet* pRoute = Routes1.GetAt(i);
						if (pRoute->IsClosed() && pRoute->size() > 2)
						{
							Polygons.Add(new C2DPolyBase);
							Polygons.GetLast()->CreateDirect( *pRoute);
						}
						else
						{
							CGrid::LogDegenerateError();
						}	
					}


					// Set up some temporary holed polygons
					C2DHoledPolyBaseSet NewComPolys;
					// Turn the set of polygons into holed polygons.
					// Only if not the intersection. (CR)
					if (!(bThisInside && bOtherInside) )
					{
						C2DHoledPolyBase::PolygonsToHoledPolygons(NewComPolys, Polygons);
						if (NewComPolys.size() != 1 && 
						   bThisInside == bOtherInside ) // union must be 1
						{
							CGrid::LogDegenerateError();
						}
					}
					else
					{
						HoledPolys << Polygons;
					}

					// Now add them all to the provided set.
					HoledPolys << NewComPolys;
				}
			}
			break;
		case CGrid::RandomPerturbation:
			{
				C2DPolyBase OtherCopy(Other);
				OtherCopy.RandomPerturb();
				GetBoolean( OtherCopy, HoledPolys, bThisInside, bOtherInside, CGrid::None);
			}
			break;
		case CGrid::DynamicGrid:
			{
				C2DRect Rect; 
				if (GetBoundingRect().Overlaps(Other.GetBoundingRect(), Rect))
				{
					double dOldGrid = CGrid::GetGridSize();
					CGrid::SetToMinGridSize(Rect);
					GetBoolean( Other, HoledPolys, bThisInside, bOtherInside, CGrid::PreDefinedGrid);
					CGrid::SetGridSize(dOldGrid);
				}
			}
			break;
		case CGrid::PreDefinedGrid:
			{
				C2DPolyBase P1(*this), P2(Other);
				P1.SnapToGrid();
				P2.SnapToGrid();
				C2DVector V1( P1.GetBoundingRect().GetTopLeft(),  P2.GetBoundingRect().GetTopLeft());
				double dPerturbation = CGrid::GetGridSize(); // ensure it snaps back to original grid positions.
				V1.i > 0 ? V1.i = dPerturbation: V1.i = -dPerturbation;	// move away slightly if possible
				V1.j > 0 ? V1.j = dPerturbation: V1.j = -dPerturbation; // move away slightly if possible
				V1.i *= 0.411923;// ensure it snaps back to original grid positions.
				V1.j *= 0.313131;// ensure it snaps back to original grid positions.

				P2.Move( V1 );
				P1.GetBoolean( P2, HoledPolys, bThisInside, bOtherInside, CGrid::None);
				HoledPolys.SnapToGrid();	
			}
			break;
		case CGrid::PreDefinedGridPreSnapped:
			{
				C2DPolyBase P2(Other);
				C2DVector V1( this->GetBoundingRect().GetTopLeft(),  P2.GetBoundingRect().GetTopLeft());
				double dPerturbation = CGrid::GetGridSize(); // ensure it snaps back to original grid positions.
				V1.i > 0 ? V1.i = dPerturbation: V1.i = -dPerturbation; // move away slightly if possible
				V1.j > 0 ? V1.j = dPerturbation: V1.j = -dPerturbation; // move away slightly if possible
				V1.i *= 0.411923;// ensure it snaps back to original grid positions.
				V1.j *= 0.313131;// ensure it snaps back to original grid positions.

				P2.Move( V1 );
				GetBoolean( P2, HoledPolys, bThisInside, bOtherInside, CGrid::None);
				HoledPolys.SnapToGrid();	
			}
			break;
		}
	}
}



/**--------------------------------------------------------------------------<BR>
C2DPolyBase::GetRoutes<BR>
\brief Returns the routes inside or outside the other polygon / holed polygon.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::GetRoutes(C2DPointSet& IntPts, CIndexSet& IntIndexes , 
						C2DLineBaseSetSet& Routes, bool bStartInside,  bool bRoutesInside ) const
{
	// Make sure the intersection indexes and points are the same size.
	if (IntIndexes.size() != IntPts.size() )
	{
		assert(0);
		return;
	}
	// Set up a new collection of routes.
	C2DLineBaseSetSet NewRoutes;
	// If the polygon has no points then return.
	if (this->GetLineCount() < 1) 
		return;
	// Sort the intersections by index so we can go through them in order.
	GeoSort::PQuickSort<CIndexSet, unsigned int , C2DPointSet, C2DPoint>(IntIndexes, IntPts);
	// Set the inside / outside flag to the same as the start inside / outside flag.
	bool bInside = bStartInside;
	// If we are inside and want route inside or outside and want routes outside then add a new route.
	if (bInside == bRoutesInside)
	{
		NewRoutes.Add(new C2DLineBaseSet);
	}

	// The current index of the intersects.
	unsigned int usCurrentIntIndex = 0;

	// cycle through the lines on the polygon.
	for (unsigned int i = 0 ; i < m_Lines.size() ; i++)
	{
		// Set up a list of intersection points on this line only.
		C2DPointSet IntsOnLine;
		// Cycle through all intersections on this line (leaving the usCurrentIntIndex at the next intersected line).
		while ( usCurrentIntIndex < IntIndexes.size() && IntIndexes[usCurrentIntIndex] == i)
		{
			// Add a copy of the points on this line that are intersections
			IntsOnLine.AddCopy( IntPts[ usCurrentIntIndex ] );
			usCurrentIntIndex++;
		}

		// If the line in question intersects the other poly then we have left / entered.
		if ( IntsOnLine.size() > 0 )
		{
			C2DLineBaseSet SubLines;
			m_Lines[i].GetSubLines( IntsOnLine, SubLines );

			while (SubLines.size() > 1)
			{
				if (bInside == bRoutesInside)
				{
					// We have 1. Left and want route in. OR 2. Entered and want routes out.
					NewRoutes.GetLast()->Add( SubLines.ExtractAt(0) );
					bInside = true ^ bRoutesInside;
				}
				else
				{
					NewRoutes.Add(new C2DLineBaseSet);
					bInside = false ^ bRoutesInside;
					delete SubLines.ExtractAt(0);
				}
			}
			if (bInside == bRoutesInside)
				NewRoutes.GetLast()->Add( SubLines.ExtractLast() );
			else
				delete SubLines.ExtractLast();
		}
		// Otherwise, if we are e.g. inside and want routes in the keep adding the end poitn of the line.
		else if (bInside == bRoutesInside)
		{
			NewRoutes.GetLast()->AddCopy(  m_Lines[i] );
		}

	}
	// Put all the new routes into the provided collection.
	Routes << NewRoutes;
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::GetRoutes<BR>
\brief Returns the routes inside or outside both polygons.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::GetRoutes(const C2DPolyBase& Poly1, bool bP1RoutesInside, 
			const C2DPolyBase& Poly2, bool bP2RoutesInside, 
			C2DLineBaseSetSet& Routes1, C2DLineBaseSetSet& Routes2)
{
		// Set up a collection of intersected points, and corresponding indexes.
		C2DPointSet IntPoints;
		CIndexSet Indexes1;
		CIndexSet Indexes2;
		// Use the line collections in each shape to find the intersections between them.
		Poly1.GetLines().GetIntersections( Poly2.GetLines(), &IntPoints, 
											&Indexes1, &Indexes2,
									&Poly1.GetBoundingRect(), &Poly2.GetBoundingRect());
		// Make a copy of the point set because this will be sorted by line index in the 
		// Get routes function later. We need an unsorted set for each polygon.
		C2DPointSet IntPointsCopy;
		IntPointsCopy.MakeCopy(IntPoints);

		// Find out whether the first poly starts inside the second.
		bool bP1StartInside = Poly2.Contains(  Poly1.GetLine(0)->GetPointFrom()  );
		// Find out if poly 2 starts inside poly 1.
		bool bP2StartInside = Poly1.Contains(  Poly2.GetLine(0)->GetPointFrom()  );	

		if (IntPoints.size() == 0 && !bP1StartInside && !bP2StartInside)
			return;	// No interaction between the 2.

		// Get the routes of poly 1 inside / outside the other, passing the unsorted
		// intersection points and polygon1 intersection indexes.
		Poly1.GetRoutes(IntPoints, Indexes1, Routes1, bP1StartInside, bP1RoutesInside);
		// Do the same for poly 2 but pass it the unsorted copy of the intersection points
		// So that they correspond to the indexes.
		Poly2.GetRoutes(IntPointsCopy, Indexes2, Routes2, bP2StartInside, bP2RoutesInside);


}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::RoutesToPolygons <BR>
\brief Function to convert routes to polygons by closing them and creating polygons from 
them.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::RoutesToPolygons(C2DPolyBaseSet& Polygons, C2DLineBaseSetSet& Routes)
{
	for (int i = Routes.size() - 1; i >= 0; i--)
	{
		Polygons.Add(new C2DPolyBase);
		Polygons.GetLast()->CreateDirect( *Routes.GetAt(i));
	}
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::SnapToGrid <BR>
\brief Snaps to the grid - see CGrid.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::SnapToGrid(void)
{
	m_Lines.SnapToGrid();
	m_LineRects.SnapToGrid();
	m_BoundingRect.SnapToGrid();
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::IsValidArcs <BR>
\brief IsValidArcs
<P>---------------------------------------------------------------------------*/
bool C2DPolyBase::IsValidArcs(void) const
{
	for (unsigned int i = 0; i < m_Lines.size(); i++)
	{
		const C2DLineBase* pLine = m_Lines.GetAt(i);
		if (pLine->GetType() == C2DBase::ArcedLine)
		{
			if (!dynamic_cast<const C2DArc*>(pLine)->IsValid())
				return false;
		}
	}
	return true;
}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::MakeValidArcs <BR>
\brief MakeValidArcs
<P>---------------------------------------------------------------------------*/
unsigned int C2DPolyBase::MakeValidArcs(void)
{
	unsigned int nResult = 0;

	for (unsigned int i = 0; i < m_Lines.size(); i++)
	{
		C2DLineBase* pLine = m_Lines.GetAt(i);
		if (pLine->GetType() == C2DBase::ArcedLine)
		{
			if (dynamic_cast<C2DArc*>(pLine)->MakeValid())
				pLine->GetBoundingRect(m_LineRects[i]);
		}
	}

	if (nResult > 0)
	{
		this->MakeBoundingRect();
	}

	return nResult;
}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::HasArcs <BR>
\brief HasArcs
<P>---------------------------------------------------------------------------*/
bool C2DPolyBase::HasArcs(void) const
{
	for (unsigned int i = 0; i < m_Lines.size(); i++)
	{
		const C2DLineBase* pLine = m_Lines.GetAt(i);
		if (pLine->GetType() == C2DBase::ArcedLine)
		{
			return true;
		}
	}

	return false;
}

 

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::RemoveNullLines <BR>
\brief Removes all lines whose end is the same is the start. Returns the number found.
<P>---------------------------------------------------------------------------*/
unsigned int C2DPolyBase::RemoveNullLines(void)
{
	unsigned int nResult = 0;

	unsigned int i = 0;
	while (i < m_Lines.size())
	{
		C2DLineBase* pLine = m_Lines.GetAt(i);
		C2DPoint p1 = pLine->GetPointFrom();
		C2DPoint p2 = pLine->GetPointTo();

		if (p1 == p2)
		{
			m_Lines.DeleteAt(i);
			m_LineRects.DeleteAt(i);
			nResult ++;
		}
		else
		{
			i++;
		}
	}

	if (nResult > 0)
	{
		this->MakeBoundingRect();
	}

	return nResult;
}



/**--------------------------------------------------------------------------<BR>
C2DPolyArc::GetArea <BR>
\brief Calculates the area of the shape by first calculating the area of the simple
polygon then adding or subtracting the segments defined by the arcs.
<P>---------------------------------------------------------------------------*/
double C2DPolyBase::GetArea(void) const
{
	return fabs(GetAreaSigned());
}

/**--------------------------------------------------------------------------<BR>
C2DPolyArc::GetAreaSigned <BR>
\brief Calculates the area of the shape by first calculating the area of the simple
polygon then adding or subtracting the segments defined by the arcs.
<P>---------------------------------------------------------------------------*/
double C2DPolyBase::GetAreaSigned(void) const
{
	double dArea = 0;

	for (unsigned int i = 0; i < m_Lines.size(); i++)
	{
		dArea += m_Lines[i].GetPointFrom().x  * m_Lines[i].GetPointTo().y - 
				 m_Lines[i].GetPointTo().x * m_Lines[i].GetPointFrom().y;
	}
	dArea = dArea / 2.0;

	for (unsigned int i = 0; i < m_Lines.size(); i++)	
	{
		if (GetLine(i)->GetType() == C2DBase::ArcedLine)
		{
			const C2DArc& Arc = dynamic_cast<const C2DArc&>(   m_Lines[i] );

			C2DSegment Seg( Arc );

			dArea += Seg.GetAreaSigned();
		}
	}
	
	return dArea;
}


/**--------------------------------------------------------------------------<BR>
C2DPolyArc::IsClockwise <BR>
\brief .
<P>---------------------------------------------------------------------------*/
bool C2DPolyBase::IsClockwise(void) const
{
	return (GetAreaSigned() < 0);
}



/**--------------------------------------------------------------------------<BR>
C2DPolyArc::MakeClockwise <BR>
\brief .
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::MakeClockwise(void)
{
	if (!IsClockwise())
		ReverseDirection();
}

/**--------------------------------------------------------------------------<BR>
C2DPolyArc::Transform <BR>
\brief Transform by the given operator.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::Transform(CTransformation* pProject)
{
	for (unsigned int i = 0; i < m_Lines.size(); i++)	
	{
		C2DLineBase* pLine = m_Lines.GetAt(i);
		pLine->Transform(pProject);
		pLine->GetBoundingRect(m_LineRects[i]);
	}

	this->MakeBoundingRect();

}

/**--------------------------------------------------------------------------<BR>
C2DPolyArc::InverseTransform <BR>
\brief Transform by the given operator.
<P>---------------------------------------------------------------------------*/
void C2DPolyBase::InverseTransform(CTransformation* pProject)
{
	for (unsigned int i = 0; i < m_Lines.size(); i++)	
	{
		C2DLineBase* pLine = m_Lines.GetAt(i);
		pLine->InverseTransform(pProject);
		pLine->GetBoundingRect(m_LineRects[i]);
	}

	this->MakeBoundingRect();
}
