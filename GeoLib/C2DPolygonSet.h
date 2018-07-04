/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DPolygonSet.h
\brief Declaration file for the C2DPolygonSet Class.

Declaration file for C2DPolygonSet, a class which represents a set of 2D polygons.

\class C2DPolygonSet.
\brief A class which represents a set of 2D polygons.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DPOLYGONSET_H 
#define _GEOLIB_C2DPOLYGONSET_H

#include "C2DPolygon.h"
#include "C2DBaseSet.h"
#include "MemoryPool.h"


class C2DBaseSet;

#ifdef _POLY_EXPORTING
	#define POLY_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define POLY_DECLSPEC
	#else
		#define POLY_DECLSPEC		__declspec(dllimport)
	#endif
#endif

class POLY_DECLSPEC C2DPolygonSet			: public C2DBaseSet			
{
public:
	_MEMORY_POOL_DECLARATION
	/// constructor
	C2DPolygonSet(void);
	/// destructor
	~C2DPolygonSet(void);

	/// Adds a copy of the other pointer array
	void AddCopy(const C2DPolygonSet& Other);
	/// Makes a copy of the other
	void MakeCopy( const C2DPolygonSet& Other);

	/// Passes ONLY the pointers of this type from the Other into this.
	void operator<<(C2DBaseSet& Other);

	/// Adds a new pointer and takes responsibility for it.
	void Add(C2DPolygon* NewItem) { C2DBaseSet::Add(NewItem);}

	/// Adds a copy of the item given
	void AddCopy(const C2DPolygon& NewItem);

	/// Deletes the current item and sets the pointer to be the new one
	void DeleteAndSet(int nIndx, C2DPolygon* NewItem) { C2DBaseSet::DeleteAndSet(nIndx, NewItem );}
	/// Extracts the current item and sets the pointer to be the new one
	C2DPolygon* ExtractAndSet(int nIndx, C2DPolygon* NewItem) { return dynamic_cast<C2DPolygon*> (C2DBaseSet::ExtractAndSet(nIndx, NewItem ));}

	/// Returns the value at the point given
	C2DPolygon* GetAt(int nIndx)  { return dynamic_cast<C2DPolygon*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns the value at the point given
	const C2DPolygon* GetAt(int nIndx) const  { return dynamic_cast<const C2DPolygon*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	C2DPolygon& operator[] (int nIndx)  { return *dynamic_cast<C2DPolygon*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	const C2DPolygon& operator[] (int nIndx) const { return *dynamic_cast<const C2DPolygon*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a pointer to the last item
	C2DPolygon* GetLast(void) { return dynamic_cast<C2DPolygon*> (C2DBaseSet::GetLast( ) ) ;}

	/// Extracts the pointer passing deletion responsibility over.
	C2DPolygon* ExtractAt(unsigned int nIndx) { return dynamic_cast<C2DPolygon*> (C2DBaseSet::ExtractAt(nIndx ) ) ;}
	/// Extracts the pointer passing deletion responsibility over.
	C2DPolygon* ExtractLast(void) { return dynamic_cast<C2DPolygon*> (C2DBaseSet::ExtractLast( ) ) ;}
	/// Insertion
	void InsertAt(unsigned int nIndex, C2DPolygon* NewItem) {C2DBaseSet::InsertAt(nIndex, NewItem);}
	/// Insertion of another array
	void InsertAt(unsigned int nIndex, C2DPolygonSet& Other) {C2DBaseSet::InsertAt(nIndex, Other);}

	/// Passes all the pointers from the Other into this
	void operator<<(C2DPolygonSet& Other) {C2DBaseSet::operator <<(Other);}
	/// Adds a new pointer and takes responsibility for it.
	void operator<<(C2DPolygon* NewItem) {C2DBaseSet::operator <<(NewItem);};

};

#endif