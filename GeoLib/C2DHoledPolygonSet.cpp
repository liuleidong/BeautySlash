/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------<BR>
\file 2DHoledPolygonSet.cpp
\brief Implementation file for the C2DHoledPolygonSet Class.

Implementation file for C2DHoledPolygonSet, a collection of holed polygons.
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "C2DHoledPolygonSet.h"
#include "C2DHoledPolyBaseSet.h"
#include "C2DHoledPolygon.h"

_MEMORY_POOL_IMPLEMENATION(C2DHoledPolygonSet)


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygonSet::C2DHoledPolygonSet
\brief Constructor
<P>---------------------------------------------------------------------------*/
C2DHoledPolygonSet::C2DHoledPolygonSet(void)
{
	m_Type = PolyLineHoledSet;

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygonSet::C2DHoledPolygonSet
\brief Destructor
<P>---------------------------------------------------------------------------*/
C2DHoledPolygonSet::~C2DHoledPolygonSet(void)
{

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygonSet::AddCopy
\brief Adds a copy of the other set.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygonSet::AddCopy(const C2DHoledPolygonSet& Other)
{
	for(unsigned int i = 0 ; i < Other.size(); i++)
	{
		AddCopy(Other[i]);
	}
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygonSet::MakeCopy
\brief Makes a copy of the other set.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygonSet::MakeCopy( const C2DHoledPolygonSet& Other)
{
	DeleteAll();
	AddCopy( Other);

}


/**--------------------------------------------------------------------------<BR>
C2DBaseSet::operator<<
\brief Passes ONLY the pointers of this type from the Other into this.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygonSet::operator<<(C2DBaseSet& Other)
{
	C2DBaseSet Temp;

	while (Other.size() > 0)
	{
		C2DBase* pLast = Other.ExtractLast();
		if (pLast->GetType() == C2DBase::PolyLineHoled)
		{
			Add( dynamic_cast<C2DHoledPolygon*>(pLast) );
		}
		else
		{
			Temp << pLast;
		}
	}

	while (Temp.size() > 0)
	{
		Other << Temp.ExtractLast();
	}
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolygonSet::UnifyBasic
\brief Simple Unification. See C2DHoledPolyBaseSet function also.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygonSet::UnifyBasic(void)
{
	C2DHoledPolyBaseSet BaseSet;

	while (size() > 0)
		BaseSet.Add( ExtractLast());

	BaseSet.UnifyBasic();

	for (unsigned int i = 0 ; i <  BaseSet.size(); i++)
	{
		Add(new C2DHoledPolygon( BaseSet[i]));
	}
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygonSet::UnifyProgressive
\brief This function unifies the set by progressively allowing polygons with 
higher numbers of lines. This is to avoid doing the union of a large polygon
with several smaller ones as it is much better to unify the small ones then add 
to the big one. By far the quickest way for large numbers of shape with lots of
lines. See C2DHoledPolyBaseSet function also.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygonSet::UnifyProgressive(CGrid::eDegenerateHandling eDegen)
{
	C2DHoledPolyBaseSet BaseSet;

	while (size() > 0)
		BaseSet.Add( ExtractLast());

	BaseSet.UnifyProgressive(eDegen);

	for (unsigned int i = 0 ; i <  BaseSet.size(); i++)
	{
		Add(new C2DHoledPolygon( BaseSet[i]));
	}
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolygonSet::operator<<
\brief Adds a new item.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolygonSet::operator<<(C2DPolygon* NewItem)
{
	C2DHoledPolygon* pNew = new C2DHoledPolygon;
	pNew->SetRimDirect( NewItem );
	this->Add( pNew);
}