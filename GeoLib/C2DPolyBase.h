/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DPolyBase.h
\brief Base class template for a 2D shape. Requires some sort of line class with the 
relevant functions.

\class C2DPolyBase
\brief A class which forms the basis of a 2 dimensional area or shape such as a 
polygon. Contains a closed set of lines of some sort, based on the abstract class
C2DLineBase. Lines can be of mixed type. 
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DPOLYBASE_H 
#define _GEOLIB_C2DPOLYBASE_H


#include "C2DLineBaseSet.h"
#include "C2DRect.h"
#include "Grid.h"
#include "C2DRectSet.h"
#include "MemoryPool.h"



class C2DHoledPolyBase;
class C2DHoledPolyBaseSet;
class C2DPolyBaseSet;
class C2DLineBaseSetSet;

#ifdef _POLY_EXPORTING
	#define POLY_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define POLY_DECLSPEC
	#else
		#define POLY_DECLSPEC		__declspec(dllimport)
	#endif
#endif

class POLY_DECLSPEC C2DPolyBase : public C2DBase
{
public:
	_MEMORY_POOL_DECLARATION

	/// Constructor
	C2DPolyBase(void);
	/// Constructor
	C2DPolyBase(const C2DPolyBase& Other);
	/// Destructor
	~C2DPolyBase(void);
	/// Assigment.
	const C2DPolyBase& operator=(const C2DPolyBase& Other);
	/// Assigment.
	void Set(const C2DPolyBase& Other);

	/// Creates directly from a set of lines by extracting them from the set provided.
	void CreateDirect(C2DLineBaseSet& Lines);
	/// Creates from the set of lines using copies.
	void Create(const C2DLineBaseSet& Lines);
	/// True if the point is in the shape.
	bool Contains(const C2DPoint& pt) const;
	/// True if it entirely contains the other.
	bool Contains(const C2DPolyBase& Other) const;
	/// True if it entirely contains the other.
	bool Contains(const C2DHoledPolyBase& Other) const;
	/// True if it entirely contains the line.
	bool Contains(const C2DLineBase& Line) const;

	/// True if any of the lines cross.
	bool HasCrossingLines(void) const;
	/// Distance of the point from the shape. Returns -ve if inside.
	double Distance(const C2DPoint& pt) const;
	/// Distance to the line.
	double Distance(const C2DLineBase& Line) const;
	/// Distance of the poly from the shape. 
	double Distance(const C2DPolyBase& Other, 
		C2DPoint* ptOnThis = 0, C2DPoint* ptOnOther = 0 ) const;

	/// True if the point is with the range given to the shape or inside.
	bool IsWithinDistance(const C2DPoint& pt, double dRange) const;
	/// Returns the bounding rectangle.
	void GetBoundingRect(C2DRect& Rect) const {Rect = m_BoundingRect; }
	/// Returns the bournding rectangle.
	const C2DRect& GetBoundingRect(void) const {return m_BoundingRect;}
	/// Returns a pointer to the line, 0 if invalid. Cycliclly handles indexes over the max.
	const C2DLineBase* GetLine(unsigned int i) const;
	/// Returns the line set.
	const C2DLineBaseSet& GetLines(void) const { return m_Lines;}
	/// Returns a pointer to the line rect.
	const C2DRect* GetLineRect(unsigned int i) const;
	/// number of line rectangles (should be the same as the number of lines!).
	unsigned int GetLineRectCount(void) const {return m_LineRects.size();}
	// Returns the number of lines.
	unsigned int GetLineCount(void) const {return m_Lines.size();}
	/// Calculates the perimeter.
	double GetPerimeter(void) const;


	/// Clears all.
	void Clear(void);
	/// Moves the shape.
	void Move(const C2DVector& vector);
	/// Rotates the polygon to the right around the origin given.
	void RotateToRight(double dAng, const C2DPoint& Origin);
	/// Grows the area around the origin.
	void Grow(double dFactor, const C2DPoint& Origin);
	/// Reflects the area about the point.
	void Reflect(const C2DPoint& point);
	/// Reflects throught the line provided.
	void Reflect(const C2DLine& Line);

