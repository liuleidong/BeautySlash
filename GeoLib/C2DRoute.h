/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DRoute.h
\brief Declaration file for a class representing a set of connected points.

Declaration file for a class representing a set of connected points which
do not necassarily join.

\class C2DRoute
\brief Class representing a set of connected points which do not necassarily join.

Class representing a set of connected points i.e. a route.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DROUTE_H 
#define _GEOLIB_C2DROUTE_H

#include "C2DPointSet.h"
#include "C2DBase.h"
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

class CLASS_DECLSPEC C2DRoute : public C2DBase
{
public:
	_MEMORY_POOL_DECLARATION
	/// Constructor.
	C2DRoute(void);
	/// Destructor.
	~C2DRoute(void);
	/// Adds a point to the end of the route.
	void AddPoint(const C2DPoint& pt);

	/// Adds a point to the end of the route.
	void AddPointDirect( C2DPoint* pt);

	/// Adds a point to the end of the route.
	void InsertPointDirect( unsigned int nIndx, C2DPoint* pt);
	/// Extracts a pointer to a C2DPoint. Removes the point from the Route.
	C2DPoint* ExtractPoint(int nIndx) {return m_Points.ExtractAt(nIndx);}
	/// Extracts the last point frm the Route.
	C2DPoint* ExtractLast(void) {return m_Points.ExtractLast();}
	/**--------------------------------------------------------------------------<BR>
	C2DRoute::GetPointFrom <BR>
	Returns the number of points in the route.
	<P>---------------------------------------------------------------------------*/
	int GetPointsCount(void) const { return static_cast<int>(m_Points.size());}
	/// Returns the number of points in the route.
	/**--------------------------------------------------------------------------<BR>
	C2DRoute::GetLinesCount <BR>
	Gets the number of lines in the route.
	<P>---------------------------------------------------------------------------*/
	unsigned int GetLinesCount(void) const;
	/// Removes the last point.
	void RemoveLast(void);
	/// Clears the route.
	void Clear(void);
	/// Returns a reference to the point required.
	const C2DPoint& GetPoint(int nPointIndex) const;
	/// Assignment to another.
	const C2DRoute& operator=(const C2DRoute& Other);
	/// Returns true if it crosses a line given and the list of intersection points.
	bool Crosses(const C2DLine& Line, C2DPointSet* IntersectionPts);
	/// Adds the points of another to this if they have a common end which is either
	/// the first or last point.
	bool AddIfCommonEnd(C2DRoute& Other);
	/// Check for repeated points. 
	bool HasRepeatedPoints(void) const;
	/// Function to remove any point repetitions. Only checks adjacent points.
	void PurgeRepeatedAdjacentPoints(void);
	/// Moves the Route by the vector provided.
	void Move(const C2DVector& Vector);
	/// Rotates the Route by the amount given.
	void RotateToRight(double dAng, const C2DPoint& Origin);
	/// Grows the Route by the factor given from origin provided.
	void Grow(double dFactor, const C2DPoint& Origin);
	/// Reflects the Route throught the point provided.
	void Reflect(const C2DPoint& Point);
	/// Reflects the in the line given.
	void Reflect(const C2DLine& Line);
	/// Returns the distance from the Route to the point.
	double Distance(const C2DPoint& TestPoint) const;
	/// Returns the distance from the Route to the point and the index of the closest line.
	double Distance(const C2DPoint& TestPoint, unsigned int& nClosestLine) const;
	/// Inserts the pointer to the C2DPoint in the line such that the perimiter is minimised.
	void InsertOptimally( C2DPoint* pt);
	/// Gets the bounding rectangle.
	void GetBoundingRect(C2DRect& Rect) const;
	/// True if the first point is the same as the last.
	bool IsClosed(void) const;
	/// Projects this onto the line given.
	void Project(const C2DLine& Line, CInterval& Interval) const;
	/// Projects this onto the vector given.
	void Project(const C2DVector& Vector, CInterval& Interval) const;
	/// Snaps this to the conceptual grid.
	void SnapToGrid(void);

private:
	/// The data, just an array of points.
	C2DPointSet m_Points;

};

#endif