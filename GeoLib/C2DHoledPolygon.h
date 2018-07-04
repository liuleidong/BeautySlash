/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DHoledPolygon.h
\brief Declaration file for the C2DHoledPolygon Class.

Declaration file for C2DHoledPolygon, a class which represents a polygon with 
holes.

\class C2DHoledPolygon.
\brief class which represents a polygon with holes.

Class which represents a polygon with holes. Simply consists of a set of simple
polygons (without holes) representing the rim and the hole.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DHOLEDPOLYGON_H 
#define _GEOLIB_C2DHOLEDPOLYGON_H


#include "C2DHoledPolyBase.h"
#include "Grid.h"
#include "MemoryPool.h"


class C2DPolygon;
class C2DHoledPolygonSet;

#ifdef _POLY_EXPORTING
	#define POLY_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define POLY_DECLSPEC
	#else
		#define POLY_DECLSPEC		__declspec(dllimport)
	#endif
#endif


class POLY_DECLSPEC C2DHoledPolygon : public C2DHoledPolyBase
{
public:
	_MEMORY_POOL_DECLARATION

	/// Constructor.
	C2DHoledPolygon(void);
	/// Constructor with assignment.
	C2DHoledPolygon(const C2DHoledPolygon& Other);
	/// Constructor with assignment.
	C2DHoledPolygon(const C2DHoledPolyBase& Other);

	/// Destructor.
	~C2DHoledPolygon(void);
	/// Sets the rim to a copy of the polygon given.
	void SetRim(const C2DPolygon& Polygon);
	/// Adds a copy of the polygon given.
	void AddHole(const C2DPolygon& Polygon);
	/// Sets the holes to be a copy of the polygon given.
	void SetHole(const C2DPolygon& Polygon, unsigned int usIndex);
	/// Sets the rim to be the polygon provided. Will delete on destruction.
	void SetRimDirect(C2DPolygon* Polygon);
	/// Adds the hole provided. Will delete on destruction.
	void AddHoleDirect(C2DPolygon* Polygon);
	/// Sets the hole to be the polygon provided. Will delete on destruction. Deletes old one.
	void SetHoleDirect(C2DPolygon* Polygon, unsigned int usIndex);
	/// Removes the hole at the index given.
	void RemoveHole(unsigned int usIndex);
	/// Returns the rim.
	C2DPolygon* GetRim(void);
	/// Returns the rim.
	const C2DPolygon* GetRim(void) const;
	/// Extracts the rim. Deletion of the rim needs to be managed elsewhere.
	C2DPolygon* ExtractRim(void);
	/// Returns a pointer to the hole specified.
	C2DPolygon* GetHole(unsigned int usIndex);
	/// Returns a pointer to the hole specified.
	const C2DPolygon* GetHole(unsigned int usIndex) const;
	/// Extracts the hole. Deletion of the hole needs to be managed elsewhere.
	C2DPolygon* ExtractHole(unsigned int usIndex);

	/// Assignement to another.
	const C2DHoledPolygon& operator=(C2DHoledPolygon& Other);

	/// Grows the polygon by the amount.
	void Grow(double dFactor);
	/// Grows the polygon by the amount.
	void Grow(double dFactor, const C2DPoint& Origin);
	/// Rotates to the right by the angle around the centroid.
	void RotateToRight(double dAng);
	/// Rotates to the right by the angle around the centroid.
	void RotateToRight(double dAng, const C2DPoint& Origin);
	/// Returns the centroid.
	C2DPoint GetCentroid(void);

	/// Area.
	double GetArea() const ;

	/// True if this overlaps the other.
	bool Overlaps(const C2DHoledPolygon& Other) const;

	/// Returns the overlaps between this and the other complex polygon.
	void GetOverlaps(const C2DHoledPolygon& Other, C2DHoledPolygonSet& HoledPolys, 
							CGrid::eDegenerateHandling eDegen = CGrid::None) const;

	/// Returns the overlaps between this and the other complex polygon.
	void GetOverlaps(const C2DHoledPolygon& Other, C2DHoledPolyBaseSet& HoledPolys, 
							CGrid::eDegenerateHandling eDegen = CGrid::None) const;

	/// Returns the difference between this and the other polygon.
	void GetNonOverlaps(const C2DHoledPolygon& Other, C2DHoledPolygonSet& HoledPolys, 
							CGrid::eDegenerateHandling eDegen = CGrid::None) const;

	/// Returns the difference between this and the other polygon.
	void GetNonOverlaps(const C2DHoledPolygon& Other, C2DHoledPolyBaseSet& HoledPolys, 
							CGrid::eDegenerateHandling eDegen = CGrid::None) const;

	/// Returns the union of this and the other.
	void GetUnion(const C2DHoledPolygon& Other, C2DHoledPolygonSet& HoledPolys,
					CGrid::eDegenerateHandling eDegen = CGrid::None) const;

	/// Returns the union of this and the other.
	void GetUnion(const C2DHoledPolygon& Other, C2DHoledPolyBaseSet& HoledPolys,
					CGrid::eDegenerateHandling eDegen = CGrid::None) const;
	/// Removes null areas, will return true if the shape is no longer valid.
	bool RemoveNullAreas(double dTolerance);


};

#endif