/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DHoledPolyArc.cpp
\brief Implementation file for the C2DHoledPolyArc Class.

Implementation file for C2DHoledPolyArc, a class which represents a curved 
polygon with holes.
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "C2DHoledPolyArc.h"
#include "C2DHoledPolyBaseSet.h"
#include "C2DHoledPolyArcSet.h"
#include "C2DPolyArc.h"
#include "C2DVector.h"


_MEMORY_POOL_IMPLEMENATION(C2DHoledPolyArc)

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::C2DHoledPolyArc
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DHoledPolyArc::C2DHoledPolyArc(void)
{
	m_Type = PolyArcHoled;
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::C2DHoledPolyArc
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DHoledPolyArc::C2DHoledPolyArc(const C2DHoledPolyBase& Other)
{
	m_Rim = new C2DPolyArc( *Other.GetRim());

	for (unsigned int i = 0 ; i < Other.GetHoleCount(); i++)
	{
		AddHoleDirect( new C2DPolyArc( *Other.GetHole(i))  );
	}

	m_Type = PolyArcHoled;
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::C2DHoledPolyArc
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DHoledPolyArc::C2DHoledPolyArc(const C2DHoledPolyArc& Other)
{
	this->SetRim(*Other.GetRim());

	for (unsigned int i = 0 ; i < Other.GetHoleCount(); i++)
	{
		AddHole( *Other.GetHole(i) );
	}

	m_Type = PolyArcHoled;
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::C2DHoledPolyArc
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DHoledPolyArc::~C2DHoledPolyArc(void)
{
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::SetRim
\brief Sets the rim. Calls base class.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArc::SetRim(const C2DPolyArc& Polygon)
{
	C2DHoledPolyBase::SetRimDirect( new C2DPolyArc( Polygon ) );
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::SetRim
\brief Adds a hole. Calls base class.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArc::AddHole(const C2DPolyArc& Polygon)
{

	C2DHoledPolyBase::AddHoleDirect( new C2DPolyArc( Polygon )  );
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::SetHole
\brief Sets a hole. Calls base class.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArc::SetHole(const C2DPolyArc& Polygon, unsigned int usIndex)
{
	C2DHoledPolyBase::SetHoleDirect( new C2DPolyArc( Polygon ) , usIndex);
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::SetRimDirect
\brief Sets the rim using a pointer to a polygon. 
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArc::SetRimDirect(C2DPolyArc* Polygon)
{
	C2DHoledPolyBase::SetRimDirect( Polygon );
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::AddHoleDirect
\brief Sets a hole using a pointer to a polygon. 
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArc::AddHoleDirect(C2DPolyArc* Polygon)
{
	C2DHoledPolyBase::AddHoleDirect(  Polygon );

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::SetHoleDirect
\brief Sets the hole using a pointer to a polygon. 
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArc::SetHoleDirect(C2DPolyArc* Polygon, unsigned int usIndex)
{
	C2DHoledPolyBase::SetHoleDirect(  Polygon , usIndex );
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::RemoveHole
\brief Removes a hole according to the index.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArc::RemoveHole(unsigned int usIndex)
{
	C2DHoledPolyBase::RemoveHole( usIndex );

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::GetRim
\brief Returns the rim.
<P>---------------------------------------------------------------------------*/
C2DPolyArc* C2DHoledPolyArc::GetRim(void) 
{
	return dynamic_cast<C2DPolyArc*>(m_Rim);
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::GetRim
\brief Returns the rim.
<P>---------------------------------------------------------------------------*/
const C2DPolyArc* C2DHoledPolyArc::GetRim(void) const 
{
	return dynamic_cast<C2DPolyArc*>(m_Rim);
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::GetHole
\brief Returns the hole.
<P>---------------------------------------------------------------------------*/
C2DPolyArc* C2DHoledPolyArc::GetHole(unsigned int usIndex)
{
	return dynamic_cast<C2DPolyArc*>( C2DHoledPolyBase::GetHole( usIndex ) );
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::GetHole
\brief Returns the hole.
<P>---------------------------------------------------------------------------*/
const C2DPolyArc* C2DHoledPolyArc::GetHole(unsigned int usIndex) const 
{
	return dynamic_cast<const C2DPolyArc*>( C2DHoledPolyBase::GetHole( usIndex )  );

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::ExtractRim
\brief Extract the rim.
<P>---------------------------------------------------------------------------*/
C2DPolyArc* C2DHoledPolyArc::ExtractRim(void)
{
	return dynamic_cast<C2DPolyArc*>(  C2DHoledPolyBase::ExtractRim()  );
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::ExtractRim
\brief Extract the hole.
<P>---------------------------------------------------------------------------*/
C2DPolyArc* C2DHoledPolyArc::ExtractHole(unsigned int usIndex)
{
	return dynamic_cast<C2DPolyArc*>( C2DHoledPolyBase::ExtractHole( usIndex ) );
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::GetArea
\brief Returns the area.
<P>---------------------------------------------------------------------------*/
double C2DHoledPolyArc::GetArea(void) const
{
	double dResult = 0;

	const C2DPolyArc* pRim = GetRim();

	if (pRim)
		dResult += pRim->GetArea();

	for (unsigned int i = 0 ; i < m_Holes.size(); i++)
	{
		const C2DPolyArc* pHole = GetHole(i);
		dResult -= pHole->GetArea();
	}
	return dResult;


}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArc::GetCentroid
\brief Returns the centroid.
<P>---------------------------------------------------------------------------*/
C2DPoint C2DHoledPolyArc::GetCentroid(void) const
{
	// Find the centroid and area of the straight line polygon.

	if (!m_Rim)
		return C2DPoint(0, 0);

	
	C2DPoint Centroid = GetRim()->GetCentroid();
	double dArea = GetRim()->GetArea();

	for (unsigned int i = 0; i < m_Holes.size(); i++)
	{
			C2DVector vec( Centroid, GetHole(i)->GetCentroid());

			double dHoleArea = GetHole(i)->GetArea();

			double dFactor =  dHoleArea / (dHoleArea + dArea);	

			vec *= dFactor;
			Centroid += vec;
			dArea += dHoleArea;

	}


	return Centroid;

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetOverlaps
\brief Returns the overlaps between this and the other complex polygon.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArc::GetOverlaps(const C2DHoledPolyArc& Other, C2DHoledPolyArcSet& HoledPolys, 
						CGrid::eDegenerateHandling eDegen) const
{
	C2DHoledPolyBaseSet BaseSet;

	C2DHoledPolyBase::GetOverlaps( Other, BaseSet, eDegen);

	for (unsigned int i = 0; i < BaseSet.size(); i++)
	{
		HoledPolys.Add(new C2DHoledPolyArc( BaseSet[i]) );
	}
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetOverlaps
\brief Returns the overlaps between this and the other complex polygon.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArc::GetOverlaps(const C2DHoledPolyArc& Other, C2DHoledPolyBaseSet& HoledPolys, 
						CGrid::eDegenerateHandling eDegen) const
{
	C2DHoledPolyBase::GetOverlaps( Other, HoledPolys, eDegen);
}



/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetNonOverlaps
\brief Returns the difference between this and the other complex polygon.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArc::GetNonOverlaps(const C2DHoledPolyArc& Other, C2DHoledPolyArcSet& HoledPolys, 
						CGrid::eDegenerateHandling eDegen) const
{
	C2DHoledPolyBaseSet BaseSet;

	C2DHoledPolyBase::GetNonOverlaps( Other, BaseSet, eDegen);

	for (unsigned int i = 0; i < BaseSet.size(); i++)
	{
		HoledPolys.Add(new C2DHoledPolyArc( BaseSet[i]) );
	}
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetNonOverlaps
\brief Returns the difference between this and the other complex polygon.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArc::GetNonOverlaps(const C2DHoledPolyArc& Other, C2DHoledPolyBaseSet& HoledPolys, 
						CGrid::eDegenerateHandling eDegen) const
{
	C2DHoledPolyBase::GetNonOverlaps( Other, HoledPolys, eDegen);
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetUnion
\brief Returns the union of this and the other. complex polygon.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArc::GetUnion(const C2DHoledPolyArc& Other, C2DHoledPolyArcSet& HoledPolys,
				CGrid::eDegenerateHandling eDegen) const
{
	C2DHoledPolyBaseSet BaseSet;

	C2DHoledPolyBase::GetUnion( Other, BaseSet, eDegen);

	for (unsigned int i = 0; i < BaseSet.size(); i++)
	{
		HoledPolys.Add(new C2DHoledPolyArc( BaseSet[i]) );

	}
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygon::GetUnion
\brief Returns the union of this and the other. complex polygon.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArc::GetUnion(const C2DHoledPolyArc& Other, C2DHoledPolyBaseSet& HoledPolys,
					CGrid::eDegenerateHandling eDegen) const
{
	C2DHoledPolyBase::GetUnion( Other, HoledPolys, eDegen);
}



