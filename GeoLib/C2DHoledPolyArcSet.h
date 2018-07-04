/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DHoledPolyArcSet.h
\brief Declaration file for the C2DHoledPolyArcSet Class.

Declaration file for C2DHoledPolyArcSet, a set of curved polygons.

\class C2DHoledPolyArcSet.
\brief A set of curved polygons.

Class which represents a set of curved polygons.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DHOLEDPOLYARCSET_H 
#define _GEOLIB_C2DHOLEDPOLYARCSET_H


#include "C2DBaseSet.h"
#include "C2DHoledPolyArc.h"
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


class POLY_DECLSPEC C2DHoledPolyArcSet	: public C2DBaseSet
{
public:
	_MEMORY_POOL_DECLARATION

	/// Constrictor
	C2DHoledPolyArcSet(void);
	/// Destrictor
	~C2DHoledPolyArcSet(void);
	/// Adds a copy of the other pointer array
	void AddCopy(const C2DHoledPolyArcSet& Other);
	/// Makes a copy of the other
	void MakeCopy( const C2DHoledPolyArcSet& Other);

	/// Passes ONLY the pointers of this type from the Other into this.
	void operator<<(C2DBaseSet& Other);

	/// Adds a new pointer and takes responsibility for it.
	void Add(C2DHoledPolyArc* NewItem) { C2DBaseSet::Add(NewItem);}

	/// Adds a copy of the item given
	void AddCopy(const C2DHoledPolyArc& NewItem) { C2DBaseSet::Add(new C2DHoledPolyArc( NewItem ) );}


	/// Deletes the current item and sets the pointer to be the new one
	void DeleteAndSet(int nIndx, C2DHoledPolyArc* NewItem) { C2DBaseSet::DeleteAndSet(nIndx, NewItem );}
	/// Extracts the current item and sets the pointer to be the new one
	C2DHoledPolyArc* ExtractAndSet(int nIndx, C2DHoledPolyArc* NewItem) { return dynamic_cast<C2DHoledPolyArc*> (C2DBaseSet::ExtractAndSet(nIndx, NewItem ));}

	/// Returns the value at the point given
	C2DHoledPolyArc* GetAt(int nIndx)  { return dynamic_cast<C2DHoledPolyArc*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns the value at the point given
	const C2DHoledPolyArc* GetAt(int nIndx) const  { return dynamic_cast<const C2DHoledPolyArc*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	C2DHoledPolyArc& operator[] (int nIndx)  { return *dynamic_cast<C2DHoledPolyArc*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	const C2DHoledPolyArc& operator[] (int nIndx) const { return *dynamic_cast<const C2DHoledPolyArc*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a pointer to the last item
	C2DHoledPolyArc* GetLast(void) { return dynamic_cast<C2DHoledPolyArc*> (C2DBaseSet::GetLast( ) ) ;}

	/// Extracts the pointer passing deletion responsibility over.
	C2DHoledPolyArc* ExtractAt(unsigned int nIndx) { return dynamic_cast<C2DHoledPolyArc*> (C2DBaseSet::ExtractAt(nIndx ) ) ;}
	/// Extracts the pointer passing deletion responsibility over.
	C2DHoledPolyArc* ExtractLast(void) { return dynamic_cast<C2DHoledPolyArc*> (C2DBaseSet::ExtractLast( ) ) ;}
	/// Insertion
	void InsertAt(unsigned int nIndex, C2DHoledPolyArc* NewItem) {C2DBaseSet::InsertAt(nIndex, NewItem);}
	/// Insertion of another array
	void InsertAt(unsigned int nIndex, C2DHoledPolyArcSet& Other) {C2DBaseSet::InsertAt(nIndex, Other);}

	/// Passes all the pointers from the Other into this
	void operator<<(C2DHoledPolyArcSet& Other) {C2DBaseSet::operator <<(Other);}
	/// Adds a new pointer and takes responsibility for it.
	void operator<<(C2DHoledPolyArc* NewItem) {C2DBaseSet::operator <<(NewItem);};

	/// Basic multiple unification.
	void UnifyBasic(void);
	/// Unification by growing shapes of fairly equal size (fastest for large groups).
	void UnifyProgressive(CGrid::eDegenerateHandling eDegen = CGrid::None);

	// Makes all arc valid if not already by adjusting radius to minimum required.
	unsigned int MakeValidArcs(void);
	// True if there are arcs in the shape.
	bool HasArcs(void) const;
	// True if all the arcs are valid
	bool IsValidArcs(void) const;

	unsigned int RemoveNullLines(void);
};

#endif