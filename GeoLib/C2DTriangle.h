/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DTriangle.h
\brief Declaration file for the C2DTriangle Class.

Declaration file for C2DTriangle, a class which represents a triangle.

\class C2DTriangle.
\brief Class which represents a triangle.

Class which represents a triangle with a variety of associated functions.
<P>---------------------------------------------------------------------------*/


#ifndef _GEOLIB_C2DTRIANGLE_H 
#define _GEOLIB_C2DTRIANGLE_H


#include "C2DBase.h"
#include "C2DPoint.h"
#include "MemoryPool.h"



class C2DRect;

#ifdef _EXPORTING
	#define CLASS_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define CLASS_DECLSPEC
	#else
		#define CLASS_DECLSPEC		__declspec(dllimport)
	#endif
#endif



class CLASS_DECLSPEC C2DTriangle : public C2DBase
{
public:
	_MEMORY_POOL_DECLARATION
	/// Constructor.
	C2DTriangle(void);
	/// Constructor with assignment.
	C2DTriangle(const C2DPoint& pt1, const C2DPoint& pt2, const C2DPoint& pt3);
	/// Destructor.
	~C2DTriangle(void);
	/// Assignement.
	void Set(const C2DPoint& pt1, const C2DPoint& pt2, const C2DPoint& pt3);
	/// True if the 3 are collinear.
	bool Collinear(void) const;
	/// Returns the area.
	double GetArea(void)  const;
	/// Returns the area signed (indicating weather clockwise or not).
	double GetAreaSigned(void)  const;
	/// True if clockwise.
	bool IsClockwise(void) const {return GetAreaSigned() < 0;}
	/// Returns the circumcentre.
	C2DPoint GetCircumCentre(void) const;
	/// Returns the Fermat point (also known as the Torricelli point).
	C2DPoint GetFermatPoint(void) const;
	/// InCentre function.
	C2DPoint GetInCentre(void) const;

	/// Returns the perimeter.
	double GetPerimeter(void) const;
	/// Returns true if the point is contained.
	bool Contains(const C2DPoint& ptTest);
	/// Moves the triangle.
	void Move(const C2DVector& Vector);
	/// Rotates the triangle.
	void RotateToRight(double dAng, const C2DPoint& Origin);
	/// Grows the triangle.
	void Grow(double dFactor, const C2DPoint& Origin);
	/// Reflects the triangle.
	void Reflect(const C2DPoint& Point);
	/// Reflects the in the line given.
	void Reflect(const C2DLine& Line);
	// Note virtual override requires this function which is why there isn't just a default for the ptOnThis
	/// Distance to a point.
	double Distance(const C2DPoint& ptTest) const;
	/// Distance to a point.
	double Distance(const C2DPoint& ptTest, C2DPoint* ptOnThis) const;
	/// Distance to another.
	double Distance(const C2DTriangle& Other, C2DPoint* ptOnThis = 0, C2DPoint* ptOnOther = 0) const;	
	/// Returns the bounding rect.
	void GetBoundingRect(C2DRect& Rect) const;
	/// Data access.
	const C2DPoint& GetPoint1(void) const { return m_pt1;}
	/// Data access.
	const C2DPoint& GetPoint2(void) const { return m_pt2;}
	/// Data access.
	const C2DPoint& GetPoint3(void) const { return m_pt3;}

	/// Static version of area signed function.
	static double GetAreaSigned(const C2DPoint& pt1, const C2DPoint& pt2, const C2DPoint& pt3);
	/// Returns whether the triangle is clockwise.
	static bool IsClockwise(const C2DPoint& pt1, const C2DPoint& pt2, const C2DPoint& pt3);
	/// Static version of circumcentre function.
	static C2DPoint GetCircumCentre(const C2DPoint& pt1, const C2DPoint& pt2, const C2DPoint& pt3);
	/// Static version of Fermat point function.
	static C2DPoint GetFermatPoint(const C2DPoint& pt1, const C2DPoint& pt2, const C2DPoint& pt3);
	/// Static version of InCentre function.
	static C2DPoint GetInCentre(const C2DPoint& pt1, const C2DPoint& pt2, const C2DPoint& pt3);
	/// Static collinear.
	static bool Collinear(const C2DPoint& pt1, const C2DPoint& pt2, const C2DPoint& pt3);

	/// Projects this onto the line given.
	void Project(const C2DLine& Line, CInterval& Interval) const;
	/// Projects this onto the vector given.
	void Project(const C2DVector& Vector, CInterval& Interval) const;
	/// Snaps to the conceptual grid.
	void SnapToGrid(void);
private:
	/// Point 1.
	C2DPoint m_pt1;
	/// Point 2.
	C2DPoint m_pt2;
	/// Point 3.
	C2DPoint m_pt3;
};

#endif