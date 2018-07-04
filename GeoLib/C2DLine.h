/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DLine.h
\brief Declaration file for the C2DLine Class.

Declaration file for C2DLine, a class which represents a line segment i.e. a 
finite line.

\class C2DLine.
\brief Class which represents a line segment i.e. a finite line.

Class which represents a line segment i.e. a finite line. Stores data as a point 
and a vector representing the movement to the new point.
<P>---------------------------------------------------------------------------*/
#ifndef _GEOLIB_C2DLINE_H 
#define _GEOLIB_C2DLINE_H

#include "C2DLineBase.h"
#include "C2DVector.h"
#include "C2DPoint.h"
#include "MemoryPool.h"


class C2DLineBaseSet;
class C2DPointSet;
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


class CLASS_DECLSPEC C2DLine : public C2DLineBase
{
public:
	_MEMORY_POOL_DECLARATION

	/// Constructor.
	C2DLine(void);
	/// ~Destructor.
	~C2DLine(void);
	/// Constructor assigns from a point and vector.
	C2DLine(const C2DPoint& PointFrom, const C2DVector& VectorTo);
	/// Constructor assigns from 2 points.
	C2DLine(const C2DPoint& PointFrom, const C2DPoint& PointTo);
	/// Constructor assigns from another line.
	C2DLine(const C2DLine& Other);

	/// Assigment from 2 points.
	void Set(const C2DPoint& PointFrom, const C2DPoint& PointTo);
	/// Assignment using a point and a vector.
	void Set(const C2DPoint& PointFrom, const C2DVector& VectorTo);
	/// Sets the point that this is going to.
	void SetPointTo(const C2DPoint& PointTo);
	/// Sets the point that this is going from changing the vector as well.
	void SetPointFrom(const C2DPoint& PointFrom);
	/// Set Length
	void SetLength (double dLength) {vector.SetLength(dLength);}
	/// Reverses the direction of the line.
	void ReverseDirection(void);
	/// True if the point is to the right of the line.
	bool IsOnRight(const C2DPoint& OtherPoint) const;

	/// Returns the second point.
	virtual C2DPoint GetPointTo(void) const;
	/// Returns the first point.
	virtual C2DPoint GetPointFrom(void) const {return point;}

	/// True if this line would cross the other if it were infinite.
	bool WouldCross(const C2DLine& Other) const;

	/// True if this line crosses the other line, returns the intersection pt.
	virtual bool Crosses(const C2DLineBase& Other, C2DPointSet* IntersectionPts = 0) const;

	/// True if this line crosses the other. Returns the point is a collection is provided.
	/// Returns whether it would cross on this or on the other. Can opt to get the point 
	/// where the cross would occur (if not parallel) even if they don't cross.
	bool Crosses(const C2DLine& Other, C2DPointSet* IntersectionPts = 0, 
		bool* pbOnThis = 0, bool* pbOnOther = 0, bool bAddPtIfFalse = false) const;
	/// True if the ray provided (infinite line starting from the first point) crosses this. 
	bool CrossesRay(const C2DLine& Ray, 
		C2DPointSet* IntersectionPts = 0) const;

	/// Returns the distance from this to the point.
	virtual double Distance(const C2DPoint& TestPoint) const;
	/// Returns the distance from this to the point.
	virtual double Distance(const C2DPoint& TestPoint, C2DPoint* ptOnThis) const;
	/// Returns the distance from this to the other line.
	double Distance(const C2DLine& Other, C2DPoint* ptOnThis = 0, 
		C2DPoint* ptOnOther = 0)  const;
	/// Distance to another line.
	double Distance(const C2DLineBase& Other, C2DPoint* ptOnThis = 0, 
		C2DPoint* ptOnOther = 0) const;

	/// Returns the distance from this to the point with this as a ray.
	double DistanceAsRay(const C2DPoint& TestPoint) const;
	/// Returns the distance from this to the point with this as a ray.
	double DistanceAsRay(const C2DPoint& TestPoint,  C2DPoint* ptOnThis) const;



	/// Gets the mid point on the line.
	C2DPoint GetMidPoint(void) const;
	/// Gets the point on the line given by the factor. e.g. 0.5 = mid point.
	C2DPoint GetPointOn(double dFactorFromStart) const;
	/// Returns the bounding rectangle.
	void GetBoundingRect(C2DRect& Rect) const;
	/// Returns the length of the line.
	virtual double GetLength(void) const {return vector.GetLength();}
	/// Moves the line by the vector given.
	virtual void Move(const C2DVector& vector) {point.Move(vector);}
	/// Rotation.
	void RotateToRight(double dAng, const C2DPoint& Origin);
	/// Grows the line in size.
	virtual void Grow(double dFactor, const C2DPoint& Origin);
	/// Point reflection.
	void Reflect(const C2DPoint& Point);
	/// Reflects the in the line given.
	void Reflect(const C2DLine& Line);

	/// Grows from the centre by the amount.
	void GrowFromCentre(double dFactor);

	/// Projects this onto the line given.
	void Project(const C2DLine& Line, CInterval& Interval) const;
	/// Projects this onto the vector given.
	void Project(const C2DVector& Vector, CInterval& Interval) const;
	/// Returns the lines that make up this defined by the points which are assumed
	/// to be on this line. i.e. splits the line up.
	void GetSubLines(const C2DPointSet& PtsOnLine, C2DLineBaseSet& LineSet) const;
	/// Snaps this to the conceptual grid.
	void SnapToGrid(void);

	double GetY(double dx) const;

	/// 
	bool OverlapsVertically(const C2DLine& Other, double& dVerticalDistance, 
									C2DPoint& ptOnThis, C2DPoint& ptOnOther) const;

	bool OverlapsAbove(const C2DLine& Other, double& dVerticalDistance, 
									C2DPoint& ptOnThis, C2DPoint& ptOnOther) const;

	bool Join(C2DLine& Other);

	virtual void Transform(CTransformation* pProject);

	virtual void InverseTransform(CTransformation* pProject);

	/// The first point.
	C2DPoint point;
	/// The vector to the second point.
	C2DVector vector;

};

#endif