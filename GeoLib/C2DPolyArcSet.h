/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DPolyArcSet.h
\brief Declaration file for the C2DPolyArcSet Class

Declaration file for C2DPolyArcSet, a class which represents a set of arcs. 

\class C2DPolyArcSet.
\brief Class which represents a set of arcs.

Class which represents a set of arcs. Inherits from the Geo set base.
<P>---------------------------------------------------------------------------*/


#ifndef _GEOLIB_C2DPOLYARCSET_H 
#define _GEOLIB_C2DPOLYARCSET_H

#include "C2DPolyArc.h"
#include "C2DBaseSet.h"
#include "MemoryPool.h"



class C2DPolyArc;

#ifdef _POLY_EXPORTING
	#define POLY_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define POLY_DECLSPEC
	#else
		#define POLY_DECLSPEC		__declspec(dllimport)
	#endif
#endif

class  POLY_DECLSPEC C2DPolyArcSet			: public C2DBaseSet			
{
public:
	_MEMORY_POOL_DECLARATION

	/// constructor
	C2DPolyArcSet(void);
	/// destructor
	~C2DPolyArcSet(void);



	/// Adds a copy of the other pointer array
	void AddCopy(const C2DPolyArcSet& Other);
	/// Makes a copy of the other
	void MakeCopy( const C2DPolyArcSet& Other);

	/// Passes ONLY the pointers of this type from the Other into this.
	void operator<<(C2DBaseSet& Other);

	/// Adds a new pointer and takes responsibility for it.
	void Add(C2DPolyArc* NewItem) { C2DBaseSet::Add(NewItem);}

	/// Adds a copy of the item given
	void AddCopy(const C2DPolyArc& NewItem);

	/// Deletes the current item and sets the pointer to be the new one
	void DeleteAndSet(int nIndx, C2DPolyArc* NewItem) { C2DBaseSet::DeleteAndSet(nIndx, NewItem );}
	/// Extracts the current item and sets the pointer to be the new one
	C2DPolyArc* ExtractAndSet(int nIndx, C2DPolyArc* NewItem) { return dynamic_cast<C2DPolyArc*> (C2DBaseSet::ExtractAndSet(nIndx, NewItem ));}

	/// Returns the value at the point given
	C2DPolyArc* GetAt(int nIndx)  { return dynamic_cast<C2DPolyArc*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns the value at the point given
	const C2DPolyArc* GetAt(int nIndx) const  { return dynamic_cast<const C2DPolyArc*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	C2DPolyArc& operator[] (int nIndx)  { return *dynamic_cast<C2DPolyArc*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	const C2DPolyArc& operator[] (int nIndx) const { return *dynamic_cast<const C2DPolyArc*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a pointer to the last item
	C2DPolyArc* GetLast(void) { return dynamic_cast<C2DPolyArc*> (C2DBaseSet::GetLast( ) ) ;}

	/// Extracts the pointer passing deletion responsibility over.
	C2DPolyArc* ExtractAt(unsigned int nIndx) { return dynamic_cast<C2DPolyArc*> (C2DBaseSet::ExtractAt(nIndx ) ) ;}
	/// Extracts the pointer passing deletion responsibility over.
	C2DPolyArc* ExtractLast(void) { return dynamic_cast<C2DPolyArc*> (C2DBaseSet::ExtractLast( ) ) ;}
	/// Insertion
	void InsertAt(unsigned int nIndex, C2DPolyArc* NewItem) {C2DBaseSet::InsertAt(nIndex, NewItem);}
	/// Insertion of another array
	void InsertAt(unsigned int nIndex, C2DPolyArcSet& Other) {C2DBaseSet::InsertAt(nIndex, Other);}

	/// Passes all the pointers from the Other into this
	void operator<<(C2DPolyArcSet& Other) {C2DBaseSet::operator <<(Other);}
	/// Adds a new pointer and takes responsibility for it.
	void operator<<(C2DPolyArc* NewItem) {C2DBaseSet::operator <<(NewItem);};

};

#endif