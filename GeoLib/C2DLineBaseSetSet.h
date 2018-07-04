/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DLineBaseSetSet.h
\brief File for the C2DLineBaseSetSet class.

File for the C2DLineBaseSetSet class, a collection of line sets. 

\class C2DLineBaseSetSet.
\brief C2DLineBaseSetSet class a collection of line sets. Used to store strands
of lines from splitting up a polygon. 
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_C2DLINEBASESETSET_H 
#define _GEOLIB_C2DLINEBASESETSET_H

#include "C2DBaseSet.h"
#include "C2DLineBaseSet.h"
#include "MemoryPool.h"


#ifdef _EXPORTING
	#define CLASS_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define CLASS_DECLSPEC
	#else
		#define CLASS_DECLSPEC		__declspec(dllimport)
	#endif
#endif

class CLASS_DECLSPEC C2DLineBaseSetSet : public C2DBaseSet
{
public:
	_MEMORY_POOL_DECLARATION

	/// Constructor
	C2DLineBaseSetSet(void);

	/// Destructor
	~C2DLineBaseSetSet(void);

	/// Adds a new pointer and takes responsibility for it.
	void Add(C2DLineBaseSet* NewItem) { C2DBaseSet::Add(NewItem);}

	/// Deletes the current item and sets the pointer to be the new one
	void DeleteAndSet(int nIndx, C2DLineBaseSet* NewItem) { C2DBaseSet::DeleteAndSet(nIndx, NewItem );}
	/// Extracts the current item and sets the pointer to be the new one
	C2DLineBaseSet* ExtractAndSet(int nIndx, C2DLineBaseSet* NewItem) { return dynamic_cast<C2DLineBaseSet*> ( C2DBaseSet::ExtractAndSet(nIndx, NewItem ));}

	/// Returns the value at the point given
	C2DLineBaseSet* GetAt(int nIndx)  { return dynamic_cast<C2DLineBaseSet*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns the value at the point given
	const C2DLineBaseSet* GetAt(int nIndx) const  { return dynamic_cast<const C2DLineBaseSet*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	C2DLineBaseSet& operator[] (int nIndx)  { return *dynamic_cast<C2DLineBaseSet*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	const C2DLineBaseSet& operator[] (int nIndx) const { return *dynamic_cast<const C2DLineBaseSet*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a pointer to the last item
	C2DLineBaseSet* GetLast(void) { return dynamic_cast<C2DLineBaseSet*> (C2DBaseSet::GetLast( ) ) ;}

	/// Extracts the pointer passing deletion responsibility over.
	C2DLineBaseSet* ExtractAt(unsigned int nIndx) { return dynamic_cast<C2DLineBaseSet*> (C2DBaseSet::ExtractAt(nIndx ) ) ;}
	/// Extracts the pointer passing deletion responsibility over.
	C2DLineBaseSet* ExtractLast(void) { return dynamic_cast<C2DLineBaseSet*> (C2DBaseSet::ExtractLast( ) ) ;}
	/// Insertion
	void InsertAt(unsigned int nIndex, C2DLineBaseSet* NewItem) {C2DBaseSet::InsertAt(nIndex, NewItem);}
	/// Insertion of another array
	void InsertAt(unsigned int nIndex, C2DLineBaseSetSet& Other) {C2DBaseSet::InsertAt(nIndex, Other);}

	/// Passes all the pointers from the Other into this
	void operator<<(C2DLineBaseSetSet& Other) {C2DBaseSet::operator <<(Other);}
	/// Adds a new pointer and takes responsibility for it.
	void operator<<(C2DLineBaseSet* NewItem) {C2DBaseSet::operator <<(NewItem);};

	/// Merges the joining routes together.
	void MergeJoining(void);
	/// Adds all the routes from the other to this if the join a routes in this.
	void AddJoining( C2DLineBaseSetSet& Other );
	/// Adds the routes in the other set that are closed.
	void AddClosed(C2DLineBaseSetSet& Other , bool bEndsOnly = true);
};


#endif