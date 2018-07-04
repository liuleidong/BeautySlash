/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file TravellingSalesman.h
\brief Declaration file for the CTravellingSalesman Class.

Declaration file for CTravellingSalesman, a class which uses heuristic methods
to help minimise routes between points. Used by the polygon for reordering points.

\class CTravellingSalesman
\brief A class which uses heuristic methods to help minimise routes between points. 
Used by the polygon for reordering points.
<P>---------------------------------------------------------------------------*/

#ifndef _CTRAVELLINGSALESMAN_H 
#define _CTRAVELLINGSALESMAN_H

#include "MemoryPool.h"


class C2DPoint;
class C2DPointSet;
class CPointList;

#ifdef _EXPORTING
	#define CLASS_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define CLASS_DECLSPEC
	#else
		#define CLASS_DECLSPEC		__declspec(dllimport)
	#endif
#endif


class CLASS_DECLSPEC CTravellingSalesman
{
public:
	_MEMORY_POOL_DECLARATION
	/// Constructor
	CTravellingSalesman(void);
	/// Destructor
	~CTravellingSalesman(void);
	/// Allocates points from a set by removing them from the set
	void SetPointsDirect(C2DPointSet& Points);
	/// Extracts the points into the set
	void ExtractPoints(C2DPointSet& Points);
	/// Inserts a point optimally into this 
	void InsertOptimally(C2DPoint* pPt);
	/// Refines the set, trying to find optimal positions for the points
	void Refine(void);
	/// Refines the set, trying to find optimal positions for the points
	void Refine2(void);
	/// Brute force optimisation
	void SimpleReorder(void);
	/// Optimises the position of the points
	void Optimize(void);
	/// Deletes all
	void DeleteAll(void);

private:
	/// The data - a list for quick insertion.
	CPointList* m_Points;

};


#endif