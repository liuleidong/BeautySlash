/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DHoledPolyArc.h
\brief Declaration file for the C2DHoledPolyArc Class.

Declaration file for C2DHoledPolyArc, a class which represents a curved polygon 
with holes.

\class C2DHoledPolyArc.
\brief class which represents a  curved polygon with holes.

Class which represents a  curved polygon with holes. Simply consists of a set of 
simple curved polygons (without holes) representing the rim and the holes.
<P>---------------------------------------------------------------------------*/
#ifndef _GEOLIB_C2DHOLEDPOLYARC_H 
#define _GEOLIB_C2DHOLEDPOLYARC_H

#include "C2DHoledPolyBase.h"
#include "MemoryPool.h"


class C2DPolyArc;
class C2DHoledPolyArcSet;

#ifdef _POLY_EXPORTING
	#define POLY_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define POLY_DECLSPEC
	#else
		#define POLY_DECLSPEC		__declspec(dllimport)
	#endif
#endif


class  POLY_DECLSPEC C2DHoledPolyArc : public C2DHoledPolyBase
{
public:
	_MEMORY_POOL_DECLARATION

	/// Constructor.
	C2DHoledPolyArc(void);
	/// Constructor.
	C2DHoledPolyArc(const C2DHoledPolyBase& Other);
	/// Constructor.
	C2DHoledPolyArc(const C2DHoledPolyArc& Other);
	/// Destructor.
	~C2DHoledPolyArc(void);
	/// Sets the rim to a copy of the polygon given.
	void SetRim(const C2DPolyArc& Polygon);
	/// Adds a copy of the polygon given.
	void AddHole(const C2DPolyArc& Polygon);
	/// Sets the holes to be a copy of the polygon given.
	void SetHole(const C2DPolyArc& Polygon, unsigned int usIndex);
	/// Sets the rim to be the polygon provided. Will delete on destruction.
	void SetRimDirect(C2DPolyArc* Polygon);
	/// Adds the hole provided. Will delete on destruction.
	void AddHoleDirect(C2DPolyArc* Polygon);
	/// Sets the hole to be the polygon provided. Will delete on destruction. Deletes old one.
	void SetHoleDirect(C2DPolyArc* Polygon, unsigned int usIndex);
	/// Removes the hole at the index given.
	void RemoveHole(unsigned int usIndex);
	/// Returns the rim.
	C2DPolyArc* GetRim(void);
	/// Returns the rim.
	const C2DPolyArc* GetRim(void) const;

	/// Extracts the rim. Deletion of the rim needs to be managed elsewhere.
	C2DPolyArc* ExtractRim(void);
	/// Returns a pointer to the hole specified.
	C2DPolyArc* GetHole(unsigned int usIndex);

	/// Returns a pointer to the hole specified.
	const C2DPolyArc* GetHole(unsigned int usIndex) const;

	/// Extracts the hole. Deletion of the hole needs to be managed elsewhere.
	C2DPolyArc* ExtractHole(unsigned int usIndex);

	/// Returns the area.
	double GetArea(void) const;
	/// Returns the centroid.
	C2DPoint GetCentroid(void) const;


	/// Returns the overlaps between this and the other complex polygon.
	void GetOverlaps(const C2DHoledPolyArc& Other, C2DHoledPolyArcSet& HoledPolys, 
							CGrid::eDegenerateHandling eDegen = CGrid::None) const;

	/// Returns the overlaps between this and the other complex polygon.
	void GetOverlaps(const C2DHoledPolyArc& Other, C2DHoledPolyBaseSet& HoledPolys, 
							CGrid::eDegenerateHandling eDegen = CGrid::None) const;

	/// Returns the difference between this and the other polygon.
	void GetNonOverlaps(const C2DHoledPolyArc& Other, C2DHoledPolyArcSet& HoledPolys, 
							CGrid::eDegenerateHandling eDegen = CGrid::None) const;

	/// Returns the difference between this and the other polygon.
	void GetNonOverlaps(const C2DHoledPolyArc& Other, C2DHoledPolyBaseSet& HoledPolys, 
							CGrid::eDegenerateHandling eDegen = CGrid::None) const;

	/// Returns the union of this and the other.
	void GetUnion(const C2DHoledPolyArc& Other, C2DHoledPolyArcSet& HoledPolys,
							CGrid::eDegenerateHandling eDegen = CGrid::None) const;

	/// Returns the union of this and the other.
	void GetUnion(const C2DHoledPolyArc& Other, C2DHoledPolyBaseSet& HoledPolys,
							CGrid::eDegenerateHandling eDegen = CGrid::None) const;

};

#endif