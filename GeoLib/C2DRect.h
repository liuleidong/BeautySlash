/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DRect.h
\brief Declaration file for the C2DRect Class

Declaration file file for the C2DRect Class which represents a rectangle.

\class C2DRect
\brief Class representing a rectangle.

C2DRect, a class for a rectangle with horizontal and vertical sides. C2DPoints
represent the top left and bottom right. 
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DRECT_H 
#define _GEOLIB_C2DRECT_H

#include "C2DPoint.h"
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


class CLASS_DECLSPEC C2DRect : public C2DBase
{
public:
	_MEMORY_POOL_DECLARATION

	/// Constructor.
	C2DRect(void);
	/// Constructor.
	~C2DRect(void);
	/// Constructor.
	C2DRect(const C2DPoint& ptTopLeft, const C2DPoint& ptBottomRight);
	/// Constructor.
	C2DRect(double dLeft, double dTop, double dRight, double dBottom);
	/// Constructor sets both the top left and bottom right to equal the rect.
	C2DRect(const C2DPoint& pt );
	/// sets both the top left and bottom right to equal the rect.
	void Set(const C2DPoint& pt);
	/// Set functions. Setting causes the to be true.
	void Set(const C2DPoint& ptTopLeft, const C2DPoint& ptBottomRight);
	/// Set.
	void Set(double dLeft, double dTop, double dRight, double dBottom);
	/**--------------------------------------------------------------------------<BR>
	C2DRect::SetTop <BR>
	Sets the top.
	<P>---------------------------------------------------------------------------*/
	void SetTop(double dTop) {m_TopLeft.y = dTop; }
	/**--------------------------------------------------------------------------<BR>
	C2DRect::SetLeft <BR>
	Sets the left side.
	<P>---------------------------------------------------------------------------*/
	void SetLeft(double dLeft) {m_TopLeft.x = dLeft; }
	/**--------------------------------------------------------------------------<BR>
	C2DRect::SetBottom <BR>
	Sets the bottom.
	<P>---------------------------------------------------------------------------*/
	void SetBottom(double dBottom) {m_BottomRight.y = dBottom; }
	/**--------------------------------------------------------------------------<BR>
	C2DRect::SetRight <BR>
	Sets the right side.
	<P>---------------------------------------------------------------------------*/
	void SetRight(double dRight) {m_BottomRight.x = dRight; }
	/// Clears the rectangle. IsSet set to false.
	void Clear(void);
	/// Expands to include the point. If not set it will be set to equal the point.
	void ExpandToInclude(const C2DPoint& NewPt) ;
	/// Expands to include the rectangle. If not set it will be set to equal the rectangle.
	void ExpandToInclude(const C2DRect& Other);
	/// True if there is an overlap, returns the overlap.
	bool Overlaps(const C2DRect& Other, C2DRect& Overlap) const ;
	/// True if the point is within the rectangle.
	bool Contains(const C2DPoint& Pt) const;
	/// True if the entire other rectangle is within.
	bool Contains(const C2DRect& Other) const;
	/// True if there is an overlap.
	bool Overlaps(const C2DRect& Other) const;
	/// If the area is positive e.g. the top is greater than the bottom.
	bool IsValid(void) const ;

	/// Returns the area.
	double GetArea(void) const ;
	/// Returns the width.
	double Width(void) const ;
	/// Returns the height.
	double Height(void) const ;
	/**--------------------------------------------------------------------------<BR>
	C2DRect::GetTopLeft <BR>
	Returns the top left.
	<P>---------------------------------------------------------------------------*/
	const C2DPoint& GetTopLeft(void) const  { return  m_TopLeft;}
	/**--------------------------------------------------------------------------<BR>
	C2DRect::GetBottomRight <BR>
	Returns the bottom right.
	<P>---------------------------------------------------------------------------*/
	const C2DPoint& GetBottomRight(void) const  { return m_BottomRight;}
	/**--------------------------------------------------------------------------<BR>
	C2DRect::GetTop <BR>
	Returns the top.
	<P>---------------------------------------------------------------------------*/
	double GetTop(void )  const {return  m_TopLeft.y;}
	/**--------------------------------------------------------------------------<BR>
	C2DRect::GetLeft <BR>
	Returns the left side.
	<P>---------------------------------------------------------------------------*/
	double GetLeft(void ) const  {return  m_TopLeft.x ;}
	/**--------------------------------------------------------------------------<BR>
	C2DRect::GetBottom <BR>
	Returns the bottom.
	<P>---------------------------------------------------------------------------*/
	double GetBottom(void ) const {return m_BottomRight.y;}
	/**--------------------------------------------------------------------------<BR>
	C2DRect::GetRight <BR>
	Returns the right side.
	<P>---------------------------------------------------------------------------*/
	double GetRight(void )  const {return m_BottomRight.x ;}
	/// Assignment.
	const C2DRect& operator=(const C2DRect& Other);
	/// Grows it from its centre.
	void Grow(double dFactor);
	/// Grow the height it from its centre.
	void GrowHeight(double dFactor);
	/// Grows the width from its centre.
	void GrowWidth(double dFactor);
	/// Expands from the centre by the fixed amount given.
	void Expand(double dRange);
	/// Grows it from the given point.
	void Grow(double dFactor, const C2DPoint& Origin);
	/// Moves it by the vector.
	void Move(const C2DVector& Vector);
	/// Reflect throught the point given. Switches Top Left and Bottom Right to maintain validity.
	void Reflect(const C2DPoint& Point);
	/// Reflect throught the line by reflecting the centre of the rect and keeping the validity.
	void Reflect(const C2DLine& Line);
	/// Rotates to the right. Note that as the horizontal/vertical line property will be
	/// preserved. If you rotate an object and its bounding box, the box may not still
	/// bound the object.
	void RotateToRight(double dAng, const C2DPoint& Origin);
	/// Returns the distance from this to the point. 0 if the point inside.
	double Distance(const C2DPoint& TestPoint) const;
	/// Returns the distance from this to the other rect. 0 if there is an overlap.
	double Distance(const C2DRect& Other) const;
	/// Returns the bounding rectangle. (Required for virtual base class).
	void GetBoundingRect(C2DRect& Rect) const { Rect = *this;}

	/**--------------------------------------------------------------------------<BR>
	C2DRect::Scale <BR>
	Scales the rectangle accordingly.
	<P>---------------------------------------------------------------------------*/
	void Scale(const C2DPoint ptScale) {m_TopLeft =  m_TopLeft * ptScale; 
								m_BottomRight = m_BottomRight * ptScale;}
	/// Returns the centre.
	C2DPoint GetCentre(void) const ;
	/// Returns the point which is closest to the origin (0,0).
	C2DPoint GetPointClosestToOrigin(void) const;
	/// Returns the point which is furthest from the origin (0,0).
	C2DPoint GetPointFurthestFromOrigin(void) const;

	/// Projection onto the line
	void Project(const C2DLine& Line, CInterval& Interval) const;
	/// Projection onto the vector
	void Project(const C2DVector& Vector, CInterval& Interval) const;
	/// Snaps this to the conceptual grid.
	void SnapToGrid(void);

	// True if this is above or below the other.
	bool OverlapsVertically( const C2DRect& Other) const;
	/// True if this is above the other.
	bool OverlapsAbove( const C2DRect& Other) const;
	/// True if this is below the other.
	bool OverlapsBelow( const C2DRect& Other) const;

private:
	/// Top left.
	C2DPoint  m_TopLeft;
	/// Bottom right.
	C2DPoint m_BottomRight;
};


#endif