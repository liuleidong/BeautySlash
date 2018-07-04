/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DHoledPolyBase.cpp
\brief Implementation file for the C2DHoledPolyBase Class.

Implementation file for C2DHoledPolyBase, the base class for an area with 
holes. The Rim and Holes are pointers to C2DPolyBase classes. 
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "C2DHoledPolyBase.h"
#include "C2DHoledPolyBaseSet.h"
#include "RandomNumber.h"
#include "C2DCircle.h"
#include "C2DLineBaseSetSet.h"
#include "C2DPolyBase.h"
#include "C2DLineBase.h"
#include "C2DVector.h"
#include "C2DLine.h"
#include "C2DArc.h"
#include "C2DPointSet.h"
#include "IndexSet.h"

_MEMORY_POOL_IMPLEMENATION(C2DHoledPolyBase)

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::C2DHoledPolyBase
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DHoledPolyBase::C2DHoledPolyBase(void) : C2DBase(PolyHoledBase)
{
	m_Rim = 0;
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::~C2DHoledPolyBase
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DHoledPolyBase::~C2DHoledPolyBase(void)
{
	Clear();
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::C2DHoledPolyBase
\brief Copy constructor.
<P>---------------------------------------------------------------------------*/
C2DHoledPolyBase::C2DHoledPolyBase(const C2DHoledPolyBase& Other) : C2DBase(PolyHoledBase)
{
	m_Rim = new C2DPolyBase;
	(*m_Rim) = *Other.GetRim();

	for (unsigned int i = 0 ; i < Other.GetHoleCount(); i++)
	{
		m_Holes.Add(new C2DPolyBase(*Other.GetHole(i)));
	}
	m_Type = Other.GetType();
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::C2DHoledPolyBase
\brief Assignment.
<P>---------------------------------------------------------------------------*/
const C2DHoledPolyBase& C2DHoledPolyBase::operator=(const C2DHoledPolyBase& Other)
{
	Clear();

	m_Rim = new C2DPolyBase;
	(*m_Rim) = *Other.GetRim();

	for (unsigned int i = 0 ; i < Other.GetHoleCount(); i++)
	{
		m_Holes.Add(new C2DPolyBase(*Other.GetHole(i)));
	}

	return *this;
}



/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::SetRim
\brief Sets the rim to a copy of the polygon given.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::SetRim(const C2DPolyBase& Polygon)
{
	if (m_Rim != 0)
		delete m_Rim;

	m_Rim = new C2DPolyBase;
	(*m_Rim) = Polygon;

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::AddHole
\brief Adds a copy of the polygon given.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::AddHole(const C2DPolyBase& Polygon)
{
	m_Holes.Add(new C2DPolyBase(Polygon));

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::SetHole
\brief Sets the holes to be a copy of the polygon given.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::SetHole(const C2DPolyBase& Polygon, unsigned int usIndex)
{
	if (usIndex < m_Holes.size())
	{
		m_Holes[usIndex] = Polygon;
	}

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::SetRimDirect
\brief Sets the rim to be the polygon provided. Will delete on destruction.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::SetRimDirect(C2DPolyBase* Polygon)
{
	assert(Polygon);

	assert (!m_Rim);

	if (m_Rim != 0)
		delete m_Rim;

	m_Rim = Polygon;

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::AddHoleDirect
\brief Adds the hole provided. Will delete on destruction.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::AddHoleDirect(C2DPolyBase* Polygon)
{
	assert(Polygon);

	m_Holes.Add(Polygon);

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::SetHoleDirect
\brief Sets the hole to be the polygon provided. Will delete on destruction. 
Deletes old one.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::SetHoleDirect(C2DPolyBase* Polygon, unsigned int usIndex)
{
	if (usIndex < m_Holes.size())
	{
		m_Holes.DeleteAndSet(usIndex, Polygon);

	}

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::RemoveHole
\brief Removes the hole at the index given.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::RemoveHole(unsigned int usIndex)
{
	if (usIndex < m_Holes.size())
	{
		m_Holes.DeleteAt(usIndex);
	}

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::ExtractRim
\brief Extracts the rim. Deletion of the rim needs to be managed elsewhere.
<P>---------------------------------------------------------------------------*/
C2DPolyBase* C2DHoledPolyBase::ExtractRim(void)
{
	C2DPolyBase* Result = m_Rim;
	m_Rim = 0;
	return Result;
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::GetHole
\brief Returns a pointer to the hole specified.
<P>---------------------------------------------------------------------------*/
C2DPolyBase* C2DHoledPolyBase::GetHole(unsigned int usIndex)
{
	if (usIndex >= m_Holes.size())
		return 0;
	else
		return m_Holes.GetAt(usIndex);

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::GetHole
\brief Returns a constant pointer to the hole specified.
<P>---------------------------------------------------------------------------*/
const C2DPolyBase* C2DHoledPolyBase::GetHole(unsigned int usIndex) const
{
	if (usIndex >= m_Holes.size())
		return 0;
	else
		return m_Holes.GetAt(usIndex);

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::ExtractHole
\brief Extracts the hole. Deletion of the hole needs to be managed elsewhere.
<P>---------------------------------------------------------------------------*/
C2DPolyBase* C2DHoledPolyBase::ExtractHole(unsigned int usIndex)
{
	if (usIndex >= m_Holes.size())
		return 0;
	else
	{
		return m_Holes.ExtractAt(usIndex);
	}

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Clear
\brief Clears the shape.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::Clear(void)
{
	if (m_Rim != 0)
		delete m_Rim;

	m_Rim = 0;

	m_Holes.DeleteAll();
}



/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::IsValid
\brief True if there are no overlaps and all shapes have at least 3 lines. All
holes must be inside the rim.
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBase::IsValid(void)
{
	if (m_Rim == 0)
		return false;

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		if (!m_Rim->Contains( m_Holes[i]))
			return false;
	}

	unsigned int h = 0;
	while (h < m_Holes.size())
	{
		unsigned int r = h + 1;
		while (r < m_Holes.size())
		{
			if (m_Holes[h].Overlaps(m_Holes[r]))
				return false;
			r++;
		}
		h++;
	}


	return true;

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::GetLineCount
\brief Returns the number of lines in the shape.
<P>---------------------------------------------------------------------------*/
unsigned int C2DHoledPolyBase::GetLineCount(void) const
{
	unsigned int nResult = 0;

	if (m_Rim != 0)
	{
		nResult += m_Rim->GetLineCount();
	}

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		nResult +=	m_Holes[i].GetLineCount();
	}

	return nResult;
}



/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::RotateToRight
\brief RotateToRight
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::RotateToRight(double dAng, const C2DPoint& Origin)
{
	if (m_Rim == 0)
		return;

	m_Rim->RotateToRight(dAng, Origin);

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		m_Holes[i].RotateToRight(dAng, Origin);
	}

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Move
\brief Move
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::Move(const C2DVector& Vector)
{
	if (m_Rim != 0)
	{
		m_Rim->Move(Vector);
	}

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		m_Holes[i].Move(Vector);
	}

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Grow
\brief Grow
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::Grow(double dFactor, const C2DPoint& Origin)
{
	if (m_Rim != 0)
	{
		m_Rim->Grow(dFactor, Origin);
	}

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		m_Holes[i].Grow(dFactor, Origin);
	}
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Reflect
\brief Reflect
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::Reflect(const C2DPoint& Point)
{
	if (m_Rim != 0)
	{
		m_Rim->Reflect(Point);
	}

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		m_Holes[i].Reflect(Point);
	}
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Reflect
\brief Reflect
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::Reflect(const C2DLine& Line)
{
	if (m_Rim != 0)
	{
		m_Rim->Reflect(Line);
	}

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		m_Holes[i].Reflect(Line);
	}

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Project
\brief Projection onto the line
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::Project(const C2DLine& Line, CInterval& Interval) const
{

	if (m_Rim != 0)
		m_Rim->Project(Line,Interval );
}




/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Project
\brief Projection onto the vector
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::Project(const C2DVector& Vector, CInterval& Interval) const
{

	if (m_Rim != 0)
		m_Rim->Project(Vector,Interval );

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::GetBoundingRect
\brief GetBoundingRect
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::GetBoundingRect(C2DRect& Rect) const
{
	if (m_Rim == 0)
	{
		Rect.Clear();
		return;
	}
	else
	{
		Rect = m_Rim->GetBoundingRect();
	}

	for (unsigned int i = 0; i < m_Holes.size(); i++)
	{
		Rect.ExpandToInclude( m_Holes[i].GetBoundingRect());
	}
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::HasCrossingLines
\brief True if it contains crossing lines.
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBase::HasCrossingLines(void) const
{
	if (m_Rim == 0)
		return false;

	C2DLineBaseSet Lines;

	Lines.AddCopy( m_Rim->GetLines());

	for (unsigned int i = 0; i < m_Holes.size(); i++)
	{
		Lines.AddCopy( m_Holes[i].GetLines());
	}

	return Lines.HasCrossingLines();
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Contains
\brief Contains
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBase::Contains(const C2DPoint& pt) const 
{
	if (m_Rim == 0)
		return false;

	if (!m_Rim->Contains(pt))
		return false;

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		if (m_Holes[i].Contains(pt))
			return false;
	}

	return true;
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Contains
\brief Contains
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBase::Contains(const C2DLineBase& Line) const
{
	if (m_Rim == 0)
		return false;

	if (!m_Rim->Contains(Line))
		return false;

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		if (m_Holes[i].Crosses(Line) || m_Holes[i].Contains( Line.GetPointFrom()))
			return false;
	}

	return true;

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Contains
\brief Contains
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBase::Contains(const C2DPolyBase& Polygon)   const 
{
	if (m_Rim == 0)
		return false;

	if (!m_Rim->Contains(Polygon))
		return false;

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		if (m_Holes[i].Overlaps(Polygon))
			return false;
	}

	return true;

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Contains
\brief Contains
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBase::Contains(const C2DHoledPolyBase& Polygon) const 
{
	if (m_Rim == 0)
		return false;
	
	if (Polygon.GetRim() == 0)
		return false;

	if (!this->Contains( *Polygon.GetRim()))
		return false;
	
	for (unsigned int i = 0 ; i < Polygon.GetHoleCount(); i++)
	{
		if (! this->Contains(  *Polygon.GetHole(i) ))
			return false;
	}

	return true;
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Distance
\brief Distance
<P>---------------------------------------------------------------------------*/
double C2DHoledPolyBase::Distance(const C2DPoint& TestPoint) const
{
	if (m_Rim == 0)
		return 0;

	double dResult = m_Rim->Distance(TestPoint);
	bool bInside = dResult < 0;
	dResult = fabs(dResult);

	for (unsigned int i = 0; i < m_Holes.size(); i++)
	{
		double dDist = m_Holes[i].Distance(TestPoint); 
		if (dDist < 0)
			bInside = false;
		if (fabs(dDist) < dResult)
			dResult = fabs(dDist);
	}

	if (bInside)
		return dResult;
	else
		return - dResult;
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Distance
\brief Distance
<P>---------------------------------------------------------------------------*/
double C2DHoledPolyBase::Distance(const C2DLineBase& Line) const
{
	if (m_Rim == 0)
		return 0;

	double dResult = m_Rim->Distance(Line);
	if (dResult == 0)
		return 0;

	bool bInside = dResult < 0;
	dResult = fabs(dResult);

	for (unsigned int i = 0; i < m_Holes.size(); i++)
	{
		double dDist = m_Holes[i].Distance(Line); 
		if (dDist == 0)
			return 0;
		

		if (dDist < 0)
			bInside = false;
		if (fabs(dDist) < dResult)
			dResult = fabs(dDist);
	}

	if (bInside)
		return dResult;
	else
		return - dResult;
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Distance
\brief Distance
<P>---------------------------------------------------------------------------*/
double C2DHoledPolyBase::Distance(const C2DPolyBase& Poly, 
						C2DPoint* ptOnThis , C2DPoint* ptOnOther ) const
{
	C2DPoint ptOnThisResult;
	C2DPoint ptOnOtherResult;

	if (m_Rim == 0)
		return 0;

	double dResult = m_Rim->Distance(Poly, ptOnThis, ptOnOther);

	if (dResult == 0)
		return 0;

	if (ptOnThis)
		ptOnThisResult = *ptOnThis; 
	if (ptOnOther)
		ptOnOtherResult = *ptOnOther; 

	bool bInside = dResult < 0;
	dResult = fabs(dResult);

	for (unsigned int i = 0; i < m_Holes.size(); i++)
	{
		double dDist = m_Holes[i].Distance(Poly, ptOnThis, ptOnOther); 
		if (dDist == 0)
			return 0;
		

		if (dDist < 0)
			bInside = false;
		if (fabs(dDist) < dResult)
		{
			if (ptOnThis)
				ptOnThisResult = *ptOnThis; 
			if (ptOnOther)
				ptOnOtherResult = *ptOnOther; 

			dResult = fabs(dDist);
		}
	}

	if (ptOnThis)
		*ptOnThis = ptOnThisResult; 
	if (ptOnOther)
		*ptOnOther = ptOnOtherResult; 

	if (bInside)
		return dResult;
	else
		return - dResult;

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::IsWithinDistance
\brief IsWithinDistance
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBase::IsWithinDistance(const C2DPoint& TestPoint, double dDist) const
{
	if (m_Rim == 0)
		return 0;
	
	if (m_Rim->IsWithinDistance( TestPoint, dDist))
		return true;


	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		if (m_Holes[i].IsWithinDistance( TestPoint,  dDist))
			return true;
	}

	return false;
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::GetPerimeter
\brief GetPerimeter
<P>---------------------------------------------------------------------------*/
double C2DHoledPolyBase::GetPerimeter(void) const
{
	if (m_Rim == 0)
		return 0;
	
	double dResult = m_Rim->GetPerimeter();

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		dResult += m_Holes[i].GetPerimeter();
	}

	return dResult;
}




/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::GetRoutes
\brief GetRoutes
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::GetRoutes(const C2DHoledPolyBase& Poly1, bool bP1RoutesInside, 
				const C2DHoledPolyBase& Poly2, bool bP2RoutesInside, 
				C2DLineBaseSetSet& Routes1, C2DLineBaseSetSet& Routes2, 
				C2DPolyBaseSet& CompleteHoles1, C2DPolyBaseSet& CompleteHoles2)
{
		const C2DPolyBase* pRim1 = Poly1.GetRim();
		const C2DPolyBase* pRim2 = Poly2.GetRim();

		if (pRim1 == 0 || pRim2 == 0)
		{
			assert(0);
			return;
		}

		if (pRim1->GetLineCount() == 0 || pRim2->GetLineCount() == 0)
		{
			assert(0);
			return;
		}

		C2DPointSet IntPointsTemp, IntPointsRim1, IntPointsRim2;
		CIndexSet	IndexesRim1, IndexesRim2;

		std::vector<C2DPointSet* > IntPoints1AllHoles, IntPoints2AllHoles;
		std::vector<CIndexSet*> Indexes1AllHoles, Indexes2AllHoles;

		unsigned int usP1Holes = Poly1.GetHoleCount();
		unsigned int usP2Holes = Poly2.GetHoleCount();

		// *** Rim Rim Intersections
		pRim1->GetLines().GetIntersections(  pRim2->GetLines(),
			&IntPointsTemp, &IndexesRim1, &IndexesRim2, 
			&pRim1->GetBoundingRect(), &pRim2->GetBoundingRect() );

		IntPointsRim1.AddCopy( IntPointsTemp );
		IntPointsRim2 << IntPointsTemp;

		// *** Rim Hole Intersections
		for (unsigned int i = 0 ; i < usP2Holes; i++)
		{
			assert(IntPointsTemp.size() == 0);

			IntPoints2AllHoles.push_back(new C2DPointSet());
			Indexes2AllHoles.push_back( new CIndexSet() );

			if (pRim1->GetBoundingRect().Overlaps( Poly2.GetHole(i)->GetBoundingRect() ))
			{
				pRim1->GetLines().GetIntersections(  Poly2.GetHole(i)->GetLines(),
						&IntPointsTemp, &IndexesRim1, Indexes2AllHoles[i],
						&pRim1->GetBoundingRect(), &Poly2.GetHole(i)->GetBoundingRect());
				
				IntPointsRim1.AddCopy( IntPointsTemp);
				*IntPoints2AllHoles[i] << IntPointsTemp;
			}
		}
		// *** Rim Hole Intersections
		for (unsigned int j = 0 ; j < usP1Holes; j++)
		{
			assert(IntPointsTemp.size() == 0);

			IntPoints1AllHoles.push_back( new C2DPointSet());
			Indexes1AllHoles.push_back( new CIndexSet());

			if (pRim2->GetBoundingRect().Overlaps( Poly1.GetHole(j)->GetBoundingRect() ))
			{
				pRim2->GetLines().GetIntersections(  Poly1.GetHole(j)->GetLines(),
						&IntPointsTemp, &IndexesRim2, Indexes1AllHoles[j],
						&pRim2->GetBoundingRect(), &Poly1.GetHole(j)->GetBoundingRect());
				
				IntPointsRim2.AddCopy( IntPointsTemp);
				*IntPoints1AllHoles[j] << IntPointsTemp;
			}

		}

		// *** Quick Escape
		bool bRim1StartInPoly2 = Poly2.Contains( pRim1->GetLine(0)->GetPointFrom() );
		bool bRim2StartInPoly1 = Poly1.Contains( pRim2->GetLine(0)->GetPointFrom() );

		if (IntPointsRim1.size() != 0 || IntPointsRim2.size() != 0 ||
					bRim1StartInPoly2 || bRim2StartInPoly1			)	
			// pos no interaction
		{	
			// *** Rim Routes
			pRim1->GetRoutes( IntPointsRim1, IndexesRim1, Routes1, 
										bRim1StartInPoly2, bP1RoutesInside);
			pRim2->GetRoutes( IntPointsRim2, IndexesRim2, Routes2,
										bRim2StartInPoly1, bP2RoutesInside);

			if( IntPointsRim1.size() % 2 != 0)	// Must be even
			{
				CGrid::LogDegenerateError();
			}

			if( IntPointsRim2.size() % 2 != 0)	// Must be even
			{
				CGrid::LogDegenerateError();
			}

			// *** Hole Hole Intersections
			for (unsigned int h = 0 ; h < usP1Holes; h++)
			{
				for (unsigned int k = 0 ; k < usP2Holes; k++)
				{
					assert(IntPointsTemp.size() == 0);	
					const C2DPolyBase* pHole1 = Poly1.GetHole(h);
					const C2DPolyBase* pHole2 = Poly2.GetHole(k);

					if ( pHole1->GetBoundingRect().Overlaps( pHole2->GetBoundingRect()) )
					{
						pHole1->GetLines().GetIntersections( pHole2->GetLines(), 
							&IntPointsTemp, Indexes1AllHoles[h], Indexes2AllHoles[k],
							&pHole1->GetBoundingRect(), &pHole2->GetBoundingRect());

						IntPoints1AllHoles[h]->AddCopy( IntPointsTemp);
						*IntPoints2AllHoles[k] << IntPointsTemp;
					}
				}
			}


			// *** Hole Routes
			for (unsigned int a = 0 ; a < usP1Holes; a++)
			{
				const C2DPolyBase* pHole = Poly1.GetHole(a);
				
				if ( IntPoints1AllHoles[a]->size() % 2 != 0)	// Must be even
				{
					CGrid::LogDegenerateError();
				}

				if (pHole->GetLineCount() != 0)
				{
					bool bHole1StartInside = Poly2.Contains( pHole->GetLine(0)->GetPointFrom() );
					if ( IntPoints1AllHoles[a]->size() == 0)
					{
						if ( bHole1StartInside == bP1RoutesInside)
							CompleteHoles1.Add( new C2DPolyBase(*pHole) );
					}
					else
					{
						pHole->GetRoutes( *IntPoints1AllHoles[a], *Indexes1AllHoles[a], Routes1, 
										bHole1StartInside, bP1RoutesInside);
					}
				}
			}
			// *** Hole Routes	
			for (unsigned int b = 0 ; b < usP2Holes; b++)
			{
				const C2DPolyBase* pHole = Poly2.GetHole(b);

				if ( IntPoints2AllHoles[b]->size() % 2 != 0)	// Must be even
				{
					CGrid::LogDegenerateError();
				}

				if (pHole->GetLineCount() != 0)
				{
					bool bHole2StartInside = Poly1.Contains( pHole->GetLine(0)->GetPointFrom() );
					if ( IntPoints2AllHoles[b]->size() == 0)
					{
						if ( bHole2StartInside == bP2RoutesInside)
							CompleteHoles2.Add( new C2DPolyBase(* pHole) );
					}
					else
					{
						pHole->GetRoutes( *IntPoints2AllHoles[b], *Indexes2AllHoles[b], Routes2, 
											bHole2StartInside, bP2RoutesInside);
					}
				}
			}	
		}


		for (unsigned int i = 0 ; i < IntPoints1AllHoles.size(); i++)
			delete IntPoints1AllHoles[i];
		for (unsigned int i = 0 ; i < IntPoints2AllHoles.size(); i++)
			delete IntPoints2AllHoles[i];
		for (unsigned int i = 0 ; i < Indexes1AllHoles.size(); i++)
			delete Indexes1AllHoles[i];
		for (unsigned int i = 0 ; i < Indexes2AllHoles.size(); i++)
			delete Indexes2AllHoles[i];

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Overlaps
\brief Overlaps
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBase::Overlaps(const C2DHoledPolyBase& Other) const
{
	if (m_Rim == 0)
		return false;

	if (Other.GetRim() == 0)
		return false;

	if (!Overlaps(*Other.GetRim()))
		return false;

	for (unsigned int i = 0 ; i < Other.GetHoleCount(); i++)
	{
		if (Other.GetHole(i)->Contains( *m_Rim))
			return false;
	}

	return true;

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Overlaps
\brief Overlaps
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBase::Overlaps(const C2DPolyBase& Other) const
{
	if (m_Rim == 0)
		return false;

	if (!m_Rim->Overlaps(Other))
		return false;

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		if (m_Holes[i].Contains(Other))
			return false;
	}

	return true;
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::GetUnion
\brief GetUnion
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::GetUnion(const C2DHoledPolyBase& Other, 
			C2DHoledPolyBaseSet& HoledPolys, CGrid::eDegenerateHandling eDegen) const
{
	GetBoolean(Other, HoledPolys,false , false, eDegen);
 }




/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::PolygonsToHoledPolygons
\brief PolygonsToHoledPolygons
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::PolygonsToHoledPolygons(C2DHoledPolyBaseSet& HoledPolys,
				C2DPolyBaseSet& Polygons)
{

	C2DPolyBaseSet Unmatched;
	C2DHoledPolyBaseSet NewHoledPolys;

	for (int i = Polygons.size() - 1 ; i >= 0  ; i--)
	{
		bool bMatched = false;

		C2DPolyBase* pPoly = Polygons.ExtractAt(i);

		// Cycle through the newly created polygons to see if it's a hole.
		for (unsigned int p = 0; p < NewHoledPolys.size(); p++)
		{
			if (  NewHoledPolys.GetAt(p)->GetRim()->Contains(  pPoly->GetLine(0)->GetPointFrom() ))
			{
				NewHoledPolys.GetAt(p)->AddHoleDirect(pPoly);
				bMatched = true;
				break;
			}
		}
		// If its not then compare it to all the other unknowns.
		if (!bMatched)
		{
			unsigned int u = 0; 

			bool bKnownRim = false;

			while (u < Unmatched.size())
			{
				if (  !bKnownRim && Unmatched.GetAt(u)->Contains(  pPoly->GetLine(0)->GetPointFrom() ))
				{
					// This is a hole.
					NewHoledPolys.Add(new C2DHoledPolyBase);
					NewHoledPolys.GetLast()->SetRimDirect( Unmatched.ExtractAt(u));
					NewHoledPolys.GetLast()->AddHoleDirect(pPoly);
					bMatched = true;
					break;

				}
				else if ( pPoly->Contains(  Unmatched.GetAt(u)->GetLine(0)->GetPointFrom() ))
				{
				//	int nCount = OverlapPolygons->GetCount();
					// This is a rim.
					if (!bKnownRim)
					{
						// If we haven't alreay worked this out then record that its a rim
						// and set up the new polygon.
						bKnownRim = true;
						NewHoledPolys.Add(new C2DHoledPolyBase);
						NewHoledPolys.GetLast()->SetRimDirect( pPoly);
						NewHoledPolys.GetLast()->AddHoleDirect(  Unmatched.ExtractAt(u) );
					}
					else
					{
						// We already worked out this was a rim so it must be the last polygon.
						NewHoledPolys.GetLast()->AddHoleDirect(  Unmatched.ExtractAt(u) );
					}
					// Record that its been matched.
					bMatched = true;
				}
				else
				{
					// Only if there was no match do we increment the counter.
					u++;
				}
			}		
		}

		if (!bMatched)
		{
			Unmatched.Add(pPoly);
		}
	}


	HoledPolys << Unmatched;
	
	HoledPolys << NewHoledPolys;
}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::RandomPerturb <BR>
\brief Perturbs the shape by a very small random amount so as to avoid degeneracies
with another polygon caused by coincident lines or points.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::RandomPerturb(void)
{
	if (m_Rim == 0)
		return;

	C2DPoint pt = m_Rim->GetBoundingRect().GetPointFurthestFromOrigin();
	double dMinEq = max(pt.x, pt.y) * conEqualityTolerance;
	CRandomNumber rn(dMinEq * 10, dMinEq * 100);

	C2DVector cVector( rn.Get(), rn.Get() );
	if (CRandomNumber::GetBool())
		cVector.i = - cVector.i ;
	if (CRandomNumber::GetBool())
		cVector.j = - cVector.j ;

	Move( cVector );
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::GetOverlaps
\brief GetOverlaps
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::GetOverlaps(const C2DHoledPolyBase& Other, 
		C2DHoledPolyBaseSet& HoledPolys, CGrid::eDegenerateHandling eDegen) const
{
	GetBoolean(Other, HoledPolys, true, true, eDegen);

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::GetNonOverlaps
\brief GetNonOverlaps
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::GetNonOverlaps(const C2DHoledPolyBase& Other, 
		C2DHoledPolyBaseSet& HoledPolys, CGrid::eDegenerateHandling eDegen) const
{
	GetBoolean(Other, HoledPolys, false, true, eDegen);
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Crosses
\brief Crosses
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBase::Crosses(const C2DLineBase& Line) const
{
	if (m_Rim == 0)
		return false;

	if (m_Rim->Crosses(Line) )
		return true;

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		if (m_Holes[i].Crosses(Line))
			return true;
	}

	return false;
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Crosses
\brief Crosses
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBase::Crosses(const C2DLineBase& Line, 
				C2DPointSet* IntersectionPts) const
{
	if (m_Rim == 0)
		return false;

	C2DPointSet IntPts;

	m_Rim->Crosses(Line, &IntPts);

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		m_Holes[i].Crosses(Line, &IntPts);
	}
	bool bResult = (IntPts.size() != 0);
	if (IntersectionPts)
		*IntersectionPts << IntPts;

	return (bResult);
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::Crosses
\brief Crosses
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBase::Crosses(const C2DPolyBase& Poly) const
{
	if (m_Rim == 0)
		return false;

	if (m_Rim->Crosses(Poly))
		return true;

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		if(m_Holes[i].Crosses(Poly))
			return true;
	}
	return false;
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::CrossesRay
\brief CrossesRay
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBase::CrossesRay(const C2DLine& Ray, 
				C2DPointSet* IntersectionPts) const
{
	if (m_Rim == 0)
		return false;

	IntersectionPts->DeleteAll();  //Could remove

	C2DPointSet IntPts;

	m_Rim->CrossesRay(Ray, &IntPts);

	(*IntersectionPts) << IntPts;

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		if (m_Holes[i].CrossesRay(Ray, &IntPts))
		{
			double dDist = Ray.point.Distance(IntPts[0]);
			unsigned int nInsert = 0;

			while (nInsert < IntersectionPts->size() && 
				Ray.point.Distance((*IntersectionPts)[nInsert]) < dDist)
			{
				nInsert++;
			}

			IntersectionPts->InsertAt(nInsert, IntPts);
		}
	}

	return (IntersectionPts->size() > 0);
}



/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::SnapToGrid
\brief Snap To Grid
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::SnapToGrid(void)
{
	if (m_Rim != 0)
	{
		m_Rim->SnapToGrid();
	}

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		m_Holes[i].SnapToGrid();
	}
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBase::GetBoolean
\brief GetBoolean
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::GetBoolean(const C2DHoledPolyBase& Other, C2DHoledPolyBaseSet& HoledPolys,
						bool bThisInside, bool bOtherInside, 
						CGrid::eDegenerateHandling eDegen ) const
{
	if (m_Rim == 0)
		return;
	
	if (Other.GetRim() == 0)
		return;

	if (m_Rim->GetLineCount() == 0 || Other.GetRim()->GetLineCount() == 0 )
		return;

	if (m_Rim->GetBoundingRect().Overlaps(Other.GetRim()->GetBoundingRect() ))
	{
		switch (eDegen)
		{
		case CGrid::None:
			{
				C2DPolyBaseSet CompleteHoles1,CompleteHoles2;
				C2DLineBaseSetSet Routes1, Routes2;
				GetRoutes( *this, bThisInside, Other, bOtherInside, Routes1, Routes2,
								CompleteHoles1, CompleteHoles2);

				Routes1 << Routes2;

				if (Routes1.size() > 0)
				{
					Routes1.MergeJoining();

					C2DPolyBaseSet Polygons;

					for (int i = Routes1.size() - 1; i >= 0; i--)
					{
						C2DLineBaseSet* pRoute = Routes1.GetAt(i);
						if (pRoute->IsClosed() )
						{
							Polygons.Add(new C2DPolyBase);
							Polygons.GetLast()->CreateDirect( *pRoute);
						}
						else
						{
							CGrid::LogDegenerateError();
						}	
					}

					C2DHoledPolyBaseSet NewComPolys;
					
					PolygonsToHoledPolygons(NewComPolys, Polygons);

					NewComPolys.AddKnownHoles( CompleteHoles1 );

					NewComPolys.AddKnownHoles( CompleteHoles2 );

					if ( !bThisInside && !bOtherInside && NewComPolys.size() != 1)
					{
						CGrid::LogDegenerateError();
					}

					HoledPolys << NewComPolys;
				}
			}
			break;
		case CGrid::RandomPerturbation:
			{
				C2DHoledPolyBase OtherCopy(Other);
				OtherCopy.RandomPerturb();
				GetBoolean( OtherCopy, HoledPolys, bThisInside, bOtherInside , CGrid::None );
			}
			break;
		case CGrid::DynamicGrid:
			{
				C2DRect Rect; 
				if (m_Rim->GetBoundingRect().Overlaps(Other.GetRim()->GetBoundingRect(), Rect))
				{
					double dOldGrid = CGrid::GetGridSize();
					CGrid::SetToMinGridSize(Rect);
					GetBoolean( Other, HoledPolys, bThisInside, bOtherInside , CGrid::PreDefinedGrid );
					CGrid::SetGridSize(dOldGrid);
				}
			}
			break;
		case CGrid::PreDefinedGrid:
			{
				C2DHoledPolyBase P1(*this), P2(Other);
				P1.SnapToGrid();
				P2.SnapToGrid();
				C2DVector V1( P1.GetRim()->GetBoundingRect().GetTopLeft(),  P2.GetRim()->GetBoundingRect().GetTopLeft());
				double dPerturbation = CGrid::GetGridSize(); // ensure it snaps back to original grid positions.
				V1.i > 0 ? V1.i = dPerturbation: V1.i = -dPerturbation;	// move away slightly if possible
				V1.j > 0 ? V1.j = dPerturbation: V1.j = -dPerturbation; // move away slightly if possible
				V1.i *= 0.411923;// ensure it snaps back to original grid positions.
				V1.j *= 0.313131;// ensure it snaps back to original grid positions.
				
				P2.Move( V1 );
				P1.GetBoolean( P2, HoledPolys, bThisInside, bOtherInside , CGrid::None );

				HoledPolys.SnapToGrid();	
			}
			break;
		case CGrid::PreDefinedGridPreSnapped:
			{
				C2DHoledPolyBase P2(Other);
				C2DVector V1( m_Rim->GetBoundingRect().GetTopLeft(),  P2.GetRim()->GetBoundingRect().GetTopLeft());
				double dPerturbation = CGrid::GetGridSize(); 
				V1.i > 0 ? V1.i = dPerturbation: V1.i = -dPerturbation; // move away slightly if possible
				V1.j > 0 ? V1.j = dPerturbation: V1.j = -dPerturbation; // move away slightly if possible
				V1.i *= 0.411923; // ensure it snaps back to original grid positions.
				V1.j *= 0.313131;// ensure it snaps back to original grid positions.
				P2.Move( V1 );
				GetBoolean( P2, HoledPolys, bThisInside, bOtherInside , CGrid::None );
				HoledPolys.SnapToGrid();	
			}
			break;
		}// switch
	}
}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::IsValidArcs <BR>
\brief IsValidArcs
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBase::IsValidArcs(void) const
{
	const C2DPolyBase* pPoly = this->GetRim();

	if (pPoly && !pPoly->IsValidArcs())
		return false;

	for (unsigned int i = 0; i < m_Holes.size(); i++)
	{
		if (!m_Holes.GetAt(i)->IsValidArcs())
			return false;
	}

	return true;
}



/**--------------------------------------------------------------------------<BR>
C2DPolyBase::MakeValidArcs <BR>
\brief MakeValidArcs
<P>---------------------------------------------------------------------------*/
unsigned int C2DHoledPolyBase::MakeValidArcs(void)
{
	unsigned int nResult = 0;

	C2DPolyBase* pPoly = this->GetRim();

	if (pPoly)
		nResult += pPoly->MakeValidArcs();

	for (unsigned int i = 0; i < m_Holes.size(); i++)
	{
		nResult += m_Holes.GetAt(i)->MakeValidArcs();
	}

	return nResult;
}

/**--------------------------------------------------------------------------<BR>
C2DPolyBase::HasArcs <BR>
\brief HasArcs
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBase::HasArcs(void) const
{
	const C2DPolyBase* pPoly = this->GetRim();

	if (pPoly && pPoly->HasArcs())
		return true;

	for (unsigned int i = 0; i < m_Holes.size(); i++)
	{
		if (m_Holes.GetAt(i)->HasArcs())
			return true;
	}

	return false;
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBase::RemoveNullLines <BR>
\brief Removes all lines whose end is the same is the start. Returns the number found.
<P>---------------------------------------------------------------------------*/
unsigned int C2DHoledPolyBase::RemoveNullLines(void)
{
	unsigned int nResult = 0;

	C2DPolyBase* pPoly = this->GetRim();

	if (pPoly)
		nResult += pPoly->MakeValidArcs();

	for (unsigned int i = 0; i < m_Holes.size(); i++)
	{
		nResult += m_Holes.GetAt(i)->MakeValidArcs();
	}

	return nResult;
}



/**--------------------------------------------------------------------------<BR>
C2DPolyArc::MakeClockwise <BR>
\brief Makes the order Clockwise.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::MakeClockwise(void)
{
	C2DPolyBase* pPoly = this->GetRim();

	if (pPoly)
		pPoly->MakeClockwise();

	for (unsigned int i = 0; i < m_Holes.size(); i++)
	{
		m_Holes.GetAt(i)->MakeClockwise();
	}
}


/**--------------------------------------------------------------------------<BR>
C2DPolyArc::Transform <BR>
\brief Transform by the given operator.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::Transform(CTransformation* pProject)
{
	C2DPolyBase* pPoly = this->GetRim();

	if (pPoly)
		pPoly->Transform(pProject);

	for (unsigned int i = 0; i < m_Holes.size(); i++)
	{
		m_Holes.GetAt(i)->Transform(pProject);
	}
}

/**--------------------------------------------------------------------------<BR>
C2DPolyArc::InverseTransform <BR>
\brief Transform by the given operator.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBase::InverseTransform(CTransformation* pProject)
{
	C2DPolyBase* pPoly = this->GetRim();

	if (pPoly)
		pPoly->InverseTransform(pProject);

	for (unsigned int i = 0; i < m_Holes.size(); i++)
	{
		m_Holes.GetAt(i)->InverseTransform(pProject);
	}
}
