/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DLineSet.cpp
\brief Implementation file for the C2DLineSet Class.

Implementation file for C2DLineSet, a set of pointer to C2DLine.
<P>---------------------------------------------------------------------------*/



#include "StdAfx.h"
#include "C2DLineSet.h"
#include "C2DBaseSet.h"
#include "C2DLine.h"

_MEMORY_POOL_IMPLEMENATION(C2DLineSet)


/**--------------------------------------------------------------------------<BR>
C2DLineSet::C2DLineSet
\brief constructor.
<P>---------------------------------------------------------------------------*/
C2DLineSet::C2DLineSet(void)
{
	this->m_Type = C2DBase::StraightLineSet;
}


/**--------------------------------------------------------------------------<BR>
C2DLineSet::~C2DLineSet
\brief destructor.
<P>---------------------------------------------------------------------------*/
C2DLineSet::~C2DLineSet(void)
{

}



/**--------------------------------------------------------------------------<BR>
C2DLineSet::MakeCopy
\brief Makes a copy of the other
<P>---------------------------------------------------------------------------*/
void C2DLineSet::MakeCopy( const C2DLineSet& Other)
{
	DeleteAll();
	AddCopy(Other);

}

/**--------------------------------------------------------------------------<BR>
C2DLineSet::AddCopy
\brief Adds line.
<P>---------------------------------------------------------------------------*/
void C2DLineSet::AddCopy(double x1, double y1, double x2, double y2)
{
	Add(new C2DLine( C2DPoint(x1, y1), C2DPoint(x2, y2)));
}


/**--------------------------------------------------------------------------<BR>
C2DLineSet::AddCopy
\brief Adds line.
<P>---------------------------------------------------------------------------*/
void C2DLineSet::AddCopy(const C2DPoint& pt1, const C2DPoint& pt2)
{
	Add(new C2DLine( pt1, pt2));	
}

/**--------------------------------------------------------------------------<BR>
C2DLineSet::AddCopy
\brief Adds line.
<P>---------------------------------------------------------------------------*/
void C2DLineSet::AddCopy(const C2DLine& Line)
{ 
	Add(new C2DLine(Line));
}

/**--------------------------------------------------------------------------<BR>
C2DLineSet::AddCopy
\brief Adds line.
<P>---------------------------------------------------------------------------*/
void C2DLineSet::AddCopy(const C2DLineSet& Other) 
{ 
	for (unsigned int i = 0 ; i < Other.size(); i++)
	{
		Add(new C2DLine(Other[i]));
	}

}
