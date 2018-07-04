/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DBaseSet.cpp
\brief Declaration file for the C2DBaseSet Class.

Declaration file for C2DBaseSet, a set of geometric entities of different types.
<P>---------------------------------------------------------------------------*/



#include "StdAfx.h"
#include "C2DBaseSet.h"
#include "C2DPoint.h"
#include "C2DLine.h"
#include "C2DSegment.h"
#include "C2DRect.h"
#include "C2DLineBaseSet.h"
#include "C2DLineSet.h"
#include "C2DTriangle.h"
#include "C2DCircle.h"
#include "C2DRoute.h"
#include "C2DBase.h"
#include "Interval.h"
#include "Sort.h"


_MEMORY_POOL_IMPLEMENATION(C2DBaseSet)


/**--------------------------------------------------------------------------<BR>
C2DBaseSet::C2DBaseSet
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DBaseSet::C2DBaseSet() : C2DBase( BaseSet)
{
	m_Data = new C2DBaseData;

	m_Type = C2DBase::BaseSet;
}


/**--------------------------------------------------------------------------<BR>
C2DBaseSet::~C2DBaseSet
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DBaseSet::~C2DBaseSet()
{
	DeleteAll();

	delete reinterpret_cast<C2DBaseData*>(m_Data);

}


/**--------------------------------------------------------------------------<BR>
C2DBaseSet::DeleteAll
\brief Deletes all.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::DeleteAll(void)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);

	for (unsigned int i = 0 ; i < Data.size(); i++)
	{
		delete Data[i];
	}
	Data.clear();

}

void C2DBaseSet::RemoveAll(void)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);

	Data.clear();
}


/**--------------------------------------------------------------------------<BR>
C2DBaseSet::Add
\brief Adds another item.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::Add(C2DBase* NewItem)
{
	(reinterpret_cast<C2DBaseData*>(m_Data))->push_back(NewItem);
}



/**--------------------------------------------------------------------------<BR>
C2DBaseSet::DeleteAndSet
\brief Deletes the current item and sets the pointer to be the new one.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::DeleteAndSet(int nIndx, C2DBase* NewItem)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);

	C2DBase* pDel = Data[nIndx];

	Data[nIndx] = NewItem;

	delete pDel;
}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::ExtractAndSet
\brief Extracts the current item and sets the pointer to be the new one.
<P>---------------------------------------------------------------------------*/
C2DBase* C2DBaseSet::ExtractAndSet(int nIndx, C2DBase* NewItem)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);

	C2DBase* pEx = Data[nIndx];

	Data[nIndx] = NewItem;

	return pEx;
}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::size
\brief Returns the size.
<P>---------------------------------------------------------------------------*/
unsigned int C2DBaseSet::size(void) const
{
	return (reinterpret_cast<C2DBaseData*>(m_Data))->size();

}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::GetAt
\brief Returns the value at the point given.
<P>---------------------------------------------------------------------------*/
C2DBase* C2DBaseSet::GetAt(int nIndx)
{
	return (*reinterpret_cast<C2DBaseData*>(m_Data))[nIndx];

}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::GetAt
\brief Returns the value at the point given.
<P>---------------------------------------------------------------------------*/
const C2DBase* C2DBaseSet::GetAt(int nIndx) const
{

	return (*reinterpret_cast<C2DBaseData*>(m_Data))[nIndx];
}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::operator[]
\brief Returns a reference to the value at the point given.
<P>---------------------------------------------------------------------------*/
C2DBase& C2DBaseSet::operator[] (int nIndx)
{
	return *(*reinterpret_cast<C2DBaseData*>(m_Data))[nIndx];
}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::operator[]
\brief Returns a reference to the value at the point given.
<P>---------------------------------------------------------------------------*/
const C2DBase& C2DBaseSet::operator[] (int nIndx) const
{
	return *(*reinterpret_cast<C2DBaseData*>(m_Data))[nIndx];
}


/**--------------------------------------------------------------------------<BR>
C2DBaseSet::GetLast
\brief Returns a pointer to the last item.
<P>---------------------------------------------------------------------------*/
C2DBase* C2DBaseSet::GetLast(void)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);

	unsigned int n = Data.size();

	if (n == 0)
		return 0;

	return Data[ n - 1 ];

}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::DeleteAt
\brief Deletes at the index.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::DeleteAt(unsigned int nIndx)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);

	C2DBase* pItem = Data[nIndx];
	if (pItem != 0)
		delete pItem;

	Data.erase(Data.begin() + nIndx);

}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::DeleteLast
\brief Deletion of the last.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::DeleteLast(void)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);

	unsigned int n = Data.size();

	if (n == 0)
		return;
	
	n--;

	delete Data[n];

	Data.erase(Data.begin() + n);

}


