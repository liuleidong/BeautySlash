/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DSegment.h
\brief Declaration file for C2DSegment.

Declaration file for C2DSegment which is a segment of a circle.

\class C2DSegment
\brief C2DSegment represents a segment which is part of a circle, sliced in 2
with a line.

C2DSegment which is a segment of a circle defined by a line, a circle radius, 
whether the circle centre and the arc are on the right or left.
<P>---------------------------------------------------------------------------*/
#ifndef _GEOLIB_C2DSEGMENT_H 
#define _GEOLIB_C2DSEGMENT_H

#include "C2DArc.h"
#include "MemoryPool.h"


#ifdef _EXPORTING
	#define CLASS_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define CLASS_DECLSPEC
	#else
		#define CLASS_DECLSPEC		__declspec(dllimport)
	#endif
#endif


class CLASS_DECLSPEC C2DSegment : public C2DBase
{
public:
	_MEMORY_POOL_DECLARATION
	/// Constructor.
	C2DSegment(void);
	/// Destructor.
	~C2DSegment(void);
	/// Constructor.
	C2DSegment(const C2DArc& Arc);
	/// Constructor.
	C2DSegment(const C2DPoint& PtFrom, const C2DPoint& PtTo, double dRadius, 
		bool bCentreOnRight = true, bool bArcOnRight = false);
	/// Constructor.
	C2DSegment(const C2DPoint& PtFrom, const C2DVector& Vector, double dRadius, 
		bool bCentreOnRight = true, bool bArcOnRight = false);
	/// Constructor.
	C2DSegment(const C2DLine& Line, double dRadius, 
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
	/// False if the radius is too small to join the points given.
	bool IsValid(void) const;
	/// Returns the centre of the corresponding circle.
	C2DPoint GetCircleCentre(void) const;
	/// Returns the perimeter of the shape.
	double GetPerimeter(void) const;
	/// Returns the length of the arc.
	double GetArcLength(void) const;
	/// Returns the bounding rectangle.
	void GetBoundingRect(C2DRect& Rect) const;
	/// Returns the inverse of this i.e. the other part of the circle to this.
	void GetInverse(C2DSegment& Other) const;
	/// Always +ve and LESS than PI. In radians.
	double GetSegmentAngle(void) const; 
	/// Returns the area.
	double GetArea(void) const;
	/// Returns the area which is positive if anti-clockwise -ve if clockwise
	double GetAreaSigned(void) const;
	/// Returns the centroid.
	C2DPoint GetCentroid(void) const;

	/**--------------------------------------------------------------------------<BR>
	C2DSegment::GetPointFrom <BR>
	Gets the first point on the straight line.
	<P>---------------------------------------------------------------------------*/
	const C2DPoint& GetPointFrom(void) const { return m_Arc.GetLine().point ;}
	/**--------------------------------------------------------------------------<BR>
	C2DSegment::GetPointTo <BR>
	Gets the second point on the straight line.
	<P>---------------------------------------------------------------------------*/
	C2DPoint GetPointTo(void)  const{ return m_Arc.GetLine().GetPointTo() ;}
	/**--------------------------------------------------------------------------<BR>
	C2DSegment::GetArc <BR>
	Returns a reference to the Arc.
	<P>---------------------------------------------------------------------------*/
	const C2DArc& GetArc(void) const {return m_Arc;}
	/**--------------------------------------------------------------------------<BR>
	C2DSegment::GetLine <BR>
	Returns a reference to the line.
	<P>---------------------------------------------------------------------------*/
	const C2DLine& GetLine(void) const {return m_Arc.GetLine();}
	/// Returns whether the point is in the shape.
	bool Contains( const C2DPoint& TestPoint) const;

	/**--------------------------------------------------------------------------<BR>
	C2DSegment::Move <BR>
	Moves the segment by the vector given.
	<P>---------------------------------------------------------------------------*/
	void Move(const C2DVector& vector) {m_Arc.Move(vector);}

	/**--------------------------------------------------------------------------<BR>
	C2DSegment::RotateToRight <BR>
	Rotates the segment to the right by the angle.
	<P>---------------------------------------------------------------------------*/
	void RotateToRight(double dAng, const C2DPoint& Origin)  
		{m_Arc.RotateToRight(dAng, Origin);}

	/**--------------------------------------------------------------------------<BR>
	C2DSegment::Grow <BR>
	Grows the segment by the factor from the origin provided.
	<P>---------------------------------------------------------------------------*/
	void Grow(double dFactor, const C2DPoint& Origin) {m_Arc.Grow(dFactor, Origin);}

	/**--------------------------------------------------------------------------<BR>
	C2DSegment::Reflect <BR>
	Reflects the shape throught the point given.
	<P>---------------------------------------------------------------------------*/
	void Reflect(const C2DPoint& point) {m_Arc.Reflect( point);}
	/// Reflects the in the line given.
	void Reflect(const C2DLine& Line) {m_Arc.Reflect( Line);}

	/// Returns the distance to the point given.
	double Distance(const C2DPoint& TestPoint) const;

	/// Projects this onto the line given.
	void Project(const C2DLine& Line, CInterval& Interval) const;
	/// Projects this onto the vector given.
	void Project(const C2DVector& Vector, CInterval& Interval) const;
	/// Snaps this to the conceptual grid.
	void SnapToGrid(void);
private:	
	/// The data (since the curve contains data about the straight line, this is all we need).
	C2DArc m_Arc;
};

#endif