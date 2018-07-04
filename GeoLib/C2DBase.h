/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DBase.h
\brief Abstract base class for all Geometric entities.

Abstract base class for all Geometric entities which should be able to perform
the same set of operations such as Move, Reflect etc.

\class C2DBase
\brief Abstract base class for all Geometric entities.

Abstract base class for all Geometric entities which should be able to perform
the same set of operations such as Move, Reflect etc. 
<P>---------------------------------------------------------------------------*/



#ifndef _GEOLIB_C2DBASE_H 
#define _GEOLIB_C2DBASE_H

#include "MemoryPool.h"

class C2DPoint;
class C2DRect;
class C2DLine;
class CInterval;
class C2DVector;

#ifdef _EXPORTING
	#define CLASS_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define CLASS_DECLSPEC
	#else
		#define CLASS_DECLSPEC		__declspec(dllimport)
	#endif
#endif

class  CLASS_DECLSPEC C2DBase
{
public:
	/// Enumeration of the types the geometric entities that will derive from this
	enum E_TYPE
	{
		// Basic entities
		Point,
		StraightLine,
		ArcedLine,
		PolyBase,
		PolyHoledBase,
		PolyLine,
		PolyLineHoled,
		PolyArc,
		PolyArcHoled,
		Route,
		Circle,
		Segment,
		Rect,
		Triangle,

		// Sets defined
		BaseSet,
		LineBaseSet,
		LineBaseSetSet,
		PointSet,
		StraightLineSet,
		ArcedLineSet,
		PolyBaseSet,
		PolyHoledBaseSet,
		PolyLineSet,
		PolyLineHoledSet,
		PolyArcSet,
		PolyArcHoledSet,
		RouteSet,
		CircleSet,
		SegmentSet,
		RectSet,
		TriangleSet,

	};
	

	/// Constructor must provide the type
	C2DBase(E_TYPE Type) {m_Type = Type ;}
	/// Destructor
	virtual ~C2DBase(void){ ;}
	/// Moves the entity
	virtual void Move(const C2DVector& Vector) = 0;
	/// Rotation around the origin given
	virtual void RotateToRight(double dAng, const C2DPoint& Origin) = 0;
	/// Grows the entity
	virtual void Grow(double dFactor, const C2DPoint& Origin) = 0;
	/// Reflection in a point
	virtual void Reflect(const C2DPoint& Point) = 0;
	/// Reflection in a line
	virtual void Reflect(const C2DLine& Line) = 0;
	/// Distance to a point
	virtual double Distance(const C2DPoint& Point) const = 0;
	/// Return the bounding rect
	virtual void GetBoundingRect(C2DRect& Rect) const = 0;
	/// Projects this onto the line provided with the interval on the line returned.
	virtual void Project(const C2DLine& Line, CInterval& Interval) const = 0;
	/// Projects this onto the vector provided with the interval on the line returned.
	virtual void Project(const C2DVector& Vector, CInterval& Interval) const = 0;
	/// Snaps this to the conceptual grid.
	virtual void SnapToGrid(void) = 0;
	/// Returns the type
	E_TYPE GetType(void) const {return m_Type;}

protected:
	/// The type of geometric entity
	E_TYPE m_Type;
};

#endif