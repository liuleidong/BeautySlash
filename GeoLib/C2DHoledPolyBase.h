/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DHoledPolyBase.h
\brief Declaration file for the C2DHoledPolyBase class.

Declaration file for the C2DHoledPolyBase class, a class which represents an 
C2DPolyBase with holes.

\class C2DHoledPolyBase.
\brief class which represents a C2DPolyBase with holes.

A class which represents an C2DPolyBase with holes. Note that some of the functions
assume that the holes are inside the area for simplicity and speed. E.g. the 
projection function will project the rim and not the holes as there should be no need
as long as it is a valid area.
<P>---------------------------------------------------------------------------*/


#ifndef _GEOLIB_C2DHOLEDPOLYBASE_H 
#define _GEOLIB_C2DHOLEDPOLYBASE_H

#include "C2DBase.h"
#include "C2DPolyBaseSet.h"
#include "Grid.h"
#include "MemoryPool.h"


class C2DLineBase;
class C2DLineBaseSetSet;
class C2DHoledPolyBaseSet;
class C2DPointSet;

#ifdef _POLY_EXPORTING
	#define POLY_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define POLY_DECLSPEC
	#else
		#define POLY_DECLSPEC		__declspec(dllimport)
	#endif
#endif


class POLY_DECLSPEC C2DHoledPolyBase : public C2DBase
{
public:
	_MEMORY_POOL_DECLARATION
	/// Constructor
	C2DHoledPolyBase(void);
	/// Copy constructor 
	C2DHoledPolyBase(const C2DHoledPolyBase& Other);
	/// Destructor
	~C2DHoledPolyBase(void);

	/// Assignment
	const C2DHoledPolyBase& operator=(const C2DHoledPolyBase& Other);

	/// Sets the rim to a copy of the polygon given.
	void SetRim(const C2DPolyBase& Polygon);
	/// Adds a copy of the polygon given.
	void AddHole(const C2DPolyBase& Polygon);
	/// Sets the holes to be a copy of the polygon given.
	void SetHole(const C2DPolyBase& Polygon, unsigned int usIndex);
	/// Sets the rim to be the polygon provided. Will delete on destruction.
	void SetRimDirect(C2DPolyBase* Polygon);
	/// Adds the hole provided. Will delete on destruction.
	void AddHoleDirect(C2DPolyBase* Polygon);
	/// Sets the hole to be the polygon provided. Will delete on destruction. Deletes old one.
	void SetHoleDirect(C2DPolyBase* Polygon, unsigned int usIndex);
	/// Removes the hole at the index given.
	void RemoveHole(unsigned int usIndex);
	/// Returns the rim.
	C2DPolyBase* GetRim(void) {return m_Rim;}
	/// Returns the rim.
	const C2DPolyBase* GetRim(void) const {return m_Rim;}

	/// Extracts the rim. Deletion of the rim needs to be managed elsewhere.
	C2DPolyBase* ExtractRim(void);
	/// Returns a pointer to the hole specified.
	C2DPolyBase* GetHole(unsigned int usIndex);

	/// Returns a pointer to the hole specified.
	const C2DPolyBase* GetHole(unsigned int usIndex) const;

	/// Extracts the hole. Deletion of the hole needs to be managed elsewhere.
	C2DPolyBase* ExtractHole(unsigned int usIndex);
	/// Returns the number of holes.
	unsigned int GetHoleCount(void) const {return m_Holes.size();}
	/// Return the number of lines.
	unsigned int GetLineCount(void) const;
	/// Clears the shape.
	void Clear(void);

	/// True if the holes are contained and non-intersecting.
	bool IsValid(void);
	// True if all the arcs are valid
	bool IsValidArcs(void) const;
	// Makes all arc valid if not already by adjusting radius to minimum required.
	// Returns the number affected.
	unsigned int MakeValidArcs(void);
	// True if there are arcs in the shape.
	bool HasArcs(void) const;

	void MakeClockwise(void);

	// Removes all lines whose end is the same is the start. Returns the number found.
	unsigned int RemoveNullLines(void);

	/// Rotates to the right by the angle around the origin.
	void RotateToRight(double dAng, const C2DPoint& Origin);

	/// Moves the polygon.
	void Move(const C2DVector& Vector);

