/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file Interval.cpp
\brief Implementation file for the CInterval Class.

Implementation file for CInterval, a class for managing an interval between 
2 numbers.
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "Interval.h"

_MEMORY_POOL_IMPLEMENATION(CInterval)

/**--------------------------------------------------------------------------<BR>
CInterval::CInterval <BR>
\brief Constructor.
<P>---------------------------------------------------------------------------*/
CInterval::CInterval(void)
{
	dMax = 0;
	dMin = 0;
}


/**--------------------------------------------------------------------------<BR>
CInterval::CInterval <BR>
\brief Destructor.
<P>---------------------------------------------------------------------------*/
CInterval::~CInterval(void)
{
}

/**--------------------------------------------------------------------------<BR>
CInterval::CInterval <BR>
\brief Constructor.
<P>---------------------------------------------------------------------------*/
CInterval::CInterval(double dMinimum, double dMaximum)
{
	dMin = dMinimum;
	dMax = dMaximum;
}

/**--------------------------------------------------------------------------<BR>
CInterval::ExpandToInclude <BR>
\brief Expands this to include the other.
<P>---------------------------------------------------------------------------*/
void CInterval::ExpandToInclude(CInterval& Other)
{
	if (Other.dMax > dMax) dMax = Other.dMax;
	if (Other.dMin < dMin) dMin = Other.dMin;
}


/**--------------------------------------------------------------------------<BR>
CInterval::ExpandToInclude <BR>
\brief Expands this to include the value.
<P>---------------------------------------------------------------------------*/
void CInterval::ExpandToInclude(double dValue)
{
	if (dValue > dMax) dMax = dValue;
	else if (dValue < dMin) dMin = dValue;
}


/**--------------------------------------------------------------------------<BR>
CInterval::operator= <BR>
\brief Assignment.
<P>---------------------------------------------------------------------------*/
const CInterval& CInterval::operator=(const CInterval& Other)
{
	dMax = Other.dMax;
	dMin = Other.dMin;	
	return *this;
}


/**--------------------------------------------------------------------------<BR>
CInterval::Overlaps
\brief True is this overlaps the other
<P>---------------------------------------------------------------------------*/
bool CInterval::Overlaps(const CInterval& Other) const
{
	return (!IsBelow(Other) && !IsAbove(Other));
}



/**--------------------------------------------------------------------------<BR>
CInterval::Overlaps
\brief True is this overlaps the other
<P>---------------------------------------------------------------------------*/
bool CInterval::Overlaps(const CInterval& Other, CInterval& Overlap) const
{
	if (Other.dMin < dMax &&
		Other.dMax > dMin)
	{
		Overlap.dMin = max(Other.dMin, dMin);
		Overlap.dMax = min(Other.dMax, dMax);
		return true;
	}
	else
	{
		return false;
	}
}


/**--------------------------------------------------------------------------<BR>
CInterval::IsAbove
\brief True is this is entirely above the other.
<P>---------------------------------------------------------------------------*/
bool CInterval::IsAbove(const CInterval& Other) const
{
	return  dMin > Other.dMax;
}


/**--------------------------------------------------------------------------<BR>
CInterval::IsBelow
\brief True is this is entirely below the other.
<P>---------------------------------------------------------------------------*/
bool CInterval::IsBelow(const CInterval& Other) const
{
	return dMax < Other.dMin;
}


/**--------------------------------------------------------------------------<BR>
CInterval::Contains
\brief True is this Contains the value
<P>---------------------------------------------------------------------------*/
bool CInterval::Contains(double dValue) const
{
	return ( dMin <= dValue && dValue <= dMax);
}


/**--------------------------------------------------------------------------<BR>
CInterval::Contains
\brief True is this Contains the other
<P>---------------------------------------------------------------------------*/
bool CInterval::Contains(const CInterval& Other) const
{
	return (   Contains(Other.dMin) && Contains (Other.dMax)   );
}