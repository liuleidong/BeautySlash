/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DPolyBaseSet.h
\brief Collection of base polygons.

\class C2DPolyBaseSet
\brief A collection of base polygons.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DPOLYBASESET_H 
#define _GEOLIB_C2DPOLYBASESET_H

#include "C2DPolyBaseSet.h"
#include "C2DPolyBase.h"
#include "MemoryPool.h"


#ifdef _POLY_EXPORTING
	#define POLY_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define POLY_DECLSPEC
	#else
		#define POLY_DECLSPEC		__declspec(dllimport)
	#endif
#endif

class POLY_DECLSPEC C2DPolyBaseSet			: public C2DBaseSet	
{
public:
	_MEMORY_POOL_DECLARATION

	/// constructor
	C2DPolyBaseSet(void);
	/// destructor
	~C2DPolyBaseSet(void);


	/// Adds a copy of the other pointer array
	void AddCopy(const C2DPolyBaseSet& Other);
	/// Makes a copy of the other
	void MakeCopy( const C2DPolyBaseSet& Other);
	/// Passes ONLY the pointers of this type or inherited types from the Other into this.
	void operator<<(C2DBaseSet& Other);
	/// Adds a new pointer and takes responsibility for it.
	void Add(C2DPolyBase* NewItem) { C2DBaseSet::Add(NewItem);}
	/// Adds a copy of the item given
	void AddCopy(const C2DPolyBase& NewItem);

	/// Deletes the current item and sets the pointer to be the new one
	void DeleteAndSet(int nIndx, C2DPolyBase* NewItem) { C2DBaseSet::DeleteAndSet(nIndx, NewItem );}
	/// Extracts the current item and sets the pointer to be the new one
	C2DPolyBase* ExtractAndSet(int nIndx, C2DPolyBase* NewItem) { return dynamic_cast<C2DPolyBase*> (C2DBaseSet::ExtractAndSet(nIndx, NewItem ));}

	/// Returns the value at the point given
	C2DPolyBase* GetAt(int nIndx)  { return dynamic_cast<C2DPolyBase*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns the value at the point given
	const C2DPolyBase* GetAt(int nIndx) const  { return dynamic_cast<const C2DPolyBase*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	C2DPolyBase& operator[] (int nIndx)  { return *dynamic_cast<C2DPolyBase*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	const C2DPolyBase& operator[] (int nIndx) const { return *dynamic_cast<const C2DPolyBase*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a pointer to the last item
	C2DPolyBase* GetLast(void) { return dynamic_cast<C2DPolyBase*> (C2DBaseSet::GetLast( ) ) ;}

	/// Extracts the pointer passing deletion responsibility over.
	C2DPolyBase* ExtractAt(unsigned int nIndx) { return dynamic_cast<C2DPolyBase*> (C2DBaseSet::ExtractAt(nIndx ) ) ;}
	/// Extracts the pointer passing deletion responsibility over.
	C2DPolyBase* ExtractLast(void) { return dynamic_cast<C2DPolyBase*> (C2DBaseSet::ExtractLast( ) ) ;}
	/// Insertion
	void InsertAt(unsigned int nIndex, C2DPolyBase* NewItem) {C2DBaseSet::InsertAt(nIndex, NewItem);}
	/// Insertion of another array
	void InsertAt(unsigned int nIndex, C2DPolyBaseSet& Other) {C2DBaseSet::InsertAt(nIndex, Other);}

	/// Passes all the pointers from the Other into this
	void operator<<(C2DPolyBaseSet& Other) {C2DBaseSet::operator <<(Other);}
	/// Adds a new pointer and takes responsibility for it.
	void operator<<(C2DPolyBase* NewItem) {C2DBaseSet::operator <<(NewItem);};

};


#endif