/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------<BR>
\file 2DHoledPolyArcSet.cpp
\brief Implementation file for the C2DHoledPolyArcSet Class.
<P>---------------------------------------------------------------------------*/


#include "StdAfx.h"
#include "C2DHoledPolyArcSet.h"
#include "C2DHoledPolyBaseSet.h"


_MEMORY_POOL_IMPLEMENATION(C2DHoledPolyArcSet)

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArcSet::C2DHoledPolyArcSet
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DHoledPolyArcSet::C2DHoledPolyArcSet(void)
{
	m_Type = PolyArcHoledSet;
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArcSet::C2DHoledPolyArcSet
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DHoledPolyArcSet::~C2DHoledPolyArcSet(void)
{
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArcSet::AddCopy
\brief Adds a copy of the other.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArcSet::AddCopy(const C2DHoledPolyArcSet& Other)
{
	for(unsigned int i = 0 ; i < Other.size(); i++)
	{
		AddCopy(Other[i]);
	}

}
/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArcSet::MakeCopy
\brief Makes a copy of the set provided.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArcSet::MakeCopy( const C2DHoledPolyArcSet& Other)
{
	DeleteAll();
	AddCopy( Other);

}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::operator<<
\brief Passes ONLY the pointers of this type from the Other into this.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArcSet::operator<<(C2DBaseSet& Other)
{
	C2DBaseSet Temp;

	while (Other.size() > 0)
	{
		C2DBase* pLast = Other.ExtractLast();
		if (pLast->GetType() == C2DBase::PolyArcHoled)
		{
			Add( dynamic_cast<C2DHoledPolyArc*>(pLast) );
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
C2DHoledPolyArcSet::UnifyBasic
\brief Simple Unification. See C2DHoledPolyBaseSet function also.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArcSet::UnifyBasic(void)
{
	C2DHoledPolyBaseSet BaseSet;

	while (size() > 0)
		BaseSet.Add( ExtractLast());

	BaseSet.UnifyBasic();

	for (unsigned int i = 0 ; i <  BaseSet.size(); i++)
	{
		Add(new C2DHoledPolyArc( BaseSet[i]));
	}
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyArcSet::UnifyProgressive
\brief This function unifies the set by progressively allowing polygons with 
higher numbers of lines. This is to avoid doing the union of a large polygon
with several smaller ones as it is much better to unify the small ones then add 
to the big one. By far the quickest way for large numbers of shape with lots of
lines. See C2DHoledPolyBaseSet function also.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyArcSet::UnifyProgressive(CGrid::eDegenerateHandling eDegen)
{
	C2DHoledPolyBaseSet BaseSet;

	while (size() > 0)
		BaseSet.Add( ExtractLast());

	BaseSet.UnifyProgressive(eDegen);

	for (unsigned int i = 0 ; i <  BaseSet.size(); i++)
	{
		Add(new C2DHoledPolyArc( BaseSet[i]));
	}
}


// Makes all arc valid if not already by adjusting radius to minimum required.
unsigned int C2DHoledPolyArcSet::MakeValidArcs(void)
{
	unsigned int nResult = 0;

	for (unsigned int i = 0 ; i <  size(); i++)
	{
		nResult += this->GetAt(i)->MakeValidArcs();
	}
	return nResult;
}


// True if there are arcs in the shape.
bool C2DHoledPolyArcSet::HasArcs(void) const
{
	for (unsigned int i = 0 ; i <  size(); i++)
	{
		if (this->GetAt(i)->HasArcs())
			return true;
	}
	return false;

}


// True if all the arcs are valid
bool C2DHoledPolyArcSet::IsValidArcs(void) const
{
	for (unsigned int i = 0 ; i <  size(); i++)
	{
		if (!this->GetAt(i)->IsValidArcs())
			return false;
	}
	return true;

}

// Makes all arc valid if not already by adjusting radius to minimum required.
unsigned int C2DHoledPolyArcSet::RemoveNullLines(void)
{
	unsigned int nResult = 0;

	for (unsigned int i = 0 ; i <  size(); i++)
	{
		nResult += this->GetAt(i)->RemoveNullLines();
	}
	return nResult;
}
