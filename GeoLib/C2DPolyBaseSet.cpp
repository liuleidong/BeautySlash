/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DPolyBaseSet.cpp
\brief Implementation file for the C2DPolyBaseSet Class.

Implementation file for C2DPolyBaseSet
<P>---------------------------------------------------------------------------*/


#include "C2DPolyBaseSet.h"
#include "C2DBaseSet.h"
#include "C2DBase.h"
#include "C2DPolyBase.h"


_MEMORY_POOL_IMPLEMENATION(C2DPolyBaseSet)

/**--------------------------------------------------------------------------<BR>
C2DPolyBaseSet::C2DPolyBaseSet
\brief constructor.
<P>---------------------------------------------------------------------------*/
C2DPolyBaseSet::C2DPolyBaseSet(void)
{
	this->m_Type = C2DBase::PolyBaseSet;
}


/**--------------------------------------------------------------------------<BR>
C2DPolyBaseSet::~C2DPolyBaseSet
\brief destructor.
<P>---------------------------------------------------------------------------*/
C2DPolyBaseSet::~C2DPolyBaseSet(void)
{

}

/**--------------------------------------------------------------------------<BR>
C2DPolyBaseSet::AddCopy
\brief Adds a copy of the other pointer array.
<P>---------------------------------------------------------------------------*/
void C2DPolyBaseSet::AddCopy(const C2DPolyBaseSet& Other)
{
	for (unsigned int i = 0 ; i < Other.size() ; i++)
	{
		C2DBaseSet::Add( new C2DPolyBase( Other[i]));
	}

}

/**--------------------------------------------------------------------------<BR>
C2DPolyBaseSet::AddCopy
\brief Makes a copy of the other pointer array.
<P>---------------------------------------------------------------------------*/
void C2DPolyBaseSet::MakeCopy( const C2DPolyBaseSet& Other)
{
	DeleteAll();
	AddCopy(Other);

}

/**--------------------------------------------------------------------------<BR>
C2DPolyBaseSet::AddCopy
\brief Adds a copy of the item given.
<P>---------------------------------------------------------------------------*/
void C2DPolyBaseSet::AddCopy(const C2DPolyBase& NewItem)
{
	C2DBaseSet::Add( new C2DPolyBase(NewItem));
}

/**--------------------------------------------------------------------------<BR>
C2DPolyBaseSet::operator<<
\brief Passes ONLY the pointers of this type from the Other into this.
<P>---------------------------------------------------------------------------*/
void C2DPolyBaseSet::operator<<(C2DBaseSet& Other)
{
	C2DBaseSet Temp;

	while (Other.size() > 0)
	{
		C2DBase* pLast = Other.ExtractLast();
		C2DBase::E_TYPE eType = pLast->GetType();
		if (eType == C2DBase::PolyBase ||
			eType == C2DBase::PolyArc ||
			eType == C2DBase::PolyLine)
		{
			C2DBaseSet::Add(pLast );
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



