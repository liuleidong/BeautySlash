/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DPolyArcSet.cpp
\brief Implementation file for the C2DPolyArcSet Class.

Implementation file for C2DPolyArcSet
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "C2DPolyArcSet.h"
#include "C2DBaseSet.h"
#include "C2DPolyArc.h"


_MEMORY_POOL_IMPLEMENATION(C2DPolyArcSet)


/**--------------------------------------------------------------------------<BR>
C2DPolyArcSet::C2DPolyArcSet
\brief constructor.
<P>---------------------------------------------------------------------------*/
C2DPolyArcSet::C2DPolyArcSet(void)
{
	this->m_Type = C2DBase::PolyArcSet;
}


/**--------------------------------------------------------------------------<BR>
C2DPolyArcSet::~C2DPolyArcSet
\brief destructor.
<P>---------------------------------------------------------------------------*/
C2DPolyArcSet::~C2DPolyArcSet(void)
{

}

/**--------------------------------------------------------------------------<BR>
C2DPolyArcSet::AddCopy
\brief Adds a copy of the other pointer array.
<P>---------------------------------------------------------------------------*/
void C2DPolyArcSet::AddCopy(const C2DPolyArcSet& Other)
{
	for(unsigned int i = 0 ; i < Other.size(); i++)
	{
		Add(new C2DPolyArc(Other[i]));

	}
}

/**--------------------------------------------------------------------------<BR>
C2DPolyArcSet::MakeCopy
\brief Makes a copy of the other.
<P>---------------------------------------------------------------------------*/
void C2DPolyArcSet::MakeCopy( const C2DPolyArcSet& Other)
{
	DeleteAll();
	AddCopy(Other);
}

/**--------------------------------------------------------------------------<BR>
C2DPolyArcSet::AddCopy
\brief Adds a copy of the item given.
<P>---------------------------------------------------------------------------*/
void C2DPolyArcSet::AddCopy(const C2DPolyArc& NewItem)
{
	Add(new C2DPolyArc(NewItem));
}

/**--------------------------------------------------------------------------<BR>
C2DPolyArcSet::operator<<
\brief Passes ONLY the pointers of this type from the Other into this.
<P>---------------------------------------------------------------------------*/
void C2DPolyArcSet::operator<<(C2DBaseSet& Other)
{
	C2DBaseSet Temp;

	while (Other.size() > 0)
	{
		C2DBase* pLast = Other.ExtractLast();
		if (pLast->GetType() == C2DBase::PolyArc)
		{
			Add( dynamic_cast<C2DPolyArc*>(pLast) );
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




