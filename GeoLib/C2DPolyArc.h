/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DPolyArc.h
\brief Declaration file for the C2DPolyArc Class

Declaration file for C2DPolyArc, a class which represents a closed area with 
lines that are either straight or curved.

\class C2DPolyArc.
\brief Class which represents a closed area with lines that are either straight 
or curved.

Class which represents a closed area with lines that are either straight 
or curved. Inherits most funcionality from the area base class.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DPOLYARC_H 
#define _GEOLIB_C2DPOLYARC_H

#include "C2DPolyBase.h"
#include "MemoryPool.h"



class C2DHoledPolyArcSet;
class C2DPolyArcSet;

#ifdef _POLY_EXPORTING
	#define POLY_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define POLY_DECLSPEC
	#else
		#define POLY_DECLSPEC		__declspec(dllimport)
	#endif
#endif

class POLY_DECLSPEC C2DPolyArc : public C2DPolyBase
{
public:
	_MEMORY_POOL_DECLARATION

	/// Constructor
	C2DPolyArc(void);
	/// Constructor
	C2DPolyArc(const C2DPolyBase& Other);
	/// Constructor
	C2DPolyArc(const C2DPolyArc& Other);
	/// Destructor
	~C2DPolyArc(void);

	/// Sets the starting point.
	void SetStartPoint(const C2DPoint& Point);
	/// Radius of the arc from the PREVIOUS point to the new one.
	/// bCentreOnRight indicates whether the centre of the arc is to the right of the line. 
	/// bArcOnRight indicates whether the curve is to the right i.e. anti-clockwise.
	void LineTo(const C2DPoint& Point, double dRadius,
		bool bCentreOnRight, bool bArcOnRight);
	/// Adds a point which is a striaght line from the previous.
	void LineTo(const C2DPoint& Point);
	/// Close with a curved line. WILL AUTO JOIN TO THE FIRST POINT.
	void Close(double dRadius, bool bCentreOnRight, bool bArcOnRight);
	/// Close with a straight line. WILL AUTO JOIN TO THE FIRST POINT.
	void Close(void);
	/// Creates a random shape e.g. for testing.
	bool CreateRandom(const C2DRect& cBoundary, int nMinPoints, int nMaxPoints);

	/// Clears.
	void Clear(void);
	/// Returns the number of points.
	int GetPointsCount(void) const {return (int)m_Lines.size();}
	/// Gets the non overlaps i.e. the parts of this that aren't in the other.
	void GetNonOverlaps(const C2DPolyArc& Other, C2DHoledPolyArcSet& HoledPolygons, 
										CGrid::eDegenerateHandling eDegen = CGrid::None) const ;
	/// Gets the non overlaps i.e. the parts of this that aren't in the other.
	void GetNonOverlaps(const C2DPolyArc& Other, C2DHoledPolyBaseSet& HoledPolygons, 
										CGrid::eDegenerateHandling eDegen = CGrid::None) const ;
	/// Gets the union of the 2 shapes.
	void GetUnion(const C2DPolyArc& Other, C2DHoledPolyArcSet& HoledPolygons,
										CGrid::eDegenerateHandling eDegen = CGrid::None) const ;
	/// Gets the union of the 2 shapes.
	void GetUnion(const C2DPolyArc& Other, C2DHoledPolyBaseSet& HoledPolygons,
										CGrid::eDegenerateHandling eDegen = CGrid::None) const ;

	/// Gets the overlaps of the 2 shapes.
	void GetOverlaps(const C2DPolyArc& Other, C2DHoledPolyArcSet& Polygons,
										CGrid::eDegenerateHandling eDegen = CGrid::None) const ;
	/// Gets the overlaps of the 2 shapes.	
	void GetOverlaps(const C2DPolyArc& Other, C2DHoledPolyBaseSet& Polygons,
										CGrid::eDegenerateHandling eDegen = CGrid::None) const ;

	/// Returns the centroid.
	C2DPoint GetCentroid(void) const;

	/// Rotates the polygon to the right around the origin given.
	void RotateToRight(double dAng, const C2DPoint& Origin);
	/// Rotates the polygon to the right around the centroid.
	void RotateToRight(double dAng);

};


#endif