	/// True if it crosses the other.
	bool Crosses(const C2DPolyBase& Other) const;
	/// Intersection with another.
	bool Crosses(const C2DPolyBase& Other, C2DPointSet* IntersectionPts) const;
	/// Intersection with a line base. Finds out what sort of line it is first.
	bool Crosses(const C2DLineBase& Line) const;
	/// True if it crosses the line. Provides the intersection points.
	bool Crosses(const C2DLineBase& Line, C2DPointSet* IntersectionPts) const;
    /// True if it crossed the line.Provides the intersection points and crossed lines
    bool Crosses(const C2DLineBase& Line,C2DPointSet* IntersectionPts,C2DLineBaseSet *IntersectionLines) const;

	/// True if it crosses the ray. Provides the intersection points.
	bool CrossesRay(const C2DLine& Ray, C2DPointSet* IntersectionPts) const;
	/// True if this overlaps the other.
	bool Overlaps(const C2DPolyBase& Other) const ;
	/// True if this overlaps the other.
	bool Overlaps(const C2DHoledPolyBase& Other) const ;

	/// True if it is a closed shape which it should be.
	bool IsClosed(void) const;
	/// Snap to the conceptual grid.
	void SnapToGrid(void);
	// True if all the arcs are valid
	bool IsValidArcs(void) const;
	// Makes all arc valid if not already by adjusting radius to minimum required.
	// Returns the number affected.
	unsigned int MakeValidArcs(void);
	// True if there are arcs in the shape.
	bool HasArcs(void) const;

	// Removes all lines whose end is the same is the start. Returns the number found.
	unsigned int RemoveNullLines(void);

	double GetArea(void) const;

	virtual double GetAreaSigned(void) const;

	bool IsClockwise(void) const;

	void MakeClockwise(void);

	/// Reverses the direction of the lines.
	void ReverseDirection(void);

	/// Returns the non-overlaps of this with another.
	void GetNonOverlaps(const C2DPolyBase& Other, C2DHoledPolyBaseSet& Polygons, 
										CGrid::eDegenerateHandling eDegen = CGrid::None) const ;

	/// Returns the union of this with another.
	void GetUnion(const C2DPolyBase& Other, C2DHoledPolyBaseSet& Polygons,
									CGrid::eDegenerateHandling eDegen = CGrid::None) const ;

	/// Returns the overlaps of this with another.
	void GetOverlaps(const C2DPolyBase& Other, C2DHoledPolyBaseSet& Polygons,
										CGrid::eDegenerateHandling eDegen = CGrid::None) const ;
	/// Returns the routes (collection of lines and sublines) either inside or outside another
	/// Given the intersection points.
	void GetRoutes(C2DPointSet& IntPts, CIndexSet& IntIndexes, 
		C2DLineBaseSetSet& Routes, bool bStartInside,  bool bRoutesInside = true) const;

	/// Gets the boolean operation with the other. e.g. union / intersection.
	void GetBoolean(const C2DPolyBase& Other, C2DHoledPolyBaseSet& Polygons,
						bool bThisInside, bool bOtherInside, 
						CGrid::eDegenerateHandling eDegen  = CGrid::None) const;

	/// Projection onto the line
	void Project(const C2DLine& Line, CInterval& Interval) const;
	/// Projection onto the vector
	void Project(const C2DVector& Vector, CInterval& Interval) const;
	/// Moves this by a tiny random amount.
	void RandomPerturb(void);

	virtual void Transform(CTransformation* pProject);

	virtual void InverseTransform(CTransformation* pProject);

	/// Returns the Route collections for 2 polygons.
	static void GetRoutes(const C2DPolyBase& Poly1, bool bP1RoutesInside, 
				const C2DPolyBase& Poly2, bool bP2RoutesInside, 
				C2DLineBaseSetSet& Routes1, C2DLineBaseSetSet& Routes2);

	/// Given a set of routes, this function converts them all to created polygon.
	static void RoutesToPolygons(C2DPolyBaseSet& Polygons, C2DLineBaseSetSet& Routes);

	

protected:


	/// Forms the bounding rectangle.
	void MakeBoundingRect(void);
	/// Forms the bounding rectangle.
	void MakeLineRects(void);
	/// The lines
	C2DLineBaseSet m_Lines;
	/// The bounding rectangle.
	C2DRect m_BoundingRect;
	/// The LINE bounding rectangles.
	C2DRectSet m_LineRects;
};


#endif
