/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DRectSet.cpp
\brief Implementation file for the C2DRectSet Class.

Implementation file for C2DRectSet
<P>---------------------------------------------------------------------------*/


#include "StdAfx.h"
#include "C2DRectSet.h"

_MEMORY_POOL_IMPLEMENATION(C2DRectSet)

/**--------------------------------------------------------------------------<BR>
C2DRectSet::C2DRectSet
\brief constructor.
<P>---------------------------------------------------------------------------*/
C2DRectSet::C2DRectSet(void)
{
	this->m_Type = C2DBase::RectSet;
}


/**--------------------------------------------------------------------------<BR>
C2DRectSet::~C2DRectSet
\brief destructor.
<P>---------------------------------------------------------------------------*/
C2DRectSet::~C2DRectSet(void)
{

}

/**--------------------------------------------------------------------------<BR>
C2DRectSet::AddCopy
\brief Adds a copy of the set provided.
<P>---------------------------------------------------------------------------*/
void C2DRectSet::AddCopy(const C2DRectSet& Other)
{
	for(unsigned int i = 0 ; i < Other.size() ; i++)
	{
		Add(new C2DRect(Other[i]));
	}
}

/**--------------------------------------------------------------------------<BR>
C2DRectSet::MakeCopy
\brief Makes a copy of the set provided.
<P>---------------------------------------------------------------------------*/
void C2DRectSet::MakeCopy( const C2DRectSet& Other)
{
	DeleteAll();
	AddCopy(Other);
}

/**--------------------------------------------------------------------------<BR>
C2DRectSet::operator<<
\brief Passes ONLY the pointers of this type from the Other into this.
<P>---------------------------------------------------------------------------*/
void C2DRectSet::operator<<(C2DBaseSet& Other)
{
	C2DBaseSet Temp;

	while (Other.size() > 0)
	{
		C2DBase* pLast = Other.ExtractLast();
		if (pLast->GetType() == C2DBase::Rect)
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

/**--------------------------------------------------------------------------<BR>
C2DRectSet::AddCopy
\brief Adds a copy of the item.
<P>---------------------------------------------------------------------------*/
void C2DRectSet::AddCopy(const C2DRect& NewItem)
{
	Add(new C2DRect(NewItem));
}

