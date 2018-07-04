/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DPolyArc.cpp
\brief Implementation file for the C2DPolyArc Class

Implementation file for C2DPolyArc, a class which represents a polygon with
both straight and curved lines. Inherits from the C2DPolyBase class which has
most of the functionality. As the user adds points, they are temporarily stored
before being used in the create function to build the polygon in the base class. 
After creation the temporary stores are deleted.
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "C2DPolyArc.h"
#include "C2DSegment.h"
#include "C2DHoledPolyBaseSet.h"
#include "C2DHoledPolyArcSet.h"
#include "C2DPolyArcSet.h"
#include "C2DPolygon.h"
#include "RandomNumber.h"
#include "C2DHoledPolyArc.h"


_MEMORY_POOL_IMPLEMENATION(C2DPolyArc)


/**--------------------------------------------------------------------------<BR>
C2DPolyArc::C2DPolyArc <BR>
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DPolyArc::C2DPolyArc(void)
{
	m_Type = PolyArc;
}


/**--------------------------------------------------------------------------<BR>
C2DPolyArc::C2DPolyArc <BR>
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DPolyArc::C2DPolyArc(const C2DPolyBase& Other) : C2DPolyBase(Other)
{
	m_Type = PolyArc;
}


/**--------------------------------------------------------------------------<BR>
C2DPolyArc::C2DPolyArc <BR>
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DPolyArc::C2DPolyArc(const C2DPolyArc& Other) : C2DPolyBase(Other)
{
	m_Type = PolyArc;
}


/**--------------------------------------------------------------------------<BR>
C2DPolyArc::~C2DPolyArc <BR>
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DPolyArc::~C2DPolyArc(void)
{
}

/**--------------------------------------------------------------------------<BR>
C2DPolyArc::Clear <BR>
\brief Clears the shape.
<P>---------------------------------------------------------------------------*/
void C2DPolyArc::Clear(void)
{
	C2DPolyBase::Clear();
}


/**--------------------------------------------------------------------------<BR>
C2DPolyArc::SetStartPoint <BR>
\brief Sets the start.
<P>---------------------------------------------------------------------------*/
void C2DPolyArc::SetStartPoint(const C2DPoint& Point)
{
	Clear();

	m_Lines << new C2DLine(Point, Point);
}

/**--------------------------------------------------------------------------<BR>
C2DPolyArc::LineTo <BR>
\brief  Radius of the arc from the PREVIOUS point to the new one.
bCentreOnRight indicates whether the centre of the arc is to the right of the line. 
bArcOnRight indicates whether the curve is to the right i.e. anti-clockwise.
<P>---------------------------------------------------------------------------*/
void C2DPolyArc::LineTo(const C2DPoint& Point, double dRadius,
	bool bCentreOnRight, bool bArcOnRight)
{
	if (m_Lines.size() == 0)
		return;

	C2DArc* pLine = new C2DArc( m_Lines.GetLast()->GetPointTo(), Point, 
								dRadius, bCentreOnRight, bArcOnRight);

	if (m_Lines.size() == 1 && m_Lines[0].GetType() == C2DBase::StraightLine &&
		m_Lines[0].GetPointTo() == m_Lines[0].GetPointFrom())
	{
		m_Lines.DeleteAndSet( 0 , pLine);
	}
	else
	{
		m_Lines << pLine;
	}
}

/**--------------------------------------------------------------------------<BR>
C2DPolyArc::LineTo <BR>
\brief Adds a point which is a striaght line from the previous.
<P>---------------------------------------------------------------------------*/
void C2DPolyArc::LineTo(const C2DPoint& Point)
{
	if (m_Lines.size() == 0)
		return;

	C2DLine* pLine = new C2DLine( m_Lines.GetLast()->GetPointTo(), Point );

	if (m_Lines.size() == 1 && m_Lines[0].GetType() == C2DBase::StraightLine &&
		m_Lines[0].GetPointTo() == m_Lines[0].GetPointFrom())
	{
		m_Lines.DeleteAndSet( 0 , pLine);
	}
	else
	{
		m_Lines << pLine;
	}
}


