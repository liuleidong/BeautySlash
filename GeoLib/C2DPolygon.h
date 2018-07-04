/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DPolygon.h
\brief Declaration file for the C2DPolygon Class

Declaration file for C2DPolygon, a class which represents a polygon. Can be 
convex or concave but doesn't contain holes.

\class C2DPolygon.
\brief Class which represents a polygon.

Class which represents a polygon which is defined as a sequence of straight lines
that are joined and closed, with no holes and no self intersections. 

User adds points and then calls the create function. Can opt to add points without 
any order and in which case the points are reordered so that the perimeter of the 
shape is minimised and there are no crossing lines. 

Supports a variety of functions including functions to find the intersection, union 
and non-intersection of two polygons.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DPOLYGON_H 
#define _GEOLIB_C2DPOLYGON_H


#include "C2DPolyBase.h"
#include "Constants.h"
#include "MemoryPool.h"


class C2DPolygonSet;
class C2DHoledPolygonSet;
class C2DRoute;
class C2DCircle;


#define MAX_SUB_AREAS 2

#ifdef _POLY_EXPORTING
	#define POLY_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define POLY_DECLSPEC
	#else
		#define POLY_DECLSPEC		__declspec(dllimport)
	#endif
#endif


class POLY_DECLSPEC C2DPolygon : public C2DPolyBase
{
public:
	_MEMORY_POOL_DECLARATION
	/// Constructor.
	C2DPolygon(void);
	/// Constructor.
	C2DPolygon(const C2DPoint* pPoint, unsigned int nNumber, bool bReorderIfNeeded = false);
	/// Constructor.
	C2DPolygon(const C2DPointSet& Points, bool bReorderIfNeeded = false);
	/// Constructor.
	C2DPolygon(const C2DPolygon& Other);
	/// Constructor.
	C2DPolygon(const C2DPolyBase& Other);
	/// Destructor.
	~C2DPolygon(void);

	/// Sets the point list to that of the route provided. Route can be closed or not. 
	bool Create(const C2DRoute& Route);
	/// Creates the polygon with optional reordering of points.
	bool Create(const C2DPoint* pPoint, unsigned int nNumber, bool bReorderIfNeeded = false);

	bool Create(const double* pPoint, unsigned int nNumber);

	/// Creates the polygon with optional reordering of points.
	bool Create(const C2DPointSet& Points, bool bReorderIfNeeded = false);
	/// Creates a regular polygon.
	bool CreateRegular(const C2DPoint& Centre, double dDistanceToPoints, int nNumberSides);
	/// Creates a convex hull from another polygon. Uses Graham's algorithm.
	bool CreateConvexHull(const C2DPolygon& Other);
	/// Creates a randon polygon.
	bool CreateRandom(const C2DRect& cBoundary, int nMinPoints, int nMaxPoints);

	/// Mophs this polygon into another by the factor given.
	bool CreateMorph(const C2DPolygon& OtherFrom, const C2DPolygon& OtherTo, double dFactor);
	/// Creates convex sub areas of the current polygon. These can then be extracted. 
	/// This function is also useful when obtaining minimum translation vectors. 
	///\sa Overlaps function.
	bool CreateConvexSubAreas(void);
	/// Removes the convex sub areas.
	void ClearConvexSubAreas(void);

	/// True if the point is contained.
	bool Contains(const C2DPoint& pt) const;
	/// True if the polygon is contained.
	bool Contains(const C2DPolygon& Other) const;

	bool Contains(const C2DHoledPolyBase& Other) const {return C2DPolyBase::Contains(Other);}
	/// True if the Line is contained.
	bool Contains(const C2DLine& Line) const;
	/// True if the Line is contained.
	bool Contains(const C2DLineBase& Line) const;

	/// True if the lines of this cross the line.
	bool Crosses(const C2DLineBase& Line) const;
	/// True if the lines of this cross the line. Returns the points in order of their position
	/// on the line.
	bool Crosses(const C2DLineBase& Line, C2DPointSet* IntersectionPts) const;
    /// True if it crossed the line.Provides the intersection points and crossed lines
    bool Crosses(const C2DLineBase& Line,C2DPointSet* IntersectionPts,C2DLineBaseSet *IntersectionLines) const;
	/// True if the lines of this cross the ray. Returns the points in order of their position
	/// on the ray. (assumes the line is infinite).
	bool CrossesRay(const C2DLine& Ray, C2DPointSet* IntersectionPts) const;
	/// True if the polygon is convex.
	bool IsConvex(void)  const;
	/// Clears all.
	void Clear(void);

	/// Rotates the polygon to the right around the origin given.
	void RotateToRight(double dAng, const C2DPoint& Origin);
	/// Rotates the polygon to the right around the centroid.
	void RotateToRight(double dAng);
	/// Moves the polygon.
	void Move(const C2DVector& Vector);
	/// Grows the polygon from the centre.
	void Grow(double dFactor);
	/// Grows from the origin given.
	void Grow(double dFactor, const C2DPoint& Origin);
	/// Reflects throught the point provided.
	void Reflect(const C2DPoint& Point);
	/// Reflects throught the line provided.
	void Reflect(const C2DLine& Line);

	/// Assignment
	const C2DPolygon& operator=(const C2DPolygon& Other);
	/// True if there are repeated points.
	bool HasRepeatedPoints(void) const;

