/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DCircle.h
\brief Declaration file for the C2DCircle Class.

Declaration file for C2DCircle, a class which represents a circle.

\class C2DCircle.
\brief class which represents a circle.

Class which represents a circle.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DCIRCLE_H 
#define _GEOLIB_C2DCIRCLE_H


#include "C2DPoint.h"
#include "MemoryPool.h"


class C2DTriangle;

#ifdef _EXPORTING
	#define CLASS_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define CLASS_DECLSPEC
	#else
		#define CLASS_DECLSPEC		__declspec(dllimport)
	#endif
#endif


class CLASS_DECLSPEC C2DCircle : public C2DBase
{
public:
	_MEMORY_POOL_DECLARATION

	/// Constructor
	C2DCircle(void);
	/// Constructor
	C2DCircle(const C2DPoint& Point, double dRadius);
	/// Copy constructor
	C2DCircle(const C2DCircle& Other);
	/// Destructor
	~C2DCircle(void);
	/// Set
	void Set(const C2DPoint& Centre, double dRadius);
	/// Set centre
	void SetCentre(const C2DPoint& Centre) {m_Centre = Centre;}
	/// Set radius
	void SetRadius(double dRadius) { m_dRadius = dRadius;}
	/// Set to be the minimum bounding circle for the 2 points
	void SetMinimum(const C2DPoint& Point1, const C2DPoint& Point2);
	/// Set to be the minimum bounding circle for the 3 points
	void SetMinimum(const C2DPoint& Point1, 
		const C2DPoint& Point2, const C2DPoint& Point3);
	/// Sets the circle to be the maximum contained circle within the 3 points provided
	void SetInscribed(const C2DPoint& Point1, 
		const C2DPoint& Point2, const C2DPoint& Point3);
	/// Sets the circle to be the maximum contained circle within the 3 points provided
	void SetInscribed(const C2DTriangle& Triangle);

	/// Set to be circle that places all 3 points on the edge
	bool SetCircumscribed (const C2DPoint& Point1, 
		const C2DPoint& Point2, const C2DPoint& Point3);
	/// Set to be circle that places all 3 points of the triangle on the edge
	bool SetCircumscribed (const C2DTriangle& Triangle);
	/// Returns the centre
	const C2DPoint& GetCentre(void) const {return m_Centre;}
	/// Returns the radius
	double GetRadius(void) const;
	/// Returns the area
	double GetArea(void) const;
	/// Returns the perimeter
	double GetPerimeter(void)const;
	/// Assignment
	const C2DCircle& operator=(const C2DCircle& Other);
	/// Bounding rect
	void GetBoundingRect(C2DRect& Rect) const;
	/// Circle-circle intersection
	bool Crosses(const C2DCircle& Other, 
		C2DPointSet* IntersectionPts = 0) const;
	/// Circle-line intersection
	bool Crosses(const C2DLine& Line, 
		C2DPointSet* IntersectionPts = 0) const;
	/// Circle-ray intersection
	bool CrossesRay(const C2DLine& Ray, 
		C2DPointSet* IntersectionPts = 0) const;
	/// Containment
	bool Contains(const C2DPoint& pt) const ;
	/// Proximity
	bool IsWithinDistance(const C2DPoint& pt, double dRange) const;
	/// Move by the verctor
	void Move(const C2DVector& Vector);
	/// Rotation
	void RotateToRight(double dAng, const C2DPoint& Origin);
	/// Grow around the origin
	void Grow(double dFactor, const C2DPoint& Origin);
	/// Reflection
	void Reflect(const C2DPoint& Point);
	/// Reflects throught the line provided.
	void Reflect(const C2DLine& Line);
	/// Distance to a point
	double Distance(const C2DPoint& TestPoint) const;
	/// Distance to a point, returns the closest point on the circle.
	double Distance(const C2DPoint& TestPoint, C2DPoint* ptOnThis = 0) const;
	/// Distance to the line, can retrieve closest points
	double Distance(const C2DLine& Line, 
			C2DPoint* ptOnThis = 0, C2DPoint* ptOnOther = 0) const;
	/// Distance to the other circles, can retrieve closest points
	double Distance(const C2DCircle& Other, 
			C2DPoint* ptOnThis = 0, C2DPoint* ptOnOther = 0) const;
	/// Projection onto the line
	void Project(const C2DLine& Line, CInterval& Interval) const;
	/// Projection onto the vector
	void Project(const C2DVector& Vector, CInterval& Interval) const;
	/// Snaps to the conceptual grid
	void SnapToGrid(void);

private:
	/// The centre
	C2DPoint m_Centre;
	/// The radius
	double m_dRadius;
};

#endif
