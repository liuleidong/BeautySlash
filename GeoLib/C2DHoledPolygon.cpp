/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DHoledPolygon.cpp
\brief Implementation file for the C2DHoledPolygon Class.

Implementation file for C2DHoledPolygon, a class which represents a polygon 
with holes.
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "C2DHoledPolygon.h"
#include "C2DHoledPolyBaseSet.h"
#include "C2DHoledPolygonSet.h"
#include "C2DPolygon.h"
#include "C2DVector.h"

_MEMORY_POOL_IMPLEMENATION(C2DHoledPolygon)

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::C2DHoledPolygon
\brief Constructor
<P>---------------------------------------------------------------------------*/
C2DHoledPolygon::C2DHoledPolygon(void) 
{
	m_Rim = 0;
	m_Type = PolyLineHoled;
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::C2DHoledPolygon
\brief Copy constructor
<P>---------------------------------------------------------------------------*/
C2DHoledPolygon::C2DHoledPolygon(const C2DHoledPolygon& Other) 
{
	this->SetRim(*Other.GetRim());

	for (unsigned int i = 0 ; i < Other.GetHoleCount(); i++)
	{
		AddHole( *Other.GetHole(i) );
	}

	m_Type = PolyLineHoled;
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::C2DHoledPolygon
\brief Constructor
<P>---------------------------------------------------------------------------*/
C2DHoledPolygon::C2DHoledPolygon(const C2DHoledPolyBase& Other)
{
	m_Rim = new C2DPolygon( *Other.GetRim());

	for (unsigned int i = 0 ; i < Other.GetHoleCount(); i++)
	{
		AddHoleDirect( new C2DPolygon( *Other.GetHole(i))  );
	}

	m_Type = PolyLineHoled;
}



/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::C2DHoledPolygon
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DHoledPolygon::~C2DHoledPolygon(void)
{
	Clear();
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::SetRim
\brief Sets the rim. Calls base class.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::SetRim(const C2DPolygon& Polygon)
{
	C2DHoledPolyBase::SetRimDirect( new C2DPolygon( Polygon ) );
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::SetRim
\brief Adds a hole. Calls base class.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::AddHole(const C2DPolygon& Polygon)
{
	C2DHoledPolyBase::AddHoleDirect(  new C2DPolygon( Polygon ) );
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::SetHole
\brief Sets a hole. Calls base class.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::SetHole(const C2DPolygon& Polygon, unsigned int usIndex)
{
	if (usIndex < m_Holes.size())
	{
		C2DPolygon* pPoly = dynamic_cast<C2DPolygon*> (m_Holes.GetAt(usIndex));
		if ( pPoly != 0)
		{
			pPoly->Set(Polygon);
		}
	}
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::SetRimDirect
\brief Sets the rim using a pointer to a polygon. 
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::SetRimDirect(C2DPolygon* Polygon)
{
	C2DHoledPolyBase::SetRimDirect( Polygon );
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::AddHoleDirect
\brief Sets a hole using a pointer to a polygon. 
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::AddHoleDirect(C2DPolygon* Polygon)
{
	C2DHoledPolyBase::AddHoleDirect(  Polygon );

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::SetHoleDirect
\brief Sets the hole using a pointer to a polygon. 
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::SetHoleDirect(C2DPolygon* Polygon, unsigned int usIndex)
{
	C2DHoledPolyBase::SetHoleDirect(  Polygon , usIndex );
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::RemoveHole
\brief Removes a hole according to the index.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::RemoveHole(unsigned int usIndex)
{
	C2DHoledPolyBase::RemoveHole( usIndex );

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetRim
\brief Rim access.
<P>---------------------------------------------------------------------------*/
C2DPolygon* C2DHoledPolygon::GetRim(void) 
{
	return dynamic_cast<C2DPolygon*>(m_Rim);
}
/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetRim
\brief Rim access.
<P>---------------------------------------------------------------------------*/
const C2DPolygon* C2DHoledPolygon::GetRim(void) const 
{
	return dynamic_cast<C2DPolygon*>(m_Rim);
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetHole
\brief Returns the hole.
<P>---------------------------------------------------------------------------*/
C2DPolygon* C2DHoledPolygon::GetHole(unsigned int usIndex)
{
	return dynamic_cast<C2DPolygon*>( C2DHoledPolyBase::GetHole( usIndex ) );
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetHole
\brief Returns the hole.
<P>---------------------------------------------------------------------------*/
const C2DPolygon* C2DHoledPolygon::GetHole(unsigned int usIndex) const 
{
	return dynamic_cast<const C2DPolygon*>( C2DHoledPolyBase::GetHole( usIndex )  );

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::ExtractRim
\brief Extract the rim.
<P>---------------------------------------------------------------------------*/
C2DPolygon* C2DHoledPolygon::ExtractRim(void)
{
	return dynamic_cast<C2DPolygon*>(  C2DHoledPolyBase::ExtractRim()  );
}



/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::ExtractHole
\brief Returns the hole and removes it from the collection of holes. 
<P>---------------------------------------------------------------------------*/
C2DPolygon* C2DHoledPolygon::ExtractHole(unsigned int usIndex)
{
	return dynamic_cast<C2DPolygon*>( C2DHoledPolyBase::ExtractHole( usIndex ) );
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::RotateToRight
\brief Rotates to the right using the centroid as the origin.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::RotateToRight(double dAng)
{
	C2DHoledPolyBase::RotateToRight(dAng, GetCentroid());
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::RotateToRight
\brief Rotates to the right.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::RotateToRight(double dAng, const C2DPoint& Origin)
{
	C2DHoledPolyBase::RotateToRight(dAng, Origin);

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::Grow
\brief Grows by the factor given around the centroid.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::Grow(double dFactor)
{
	C2DHoledPolyBase::Grow( dFactor , GetCentroid() );
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::Grow
\brief Grows by the factor given around the origin provided.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::Grow(double dFactor, const C2DPoint& Origin)
{
	C2DHoledPolyBase::Grow(dFactor, Origin);

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::operator=
\brief Assignment.
<P>---------------------------------------------------------------------------*/
const C2DHoledPolygon& C2DHoledPolygon::operator=(C2DHoledPolygon& Other)
{
	Clear();

	m_Rim = new C2DPolygon;
	(*m_Rim) = *Other.GetRim();

	for (unsigned int i = 0 ; i < Other.GetHoleCount(); i++)
	{
		m_Holes.Add(new C2DPolygon(*Other.GetHole(i)) );
	}

	return *this;
}




/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::Overlaps
\brief True if the rims overlap.
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolygon::Overlaps(const C2DHoledPolygon& Other) const
{
	if (m_Rim == 0)
		return false;

	if (Other.GetRim() == 0)
		return false;

	return m_Rim->Overlaps(*Other.GetRim());

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetCentroid
\brief Calculates the centroid of the polygon by moving it according to each holes
weighted centroid.
<P>---------------------------------------------------------------------------*/
C2DPoint C2DHoledPolygon::GetCentroid(void)
{
	C2DPoint HoleCen(0, 0);

	if (m_Rim == 0)
		return HoleCen;

	const C2DPolygon* pRim = GetRim();

	if (m_Holes.size() == 0)
		return pRim->GetCentroid();

	C2DPoint PolyCen = pRim->GetCentroid();

	double dPolyArea = pRim->GetArea();
	double dHoleArea = 0;

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		const C2DPolygon* pHole  = GetHole(i);
		dHoleArea += pHole->GetArea();
	}


	if (dHoleArea == 0 || dHoleArea == dPolyArea)
		return pRim->GetCentroid();
	else
	{
		for (unsigned int i = 0 ; i < m_Holes.size(); i++)
		{
			const C2DPolygon* pHole  = GetHole(i);
			HoleCen += pHole->GetCentroid() * (pHole->GetArea() / dHoleArea);
		}
	}

	C2DVector Vec(HoleCen, PolyCen);

	Vec *= dHoleArea / (dPolyArea - dHoleArea);

	PolyCen.Move(Vec);

	return PolyCen;

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetArea
\brief Calculates the area.
<P>---------------------------------------------------------------------------*/
double C2DHoledPolygon::GetArea() const 
{
	double dResult = 0;

	const C2DPolygon* pRim = GetRim();

	if (pRim)
		dResult += pRim->GetArea();

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		const C2DPolygon* pHole = GetHole(i);
		dResult -= pHole->GetArea();
	}
	return dResult;
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetOverlaps
\brief Returns the overlaps between this and the other complex polygon.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::GetOverlaps(const C2DHoledPolygon& Other, C2DHoledPolygonSet& HoledPolys, 
						CGrid::eDegenerateHandling eDegen) const
{
	C2DHoledPolyBaseSet BaseSet;

	C2DHoledPolyBase::GetOverlaps( Other, BaseSet, eDegen);

	for (unsigned int i = 0; i < BaseSet.size(); i++)
	{
		HoledPolys.Add(new C2DHoledPolygon( BaseSet[i]) );
	}
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetOverlaps
\brief Returns the overlaps between this and the other complex polygon.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::GetOverlaps(const C2DHoledPolygon& Other, C2DHoledPolyBaseSet& HoledPolys, 
						CGrid::eDegenerateHandling eDegen) const
{
	C2DHoledPolyBase::GetOverlaps( Other, HoledPolys, eDegen);
}



/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetNonOverlaps
\brief Returns the difference between this and the other complex polygon.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::GetNonOverlaps(const C2DHoledPolygon& Other, C2DHoledPolygonSet& HoledPolys, 
						CGrid::eDegenerateHandling eDegen) const
{
	C2DHoledPolyBaseSet BaseSet;

	C2DHoledPolyBase::GetNonOverlaps( Other, BaseSet, eDegen);

	for (unsigned int i = 0; i < BaseSet.size(); i++)
	{
		HoledPolys.Add(new C2DHoledPolygon( BaseSet[i]) );
	}
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetNonOverlaps
\brief Returns the difference between this and the other complex polygon.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::GetNonOverlaps(const C2DHoledPolygon& Other, C2DHoledPolyBaseSet& HoledPolys, 
						CGrid::eDegenerateHandling eDegen) const
{
	C2DHoledPolyBase::GetNonOverlaps( Other, HoledPolys, eDegen);
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetUnion
\brief Returns the union of this and the other. complex polygon.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::GetUnion(const C2DHoledPolygon& Other, C2DHoledPolygonSet& HoledPolys,
				CGrid::eDegenerateHandling eDegen) const
{
	C2DHoledPolyBaseSet BaseSet;

	C2DHoledPolyBase::GetUnion( Other, BaseSet, eDegen);

	for (unsigned int i = 0; i < BaseSet.size(); i++)
	{
		HoledPolys.Add(new C2DHoledPolygon( BaseSet[i]) );

	}
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetUnion
\brief Returns the union of this and the other. complex polygon.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygon::GetUnion(const C2DHoledPolygon& Other, C2DHoledPolyBaseSet& HoledPolys,
							   CGrid::eDegenerateHandling eDegen) const
{
	C2DHoledPolyBase::GetUnion( Other, HoledPolys, eDegen);
}



/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::RemoveNullAreas
\brief Removes null areas, will return true if the shape is no longer valid.
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolygon::RemoveNullAreas(double dTolerance)
{
	if (m_Rim == NULL)
		return true;

    if (GetRim()->RemoveNullAreas(dTolerance))
    {
        return true;
    }

    unsigned int i = 0;
    while ( i < m_Holes.size())
    {
        if (GetHole(i)->RemoveNullAreas(dTolerance))
        {
            m_Holes.DeleteAt(i);
        }
        else
        {
            i++;
        }
    }
    return false;
}