	/// Grows around the origin.
	void Grow(double dFactor, const C2DPoint& Origin);
	/// Point reflection.
	void Reflect(const C2DPoint& Point);
	/// Reflects throught the line provided.
	void Reflect(const C2DLine& Line);
	/// Distance from the point.
	double Distance(const C2DPoint& TestPoint) const;
	/// Distance from the line provided
	double Distance(const C2DLineBase& Line) const;
	/// Distance from the polygon provided. Also returns points if provided.
	double Distance(const C2DPolyBase& Poly, 
					C2DPoint* ptOnThis = 0, C2DPoint* ptOnOther = 0 ) const;
	/// Distance from the point.
	bool IsWithinDistance(const C2DPoint& TestPoint, double dDist) const;
	/// Returns the perimeter.
	double GetPerimeter(void) const;

	/// Projection onto the line
	void Project(const C2DLine& Line, CInterval& Interval) const;
	/// Projection onto the vector
	void Project(const C2DVector& Vector, CInterval& Interval) const;
	/// True if there are crossing lines.
	bool HasCrossingLines(void) const;
	/// Returns the bounding rectangle.
	void GetBoundingRect(C2DRect& Rect) const;

	/// Point inside test.
	bool Contains(const C2DPoint& pt) const ;
	/// Line entirely inside test.
	bool Contains(const C2DLineBase& Line) const;
	/// Polygon entirely inside test.
	bool Contains(const C2DPolyBase& Polygon) const ;
	/// Polygon entirely inside test.
	bool Contains(const C2DHoledPolyBase& Polygon) const ;


	/// True if this crosses the line.
	bool Crosses(const C2DLineBase& Line) const;
	/// True if this crosses the line, returns the intersection points.
	bool Crosses(const C2DLineBase& Line, C2DPointSet* IntersectionPts) const;
	/// True if it crosses the other.
	bool Crosses(const C2DPolyBase& Poly) const;

	/// True if this crosses the ray, returns the intersection points.
	bool CrossesRay(const C2DLine& Ray, C2DPointSet* IntersectionPts) const;


	/// True if this overlaps the other.
	bool Overlaps(const C2DHoledPolyBase& Other) const;
	/// True if this overlaps the other.
	bool Overlaps(const C2DPolyBase& Other) const;

	/// Function to convert polygons to complex polygons. Assigning holes to those that are contained.
	static void PolygonsToHoledPolygons(C2DHoledPolyBaseSet& HoledPolys,
				C2DPolyBaseSet& Polygons);

	/// Returns the overlaps between this and the other complex polygon.
	void GetOverlaps(const C2DHoledPolyBase& Other, C2DHoledPolyBaseSet& HoledPolys, 
							CGrid::eDegenerateHandling eDegen = CGrid::None) const;


	/// Returns the difference between this and the other polygon.
	void GetNonOverlaps(const C2DHoledPolyBase& Other, C2DHoledPolyBaseSet& HoledPolys, 
							CGrid::eDegenerateHandling eDegen = CGrid::None) const;

	/// Returns the union of this and the other.
	void GetUnion(const C2DHoledPolyBase& Other, C2DHoledPolyBaseSet& HoledPolys,
						CGrid::eDegenerateHandling eDegen = CGrid::None) const;

	/// Returns the routes (multiple lines or part polygons) either inside or
	/// outside the polygons provided. These are based on the intersections
	/// of the 2 polygons e.g. the routes / part polygons of one inside the other.
	static void GetRoutes(const C2DHoledPolyBase& Poly1, bool bP1RoutesInside, 
				const C2DHoledPolyBase& Poly2, bool bP2RoutesInside, 
				C2DLineBaseSetSet& Routes1, C2DLineBaseSetSet& Routes2, 
				C2DPolyBaseSet& CompleteHoles1, C2DPolyBaseSet& CompleteHoles2);

	/// Moves this by a small random amount.
	void RandomPerturb(void);
	/// Snaps this to the conceptual grip.
	void SnapToGrid(void);
	/// Returns the boolean result (e.g. union) of 2 shapes. Boolean Operation defined by 
	/// the inside / outside flags.
	void GetBoolean(const C2DHoledPolyBase& Other, C2DHoledPolyBaseSet& HoledPolys,
						bool bThisInside, bool bOtherInside, 
						CGrid::eDegenerateHandling eDegen  = CGrid::None) const;

	/// Transform by the given operator.
	virtual void Transform(CTransformation* pProject);
	/// Transform by the given operator.
	virtual void InverseTransform(CTransformation* pProject);

protected:
	/// The rim.
	C2DPolyBase* m_Rim;
	/// The holes.
	C2DPolyBaseSet m_Holes;

};

#endif