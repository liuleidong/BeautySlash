/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DPolygon.cpp
\brief Implementation file for the C2DPolygon Class

Implementation file for C2DPolygon, a class which represents a polygon. Can be 
convex or concave but doesn't contain holes.
<P>---------------------------------------------------------------------------*/


#include "StdAfx.h"
#include "C2DPolygon.h"
#include "Sort.h"
#include "RandomNumber.h"
#include "C2DHoledPolygon.h"
#include "TravellingSalesman.h"
#include "C2DTriangle.h"
#include "C2DPolygonSet.h"
#include "C2DHoledPolygonSet.h"
#include "C2DHoledPolyBaseSet.h"
#include "C2DRoute.h"
#include "Interval.h"
#include "C2DLine.h"

_MEMORY_POOL_IMPLEMENATION(C2DPolygon)

/**--------------------------------------------------------------------------<BR>
C2DPolygon::C2DPolygon <BR>
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DPolygon::C2DPolygon(void) 
{
	m_SubArea[0] = 0;
	m_SubArea[1] = 0;

	m_Type = PolyLine;

	Clear();
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::C2DPolygon <BR>
\brief Copy constructor.
<P>---------------------------------------------------------------------------*/
C2DPolygon::C2DPolygon(const C2DPolygon& Other)
{
	m_SubArea[0] = 0;
	m_SubArea[1] = 0;

	m_Type = PolyLine;

	*this = Other;
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::C2DPolygon <BR>
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DPolygon::C2DPolygon(const C2DPointSet& Points, bool bReorderIfNeeded)
{
	m_SubArea[0] = 0;
	m_SubArea[1] = 0;

	m_Type = PolyLine;

	Create(Points, bReorderIfNeeded);
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::C2DPolygon <BR>
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DPolygon::C2DPolygon(const C2DPoint* pPoint, unsigned int nNumber, 
					   bool bReorderIfNeeded)
{
	m_SubArea[0] = 0;
	m_SubArea[1] = 0;

	m_Type = PolyLine;

	Create(pPoint,nNumber , bReorderIfNeeded);
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::C2DPolygon <BR>
\brief Copy constructor.
<P>---------------------------------------------------------------------------*/
C2DPolygon::C2DPolygon(const C2DPolyBase& Other) : C2DPolyBase(Other)
{
	m_SubArea[0] = 0;
	m_SubArea[1] = 0;

	m_Type = PolyLine;
}

const C2DLine* C2DPolygon::GetLine(unsigned int i) const
{ 
	return dynamic_cast<const C2DLine*> (C2DPolyBase::GetLine(i));
}





/**--------------------------------------------------------------------------<BR>
C2DPolygon::Create <BR>
\brief Copies the points from the route.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::Create(const C2DRoute& Route)
{
	Clear();

	int nCount = Route.GetPointsCount();

	if (nCount == 0 ) 
		return false;

	C2DPointSet Points;

	for (int i = 0 ; i < nCount; i++)
		Points.AddCopy(Route.GetPoint(i));

	if (Points[0] == Points[nCount - 1])
		Points.DeleteAt(nCount - 1);

	return Create(Points);
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::CreateRandom <BR>
\brief Creates a random polygon.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::CreateRandom(const C2DRect& cBoundary, int nMinPoints, int nMaxPoints)
{
	Clear();

	assert(nMinPoints <= nMaxPoints);

	if (nMinPoints < 3) 
		return false;
	if (nMinPoints > nMaxPoints)
		return false;

	int nNumber = static_cast<int>(nMinPoints + (float) rand() / (float)RAND_MAX 
		* (float)(nMaxPoints - nMinPoints) + 0.5);

	C2DPoint pt;
	CRandomNumber rnX(cBoundary.GetTopLeft().x, cBoundary.GetBottomRight().x);
	CRandomNumber rnY(cBoundary.GetBottomRight().y, cBoundary.GetTopLeft().y);

	C2DPointSet Points;

	for (int i = 0 ; i < nNumber; i++)
	{
		pt.x = rnX.Get();
		pt.y = rnY.Get();

		Points.AddCopy(pt);
	}

	return Create(Points, true);
}



/**--------------------------------------------------------------------------<BR>
C2DPolygon::~C2DPolygon <BR>
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DPolygon::~C2DPolygon(void)
{
	Clear();
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::Clear <BR>
\brief Clears all.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::Clear(void)
{
	for (int i = 0; i < MAX_SUB_AREAS ; i ++)
	{
		if (m_SubArea[i] != 0) delete m_SubArea[i];
		m_SubArea[i] = 0;
	}

	C2DPolyBase::Clear();
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::operator = <BR>
\brief Assignment.
<P>---------------------------------------------------------------------------*/
const C2DPolygon& C2DPolygon::operator=(const C2DPolygon& Other)
{
	Clear();

	const C2DPolygon* pSubArea = 0;
	for (int i = 0; i < MAX_SUB_AREAS; i++)
	{
		pSubArea = Other.GetSubArea(i);
		if (pSubArea != 0)
		{
			m_SubArea[i] = new C2DPolygon;
			(*m_SubArea[i]) = (*pSubArea);
		}
	}

	C2DPolyBase::Set(static_cast< const C2DPolyBase& >(Other));

	return *this;
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::GetSubArea <BR>
\brief Returns the sub area if created
<P>---------------------------------------------------------------------------*/
const C2DPolygon* C2DPolygon::GetSubArea(int nIndex) const
{
	if (nIndex < 0 || nIndex >= MAX_SUB_AREAS)
		return 0;

	return m_SubArea[nIndex];
}





/**--------------------------------------------------------------------------<BR>
C2DPolygon::Contains <BR>
\brief True if the point is inside. 
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::Contains(const C2DPoint& pt) const
{
	return C2DPolyBase::Contains(pt);
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::Contains <BR>
\brief True if the other is inside.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::Contains(const C2DPolygon& Other) const
{
	return C2DPolyBase::Contains(
		static_cast<const C2DPolyBase&>(Other));
	
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::Contains <BR>
\brief True if the line is inside this.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::Contains(const C2DLine& Line) const
{
	return C2DPolyBase::Contains(Line);
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::Contains <BR>
\brief True if the line is inside this.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::Contains(const C2DLineBase& Line) const
{
	return C2DPolyBase::Contains(Line);
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::GetPoint <BR>
\brief Returns the point specified by the index. Cyclic.
<P>---------------------------------------------------------------------------*/
const C2DPoint* C2DPolygon::GetPoint(unsigned int nPointIndex) const
{
	unsigned int nLines = m_Lines.size();

	if (nLines == 0)
		return 0;

	const C2DLineBase* pLineBase = m_Lines.GetAt(nPointIndex  %  nLines);

	if (pLineBase != 0 && pLineBase->GetType() == C2DBase::StraightLine)
	{
		const C2DLine* pLine = dynamic_cast<const C2DLine*> (pLineBase);
		return &pLine->point;
	}
	else
	{	
		return 0;
	}	
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::IsPointInflected <BR>
\brief True if the point is inflected.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::IsPointInflected(unsigned int nIndex) const
{
	unsigned int usBefore;
	if (nIndex == 0)
		usBefore = m_Lines.size() - 1;
	else
		usBefore = nIndex - 1;

	C2DLine TestLine(*GetPoint(usBefore), *GetPoint(nIndex));

	assert(IsClockwise());

	return !TestLine.IsOnRight(*GetPoint(nIndex + 1 ) );
}



/**--------------------------------------------------------------------------<BR>
C2DPolygon::RemovesInflection <BR>
\brief True if joining the 2 points removes the inflection.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::RemovesInflection(unsigned int nStart, unsigned int nEnd)
{
	unsigned int usBefore;
	if (nStart == 0)
		usBefore = m_Lines.size() - 1;
	else
		usBefore = nStart - 1;

	C2DLine Line1(*GetPoint(usBefore), *GetPoint(nStart));
	C2DLine Line2(*GetPoint(nStart), *GetPoint(nStart + 1));

	assert(IsClockwise());

	return (Line1.IsOnRight(*GetPoint(nEnd)) &&
			Line2.IsOnRight(*GetPoint(nEnd)));
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::FindFirstInflection <BR>
\brief Finds the first inflected point. Returns -1 if there are none.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::FindFirstInflection(unsigned int& nFirstInflection)  const
{
	for (unsigned int i = 0; i < m_Lines.size(); i++)
	{
		if(IsPointInflected(i))
		{
			nFirstInflection = i;
			return true;
		}
	}
	return false;
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::CanPointsBeJoined <BR>
\brief True if the 2 point given by the indexes can be joined together without crossing
any other existing line and such that the new line is entirely within the area.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::CanPointsBeJoined(unsigned int nStart, unsigned int nEnd)
{
	unsigned int usBefore;
	if (nStart == 0)
		usBefore = m_Lines.size() - 1;
	else
		usBefore = nStart - 1;


	C2DVector VecBefore(*GetPoint(nStart), *GetPoint(usBefore)); 
	C2DVector VecAfter(*GetPoint(nStart), *GetPoint(nStart + 1)); 

	C2DLine TestLine( *GetPoint(nStart), *GetPoint(nEnd));

	assert(IsClockwise());

	if( VecAfter.AngleToRight(TestLine.vector) < 
	VecAfter.AngleToRight(VecBefore))
	{
		TestLine.GrowFromCentre(0.99999);
		if (!this->Crosses(TestLine)) return true;
	}

	return false;
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::CreateRegular <BR>
\brief Creates a regular polygon.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::CreateRegular(const C2DPoint& Centre, double dDistanceToPoints, int nNumberSides)
{
	Clear();

	if (dDistanceToPoints == 0 || nNumberSides < 3) return false;

	double dAngle =  conTWOPI / nNumberSides ;
	C2DVector Vector( 0 , dDistanceToPoints);
	C2DLine LineToEachPt(Centre, Vector);

	C2DPointSet Points;

	for (int i = 0 ; i < nNumberSides; i ++)
	{
		C2DPoint* pNewPt = new C2DPoint;
		(*pNewPt) = LineToEachPt.GetPointTo();
		Points.Add(pNewPt);
		LineToEachPt.vector.TurnRight(dAngle);
	}

	return Create(Points);
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::CreateConvexHull <BR>
\brief Creates a convex hull from the other polygon. Uses Graham's algorithm.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::CreateConvexHull(const C2DPolygon& Other)
{
	Clear();

	C2DPointSet Points;
	Other.GetPointsCopy(Points);

	C2DPointSet Hull;
	Hull.ExtractConvexHull(Points);

	return Create(Hull);
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::GetLeftMostPoint <BR>
\brief Returns the left most point.
<P>---------------------------------------------------------------------------*/
int C2DPolygon::GetLeftMostPoint(void) const 
{
	if (m_Lines.size() < 2) return 0;
	
	int nRes = 0;

	for (unsigned int i = 1 ; i < m_Lines.size(); i++)
	{
		if (m_Lines[i].GetPointFrom().x < m_Lines[nRes].GetPointFrom().x)
			nRes = i;
	}

	return nRes;
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::GetPointsCopy <BR>
\brief Returns a copy of the points used.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::GetPointsCopy(C2DPointSet& PointCopy) const
{
	for (unsigned int i = 0; i < m_Lines.size();i++)
	{
		PointCopy.AddCopy(m_Lines[i].GetPointFrom());
	}
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::CreateMorph <BR>
\brief Morphs this into another by the specified factor (0-1).
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::CreateMorph(const C2DPolygon& OtherFrom, const C2DPolygon& OtherTo, double dFactor)
{
	unsigned int nOtherFromCount = OtherFrom.GetPointsCount();
	unsigned int nOtherToCount = OtherTo.GetPointsCount();

	if (nOtherToCount < 3 || nOtherFromCount < 3)
		return false;

	if (nOtherFromCount > nOtherToCount)
	{
		return CreateMorph(OtherTo, OtherFrom, 1 - dFactor);
	}
	else
	{
		// Going from poly with less points to poly with more.
		C2DPointSet Points;

		unsigned int nOtherFromLeft = OtherFrom.GetLeftMostPoint();
		// Add the OtherFroms points starting from the left most.
		for (unsigned int i = 0; i < OtherFrom.GetPointsCount(); i++)
		{
			C2DPoint* pNewPoint = new C2DPoint;
			*pNewPoint = *OtherFrom.GetPoint(i + nOtherFromLeft);
			Points.Add(pNewPoint);
		}

		unsigned int nPointsToAdd = nOtherToCount - nOtherFromCount; // we know this is positive.

		unsigned int nPointsAdded = 0;
		unsigned int nLine = 0;
		
		// Add points to the list so that it is the same size as OtherTo.
		while (nPointsAdded < nPointsToAdd)
		{
			C2DLine TempLine( *Points.GetAt(nLine), *Points.GetAt(nLine + 1));

			C2DPoint* pNewPoint = new C2DPoint;
			(*pNewPoint) = TempLine.GetMidPoint();
			Points.InsertAt(nLine + 1, pNewPoint);
			nLine +=2;
			nPointsAdded ++;
			if (nLine > Points.size() -2 )
				nLine = 0;
		}

		unsigned int nOtherToLeft = OtherTo.GetLeftMostPoint();

		assert(Points.size() == nOtherToCount);

		for (unsigned int i = 0 ; i < nOtherToCount ; i++ )
		{
			C2DVector vMove(*Points.GetAt(i) , *OtherTo.GetPoint(nOtherToLeft + i) );
			vMove = vMove * dFactor;

			Points.GetAt(i)->Move(vMove);
		}

		return Create(Points);
	}
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::Create <BR>
\brief Creates from the points given.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::Create(const C2DPoint* pPoint, unsigned int nNumber, 
						bool bReorderIfNeeded)
{

	C2DPointSet pts;

	unsigned int nIndex = 0;

	while (nIndex < nNumber)
	{
		pts.AddCopy( *(pPoint + nIndex) );
		nIndex ++ ;
	}


	return Create(pts, bReorderIfNeeded);
}



bool C2DPolygon::Create(const double* pPoint, unsigned int nNumber)
{
	for (int i = 0; i < MAX_SUB_AREAS ; i ++)
	{
		if (m_SubArea[i] != 0) 
			delete m_SubArea[i];
		m_SubArea[i] = 0;
	}

	m_Lines.DeleteAll();

	m_LineRects.DeleteAll();

	for (unsigned int i = 0; i < nNumber; i++)
	{
		C2DLine* pLine = new C2DLine;
		// Index is double - 2 for every pt
		unsigned int nIndex = i * 2;
		pLine->point.x = pPoint[nIndex];
		pLine->point.y = pPoint[nIndex + 1];

		// Move to the next point
		nIndex += 2;
		// Make sure we don't overrun.
		nIndex %= (nNumber * 2);
		pLine->vector.i = pPoint[nIndex] - pLine->point.x;
		pLine->vector.j = pPoint[nIndex + 1] - pLine->point.y;

		C2DRect* pRect = new C2DRect();

		pLine->GetBoundingRect(*pRect);

		m_Lines.Add(pLine);

		m_LineRects.Add(pRect);
	}

	MakeBoundingRect();

	return true;
}



/**--------------------------------------------------------------------------<BR>
C2DPolygon::Create <BR>
\brief Main function which reorders the points if there are crossing lines 
(or not if not specified) then ensures the points are ordered clockwise. 
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::Create(const C2DPointSet& Points, bool bReorderIfNeeded)
{
	for (int i = 0; i < MAX_SUB_AREAS ; i ++)
	{
		if (m_SubArea[i] != 0) 
			delete m_SubArea[i];
		m_SubArea[i] = 0;
	}

	if (Points.size() < 3)
		return false;

	MakeLines(Points);

	MakeLineRects();

	if (!IsClockwise())
		ReverseDirection();

	MakeBoundingRect();

#if 0
	if (bReorderIfNeeded && HasCrossingLines())
        return Reorder();
#endif

    if (bReorderIfNeeded)
        return Reorder();

	return true;
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::ClearConvexSubAreas <BR>
\brief Clears the convex sub areas.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::ClearConvexSubAreas(void)
{
	if (m_SubArea[0] != 0 && m_SubArea[1] != 0)
	{
		delete m_SubArea[0];
		delete m_SubArea[1];
		m_SubArea[0] = 0;
		m_SubArea[1] = 0;
	}
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::GetConvexSubAreas <BR>
\brief Returns an array of convex sub areas if they have been created.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::GetConvexSubAreas(C2DPolygonSet& SubAreas) const
{

	if (m_SubArea[0] != 0 && m_SubArea[1] != 0)
	{
		m_SubArea[0]->GetConvexSubAreas(SubAreas);
		m_SubArea[1]->GetConvexSubAreas(SubAreas);
	}
	else
	{
		SubAreas.Add(new C2DPolygon(*this));
	}
}




/**--------------------------------------------------------------------------<BR>
C2DPolygon::CreateConvexSubAreas <BR>
\brief Creates convex sub areas by joining inflection if possible or joining
inflected points with other points that will remove the inflection.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::CreateConvexSubAreas(void)
{
	if (m_SubArea[0] != 0)
	{
		delete m_SubArea[0];
		m_SubArea[0] = 0;
	}
	if ( m_SubArea[1] != 0)
	{
		delete m_SubArea[1];
		m_SubArea[1] = 0;
	}

	unsigned int nLineCount = m_Lines.size();

	if ( nLineCount < 4 )
		return true;

	bool bInflection = false;
	for (unsigned int nStart = 0 ; nStart < nLineCount; nStart++)
	{
		if (IsPointInflected(nStart))
		{
			bInflection = true;

			unsigned int nEnd = nStart + 2;
			bool bContinue = true;
			while (bContinue)
			{
				if (C2DTriangle::IsClockwise(*GetPoint(nEnd - 2),*GetPoint(nEnd - 1),*GetPoint(nEnd))
					&& C2DTriangle::IsClockwise( *GetPoint(nEnd - 1), *GetPoint(nEnd), *GetPoint(nStart))
					&& C2DTriangle::IsClockwise( *GetPoint(nEnd), *GetPoint(nStart), *GetPoint(nStart + 1)) 
					&& CanPointsBeJoined(nStart, nEnd))
				{
					nEnd++;
				}
				else
				{
					nEnd--;
					bContinue = false;
				}
			}
			if (nEnd >= nStart + 2)
			{
				bool bRes = CreateSubAreas(nStart, nEnd, m_SubArea[0], m_SubArea[1]);
				bRes &= m_SubArea[0]->CreateConvexSubAreas();
				bRes &= m_SubArea[1]->CreateConvexSubAreas();
				return bRes;
			}
		}
	}

	if (!bInflection)
		return true;

	for (unsigned int nStart = 2 * nLineCount - 1 ; nStart >= nLineCount; nStart--)
	{
		if (IsPointInflected(nStart))
		{
			bInflection = true;

			unsigned int nEnd = nStart - 2;
			bool bContinue = true;
			while (bContinue)
			{
				if (!C2DTriangle::IsClockwise(*GetPoint(nEnd + 2),*GetPoint(nEnd + 1),*GetPoint(nEnd)) 
					&& !C2DTriangle::IsClockwise( *GetPoint(nEnd + 1), *GetPoint(nEnd), *GetPoint(nStart)) 
					&& !C2DTriangle::IsClockwise( *GetPoint(nEnd), *GetPoint(nStart), *GetPoint(nStart - 1)) 
					&& CanPointsBeJoined(nStart, nEnd))
				{
					nEnd--;
				}
				else
				{
					nEnd++;
					bContinue = false;
				}
			}
			if (nEnd <= nStart - 2)
			{
				bool bRes = CreateSubAreas(nStart, nEnd, m_SubArea[0], m_SubArea[1]);
				bRes &= m_SubArea[0]->CreateConvexSubAreas();
				bRes &= m_SubArea[1]->CreateConvexSubAreas();
				return bRes;
			}
		}
	}


	assert(0);
	return false;
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::RotateToRight <BR>
\brief Rotates to the right around the origin given.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::RotateToRight(double dAng, const C2DPoint& Origin)
{
	if (m_SubArea[0] != 0 && m_SubArea[1] != 0)
	{
		m_SubArea[0]->RotateToRight( dAng, Origin);
		m_SubArea[1]->RotateToRight( dAng, Origin);
	}

	C2DPolyBase::RotateToRight( dAng, Origin);
}



/**--------------------------------------------------------------------------<BR>
C2DPolygon::RotateToRight <BR>
\brief Rotates to the right around the centroid.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::RotateToRight(double dAng)
{
	RotateToRight(dAng, GetCentroid());
}



/**--------------------------------------------------------------------------<BR>
C2DPolygon::GetCentroid <BR>
\brief Returns the centroid.
<P>---------------------------------------------------------------------------*/
const C2DPoint C2DPolygon::GetCentroid(void) const
{
	C2DPoint Centroid(0, 0);
	C2DPoint pti;
	C2DPoint ptii;
	double dArea = 0;

	for (unsigned int i = 0; i < m_Lines.size(); i++)
	{
		pti = *GetPoint(i);
		ptii = *GetPoint(i+1);

		Centroid.x += (pti.x + ptii.x) * (pti.x * ptii.y - ptii.x * pti.y);
		Centroid.y += (pti.y + ptii.y) * (pti.x * ptii.y - ptii.x * pti.y);

		dArea += pti.x * ptii.y - ptii.x * pti.y;
	}
	dArea = dArea / 2.0;

	Centroid.x = Centroid.x / (6.0 * dArea);
	Centroid.y = Centroid.y / (6.0 * dArea);

	return Centroid;
}



/**--------------------------------------------------------------------------<BR>
C2DPolygon::GetArea <BR>
\brief Returns the area signed.
<P>---------------------------------------------------------------------------*/
double C2DPolygon::GetAreaSigned()  const
{
//	double dArea = 0;

//	for (unsigned int i = 0; i < m_Lines.size(); i++)
	//{
//		dArea += GetPoint(i)->x * GetPoint(i + 1)->y - GetPoint(i + 1)->x * GetPoint(i)->y;
//	}
//	dArea = dArea / 2.0;

//	return dArea;


	double dArea = 0;

	unsigned int nCount = m_Lines.size();

	if (nCount == 0)
		return dArea;

	unsigned int nLast = nCount - 1;


	const C2DLine* pFirst = dynamic_cast<const C2DLine*>(m_Lines.GetAt(0));
	const C2DLine* p1 = pFirst;
	const C2DLine* p2 = 0;

	for (unsigned int i = 1; i <= nCount; i++)
	{
		if (i == nCount)
		{
			p2 = pFirst;
		}
		else
		{
			p2 = dynamic_cast<const C2DLine*>(m_Lines.GetAt((i)));
		}

		dArea += p1->point.x * p2->point.y - p2->point.x * p1->point.y;

		p1 = p2;
	}
	dArea = dArea / 2.0;

	return dArea;
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::CreateSubAreas <BR>
\brief Create 2 sub areas by splitting the points according to the indexes given.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::CreateSubAreas(unsigned int nPt1, unsigned int nPt2, C2DPolygon*& pNewArea1, C2DPolygon*& pNewArea2)
{
	for (int i = 0; i < MAX_SUB_AREAS ; i ++)
	{
		if (m_SubArea[i] != 0) delete m_SubArea[i];
		m_SubArea[i] = 0;
	}

	pNewArea1 = new C2DPolygon;
	pNewArea2 = new C2DPolygon;

	while (nPt2 < nPt1) nPt2 += m_Lines.size();

	C2DPointSet Points1;
	for (unsigned int i = nPt1; i <= nPt2; i++)
	{
		Points1.AddCopy(*GetPoint(i));
	}
	bool bRes1 = pNewArea1->Create(Points1);

	while (nPt1 < nPt2) nPt1 += m_Lines.size();

	C2DPointSet Points2;
	for (unsigned int j = nPt2; j <= nPt1; j++)
	{
		Points2.AddCopy(*GetPoint(j));
	}
	bool bRes2 = pNewArea2->Create(Points2);

	return bRes1 && bRes2;
}



/**--------------------------------------------------------------------------<BR>
C2DPolygon::CrossesRay <BR>
\brief True if the ray crosses the polygon, records the points in order.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::CrossesRay(const C2DLine& Ray, C2DPointSet* IntersectionPts) const
{
	return C2DPolyBase::CrossesRay(Ray, IntersectionPts);

}




/**--------------------------------------------------------------------------<BR>
C2DPolygon::Crosses <BR>
\brief True if a given lines crosses any line in the polygon.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::Crosses(const C2DLineBase& Line) const
{
	return C2DPolyBase::Crosses(Line);
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::Crosses <BR>
\brief True if a given lines crosses any line in the polygon. Records the intersection
points in order.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::Crosses(const C2DLineBase& Line, C2DPointSet* IntersectionPts) const
{
    return C2DPolyBase::Crosses(Line, IntersectionPts);
}

bool C2DPolygon::Crosses(const C2DLineBase &Line, C2DPointSet *IntersectionPts, C2DLineBaseSet *IntersectionLines) const
{
    return C2DPolyBase::Crosses(Line, IntersectionPts, IntersectionLines);
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::IsConvex <BR>
\brief True if it is convex i.e. no inflected points.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::IsConvex(void) const
{
	if (m_Lines.size() < 4) return true;
	unsigned int nTemp;
	return !FindFirstInflection(nTemp);
}




/**--------------------------------------------------------------------------<BR>
C2DPolygon::Reorder <BR>
\brief Reorders the points to minimise the perimter and to remove crossing lines.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::Reorder(void)
{
	if (m_Lines.size() < 4 )
		return true;
	// Get a copy of the points.
	C2DPointSet Points;
	GetPointsCopy(Points);

	// Make a convex hull from them.
	C2DPointSet Hull;
	Hull.ExtractConvexHull(Points);
	// Close the hull.
	Hull.AddCopy(Hull[0]);
	// Get the bounding rect for the hull and sort the rest by the distance from it.
	C2DRect Rect;
	Hull.GetBoundingRect(Rect);
	Points.SortByDistance(Rect.GetCentre(), false);

	// Set up the travelling saleman and give him the route i.e. the now closed hull.
	CTravellingSalesman TS;
	TS.SetPointsDirect(Hull);

	// Insert the rest starting with the closest (hopefully).
	while (Points.size() > 0)
		TS.InsertOptimally(Points.ExtractLast());

	// Refine the TS.
	TS.Refine();

	// Get the points back
	TS.ExtractPoints(Points);

	// Remove the closure.
	Points.DeleteLast();

	// Make the lines again.
	MakeLines(Points);
	// Make the rectangles again.
	MakeLineRects();
	// Reverse direction if needed.
	if (!IsClockwise())
		ReverseDirection();
	// Remake the bounding rectangle.
	MakeBoundingRect();
	// Eliminate crossing lines.
	if (!EliminateCrossingLines())
		return false;

	return true;
}



/**--------------------------------------------------------------------------<BR>
C2DPolygon::EliminateCrossingLines <BR>
\brief Removes crossing lines.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::EliminateCrossingLines(void)
{
	static const int conMaxIterations = 30;
	bool bReordered = false;
	bool bRepeat = true;
	unsigned int nIt = 0;

	while (bRepeat && nIt < conMaxIterations)
	{
		nIt++;
		bRepeat = false;
		for (unsigned int nCross1 = 0; nCross1 < m_Lines.size() ; nCross1++)
		{
			for (unsigned int nCross2 = nCross1 + 2; nCross2 < m_Lines.size() ; nCross2++)
			{
				if ( (nCross1 == 0) && (nCross2 == (m_Lines.size() - 1)) ) continue;

				if (GetLineRect(nCross1)->Overlaps(*GetLineRect(nCross2)) &&
					GetLine(nCross1)->Crosses(*GetLine(nCross2)))
				{
					unsigned int nSwapStart = nCross1 + 1; // end of first line
					unsigned int nSwapEnd = nCross2;
					assert(nSwapEnd > nSwapStart);
					unsigned int nHalfway =	(nSwapEnd - nSwapStart) / 2;

					for (unsigned int nPoint = 0; nPoint <= nHalfway; nPoint++)
					{
						SwapPositions( nSwapStart + nPoint, nSwapEnd - nPoint);
					}
					bReordered = true;	
					bRepeat = true;
				}
 			}
		}
	}

	MakeBoundingRect();

	return (!bRepeat);
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::InsertPoint <BR>
\brief Inserts a new point in the polygon.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::InsertPoint( unsigned int nPointIndex, const C2DPoint& Point)
{
	if (nPointIndex >=  m_Lines.size() )
		nPointIndex -= m_Lines.size();

	unsigned int nPointIndexBefore;
	(nPointIndex == 0) ? nPointIndexBefore = m_Lines.size() - 1 : nPointIndexBefore = nPointIndex - 1;

	unsigned int nPointIndexAfter;
	(nPointIndex == m_Lines.size() - 1) ? nPointIndexAfter = 0 : nPointIndexAfter = nPointIndex + 1;

	C2DLine* pInsert = new C2DLine(Point, m_Lines[nPointIndex].GetPointFrom());
	C2DRect* pInsertRect = new C2DRect;

	pInsert->GetBoundingRect(*pInsertRect);

	
	C2DLineBase* pLineBase = m_Lines.GetAt(nPointIndexBefore);
	if (pLineBase != 0 && pLineBase->GetType() == C2DBase::StraightLine)
	{
		C2DLine* pLineBefore = dynamic_cast<C2DLine*>(pLineBase);
		pLineBefore->SetPointTo(Point);
		pLineBefore->GetBoundingRect(m_LineRects[nPointIndexBefore]);

		m_Lines.InsertAt(nPointIndex, pInsert);

		m_LineRects.InsertAt(nPointIndex, pInsertRect);
	}

}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::RemovePoint <BR>
\brief Removes the point from the polygon.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::RemovePoint(unsigned int nPointIndex)
{
	if (nPointIndex >=  m_Lines.size() )
		nPointIndex -= m_Lines.size();

	unsigned int nPointIndexBefore;
	(nPointIndex == 0) ? nPointIndexBefore = m_Lines.size() - 1 : nPointIndexBefore = nPointIndex - 1;

	unsigned int nPointIndexAfter;
	(nPointIndex == m_Lines.size() - 1) ? nPointIndexAfter = 0 : nPointIndexAfter = nPointIndex + 1;

	C2DLineBase* pLineBase = m_Lines.GetAt(nPointIndexBefore);
	if (pLineBase != 0 && pLineBase->GetType() == C2DBase::StraightLine)
	{
		C2DLine* pLineBefore = dynamic_cast<C2DLine*>(pLineBase);
		pLineBefore->SetPointTo(  m_Lines[nPointIndexAfter].GetPointFrom() );
		pLineBefore->GetBoundingRect(m_LineRects[nPointIndexBefore]);

		m_Lines.DeleteAt(nPointIndex);
		m_LineRects.DeleteAt(nPointIndex);
	}
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::SetPoint <BR>
\brief Sets a point at a given index.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::SetPoint(const C2DPoint& Point, unsigned int nPointIndex)
{
	if (nPointIndex >=  m_Lines.size() )
		nPointIndex -= m_Lines.size();

	unsigned int nPointIndexBefore;
	(nPointIndex == 0) ? nPointIndexBefore = m_Lines.size() - 1 : nPointIndexBefore = nPointIndex - 1;

	unsigned int nPointIndexAfter;
	(nPointIndex == m_Lines.size() - 1) ? nPointIndexAfter = 0 : nPointIndexAfter = nPointIndex + 1;


	C2DLineBase* pLineBase = m_Lines.GetAt(nPointIndex);
	if (pLineBase != 0 && pLineBase->GetType() == C2DBase::StraightLine)
	{
		C2DLine* pLine = dynamic_cast<C2DLine*>(pLineBase);
		pLine->Set(Point, m_Lines[nPointIndexAfter].GetPointFrom());
		pLine->GetBoundingRect(m_LineRects[nPointIndex]);

		C2DLineBase* pLineBaseBefore = m_Lines.GetAt(nPointIndexBefore);
		if (pLineBaseBefore != 0 && pLineBaseBefore->GetType() == C2DBase::StraightLine)
		{
			C2DLine* pLineBefore = dynamic_cast<C2DLine*>(pLineBaseBefore);
			pLineBefore->SetPointTo(Point);
			pLineBefore->GetBoundingRect(m_LineRects[nPointIndexBefore]);
		}
	}
}



/**--------------------------------------------------------------------------<BR>
C2DPolygon::SwapPositions <BR>
\brief Swaps point positions.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::SwapPositions(unsigned int Pos1, unsigned int Pos2)
{
	C2DPoint temp = *GetPoint(Pos1);

	SetPoint(*GetPoint(Pos2), Pos1);
	SetPoint(temp, Pos2);
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::Move <BR>
\brief Moves the polygon.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::Move(const C2DVector& Vector)
{
	if (m_SubArea[0] != 0 && m_SubArea[1] != 0)
	{
		m_SubArea[0]->Move(Vector);
		m_SubArea[1]->Move(Vector);
	}
	
	C2DPolyBase::Move(Vector);
	
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::Reflect <BR>
\brief Reflects the polygon through the point.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::Reflect(const C2DPoint& Point)
{
	if (m_SubArea[0] != 0 && m_SubArea[1] != 0)
	{
		m_SubArea[0]->Reflect(Point);
		m_SubArea[1]->Reflect(Point);
	}
	
	C2DPolyBase::Reflect(Point);
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::Reflect <BR>
\brief Reflects the polygon through the line.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::Reflect(const C2DLine& Line)
{
	if (m_SubArea[0] != 0 && m_SubArea[1] != 0)
	{
		m_SubArea[0]->Reflect(Line);
		m_SubArea[1]->Reflect(Line);
	}
	
	C2DPolyBase::Reflect(Line);

}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::HasRepeatedPoints <BR>
\brief True if there are repeated points.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::HasRepeatedPoints(void) const
{
	for (unsigned int i = 0 ; i < m_Lines.size(); i++)
	{
		for (unsigned int r = i + 1; r < m_Lines.size(); r++)
		{
			if (m_Lines[i].GetPointFrom() == m_Lines[r].GetPointFrom()) 
				return true;
		}
	}
	return false;
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::Grow <BR>
\brief Grows the polygon by the factor around the centroid.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::Grow(double dFactor)
{
	C2DPoint cen = GetCentroid();

	Grow(dFactor, cen);
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::Grow <BR>
\brief Grows the polygon by the factor around the origin given.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::Grow(double dFactor, const C2DPoint& Origin)
{
	C2DPolyBase::Grow(dFactor, Origin);
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::Project <BR>
\brief Prjects this onto vector given. Returns the interval, i.e. the min, max values
of the projection along the vector.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::Project(const C2DVector& Vector, CInterval& Interval) const
{
	if (m_Lines.size() < 1) return;

	Interval.dMax = m_Lines[0].GetPointFrom().Project(Vector);
	Interval.dMin = Interval.dMax;

	for (unsigned int i = 1; i < m_Lines.size(); i++)
	{
		double dDist = m_Lines[i].GetPointFrom().Project(Vector);
		if (dDist < Interval.dMin)
			Interval.dMin = dDist;
		if (dDist > Interval.dMax)
			Interval.dMax = dDist;
	}
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::Project <BR>
\brief Prjects this onto vector given. Returns the interval, i.e. the min, max values
of the projection along the vector.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::Project(const C2DLine& Line, CInterval& Interval) const
{
	if (m_Lines.size() < 1) return;

	Interval.dMax = m_Lines[0].GetPointFrom().Project(Line);
	Interval.dMin = Interval.dMax;

	for (unsigned int i = 1; i < m_Lines.size(); i++)
	{
		double dDist = m_Lines[i].GetPointFrom().Project(Line);
		Interval.ExpandToInclude(dDist);
	}
}




/**--------------------------------------------------------------------------<BR>
C2DPolygon::GetMinProjection <BR>
\brief Returns the vector on which the projection of the whole set of polygons is minimal.
Also returns the projection interval on that vector.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::GetMinProjection(C2DPolygonSet& Polygons, C2DVector& Result, CInterval& Interval)
{
	CInterval Single;
	CInterval Combined;

	for (unsigned int i = 0 ; i < Polygons.size(); i++)
	{
		for (unsigned int l = 0; l < Polygons[i].GetPointsCount() ; l++)
		{
			C2DLine Line(  *Polygons[i].GetPoint(l), *Polygons[i].GetPoint(l + 1));
			Line.vector.TurnRight();
		
			Polygons[0].Project(Line.vector, Single);
			Combined = Single;
			
			for (unsigned int n = 1 ; n < Polygons.size(); n++)
			{
				Polygons[n].Project(Line.vector, Single);
				Combined.ExpandToInclude(Single);
			}

			if ((i * l) == 0)
			{
				Interval = Combined;
				Result = Line.vector;
			}
			else
			{
				if (Combined.GetLength() < Interval.GetLength())
				{
					Interval = Combined;
					Result = Line.vector;
				}
			}
		}
    }
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::Overlaps <BR>
\brief Returns true if there is an overlap between this and the other polygon and the
vector required to move them apart from one another. Exact for a convex polygon. 
For non-convex, the function effectively works on the convex hull. However, a
non-convex polygon can be split into convex polygons and then the calculated
translation vector is more accurate.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::Overlaps(const C2DPolygon& Other, C2DVector& MinimumTranslationVector) const 
{

	if (m_Lines.size() < 2 || Other.GetPointsCount() < 2) return false;

	if (!m_BoundingRect.Overlaps(Other.GetBoundingRect()))
		return false;

	if (m_SubArea[0] != 0 && m_SubArea[1] != 0)
	{

		C2DVector v1, v2;
		bool b1 = m_SubArea[0]->Overlaps(Other, v1);
		bool b2 = m_SubArea[1]->Overlaps(Other, v2);
		if (b1 && b2) MinimumTranslationVector = v1 + v2;
		else if (b1) MinimumTranslationVector = v1;
		else if (b2) MinimumTranslationVector = v2;
		return b1 || b2;

	}
	else if (Other.GetSubArea(0) != 0 && Other.GetSubArea(1) != 0)
	{
		bool bRes = Other.Overlaps(*this, MinimumTranslationVector);
		if (bRes) MinimumTranslationVector.Reverse();
		return bRes;
	}
	else
	{
		CInterval ThisProj;
		CInterval OtherProj;

		C2DLine ProjLine;

		bool bVecFound = false;

		for (unsigned int i = 0 ; i < m_Lines.size() + Other.GetPointsCount(); i++)
		{
			if ( i < m_Lines.size())
				ProjLine.Set(*GetPoint(i), *GetPoint(i+1));
			else
				ProjLine.Set( *Other.GetPoint(i - m_Lines.size()), 
					*Other.GetPoint(i - m_Lines.size() + 1));

			ProjLine.vector.TurnRight();
			ProjLine.vector.MakeUnit();

			this->Project(ProjLine.vector, ThisProj );
			Other.Project(ProjLine.vector, OtherProj );

			if (ThisProj.dMin < OtherProj.dMax && ThisProj.dMax > OtherProj.dMax)
			{
				if (!bVecFound || 
					(OtherProj.dMax - ThisProj.dMin) < MinimumTranslationVector.GetLength())
				{
					MinimumTranslationVector = ProjLine.vector;
					MinimumTranslationVector.SetLength(OtherProj.dMax - ThisProj.dMin);
					MinimumTranslationVector = MinimumTranslationVector * 1.001;

					bVecFound = true;
				}
			}
			else if (OtherProj.dMin <ThisProj.dMax && OtherProj.dMax > ThisProj.dMax )
			{
				if (!bVecFound || 
					(ThisProj.dMax - OtherProj.dMin) < MinimumTranslationVector.GetLength())
				{
					MinimumTranslationVector = ProjLine.vector;
					MinimumTranslationVector.SetLength(ThisProj.dMax - OtherProj.dMin);
					MinimumTranslationVector.Reverse();
					MinimumTranslationVector = MinimumTranslationVector * 1.001;
					bVecFound = true;
				}
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::Overlaps <BR>
\brief True if this overlaps the other.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::Overlaps( const C2DPolygon& Other) const    
{
	return C2DPolyBase::Overlaps( static_cast<const C2DPolyBase&>(Other));
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::Avoid <BR>
\brief Calls the Overlaps function and moves this away from the other.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::Avoid(const C2DPolygon& Other)
{
	C2DVector vTrans;

	if (this->Overlaps(Other, vTrans))
		this->Move(vTrans);
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::Smooth <BR>
\brief The polygon is smoothed so that no angle is less than the minimum angle provided 
which must be less than PI. The acute points defined by this limit are cropped 
by the factor provided; 0.8 means they are cropped towards the end of the line.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::Smooth(double dMinAngle, double dCropFactor)
{
	unsigned int i = 0;

	if (m_Lines.size() < 3)
		return;

	assert(IsClockwise());

	unsigned int nCount = m_Lines.size();
	unsigned int nIt = 0;

	while (nIt < nCount )
	{
		C2DLine Line1 = * dynamic_cast<const C2DLine*>(GetLine(i)  ) ;
		C2DLine Line2 = *dynamic_cast<const C2DLine*>( GetLine(i + 1) );
		
		C2DVector Vec = Line1.vector;

		Vec.Reverse();

		double dAng =  Line2.vector.AngleToRight(Vec) ;
		if(dAng <  dMinAngle || dAng > (conTWOPI - dMinAngle))
		{
			SetPoint( Line1.GetPointOn(dCropFactor),  (i + 1));
			InsertPoint(i +2, Line2.GetPointOn(1 - dCropFactor));
			nCount++;
			i += 2;
			nIt = 0;
			
		}
		else
		{
			i++;
			nIt++;
		}
		if (i >= nCount)
			i = 0;
	}

	MakeBoundingRect();

	if (m_SubArea[0] != 0 && m_SubArea[1] != 0)
		CreateConvexSubAreas();
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::MakeLines <BR>
\brief Make the lines for the base class to work.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::MakeLines(const C2DPointSet& Points)
{
	m_Lines.DeleteAll();

	unsigned int nCount = Points.size();

	const C2DPoint* previous = Points.GetAt(0);
	for (unsigned int i = 1; i <= nCount; i++)
	{
		const C2DPoint* next = Points.GetAt(i % nCount);
		m_Lines.Add(new C2DLine(*previous, *next));
		previous = next;
	}


}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::GetBoundingCircle <BR>
\brief Returns the minimum bounding circle for the polygon.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::GetBoundingCircle(C2DCircle& Circle) const
{
	C2DPointSet Points;
	GetPointsCopy(Points);

	Points.GetBoundingCircle(Circle);
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::GetNonOverlaps <BR>
\brief Calls the base class to obtain the parts of this that are not overlapping with
the other.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::GetNonOverlaps(const C2DPolygon& Other, C2DHoledPolygonSet& HoledPolygons, 
										CGrid::eDegenerateHandling eDegen) const 
{
	C2DHoledPolyBaseSet BaseSet;

	C2DPolyBase::GetNonOverlaps( Other, BaseSet, eDegen);

	for (unsigned int i = 0 ; i < BaseSet.size(); i++)
	{
		HoledPolygons.Add( new C2DHoledPolygon( BaseSet[i]));
	}
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::GetNonOverlaps <BR>
\brief Calls the base class to obtain the parts of this that are not overlapping with
the other. Access to the base class to return generic polygons.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::GetNonOverlaps(const C2DPolygon& Other, C2DHoledPolyBaseSet& HoledPolygons, 
										CGrid::eDegenerateHandling eDegen) const 
{
	C2DPolyBase::GetNonOverlaps( Other, HoledPolygons, eDegen);
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::GetUnion <BR>
\brief Calls the base class to obtain the union of this with the other.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::GetUnion(const C2DPolygon& Other, C2DHoledPolygonSet& HoledPolygons,
										CGrid::eDegenerateHandling eDegen) const 
{
	C2DHoledPolyBaseSet BaseSet;

	C2DPolyBase::GetUnion( Other, BaseSet, eDegen);

	for (unsigned int i = 0 ; i < BaseSet.size(); i++)
	{
		HoledPolygons.Add( new C2DHoledPolygon( BaseSet[i]));
	}
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::GetUnion <BR>
\brief Calls the base class to obtain the union of this with the other.
Access to the base class to return generic polygons.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::GetUnion(const C2DPolygon& Other, C2DHoledPolyBaseSet& HoledPolygons,
										CGrid::eDegenerateHandling eDegen ) const 
{
	C2DPolyBase::GetUnion( Other, HoledPolygons, eDegen);
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::GetOverlaps <BR>
\brief Calls the base class to obtain the parts of this that are overlapping with
the other.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::GetOverlaps(const C2DPolygon& Other, C2DHoledPolygonSet& Polygons,
										CGrid::eDegenerateHandling eDegen  ) const 
{
	C2DHoledPolyBaseSet BaseSet;

	C2DPolyBase::GetOverlaps( Other, BaseSet, eDegen);

	for (unsigned int i = 0 ; i < BaseSet.size(); i++)
	{
		Polygons.Add( new C2DHoledPolygon( BaseSet[i]));
	}
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::GetOverlaps <BR>
\brief Calls the base class to obtain the parts of this that are overlapping with
the other. Access to the base class to return generic polygons.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::GetOverlaps(const C2DPolygon& Other, C2DHoledPolyBaseSet& Polygons,
										CGrid::eDegenerateHandling eDegen ) const 
{
	C2DPolyBase::GetOverlaps( Other, Polygons, eDegen);
}



/**--------------------------------------------------------------------------<BR>
C2DPolygon::OverlapsAbove <BR>
\brief True if this polygon is above the other. Returns the vertical distance 
and the points on both polygons.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::OverlapsAbove( const C2DPolygon& Other, double& dVerticalDistance,
										C2DPoint& ptOnThis, C2DPoint& ptOnOther) const
{
	const C2DRect& OtherBoundingRect = Other.GetBoundingRect();

	if ( !m_BoundingRect.OverlapsAbove( OtherBoundingRect)  )
		return false;

	unsigned int nLineCount = m_Lines.size();

	if ( nLineCount != m_LineRects.size())
		return false;

	unsigned int nOtherLineCount = Other.GetLines().size();

	if ( nOtherLineCount != Other.GetLineRectCount())
		return false;


	struct sLine
	{
		const C2DLine* pLine;
		const C2DRect* pRect;
		bool bSetFlag;
	};

	std::vector<sLine*> Lines;
	std::vector<double> xValues;

	for (unsigned int i = 0 ; i < nLineCount; i++)
	{
		if ( m_LineRects[i].OverlapsAbove( OtherBoundingRect ) )
		{
			sLine* pNewLine = new sLine;
			pNewLine->pLine = GetLine(i);
			pNewLine->pRect = &m_LineRects[i];
			pNewLine->bSetFlag = true;
			Lines.push_back( pNewLine );
			xValues.push_back( m_LineRects[i].GetLeft());
		}
	}

	for (unsigned int i = 0 ; i < nOtherLineCount; i++)
	{
		if ( Other.GetLineRect(i)->OverlapsBelow( this->m_BoundingRect ) )
		{
			sLine* pNewLine = new sLine;
			pNewLine->pLine = Other.GetLine(i);
			pNewLine->pRect = Other.GetLineRect(i);
			pNewLine->bSetFlag = false;
			Lines.push_back( pNewLine );
			xValues.push_back( Other.GetLineRect(i)->GetLeft());
		}
	}

	GeoSort::PQuickSort< std::vector<double>, double, std::vector<sLine*>, sLine*>( xValues, Lines);

	bool bResult = false;

	unsigned int j = 0;
	while (j < Lines.size())
	{
		unsigned int r = j + 1;

		double dXLimit = Lines[j]->pRect->GetRight();

		while (r < Lines.size() && 
			   Lines[r]->pRect->GetLeft() < dXLimit)
		{
			double dDistTemp;
			C2DPoint ptOnThisTemp;
			C2DPoint ptOnOtherTemp;
			bool bOverlap = false;
			if (  Lines[j]->bSetFlag )
			{
				if ( !Lines[r]->bSetFlag && 
					Lines[j]->pLine->OverlapsAbove( *Lines[r]->pLine , dDistTemp, 
													ptOnThisTemp,  ptOnOtherTemp ))
				{
					bOverlap = true;
				}
			}
			else 
			{
				if ( Lines[r]->bSetFlag && 
					Lines[r]->pLine->OverlapsAbove( *Lines[j]->pLine , dDistTemp, 
													ptOnThisTemp,  ptOnOtherTemp ))
				{
					bOverlap = true;
				}
			}

			if ( bOverlap && (dDistTemp < dVerticalDistance || !bResult) )
			{
				bResult = true;
				dVerticalDistance = dDistTemp;
				ptOnThis = ptOnThisTemp;
				ptOnOther = ptOnOtherTemp;
				if ( dDistTemp == 0)
				{
					j += Lines.size(); // escape;
					r += Lines.size(); // escape;
				}
			}

			r++;
		}

		j++;
	}


	for ( unsigned int i = 0 ; i < Lines.size(); i++)
	{
		delete Lines[i];
	}

	return bResult;
}

/**--------------------------------------------------------------------------<BR>
C2DPolygon::OverlapsVertically <BR>
\brief True if this polygon is above or below the other. Returns the vertical distance 
and the points on both polygons.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::OverlapsVertically( const C2DPolygon& Other, double& dVerticalDistance,
										C2DPoint& ptOnThis, C2DPoint& ptOnOther) const
{
	const C2DRect& OtherBoundingRect = Other.GetBoundingRect();

	if ( !m_BoundingRect.OverlapsVertically( OtherBoundingRect)  )
		return false;

	unsigned int nLineCount = m_Lines.size();

	if ( nLineCount != m_LineRects.size())
		return false;

	unsigned int nOtherLineCount = Other.GetLines().size();

	if ( nOtherLineCount != Other.GetLineRectCount())
		return false;


	struct sLine
	{
		const C2DLine* pLine;
		const C2DRect* pRect;
		bool bSetFlag;
	};

	std::vector<sLine*> Lines;
	std::vector<double> xValues;

	for (unsigned int i = 0 ; i < nLineCount; i++)
	{
		if ( m_LineRects[i].OverlapsVertically( OtherBoundingRect ) )
		{
			sLine* pNewLine = new sLine;
			pNewLine->pLine = GetLine(i);
			pNewLine->pRect = &m_LineRects[i];
			pNewLine->bSetFlag = true;
			Lines.push_back( pNewLine );
			xValues.push_back( m_LineRects[i].GetLeft());
		}
	}

	for (unsigned int i = 0 ; i < nOtherLineCount; i++)
	{
		if ( Other.GetLineRect(i)->OverlapsVertically( this->m_BoundingRect ) )
		{
			sLine* pNewLine = new sLine;
			pNewLine->pLine = Other.GetLine(i);
			pNewLine->pRect = Other.GetLineRect(i);
			pNewLine->bSetFlag = false;
			Lines.push_back( pNewLine );
			xValues.push_back( Other.GetLineRect(i)->GetLeft());
		}
	}

	GeoSort::PQuickSort< std::vector<double>, double, std::vector<sLine*>, sLine*>( xValues, Lines);

	bool bResult = false;

	unsigned int j = 0;
	while (j < Lines.size())
	{
		unsigned int r = j + 1;

		double dXLimit = Lines[j]->pRect->GetRight();

		while (r < Lines.size() && 
			   Lines[r]->pRect->GetLeft() < dXLimit)
		{
			double dDistTemp;
			C2DPoint ptOnThisTemp;
			C2DPoint ptOnOtherTemp;
			bool bOverlap = false;
			if (  Lines[j]->bSetFlag )
			{
				if ( !Lines[r]->bSetFlag && 
					Lines[j]->pLine->OverlapsVertically( *Lines[r]->pLine , dDistTemp, 
													ptOnThisTemp,  ptOnOtherTemp ))
				{
					bOverlap = true;
				}
			}
			else 
			{
				if ( Lines[r]->bSetFlag && 
					Lines[r]->pLine->OverlapsVertically( *Lines[j]->pLine , dDistTemp, 
													ptOnThisTemp,  ptOnOtherTemp ))
				{
					bOverlap = true;
				}
			}

			if ( bOverlap && (dDistTemp < dVerticalDistance || !bResult) )
			{
				bResult = true;
				dVerticalDistance = dDistTemp;
				ptOnThis = ptOnThisTemp;
				ptOnOther = ptOnOtherTemp;
				if ( dDistTemp == 0)
				{
					j += Lines.size(); // escape;
					r += Lines.size(); // escape;
				}
			}

			r++;
		}

		j++;
	}


	for ( unsigned int i = 0 ; i < Lines.size(); i++)
	{
		delete Lines[i];
	}

	return bResult;
}


/**--------------------------------------------------------------------------<BR>
C2DPolygon::GetMinBoundingBox <BR>
\brief Returns the minimum bounding box that is not necassarily horiztonal i.e. 
the box can be at an angle and is defined by a line and the width to the right.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::GetMinBoundingBox( C2DLine& Line, double& dWidthToRight) const
{
	unsigned int nCount = m_Lines.size();
	if (nCount == 0)
		return;

	if (!IsConvex())
	{
		C2DPolygon CH;
		CH.CreateConvexHull( *this );
		return CH.GetMinBoundingBox( Line, dWidthToRight);
	}


	unsigned int nP1 = 0;//index of vertex with minimum y-coordinate;
	unsigned int nP2 = 0;//index of vertex with maximum y-coordinate;

 	unsigned int nP3 = 0;//index of vertex with minimum x-coordinate;
	unsigned int nP4 = 0;//index of vertex with maximum x-coordinate;

	double dMinY = m_Lines[0].GetPointFrom().y;
	double dMaxY = dMinY;

	double dMinX = m_Lines[0].GetPointFrom().x;
	double dMaxX = dMinX;

	for ( unsigned int i = 1 ; i < m_Lines.size(); i++)
	{
		C2DPoint pt = m_Lines[i].GetPointFrom();
		if (pt.y < dMinY)
		{
			dMinY = pt.y;
			nP1 = i;
		}
		else if (pt.y > dMaxY)
		{
			dMaxY = pt.y;
			nP2 = i;
		}

		if (pt.x < dMinX)
		{
			dMinX = pt.x;
			nP3 = i;
		}
		else if (pt.x > dMaxX)
		{
			dMaxX = pt.x;
			nP4 = i;
		}
	}


	double dRotatedAngle = 0;
	double dMinArea = 1.7E+308;
 
// 222222
// 3	4
// 3    4
// 3    4
// 111111

	C2DLine Caliper1( m_Lines[nP1].GetPointFrom(), C2DVector(-1,0) );    // Caliper 1 points along the negative x-axis
	C2DLine Caliper2( m_Lines[nP2].GetPointFrom(), C2DVector(1,0) );   // Caliper 2 points along the positive x-axis

	C2DLine Caliper3( m_Lines[nP3].GetPointFrom(), C2DVector(0,1) );    // Caliper 3 points along the positive y-axis
	C2DLine Caliper4( m_Lines[nP4].GetPointFrom(), C2DVector(0,-1) );   // Caliper 4 points along the negative y-axis

	while( dRotatedAngle < conPI)
	{
   		// Determine the angle between each caliper and the next adjacent edge in the polygon
		double dAngle1 = Caliper1.vector.AngleToRight(GetLine( nP1 )->vector);
		double dAngle2 = Caliper2.vector.AngleToRight(GetLine( nP2 )->vector);
		double dAngle3 = Caliper3.vector.AngleToRight(GetLine( nP3 )->vector);
		double dAngle4 = Caliper4.vector.AngleToRight(GetLine( nP4 )->vector);

		double dMinAngle;
		if (dAngle1 < dAngle2 &&
			dAngle1 < dAngle3 &&
			dAngle1 < dAngle4)
		{
			dMinAngle = dAngle1;
			nP1++;
			Caliper1.point = GetLine( nP1 )->point;
		}
		else if (dAngle2 < dAngle3 &&
			dAngle2 < dAngle4)
		{
			dMinAngle = dAngle2;
			nP2++;
			Caliper2.point = GetLine( nP2 )->point;
		}
		else if ( dAngle3 < dAngle4)
		{
			dMinAngle = dAngle3;
			nP3++;
			Caliper3.point = GetLine( nP3 )->point;
		}
		else
		{
			dMinAngle = dAngle4;
			nP4++;
			Caliper4.point = GetLine( nP4 )->point;
		}
		dRotatedAngle += dMinAngle;
		dMinAngle -= 0.00000001;
		Caliper1.vector.TurnRight( dMinAngle);
		Caliper2.vector.TurnRight( dMinAngle);
		Caliper3.vector.TurnRight( dMinAngle);
		Caliper4.vector.TurnRight( dMinAngle);


		double dWidth1 = Caliper1.DistanceAsRay( Caliper2.point );		
		double dWidth2 = Caliper3.DistanceAsRay( Caliper4.point );	
	
		double dArea = dWidth1 * dWidth2;

		if (dArea < dMinArea)
		{
			dMinArea = dArea;
			Line = Caliper1;
			Line.point.ProjectOnRay( Caliper4 );
			Line.vector.SetLength( dWidth2 );
			
			dWidthToRight = dWidth1;
		}
	}
}



/**--------------------------------------------------------------------------<BR>
C2DPolygon::SimpleBuffer <BR>
\brief Simple buffer around the polygon at a fixed amount. No attemp to ensure validity
as intended for small buffer amounts.
<P>---------------------------------------------------------------------------*/
void C2DPolygon::SimpleBuffer(double dBuffer)
{
    ClearConvexSubAreas();

    bool bClockwise = IsClockwise();

    for (unsigned int i = 0; i < this->m_Lines.size(); i++)
    {
		if (m_Lines[i].GetType() == C2DBase::StraightLine )
        {
            C2DLine& Line = dynamic_cast<C2DLine& >( m_Lines[i]);
            C2DVector v(Line.vector);
            if (bClockwise)
                v.TurnLeft();
            else
                v.TurnRight();
            v.SetLength(dBuffer);
            Line.Move(v);
        }
        else
        {
            assert(false);
            return;
        }
    }

    for (unsigned int i = 1; i < m_Lines.size(); i++)
    {
        dynamic_cast<C2DLine& >( m_Lines[i - 1]).Join(   dynamic_cast<C2DLine& >( m_Lines[i])  );
    }

    dynamic_cast<C2DLine& >( m_Lines[m_Lines.size() - 1]).Join(   dynamic_cast<C2DLine& >( m_Lines[0])  );

    MakeLineRects();
    MakeBoundingRect();

}



/**--------------------------------------------------------------------------<BR>
C2DPolygon::RemoveNullAreas <BR>
\brief True if the polygon is no longer valid i.e. completely null.
<P>---------------------------------------------------------------------------*/
bool C2DPolygon::RemoveNullAreas(double dTolerance)
{
    ClearConvexSubAreas();

    unsigned int i = 0;
    bool bChanged = false;
    while (i < m_Lines.size() && m_Lines.size() > 2)
    {
        unsigned int nNext = (i + 1) % m_Lines.size();
		double dArea = C2DTriangle::GetAreaSigned(m_Lines[i].GetPointFrom(),
                                                    m_Lines[i].GetPointTo(),
                                                    m_Lines[nNext].GetPointTo());

        if (fabs(dArea) < dTolerance)
        {
			if (m_Lines[i].GetType() == C2DBase::StraightLine)
            {
                dynamic_cast<C2DLine&>(m_Lines[i]).SetPointTo(m_Lines[(i + 1) % m_Lines.size()].GetPointTo());
                m_Lines.DeleteAt(nNext);
                bChanged = true;
            }
            else
            {
                assert(false);
                return true;
            }
        }
        else
        {
            i++;
        }
    }

    if (m_Lines.size() <= 2)
    {
        Clear();
        return true;
    }

    if (bChanged)
    {
        MakeLineRects();
        MakeBoundingRect();
    }

    return false;
}
