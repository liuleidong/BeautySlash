/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DLineBaseSet.h
\brief File for the C2DLineBaseSet class.

File for the C2DLineBaseSet class, a collection of lines.

\class C2DLineBaseSet.
\brief C2DLineBaseSet class, a collection of lines.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DLINEBASESET_H 
#define _GEOLIB_C2DLINEBASESET_H


#include "C2DBaseSet.h"
#include "C2DLineBase.h"
#include "MemoryPool.h"


class C2DBaseSet;
class CIndexSet;

#ifdef _EXPORTING
	#define CLASS_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define CLASS_DECLSPEC
	#else
		#define CLASS_DECLSPEC		__declspec(dllimport)
	#endif
#endif

class CLASS_DECLSPEC C2DLineBaseSet :  public C2DBaseSet
{
public:
	_MEMORY_POOL_DECLARATION

	/// Constructor
	C2DLineBaseSet(void);
	/// Destructor
	~C2DLineBaseSet(void);

	/// Adds a copy of the other pointer array
	void AddCopy(const C2DLineBaseSet& Other);
	/// Makes a copy of the other
	void MakeCopy( const C2DLineBaseSet& Other);

	/// Passes ONLY the pointers of this type from the Other into this.
	void operator<<(C2DBaseSet& Other);

	/// Adds a new pointer and takes responsibility for it.
	void Add(C2DLineBase* NewItem) { C2DBaseSet::Add(NewItem);}

	/// Adds a copy of the item given
	void AddCopy(const C2DLineBase& NewItem);

	/// Deletes the current item and sets the pointer to be the new one
	void DeleteAndSet(int nIndx, C2DLineBase* NewItem) { C2DBaseSet::DeleteAndSet(nIndx, NewItem );}
	/// Extracts the current item and sets the pointer to be the new one
	C2DLineBase* ExtractAndSet(int nIndx, C2DLineBase* NewItem) { return dynamic_cast<C2DLineBase*> (C2DBaseSet::ExtractAndSet(nIndx, NewItem ));}

	/// Returns the value at the point given
	C2DLineBase* GetAt(int nIndx)  { return dynamic_cast<C2DLineBase*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns the value at the point given
	const C2DLineBase* GetAt(int nIndx) const  { return dynamic_cast<const C2DLineBase*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	C2DLineBase& operator[] (int nIndx)  { return *dynamic_cast<C2DLineBase*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	const C2DLineBase& operator[] (int nIndx) const { return *dynamic_cast<const C2DLineBase*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a pointer to the last item
	C2DLineBase* GetLast(void) { return dynamic_cast<C2DLineBase*> (C2DBaseSet::GetLast( ) ) ;}

	/// Extracts the pointer passing deletion responsibility over.
	C2DLineBase* ExtractAt(unsigned int nIndx) { return dynamic_cast<C2DLineBase*> (C2DBaseSet::ExtractAt(nIndx ) ) ;}
	/// Extracts the pointer passing deletion responsibility over.
	C2DLineBase* ExtractLast(void) { return dynamic_cast<C2DLineBase*> (C2DBaseSet::ExtractLast( ) ) ;}
	/// Insertion
	void InsertAt(unsigned int nIndex, C2DLineBase* NewItem) {C2DBaseSet::InsertAt(nIndex, NewItem);}
	/// Insertion of another array
	void InsertAt(unsigned int nIndex, C2DLineBaseSet& Other) {C2DBaseSet::InsertAt(nIndex, Other);}

	/// Passes all the pointers from the Other into this
	void operator<<(C2DLineBaseSet& Other) {C2DBaseSet::operator <<(Other);}
	/// Adds a new pointer and takes responsibility for it.
	void operator<<(C2DLineBase* NewItem) {C2DBaseSet::operator <<(NewItem);};

	/// Calls base class
	void GetIntersections(C2DPointSet* pPoints, CIndexSet* pIndexes1 = 0, 
			CIndexSet* pIndexes2 = 0) const;
	/// Calls base class
	void GetIntersections(const C2DLineBaseSet& Other, C2DPointSet* pPoints, 
			CIndexSet* pIndexesThis = 0, CIndexSet* pIndexesOther  = 0,
			const C2DRect* pBoundingRectThis = 0, const C2DRect* pBoundingRectOther = 0) const;
	/// True if there are crossing lines in the set.
	bool HasCrossingLines(void) const;

	/// Checks for closure i.e. it forms a closed shape.
	bool IsClosed(bool bEndsOnly = true) const;

	/// Adds the other to this if there is a common end i.e. they can be joined up.
	bool AddIfCommonEnd( C2DLineBaseSet& Other);
	/// Removes lines that are small, based on the tolerance. 
	void Remove0Lines(double dTolerance);
	/// Reverses the direction.
	void ReverseDirection(void );

};


#endif