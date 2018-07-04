/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DPolygonSet.cpp
\brief Implementation file for the C2DPolygonSet Class.

Implementation file for C2DPolygonSet
<P>---------------------------------------------------------------------------*/



#include "StdAfx.h"
#include "C2DPolygonSet.h"
#include "C2DBaseSet.h"
#include "C2DPolygon.h"

_MEMORY_POOL_IMPLEMENATION(C2DPolygonSet)

/**--------------------------------------------------------------------------<BR>
C2DPolygonSet::C2DPolygonSet
\brief constructor.
<P>---------------------------------------------------------------------------*/
C2DPolygonSet::C2DPolygonSet(void)
{
	this->m_Type = C2DBase::PolyLineSet;
}


/**--------------------------------------------------------------------------<BR>
C2DPolygonSet::~C2DPolygonSet
\brief destructor.
<P>---------------------------------------------------------------------------*/
C2DPolygonSet::~C2DPolygonSet(void)
{

}




/**--------------------------------------------------------------------------<BR>
C2DPolygonSet::AddCopy
\brief Adds a copy of the set provided.
<P>---------------------------------------------------------------------------*/
void C2DPolygonSet::AddCopy(const C2DPolygonSet& Other)
{
	for (unsigned int i =0; i < Other.size(); i++)
	{
		C2DBaseSet::Add( new C2DPolygon(Other[i]));
	}

}

/**--------------------------------------------------------------------------<BR>
C2DPolygonSet::MakeCopy
\brief Makes a copy of the set provided.
<P>---------------------------------------------------------------------------*/
void C2DPolygonSet::MakeCopy( const C2DPolygonSet& Other)
{
	DeleteAll( );
	AddCopy(Other);

}

/**--------------------------------------------------------------------------<BR>
C2DPolygonSet::AddCopy
\brief Adds a copy of the item.
<P>---------------------------------------------------------------------------*/
void C2DPolygonSet::AddCopy(const C2DPolygon& NewItem)
{
	C2DBaseSet::Add( new C2DPolygon(NewItem));
}



/**--------------------------------------------------------------------------<BR>
C2DPolygonSet::operator<<
\brief Passes ONLY the pointers of this type from the Other into this.
<P>---------------------------------------------------------------------------*/
void C2DPolygonSet::operator<<(C2DBaseSet& Other)
{
	C2DBaseSet Temp;

	while (Other.size() > 0)
	{
		C2DBase* pLast = Other.ExtractLast();
		if (pLast->GetType() == C2DBase::PolyLine)
		{
			C2DBaseSet::Add( pLast );
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