/**--------------------------------------------------------------------------<BR>
C2DBaseSet::ExtractAt
\brief Extracts the pointer passing deletion responsibility over.
<P>---------------------------------------------------------------------------*/
C2DBase* C2DBaseSet::ExtractAt(unsigned int nIndx)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);

	C2DBase* Result = Data[nIndx];
	Data.erase( Data.begin() + nIndx);
	return Result;

}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::ExtractLast
\brief Extracts the pointer passing deletion responsibility over.
<P>---------------------------------------------------------------------------*/
C2DBase* C2DBaseSet::ExtractLast(void)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);

	C2DBase* Result = Data.back();
	Data.pop_back();
	return Result;

}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::InsertAt
\brief Inserts the pointer.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::InsertAt(unsigned int nIndx, C2DBase* NewItem)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);

	if (NewItem != 0)
	{
		Data.insert( Data.begin() + nIndx, NewItem);
	}
}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::InsertAt
\brief Inserts the pointer.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::InsertAt(unsigned int nIndx, C2DBaseSet& Other)
{
	while (Other.size() > 0)
	{
		InsertAt(nIndx, Other.ExtractLast());
	}
}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::ReverseOrder
\brief Reverses the order.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::ReverseOrder(void)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);

	C2DBase* Temp;
	unsigned int nCount = Data.size(); 
	unsigned int nHalfWay = nCount / 2;
	for (unsigned int i = 0; i < nHalfWay ; i++)
	{
		Temp = Data[i];
		Data[i] = Data[nCount - 1 - i];
		Data[nCount - 1 - i] = Temp;
	}	
}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::operator<<
\brief Passes all the pointers from the Other into this.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::operator<<(C2DBaseSet& Other)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);

	C2DBaseData Temp;
	while (Other.size() > 0)
		Temp.push_back(Other.ExtractLast());

	while (Temp.size() > 0)
	{
		Data.push_back(Temp.back());
		Temp.pop_back();
	}

}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::operator<<
\brief Adds a new pointer and takes responsibility for it..
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::operator<<(C2DBase* pNewItem)
{
	Add(pNewItem);
}




/**--------------------------------------------------------------------------<BR>
C2DBaseSet::Move
\brief Moves the collection.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::Move(const C2DVector& Vector)
{
	for (unsigned int i = 0 ; i < size(); i++)
		GetAt(i)->Move(Vector);
	
}


/**--------------------------------------------------------------------------<BR>
C2DBaseSet::RotateToRight
\brief Rotate the collection to right.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::RotateToRight(double dAng, const C2DPoint& Origin)
{
	for (unsigned int i = 0 ; i < size(); i++)
		GetAt(i)->RotateToRight(dAng, Origin);

}


/**--------------------------------------------------------------------------<BR>
C2DBaseSet::Grow
\brief Grows the collection.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::Grow(double dFactor, const C2DPoint& Origin)
{
	for (unsigned int i = 0 ; i < size(); i++)
		GetAt(i)->Grow(dFactor, Origin);

}


/**--------------------------------------------------------------------------<BR>
C2DBaseSet::Reflect
\brief Reflects the collection.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::Reflect(const C2DPoint& Point)
{
	for (unsigned int i = 0 ; i < size(); i++)
		GetAt(i)->Reflect(Point);
}


/**--------------------------------------------------------------------------<BR>
C2DBaseSet::Reflect
\brief Reflects the collection.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::Reflect(const C2DLine& Line)
{
	for (unsigned int i = 0 ; i < size(); i++)
		GetAt(i)->Reflect(Line);
}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::Distance
\brief Minimum distance from the point.
<P>---------------------------------------------------------------------------*/
double C2DBaseSet::Distance(const C2DPoint& Point) const
{
	if (size() == 0)
		return 0;
	double dResult = GetAt(0)->Distance(Point);
	if (dResult == 0)
		return 0;
	double dDist;
	for (unsigned int i = 0 ; i < size(); i++)
	{
		dDist = GetAt(i)->Distance(Point);
		if (dDist < dResult)
		{
			if (dDist == 0)
				return 0;
			else
				dResult = dDist;
		}
	}

	return dResult;
}



/**--------------------------------------------------------------------------<BR>
C2DBaseSet::SortByDistance
\brief Sorts the collection by the distance from the point.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::SortByDistance(const C2DPoint& pt,  bool bAscending)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);


	std::vector<double> dDists;

	for (unsigned int i = 0 ; i < size(); i++)
	{
		dDists.push_back( GetAt(i)->Distance(pt));
	}

	GeoSort::PQuickSort<std::vector<double>, double, C2DBaseData, C2DBase*>(dDists, Data);

	if (! bAscending)
		ReverseOrder();
}



/**--------------------------------------------------------------------------<BR>
C2DBaseSet::GetBoundingRect
\brief Returns the bounding rectangle for the whole collection.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::GetBoundingRect(C2DRect& Rect) const
{
	Rect.Clear();

	if ( size() == 0 )
	{
		Rect.Clear();
		return;
	}


	GetAt(0)->GetBoundingRect(Rect);

	C2DRect RectTemp;
	for (unsigned int i = 1 ; i < size(); i++)
	{
		GetAt(i)->GetBoundingRect(RectTemp);
		Rect.ExpandToInclude(RectTemp);
	}
}


/**--------------------------------------------------------------------------<BR>
C2DBaseSet::SnapToGrid
\brief SnapToGrid.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::SnapToGrid(void)
{
	for (unsigned int i = 0 ; i < size(); i++)
	{
		GetAt(i)->SnapToGrid();
	}
}

/**--------------------------------------------------------------------------<BR>
C2DBaseSet::Project
\brief Set projection.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::Project(const C2DLine& Line, CInterval& Interval) const
{
	if (size() ==0)
		return;

	CInterval TempInt;

	GetAt(0)->Project( Line, TempInt);

	for (unsigned int i = 1 ; i < size(); i++)
	{
		GetAt(i)->Project( Line, TempInt);
		Interval.ExpandToInclude(TempInt);
	}
}


/**--------------------------------------------------------------------------<BR>
C2DBaseSet::Project
\brief Set projection.
<P>---------------------------------------------------------------------------*/
void C2DBaseSet::Project(const C2DVector& Vector, CInterval& Interval) const
{
	if (size() ==0)
		return;

	CInterval TempInt;

	GetAt(0)->Project( Vector, TempInt);

	for (unsigned int i = 1 ; i < size(); i++)
	{
		GetAt(i)->Project( Vector, TempInt);
		Interval.ExpandToInclude(TempInt);
	}

}