/**--------------------------------------------------------------------------<BR>
C2DPolyArc::Close <BR>
\brief Close with a curved line.
<P>---------------------------------------------------------------------------*/
void C2DPolyArc::Close(double dRadius, bool bCentreOnRight, bool bArcOnRight)
{
	if (m_Lines.size() == 0)
		return;

	C2DPoint pt1(m_Lines.GetLast()->GetPointTo());
	C2DPoint pt2(m_Lines[0].GetPointFrom());

	if (pt1 != pt2)
		m_Lines << new C2DArc(pt1, pt2, dRadius, bCentreOnRight, bArcOnRight);

	MakeLineRects();
	MakeBoundingRect();
}

/**--------------------------------------------------------------------------<BR>
C2DPolyArc::Close <BR>
\brief Close with a straight line
<P>---------------------------------------------------------------------------*/
void C2DPolyArc::Close(void)
{
	if (m_Lines.size() == 0)
		return;

	C2DPoint pt1(m_Lines.GetLast()->GetPointTo());
	C2DPoint pt2(m_Lines[0].GetPointFrom());

	if (pt1 != pt2)
		m_Lines << new C2DLine(pt1, pt2);

	MakeLineRects();
	MakeBoundingRect();
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::CreateRandom <BR>
\brief .
<P>---------------------------------------------------------------------------*/
bool C2DPolyArc::CreateRandom(const C2DRect& cBoundary, int nMinPoints, int nMaxPoints)
{
	C2DPolygon Poly;
	if (!Poly.CreateRandom(cBoundary, nMinPoints, nMaxPoints))
		return false;

	CRandomNumber rCenOnRight(0, 1);

	this->Set( Poly );

	for (unsigned int i = 0 ; i < m_Lines.size(); i ++)
	{
		C2DLineBase* pLine = m_Lines.GetAt(i);

		bool bCenOnRight = (rCenOnRight.GetInt() > 0 );
		double dLength = pLine->GetLength();
		CRandomNumber Radius(dLength , dLength * 3);


		C2DArc* pNew = new C2DArc( pLine->GetPointFrom(), pLine->GetPointTo(), 
							Radius.Get(), bCenOnRight, !bCenOnRight);

		if (this->Crosses( *pNew ))
		{
			delete pNew;
		}
		else
		{
			m_Lines.DeleteAndSet( i, pNew );
			m_Lines[i].GetBoundingRect( m_LineRects[i] );
		}
	}

	this->MakeBoundingRect();

	return true;
}





/**--------------------------------------------------------------------------<BR>
C2DPolyArc::GetCentroid <BR>
\brief Calculates the centroid of the shape by calculating the centroid of the simple
polygon then shifting it by the area weighted centroids of the segments defined
by the arcs.
<P>---------------------------------------------------------------------------*/
C2DPoint C2DPolyArc::GetCentroid(void) const
{
	// Find the centroid and area of the straight line polygon.
	C2DPoint Centroid(0, 0);
	C2DPoint pti;
	C2DPoint ptii;
	double dArea = 0;

	for (unsigned int i = 0; i < m_Lines.size(); i++)
	{
		pti = m_Lines[i].GetPointFrom();
		ptii = m_Lines[i].GetPointTo();

		Centroid.x += (pti.x + ptii.x) * (pti.x * ptii.y - ptii.x * pti.y);
		Centroid.y += (pti.y + ptii.y) * (pti.x * ptii.y - ptii.x * pti.y);

		dArea += pti.x * ptii.y - ptii.x * pti.y;
	}
	dArea = dArea / 2.0;

	Centroid.x = Centroid.x / (6.0 * dArea);
	Centroid.y = Centroid.y / (6.0 * dArea);

	std::vector<double> dSegAreas;
	double dTotalArea = dArea;
	std::vector<C2DPoint> SegCentroids;

	for (unsigned int i = 0; i < m_Lines.size(); i++)
	{
		if (m_Lines[i].GetType() == C2DLineBase::ArcedLine)
		{
			C2DSegment Seg( dynamic_cast<const C2DArc&>( m_Lines[i] ) );
			double dSegArea = Seg.GetAreaSigned();
			dTotalArea += dSegArea;
			dSegAreas.push_back( dSegArea );
			SegCentroids.push_back( Seg.GetCentroid() );
		}
	}

	Centroid = Centroid * dArea;

	for (unsigned int i = 0; i < dSegAreas.size(); i++)
	{
		Centroid += SegCentroids[i] * dSegAreas[i];
	}

	Centroid = Centroid / dTotalArea;

	return Centroid;

}

/**--------------------------------------------------------------------------<BR>
C2DPolyArc::RotateToRight <BR>
\brief Rotates the polygon to the right around the origin given. Calls base class.
<P>---------------------------------------------------------------------------*/
void C2DPolyArc::RotateToRight(double dAng, const C2DPoint& Origin)
{
	C2DPolyBase::RotateToRight( dAng, Origin);
}


/**--------------------------------------------------------------------------<BR>
C2DPolyArc::RotateToRight <BR>
\brief Rotates the polygon to the right around the centroid.
<P>---------------------------------------------------------------------------*/
void C2DPolyArc::RotateToRight(double dAng)
{
	C2DPolyBase::RotateToRight( dAng, GetCentroid());
}


/**--------------------------------------------------------------------------<BR>
C2DPolyArc::GetNonOverlaps <BR>
\brief Calls the base class to obtain the parts of this that are not overlapping with
the other.
<P>---------------------------------------------------------------------------*/
void C2DPolyArc::GetNonOverlaps(const C2DPolyArc& Other, C2DHoledPolyArcSet& HoledPolygons, 
									CGrid::eDegenerateHandling eDegen ) const 
{
	C2DHoledPolyBaseSet BaseSet;

	C2DPolyBase::GetNonOverlaps( Other, BaseSet, eDegen);

	for (unsigned int i = 0 ; i < BaseSet.size(); i++)
	{
		HoledPolygons.Add(new C2DHoledPolyArc(BaseSet[i]));
	}

}

/**--------------------------------------------------------------------------<BR>
C2DPolyArc::GetNonOverlaps <BR>
\brief Calls the base class to obtain the parts of this that are not overlapping with
the other.
<P>---------------------------------------------------------------------------*/
void C2DPolyArc::GetNonOverlaps(const C2DPolyArc& Other, C2DHoledPolyBaseSet& HoledPolygons, 
									CGrid::eDegenerateHandling eDegen ) const 
{
	C2DPolyBase::GetNonOverlaps( Other, HoledPolygons, eDegen);
}



/**--------------------------------------------------------------------------<BR>
C2DPolyArc::GetUnion <BR>
\brief Calls the base class to obtain the union of this with the other.
<P>---------------------------------------------------------------------------*/
void C2DPolyArc::GetUnion(const C2DPolyArc& Other, C2DHoledPolyArcSet& HoledPolygons,
						  CGrid::eDegenerateHandling eDegen) const 
{
	C2DHoledPolyBaseSet BaseSet;

	C2DPolyBase::GetUnion( Other, BaseSet, eDegen);

	for (unsigned int i = 0 ; i < BaseSet.size(); i++)
	{
		HoledPolygons.Add(new C2DHoledPolyArc(BaseSet[i]));
	}
}

/**--------------------------------------------------------------------------<BR>
C2DPolyArc::GetUnion <BR>
\brief Calls the base class to obtain the union of this with the other.
<P>---------------------------------------------------------------------------*/
void C2DPolyArc::GetUnion(const C2DPolyArc& Other, C2DHoledPolyBaseSet& HoledPolygons,
									CGrid::eDegenerateHandling eDegen) const 
{
	C2DPolyBase::GetUnion( Other, HoledPolygons, eDegen);
}


/**--------------------------------------------------------------------------<BR>
C2DPolyArc::GetOverlaps <BR>
\brief Calls the base class to obtain the parts of this that are overlapping with
the other.
<P>---------------------------------------------------------------------------*/
void C2DPolyArc::GetOverlaps(const C2DPolyArc& Other, C2DHoledPolyArcSet& Polygons,
									CGrid::eDegenerateHandling eDegen ) const 
{

	C2DHoledPolyBaseSet BaseSet;

	C2DPolyBase::GetUnion( Other, BaseSet, eDegen);

	for (unsigned int i = 0 ; i < BaseSet.size(); i++)
	{
		Polygons.Add(new C2DHoledPolyArc(BaseSet[i]));
	}
}

/**--------------------------------------------------------------------------<BR>
C2DPolyArc::GetOverlaps <BR>
\brief Calls the base class to obtain the parts of this that are overlapping with
the other.
<P>---------------------------------------------------------------------------*/
void C2DPolyArc::GetOverlaps(const C2DPolyArc& Other, C2DHoledPolyBaseSet& Polygons,
									CGrid::eDegenerateHandling eDegen ) const 
{
	C2DPolyBase::GetOverlaps( Other, Polygons, eDegen);
}

