/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DBaseSet.h
\brief Declaration file for the C2DBaseSet Class.

Declaration file for C2DBaseSet, a set of geometric entities of different types.

\class C2DBaseSet.
\brief A set of geometric entities of different types held as pointers to a base
class geometric entity.
<P>---------------------------------------------------------------------------*/



#ifndef _GEOLIB_C2DBASESET_H 
#define _GEOLIB_C2DBASESET_H


#include "C2DBase.h"
#include "MemoryPool.h"


class C2DBase;
class C2DVector;
class C2DRect;
class C2DPoint;
class C2DLine;

#ifdef _EXPORTING
	#define CLASS_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define CLASS_DECLSPEC
	#else
		#define CLASS_DECLSPEC		__declspec(dllimport)
	#endif
#endif

class CLASS_DECLSPEC  C2DBaseSet : public C2DBase
{
public:
	_MEMORY_POOL_DECLARATION
	/// Constructor
	C2DBaseSet();
	/// Constructor
	~C2DBaseSet();
	/// Deletes all the pointers and removes them.
	void DeleteAll(void);
	/// Removes all the pointers DOES NOT DELETE.
	void RemoveAll(void);
	/// Adds a new pointer and takes responsibility for it.
	void Add(C2DBase* NewItem);
	/// Deletes the current item and sets the pointer to be the new one
	void DeleteAndSet(int nIndx, C2DBase* NewItem);
	/// Extracts the current item and sets the pointer to be the new one
	C2DBase* ExtractAndSet(int nIndx, C2DBase* NewItem);

	/// returns the size
	unsigned int size(void) const;
	/// Returns the value at the point given
	C2DBase* GetAt(int nIndx);
	/// Returns the value at the point given
	const C2DBase* GetAt(int nIndx) const;
	/// Returns a reference to the value at the point given.
	C2DBase& operator[] (int nIndx);
	/// Returns a reference to the value at the point given.
	const C2DBase& operator[] (int nIndx) const;
	/// Returns a pointer to the last item
	C2DBase* GetLast(void);
	/// Deletion
	void DeleteAt(unsigned int nIndx);
	/// Deletion of the last
	void DeleteLast(void);
	/// Extracts the pointer passing deletion responsibility over.
	C2DBase* ExtractAt(unsigned int nIndx);
	/// Extracts the pointer passing deletion responsibility over.
	C2DBase* ExtractLast(void);
	/// Insertion
	void InsertAt(unsigned int nIndx, C2DBase* NewItem);
	/// Insertion of another array
	void InsertAt(unsigned int nIndx, C2DBaseSet& Other);
	/// Reverses the order
	void ReverseOrder(void);
	/// Passes all the pointers from the Other into this
	void operator<<(C2DBaseSet& Other);
	/// Adds a new pointer and takes responsibility for it.
	void operator<<(C2DBase* NewItem);

	/// Moves the collection by the vector provided
	void Move(const C2DVector& Vector);
	/// Rotates the collection by the angles, around the origin
	void RotateToRight(double dAng, const C2DPoint& Origin);
	/// Grows the collection by the factor, around the origin
	void Grow(double dFactor, const C2DPoint& Origin);
	/// Reflects the collection in the point given
	void Reflect(const C2DPoint& Point);
	/// Reflects throught the line provided.
	void Reflect(const C2DLine& Line);
	/// Calculcates the minimum distance, of the whole collection from the point
	double Distance(const C2DPoint& Point) const;	
	/// Sorts the collection by the distance from the point
	void SortByDistance(const C2DPoint& pt, bool bAscending = true);
	/// Returns the bounding rectangle for the whole collection
	void GetBoundingRect(C2DRect& Rect) const;
	/// Snaps all in the collection to the grid. see CGrid.
	void SnapToGrid(void);
	/// Projects the whole set onto the line given.
	void Project(const C2DLine& Line, CInterval& Interval) const;
	/// Projects the whole set onto the vector given.
	void Project(const C2DVector& Vector, CInterval& Interval) const;

protected:

	/// This is a pointer to the data which is defined is the constructor.
	void* m_Data;
};

#endif