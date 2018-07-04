/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DRectSet.h
\brief File for the C2DLineSet class.

File for the C2DRectSet class, a collection of rects. 

\class C2DRectSet.
\brief C2DRectSet class a collection of rects.
<P>---------------------------------------------------------------------------*/


#ifndef _GEOLIB_C2DRECTSET_H 
#define _GEOLIB_C2DRECTSET_H


#include "C2DRect.h"
#include "C2DBaseSet.h"
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


class CLASS_DECLSPEC C2DRectSet : public C2DBaseSet
{
public:
	_MEMORY_POOL_DECLARATION
	/// constructor
	C2DRectSet(void);
	/// destructor
	~C2DRectSet(void);


	/// Adds a copy of the other pointer array
	void AddCopy(const C2DRectSet& Other);
	/// Makes a copy of the other
	void MakeCopy( const C2DRectSet& Other);
	/// Passes ONLY the pointers of this type from the Other into this.
	void operator<<(C2DBaseSet& Other);
	/// Adds a new pointer and takes responsibility for it.
	void Add(C2DRect* NewItem) { C2DBaseSet::Add(NewItem);}
	/// Adds a copy of the item given
	void AddCopy(const C2DRect& NewItem);
	/// Deletes the current item and sets the pointer to be the new one
	void DeleteAndSet(int nIndx, C2DRect* NewItem) { C2DBaseSet::DeleteAndSet(nIndx, NewItem );}
	/// Extracts the current item and sets the pointer to be the new one
	C2DRect* ExtractAndSet(int nIndx, C2DRect* NewItem) { return dynamic_cast<C2DRect*> (C2DBaseSet::ExtractAndSet(nIndx, NewItem ));}

	/// Returns the value at the point given
	C2DRect* GetAt(int nIndx)  { return dynamic_cast<C2DRect*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns the value at the point given
	const C2DRect* GetAt(int nIndx) const  { return dynamic_cast<const C2DRect*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	C2DRect& operator[] (int nIndx)  { return *dynamic_cast<C2DRect*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	const C2DRect& operator[] (int nIndx) const { return *dynamic_cast<const C2DRect*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a pointer to the last item
	C2DRect* GetLast(void) { return dynamic_cast<C2DRect*> (C2DBaseSet::GetLast( ) ) ;}

	/// Extracts the pointer passing deletion responsibility over.
	C2DRect* ExtractAt(unsigned int nIndx) { return dynamic_cast<C2DRect*> (C2DBaseSet::ExtractAt(nIndx ) ) ;}
	/// Extracts the pointer passing deletion responsibility over.
	C2DRect* ExtractLast(void) { return dynamic_cast<C2DRect*> (C2DBaseSet::ExtractLast( ) ) ;}
	/// Insertion
	void InsertAt(unsigned int nIndex, C2DRect* NewItem) {C2DBaseSet::InsertAt(nIndex, NewItem);}
	/// Insertion of another array
	void InsertAt(unsigned int nIndex, C2DRectSet& Other) {C2DBaseSet::InsertAt(nIndex, Other);}

	/// Passes all the pointers from the Other into this
	void operator<<(C2DRectSet& Other) {C2DBaseSet::operator <<(Other);}
	/// Adds a new pointer and takes responsibility for it.
	void operator<<(C2DRect* NewItem) {C2DBaseSet::operator <<(NewItem);};

};

#endif