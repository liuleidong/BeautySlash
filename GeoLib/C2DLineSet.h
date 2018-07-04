/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DLineSet.h
\brief File for the C2DLineSet class.

File for the C2DLineSet class, a collection of lines. 

\class C2DLineSet.
\brief C2DLineSet class a collection of lines.
<P>---------------------------------------------------------------------------*/



#ifndef _GEOLIB_C2DLINESET_H 
#define _GEOLIB_C2DLINESET_H

#include "C2DLine.h"
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


class CLASS_DECLSPEC C2DLineSet :  public C2DBaseSet
{
public:
	_MEMORY_POOL_DECLARATION

	/// constructor
	C2DLineSet(void);
	/// destructor
	~C2DLineSet(void);

	/// Adds a copy of the other pointer array
	void AddCopy(const C2DLineSet& Other);
	/// Makes a copy of the other
	void MakeCopy( const C2DLineSet& Other);

	/// Passes ONLY the pointers of this type from the Other into this.
	void operator<<(C2DBaseSet& Other);

	/// Adds a copy of the item given
	void AddCopy(const C2DLine& NewItem);

	void AddCopy(double x1, double y1, double x2, double y2);
	///// Add a line
	void AddCopy(const C2DPoint& pt1, const C2DPoint& pt2);

	/// Adds a new pointer and takes responsibility for it.
	void Add(C2DLine* NewItem) { C2DBaseSet::Add(NewItem);}

	/// Deletes the current item and sets the pointer to be the new one
	void DeleteAndSet(int nIndx, C2DLine* NewItem) { C2DBaseSet::DeleteAndSet(nIndx, NewItem );}
	/// Extracts the current item and sets the pointer to be the new one
	C2DLineBase* ExtractAndSet(int nIndx, C2DLine* NewItem) { return dynamic_cast<C2DLine*> (C2DBaseSet::ExtractAndSet(nIndx, NewItem ));}

	/// Returns the value at the point given
	C2DLine* GetAt(int nIndx)  { return dynamic_cast<C2DLine*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns the value at the point given
	const C2DLine* GetAt(int nIndx) const  { return dynamic_cast<const C2DLine*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	C2DLine& operator[] (int nIndx)  { return *dynamic_cast<C2DLine*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a reference to the value at the point given.
	const C2DLine& operator[] (int nIndx) const { return *dynamic_cast<const C2DLine*> (C2DBaseSet::GetAt(nIndx ) ) ;}
	/// Returns a pointer to the last item
	C2DLine* GetLast(void) { return dynamic_cast<C2DLine*> (C2DBaseSet::GetLast( ) ) ;}

	/// Extracts the pointer passing deletion responsibility over.
	C2DLine* ExtractAt(unsigned int nIndx) { return dynamic_cast<C2DLine*> (C2DBaseSet::ExtractAt(nIndx ) ) ;}
	/// Extracts the pointer passing deletion responsibility over.
	C2DLine* ExtractLast(void) { return dynamic_cast<C2DLine*> (C2DBaseSet::ExtractLast( ) ) ;}
	/// Insertion
	void InsertAt(unsigned int nIndex, C2DLine* NewItem) {C2DBaseSet::InsertAt(nIndex, NewItem);}
	/// Insertion of another array
	void InsertAt(unsigned int nIndex, C2DLineSet& Other) {C2DBaseSet::InsertAt(nIndex, Other);}

	/// Passes all the pointers from the Other into this
	void operator<<(C2DLineSet& Other) {C2DBaseSet::operator <<(Other);}
	/// Adds a new pointer and takes responsibility for it.
	void operator<<(C2DLine* NewItem) {C2DBaseSet::operator <<(NewItem);};

};


#endif