	/// Projects the polygon onto a vector providing the interval of projection on the vector.
	void Project(const C2DVector& Vector, CInterval& Interval) const;
	/// Projects the polygon onto a line providing the interval of projection on the vector.
	void Project(const C2DLine& Line, CInterval& Interval) const;
	/// Returns a pointer to the sub area.
	const C2DPolygon* GetSubArea(int nIndex) const;
	/// Returns the convex sub areas if created.
	void GetConvexSubAreas(C2DPolygonSet& SubAreas) const;
	/// True if this overlaps another and returns the translation vector required to move
	/// this apart. Exact if this is convex, approximate if concave. Better approximation
	/// if convex sub areas have been created.
	bool Overlaps(const C2DPolygon& Other, C2DVector& MinimumTranslationVector)const ;
	/// True if this overlap the other.
	bool Overlaps( const C2DPolygon& Other)  const  ;
	/// Calls the Overlaps function and moves this away from the other.
	void Avoid(const C2DPolygon& Other);
	/// Returns the number of points.
	unsigned int GetPointsCount(void) const { return m_Lines.size();}
	/// Returns the centroid.
	const C2DPoint GetCentroid(void) const;

	/// Returns the area signed (-ve if clockwise).
	virtual double GetAreaSigned(void) const;
	/// Returns a pointer to the point required;
	const C2DPoint* GetPoint(unsigned int nPointIndex) const;
	/// Copies the points into the set object provided.
	void GetPointsCopy(C2DPointSet& PointCopy) const;
	/// Returns a constant pointer to the line.
	const C2DLine* GetLine(unsigned int i) const;

	/// Returns the left most point.
	int GetLeftMostPoint(void) const;
	/// Smooths the polygon. 
	void Smooth(double dMinAngle = conPI * 0.8, double dCropFactor = 0.8);
	/// Get the minimum bounding circle
	void GetBoundingCircle(C2DCircle& Circle) const;

	/// Returns the difference between this and the other.
	void GetNonOverlaps(const C2DPolygon& Other, C2DHoledPolygonSet& HoledPolygons, 
										CGrid::eDegenerateHandling eDegen = CGrid::None) const ;
	/// Returns the difference between this and the other. Access to base class.
	void GetNonOverlaps(const C2DPolygon& Other, C2DHoledPolyBaseSet& HoledPolygons, 
										CGrid::eDegenerateHandling eDegen = CGrid::None) const ;

	/// Returns the union of tihis and the other.
	void GetUnion(const C2DPolygon& Other, C2DHoledPolygonSet& HoledPolygons,
										CGrid::eDegenerateHandling eDegen = CGrid::None) const ;
	/// Returns the union of tihis and the other. Access to base class.
	void GetUnion(const C2DPolygon& Other, C2DHoledPolyBaseSet& HoledPolygons,
										CGrid::eDegenerateHandling eDegen = CGrid::None) const ;

	/// Returns the overlap between this and the other.
	void GetOverlaps(const C2DPolygon& Other, C2DHoledPolygonSet& Polygons,
										CGrid::eDegenerateHandling eDegen = CGrid::None) const ;

	/// Returns the overlap between this and the other. Access to base class.
	void GetOverlaps(const C2DPolygon& Other, C2DHoledPolyBaseSet& Polygons,
										CGrid::eDegenerateHandling eDegen = CGrid::None) const ;

	/// True if this polygon is above the other. 
	bool OverlapsAbove( const C2DPolygon& Other, double& dVerticalDistance,
										C2DPoint& ptOnThis, C2DPoint& ptOnOther) const;

	/// True if this polygon is above the other. 
	bool OverlapsVertically( const C2DPolygon& Other, double& dVerticalDistance,
										C2DPoint& ptOnThis, C2DPoint& ptOnOther) const;
	/// Returns the minimum bounding box that is not necassarily horiztonal.
	void GetMinBoundingBox( C2DLine& Line, double& dWidthToRight) const;
	
	// Simple buffer around the polygon at a fixed amount.
	void SimpleBuffer(double dBuffer);
	/// Function to remove small areas of the polygon. E.g. when 3 point are or almost
	/// are collinear.
	bool RemoveNullAreas(double dTolerance);

	/// Returns the vector which results in the smallest projection of all the polygons in the
	/// array and the interval representing that translation.
	static void GetMinProjection(C2DPolygonSet& Polygons, C2DVector& Result, CInterval& Interval);


private:
	/// Finds the first inflection.
	bool FindFirstInflection(unsigned int& nFirstInflection)  const;
	/// True if the point is inflected.
	bool IsPointInflected(unsigned int nIndex)  const;
	/// True if the points can be joined with no resulting crossing lines.
	bool CanPointsBeJoined(unsigned int nStart, unsigned int nEnd);
	/// True if joining the 2 point removes any inflection.
	bool RemovesInflection(unsigned int nStart, unsigned int nEnd);
	/// Creates to sub areas given 2 point indexes and pointers to the new areas.
	bool CreateSubAreas(unsigned int nPt1, unsigned int nPt2, C2DPolygon*& pNewArea1, C2DPolygon*& pNewArea2);
	/// Reorders the points.
	bool Reorder(void);
	/// Reorders to eliminate crossing lines.
	bool EliminateCrossingLines(void);
	/// Swaps the two points.
	void SwapPositions(unsigned int Pos1, unsigned int Pos2);
	/// Sets the point to the point provided.
	void SetPoint(const C2DPoint& Point, unsigned int nPointIndex);
	/// Inserts a point to the set.
	void InsertPoint( unsigned int nPointIndex, const C2DPoint& Point);
	/// Removes a point from the set.
	void RemovePoint(unsigned int nPointIndex);
	/// Make the lines related to the points for the base class to work on.
	void MakeLines(const C2DPointSet& Points);

	/// The subareas which are used to split into convex sub areas.
	C2DPolygon* m_SubArea[MAX_SUB_AREAS];


};


#endif
