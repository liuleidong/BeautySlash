/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DHoledPolygonSet.h
\brief Declaration file for the C2DHoledPolygonSet Class.

Declaration file for C2DHoledPolygonSet, a collection of holed polygons.

\class C2DHoledPolygonSet.
\brief A collection of holed polygons.

Class which represents a collection of holed polygons.
<P>---------------------------------------------------------------------------*/


#ifndef _GEOLIB_C2DHOLEDPOLYGONSET_H 
#define _GEOLIB_C2DHOLEDPOLYGONSET_H


#include "C2DBaseSet.h"
#include "C2DHoledPolygon.h"
#include "MemoryPool.h"


class C2DPolygon;

#ifdef _POLY_EXPORTING
	#define POLY_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define POLY_DECLSPEC
	#else
		#define POLY_DECLSPEC		__declspec(dllimport)
	#endif
#endif

class  POLY_DECLSPEC C2DHoledPolygonSet		: public C2DBaseSet	
{
public:
	_MEMORY_POOL_DECLARATION

	/// Constructor
	C2DHoledPolygonSet(void);
	/// Destructor
	~C2DHoledPolygonSet(void);
	/// Adds a copy of the other pointer array
	void AddCopy(const C2DHoledPolygonSet& Other);
	/// Makes a copy of the other
	void MakeCopy( const C2DHoledPolygonSet& Other);

	/// Passes ONLY the pointers of this type from the Other into this.
	void operator<<(C2DBaseSet& Other);

	/// Adds a new pointer and takes responsibility for it.
	void Add(C2DHoledPolygon* NewItem) { C2DBaseSet::Add(NewItem);}

	/// Adds a copy of the item given
	void AddCopy(const C2DHoledPolygon& NewItem) { C2DBaseSet::Add(new C2DHoledPolygon( NewItem ) );}

	/// Deletes the current item and sets the pointer to be the new one
	void DeleteAndSet(int nIndx, C2DHoledPolygon* NewItem) { C2DBaseSet::DeleteAndSet(nIndx, NewItem );}
	/// Extracts the current item and sets the pointer to be the new one
	C2DHoledPolygon* ExtractAndSet(int nIndx, C2DHoledPolygon* NewItem) { return dynamic_cast<C2DHoledPolygon*> (C2DBaseSet::ExtractAndSet(nIndx, NewItem ));}

	/// Returns the value at the point given
	C2DHoledPolygon* GetAt(int nIndx)  { return dynamic_cast<C2DHoledPolygon*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns the value at the point given
	const C2DHoledPolygon* GetAt(int nIndx) const  { return dynamic_cast<const C2DHoledPolygon*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	C2DHoledPolygon& operator[] (int nIndx)  { return *dynamic_cast<C2DHoledPolygon*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	const C2DHoledPolygon& operator[] (int nIndx) const { return *dynamic_cast<const C2DHoledPolygon*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a pointer to the last item
	C2DHoledPolygon* GetLast(void) { return dynamic_cast<C2DHoledPolygon*> (C2DBaseSet::GetLast( ) ) ;}

	/// Extracts the pointer passing deletion responsibility over.
	C2DHoledPolygon* ExtractAt(unsigned int nIndx) { return dynamic_cast<C2DHoledPolygon*> (C2DBaseSet::ExtractAt(nIndx ) ) ;}
	/// Extracts the pointer passing deletion responsibility over.
	C2DHoledPolygon* ExtractLast(void) { return dynamic_cast<C2DHoledPolygon*> (C2DBaseSet::ExtractLast( ) ) ;}
	/// Insertion
	void InsertAt(unsigned int nIndex, C2DHoledPolygon* NewItem) {C2DBaseSet::InsertAt(nIndex, NewItem);}
	/// Insertion of another array
	void InsertAt(unsigned int nIndex, C2DHoledPolygonSet& Other) {C2DBaseSet::InsertAt(nIndex, Other);}

	/// Passes all the pointers from the Other into this
	void operator<<(C2DHoledPolygonSet& Other) {C2DBaseSet::operator <<(Other);}
	/// Adds a new pointer and takes responsibility for it.
	void operator<<(C2DHoledPolygon* NewItem) {C2DBaseSet::operator <<(NewItem);};

	/// Basic multiple unification.
	void UnifyBasic(void);
	/// Unification by growing shapes of fairly equal size (fastest for large groups).
	void UnifyProgressive(CGrid::eDegenerateHandling eDegen = CGrid::None);
	/// Add a new item.
	void operator<<(C2DPolygon* NewItem);
};


#endif