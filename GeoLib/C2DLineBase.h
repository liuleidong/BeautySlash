/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DLineBase.h
\brief File for the C2DLineBase class.

File for the C2DLineBase class, an abstract class which defines the functionality
that a Line class must have.

\class C2DLineBase.
\brief C2DLineBase class, an abstract class which defines the functionality
that a Line class must have.

C2DLineBase class, an abstract class which defines the functionality
that a Line class must have (in addition to that in C2DBase) to be a line. Lines
that inherit properly from this can be used in the C2DPolyBase class to form a
closed area / shape with all the associated functions such as distance and 
intersections.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DLINEBASE_H 
#define _GEOLIB_C2DLINEBASE_H

#include "C2DBase.h"


class C2DPointSet;
class C2DLineBaseSet;
class CTransformation;

#ifdef _EXPORTING
	#define CLASS_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define CLASS_DECLSPEC
	#else
		#define CLASS_DECLSPEC		__declspec(dllimport)
	#endif
#endif

class CLASS_DECLSPEC C2DLineBase :		public C2DBase
{
public:

	/// Constructor
	C2DLineBase(C2DBase::E_TYPE Type);
	/// Destructor
	~C2DLineBase(void);
	/// Intersection with another
	virtual bool Crosses(const C2DLineBase& Other, C2DPointSet* IntersectionPts = 0) const = 0;
	/// Minimum distance to a point.
	virtual double Distance(const C2DPoint& Point) const = 0;
	/// Minimum distance to a point.
	virtual double Distance(const C2DPoint& TestPoint, C2DPoint* ptOnThis) const = 0;
	/// Minimum distance to another.
	virtual double Distance(const C2DLineBase& Other, C2DPoint* ptOnThis = 0, 
		C2DPoint* ptOnOther = 0) const = 0;
	/// The point from.
	virtual C2DPoint GetPointFrom(void) const = 0;
	/// The point to.
	virtual C2DPoint GetPointTo(void) const = 0;
	/// The length.
	virtual double GetLength(void) const = 0;
	/// Reverse direction of the line.
	virtual void ReverseDirection(void) = 0;
	/// Given a set of points on the line, this function creates sub lines defined by those points.
	/// Required by intersection, union and difference functions in the C2DPolyBase class.
	virtual	void GetSubLines(const C2DPointSet& PtsOnLine, C2DLineBaseSet& LineSet) const = 0;
	/// Snaps this to the conceptual grid.
	virtual void SnapToGrid(void) = 0;
	/// Transformation by the provided operator.
	virtual void Transform(CTransformation* pProject) = 0;
	/// Transformation by the provided operator.
	virtual void InverseTransform(CTransformation* pProject) = 0;
};

#endif