/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------<BR>
\file 2DHoledPolyBaseSet.h
\brief Declaration file for the C2DHoledPolyBaseSet Class.

Declaration file for C2DHoledPolyBaseSet, a collection of holed polygons.

\class C2DHoledPolyBaseSet.
\brief A collection of holed polygons.

Class which represents a collection of holed polygons.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DHOLEDPOLYBASESET_H 
#define _GEOLIB_C2DHOLEDPOLYBASESET_H


#include "Grid.h"
#include "C2DHoledPolyBase.h"
#include "C2DBaseSet.h"
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

class  POLY_DECLSPEC C2DHoledPolyBaseSet	: public C2DBaseSet
{
public:
	_MEMORY_POOL_DECLARATION

	/// Constructor
	C2DHoledPolyBaseSet(void);
	/// Destructor
	~C2DHoledPolyBaseSet(void);
	/// Adds a copy of the other pointer array
	void AddCopy(const C2DHoledPolyBaseSet& Other);
	/// Makes a copy of the other
	void MakeCopy( const C2DHoledPolyBaseSet& Other);

	/// Passes ONLY the pointers of this type or inherited types from the Other into this.
	void operator<<(C2DBaseSet& Other);

	/// Adds a new pointer and takes responsibility for it.
	void Add(C2DHoledPolyBase* NewItem) { C2DBaseSet::Add(NewItem);}

	/// Adds a copy of the item given
	void AddCopy(const C2DHoledPolyBase& NewItem) { C2DBaseSet::Add(new C2DHoledPolyBase( NewItem ) );}

	/// Deletes the current item and sets the pointer to be the new one
	void DeleteAndSet(int nIndx, C2DHoledPolyBase* NewItem) { C2DBaseSet::DeleteAndSet(nIndx, NewItem );}
	/// Extracts the current item and sets the pointer to be the new one
	C2DHoledPolyBase* ExtractAndSet(int nIndx, C2DHoledPolyBase* NewItem) { return dynamic_cast<C2DHoledPolyBase*> (C2DBaseSet::ExtractAndSet(nIndx, NewItem ) );}

	/// Returns the value at the point given
	C2DHoledPolyBase* GetAt(int nIndx)  { return dynamic_cast<C2DHoledPolyBase*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns the value at the point given
	const C2DHoledPolyBase* GetAt(int nIndx) const  { return dynamic_cast<const C2DHoledPolyBase*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	C2DHoledPolyBase& operator[] (int nIndx)  { return *dynamic_cast<C2DHoledPolyBase*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	const C2DHoledPolyBase& operator[] (int nIndx) const { return *dynamic_cast<const C2DHoledPolyBase*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a pointer to the last item
	C2DHoledPolyBase* GetLast(void) { return dynamic_cast<C2DHoledPolyBase*> (C2DBaseSet::GetLast( ) ) ;}

	/// Extracts the pointer passing deletion responsibility over.
	C2DHoledPolyBase* ExtractAt(unsigned int nIndx) { return dynamic_cast<C2DHoledPolyBase*> (C2DBaseSet::ExtractAt(nIndx ) ) ;}
	/// Extracts the pointer passing deletion responsibility over.
	C2DHoledPolyBase* ExtractLast(void) { return dynamic_cast<C2DHoledPolyBase*> (C2DBaseSet::ExtractLast( ) ) ;}
	/// Insertion
	void InsertAt(unsigned int nIndex, C2DHoledPolyBase* NewItem) {C2DBaseSet::InsertAt(nIndex, NewItem);}
	/// Insertion of another array
	void InsertAt(unsigned int nIndex, C2DHoledPolyBaseSet& Other) {C2DBaseSet::InsertAt(nIndex, Other);}

	/// Passes all the pointers from the Other into this
	void operator<<(C2DHoledPolyBaseSet& Other) {C2DBaseSet::operator <<(Other);}
	/// Adds a new pointer and takes responsibility for it.
	void operator<<(C2DHoledPolyBase* NewItem) {C2DBaseSet::operator <<(NewItem);};

	/// Basic multiple unification.
	void UnifyBasic(void);
	/// Unification by growing shapes of fairly equal size (fastest for large groups).
	void UnifyProgressive(CGrid::eDegenerateHandling eDegen = CGrid::None);
	/// Assumes current set is distinct.
	void AddAndUnify(C2DHoledPolyBase* pPoly);
	/// Assumes both sets are distinct.
	void AddAndUnify(C2DHoledPolyBaseSet& pOther);
	/// Assumes current set is distinct.
	bool AddIfUnify(C2DHoledPolyBase* pPoly);
	/// Adds polygons known to be holes of those currently in the set.
	void AddKnownHoles( C2DPolyBaseSet& pOther );

	/// Total Line count for all polygons contained.
	unsigned int GetLineCount(void);
	/// Minimum line count of all polys.
	unsigned int GetMinLineCount(void);

	// Passes all the pointers from the Other into this
	void operator<<(C2DPolyBaseSet& Other);
	///// Adds a new pointer and takes responsibility for it.
	void operator<<(C2DPolyBase* NewItem);

	// Makes all arc valid if not already by adjusting radius to minimum required.
	unsigned int MakeValidArcs(void);
	// True if there are arcs in the shape.
	bool HasArcs(void) const;
	// True if all the arcs are valid
	bool IsValidArcs(void) const;

	// Removes all lines whose end is the same is the start. Returns the number found.
	unsigned int RemoveNullLines(void);

	void MakeClockwise(void);


	void Transform(CTransformation* pProject);

	void InverseTransform(CTransformation* pProject);

};

#endif