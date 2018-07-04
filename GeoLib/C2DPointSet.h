/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DPointSet.h
\brief Declaration file for the C2DPointSet Class

Declaration file for C2DPointSet, a class which represents a set of points. 

\class C2DPointSet.
\brief Class which represents a set of points.

Class which represents a set of points. Inherits from the Geo set base and adds
sorting functions and a convex hull algorithm.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DPOINTSET_H 
#define _GEOLIB_C2DPOINTSET_H

#include "C2DBaseSet.h"
#include "C2DPoint.h"
#include "MemoryPool.h"



class C2DBaseSet;
class C2DCircle;
struct sPointIndex;
class PointIndexSet;


#ifdef _EXPORTING
	#define CLASS_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define CLASS_DECLSPEC
	#else
		#define CLASS_DECLSPEC		__declspec(dllimport)
	#endif
#endif

class CLASS_DECLSPEC C2DPointSet :  public C2DBaseSet
{
public:
	_MEMORY_POOL_DECLARATION

	/// Constructor.
	C2DPointSet(void);
	/// Destructor.
	~C2DPointSet(void);
	/// Adds a new point.
	void AddCopy(double x, double y);
	/// Adds a new copy of the pt set given point.
	void AddCopy(const C2DPointSet& Other);
	/// Makes a copy of the set given.
	void MakeCopy(const C2DPointSet& Other);
	/// Passes ONLY the pointers of this type from the Other into this.
	void operator<<(C2DBaseSet& Other);

	/// Adds a new pointer and takes responsibility for it.
	void Add(C2DPoint* NewItem) { C2DBaseSet::Add(NewItem);}

	/// Adds a copy of the item given
	void AddCopy(const C2DPoint& NewItem) { C2DBaseSet::Add(new C2DPoint( NewItem ) );}

	/// Deletes the current item and sets the pointer to be the new one
	void DeleteAndSet(int nIndx, C2DPoint* NewItem) { C2DBaseSet::DeleteAndSet(nIndx, NewItem );}
	/// Extracts the current item and sets the pointer to be the new one
	C2DPoint* ExtractAndSet(int nIndx, C2DPoint* NewItem) { return dynamic_cast<C2DPoint*> (C2DBaseSet::ExtractAndSet(nIndx, NewItem ));}

	/// Returns the value at the point given
	C2DPoint* GetAt(int nIndx)  { return dynamic_cast<C2DPoint*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns the value at the point given
	const C2DPoint* GetAt(int nIndx) const  { return dynamic_cast<const C2DPoint*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	C2DPoint& operator[] (int nIndx)  { return *dynamic_cast<C2DPoint*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	const C2DPoint& operator[] (int nIndx) const { return *dynamic_cast<const C2DPoint*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a pointer to the last item
	C2DPoint* GetLast(void) { return dynamic_cast<C2DPoint*> (C2DBaseSet::GetLast( ) ) ;}

	/// Extracts the pointer passing deletion responsibility over.
	C2DPoint* ExtractAt(unsigned int nIndx) { return dynamic_cast<C2DPoint*> (C2DBaseSet::ExtractAt(nIndx ) ) ;}
	/// Extracts the pointer passing deletion responsibility over.
	C2DPoint* ExtractLast(void) { return dynamic_cast<C2DPoint*> (C2DBaseSet::ExtractLast( ) ) ;}
	/// Insertion
	void InsertAt(unsigned int nIndex, C2DPoint* NewItem) {C2DBaseSet::InsertAt(nIndex, NewItem);}
	/// Insertion of another array
	void InsertAt(unsigned int nIndex, C2DPointSet& Other) {C2DBaseSet::InsertAt(nIndex, Other);}

	/// Passes all the pointers from the Other into this
	void operator<<(C2DPointSet& Other) {C2DBaseSet::operator <<(Other);}
	/// Adds a new pointer and takes responsibility for it.
	void operator<<(C2DPoint* NewItem) {C2DBaseSet::operator <<(NewItem);};

	/// Removes the convex hull from the point set given.
	void ExtractConvexHull( C2DPointSet& Other);
	/// Sorts by the angle from north relative to the origin given.
	void SortByAngleFromNorth( const C2DPoint& Origin);
	/// Sorts by the angle to the right of the line.
	void SortByAngleToRight( const C2DLine& Line);
	/// Sorts by the angle to the left of the line.
	void SortByAngleToLeft( const C2DLine& Line);

	/// Returns the bounding rect.
	void GetBoundingRect(C2DRect& Rect) const;
	/// Gets the minimum bounding circle.
	void GetBoundingCircle(C2DCircle& Circle) const;
	/// Returns an estimate of the furthest points, usually correct.
	void GetExtremePointsEst(unsigned int& nIndx1, unsigned int& nIndx2, 
		double& dDist, unsigned int nStartEst = 0) const;
	/// Returns the furthest points, accurate but slightly slower than the estimate.
	void GetExtremePoints(unsigned int& nIndx1, unsigned int& nIndx2, 
		double& dDist) const;
	/// Removes all repeated points.
	void RemoveRepeatedPoints(void);
	/// Gets the closts pair of points in the set.
	double GetClosestPair(unsigned int& nIndex1, unsigned int& nIndex2) const;
	/// Sorts from left to right.
	void SortLeftToRight(void);

private:
	/// Returns the furthest point from the one given.
	unsigned int GetFurthestPoint(unsigned int nIndex, double& dDist) const;
	/// Gets the closts pair of points in the set.
	static double GetClosestPair(PointIndexSet& Pts, unsigned int& nIndex1, unsigned int& nIndex2);

};


#endif