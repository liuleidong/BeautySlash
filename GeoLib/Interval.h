/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file Interval.h
\brief Declaration file for the CInterval Class.

Declaration file for CInterval, a class for managing an interval between 2 numbers.

\class CInterval
\brief A class for managing an interval [a, b] between 2 numbers.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_INTERVAL_H 
#define _GEOLIB_INTERVAL_H


#include "MemoryPool.h"



#ifdef _EXPORTING
	#define CLASS_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define CLASS_DECLSPEC
	#else
		#define CLASS_DECLSPEC		__declspec(dllimport)
	#endif
#endif

class CLASS_DECLSPEC CInterval
{
public:
	_MEMORY_POOL_DECLARATION
	/// Constructor
	CInterval(void);
	/// Destructor
	~CInterval(void);
	/// Constructor
	CInterval(double dMin, double dMax);
	/// Expands the interval to include the other
	void ExpandToInclude(CInterval& Other);
	/// Expands the interval to include the value
	void ExpandToInclude(double dValue);
	/// Returns the distance between the min and the max
	double GetLength(void) const {return dMax - dMin;}
	/// Assignement
	const CInterval& operator=(const CInterval& Other);
	/// True if this overlaps the other
	bool Overlaps(const CInterval& Other) const;
	/// True if this overlaps the other
	bool Overlaps(const CInterval& Other, CInterval& Overlap) const;
	/// True if this contains the value
	bool Contains(double dValue) const;
	/// True if this contains the other
	bool Contains(const CInterval& Other) const;
	/// True if this is entirely above the other
	bool IsAbove(const CInterval& Other) const;
	/// True if this is entirely below the other
	bool IsBelow(const CInterval& Other) const;
	/// The min 
	double dMin;
	/// The max
	double dMax;

};

#endif