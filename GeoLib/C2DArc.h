/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DArc.h
\brief Declaration file for the C2DArc Class.

Declaration file for C2DArc, a class which represents a curved line.

\class C2DArc.
\brief Class which represents a curved line.

Class which represents a curved line, the curve being a part of a circle. Data
is held as a line and a radius with information about the orientation of the 
associated circle.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DARC_H 
#define _GEOLIB_C2DARC_H

#include "C2DLine.h"
#include "MemoryPool.h"



class C2DLineBaseSet;


#ifdef _EXPORTING
	#define CLASS_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define CLASS_DECLSPEC
	#else
		#define CLASS_DECLSPEC		__declspec(dllimport)
	#endif
#endif

class CLASS_DECLSPEC C2DArc : public C2DLineBase
{
public:
	_MEMORY_POOL_DECLARATION
	/// Contructor.
	C2DArc(void);
	/// Contructor.
	C2DArc(const C2DArc& Other);
	/// Destructor.
	~C2DArc(void);
	/// Constructor with assignment.
	C2DArc(const C2DPoint& PtFrom, const C2DPoint& PtTo, double dRadius, 
		bool bCentreOnRight = true, bool bArcOnRight = false);
	/// Constructor with assignment.
	C2DArc(const C2DPoint& PtFrom, const C2DVector& Vector, double dRadius, 
		bool bCentreOnRight = true, bool bArcOnRight = false);
	/// Constructor with assignment.
	C2DArc(const C2DLine& Line, double dRadius, 
		bool bCentreOnRight = true, bool bArcOnRight = false);
	/// Assignment.
	void Set(const C2DPoint& PtFrom, const C2DPoint& PtTo, double dRadius, 
		bool bCentreOnRight = true, bool bArcOnRight = false);
	/// Assignment.
	void Set(const C2DPoint& PtFrom, const C2DVector& Vector, double dRadius, 
		bool bCentreOnRight = true, bool bArcOnRight = false);
	/// Assignment.
	void Set(const C2DLine& Line, double dRadius, 
		bool bCentreOnRight = true, bool bArcOnRight = false);
	/// Assignment.
	void Set(const C2DLine& Line, const C2DPoint ptOnArc);


	/// True if the radius is large enough to join the points.
	bool IsValid(void) const;

	bool MakeValid(void);

	/// Returns the corresponding circle's centre.
	C2DPoint GetCircleCentre(void) const;
	/// Returns the length of the curve.
	virtual double GetLength(void) const ;

	/// Gets the bounding rectangle.	
	void GetBoundingRect(C2DRect& Rect) const;
	/// Gets the angle of the minimum segment. Always +ve and less than PI. In radians.
	double GetSegmentAngle(void) const;


	/// Returns the first point.
	virtual C2DPoint GetPointFrom(void) const { return m_Line.point ;}
	/// Returns the seconds point.
	virtual C2DPoint GetPointTo(void) const { return m_Line.GetPointTo() ;}

	/// True if this crosses the line given as a base class.
	virtual bool Crosses(const C2DLineBase& Other, 
		C2DPointSet* IntersectionPts = 0) const;

	/// True if this crosses a line.
	bool Crosses(const C2DLine& Line, 
		C2DPointSet* IntersectionPts = 0) const;
	/// True if this crosses a curve.
	bool Crosses(const C2DArc& Other, 
		C2DPointSet* IntersectionPts = 0) const;
	/// True if this crosses the ray given.
	bool CrossesRay(const C2DLine& Ray, 
		C2DPointSet* IntersectionPts = 0) const;


	/// Distance between this and the test point.
	virtual double Distance(const C2DPoint& TestPoint) const;
	/// Distance between this and the test point.
	virtual double Distance(const C2DPoint& TestPoint, C2DPoint* ptOnThis) const;

	/// Distance between this and another line.
	double Distance(const C2DArc& Other, 
			C2DPoint* ptOnThis = 0, C2DPoint* ptOnOther = 0) const;

	/// Distance between this and another straight line.
	double Distance(const C2DLine& Line, 
			C2DPoint* ptOnThis = 0, C2DPoint* ptOnOther = 0) const;

	/// Returns the distance from the other line to this providing points.
	virtual double Distance(const C2DLineBase& Other, C2DPoint* ptOnThis = 0, 
		C2DPoint* ptOnOther = 0) const;

	/// Returns the projection of this onto the line provided given as the interval on
	/// (or off) the line. Interval given as distance from the start of the line.
	void Project(const C2DLine& Line, CInterval& Interval) const;
	/// Returns the projection of this onto the vector. The vector is equivalent to a line
	/// from (0, 0).
	void Project(const C2DVector& Vector, CInterval& Interval) const;


	/// Gets the point half way along the curve.
	C2DPoint GetMidPoint(void) const;
	/// Gets the point on the curve determined by the factor.
	C2DPoint GetPointOn(double dFactorFromStart) const;
	/// Gets the radius.
	double GetRadius(void) const {return m_dRadius;}
	/// Gets the straight line.
	const C2DLine& GetLine(void) const {return m_Line;}
	/// True if the centre is on the right.
	bool GetCentreOnRight(void) const {return m_bCentreOnRight;}
	/// True is the curve is on the right.
	bool GetArcOnRight(void) const {return m_bArcOnRight;}
	/// Move by the vector given.
	void Move(const C2DVector& vector) {m_Line.Move(vector);}
	/// Rotates the to the right.
	void RotateToRight(double dAng, const C2DPoint& Origin)  
		{m_Line.RotateToRight(dAng, Origin);}
	/// Grow relative to the origin.
	void Grow(double dFactor, const C2DPoint& Origin);
	/// Reflect in the point.
	void Reflect(const C2DPoint& point);
	/// Reflects throught the line provided.
	void Reflect(const C2DLine& Line);
	/// Reverses the direction.
	virtual void ReverseDirection(void);
	/// Returns the lines that go to make this up based on the set of points 
	// provided which are assumed to be on the line.
	void GetSubLines(const C2DPointSet& PtsOnLine, C2DLineBaseSet& LineSet) const;
	/// Snaps this to the conceptual grid.
	void SnapToGrid(void);

	virtual void Transform(CTransformation* pProject);

	virtual void InverseTransform(CTransformation* pProject);

private:
	/// The radius
	double m_dRadius;
	/// Whether the associated circle centre is to the right of the line.
	bool m_bCentreOnRight;
	/// Whether the arc is to the right of the line.
	bool m_bArcOnRight;
	/// The straight line used to define the start and end points of the line
	C2DLine m_Line;
};


#endif