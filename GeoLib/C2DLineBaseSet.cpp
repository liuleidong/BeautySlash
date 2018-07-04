/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DLineBaseSet.cpp
\brief Implementation file for the C2DLineBaseSet Class.

Implementation file for C2DLineBaseSet, a set of pointer to C2DLineBase.
<P>---------------------------------------------------------------------------*/



#include "StdAfx.h"
#include "C2DLineBaseSet.h"
#include "C2DBaseSet.h"
#include "C2DLineBase.h"
#include "C2DLine.h"
#include "C2DArc.h"
#include "C2DRect.h"
#include "Sort.h"
#include "C2DPointSet.h"
#include "IndexSet.h"

_MEMORY_POOL_IMPLEMENATION(C2DLineBaseSet)

/**--------------------------------------------------------------------------<BR>
C2DLineBaseSet::C2DLineBaseSet
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DLineBaseSet::C2DLineBaseSet(void) 
{
	m_Type = LineBaseSet;
}


/**--------------------------------------------------------------------------<BR>
C2DLineBaseSet::~C2DLineBaseSet
\brief Destructor.
<P>---------------------------------------------------------------------------*/
C2DLineBaseSet::~C2DLineBaseSet(void) 
{
}


/**--------------------------------------------------------------------------<BR>
C2DLineBaseSet::MakeCopy
\brief Makes a copy of the other set.
<P>---------------------------------------------------------------------------*/
void C2DLineBaseSet::MakeCopy( const C2DLineBaseSet& Other)
{
	DeleteAll();
	AddCopy(Other);

}


/**--------------------------------------------------------------------------<BR>
C2DLineBaseSet::operator<<
\brief Passes ONLY the pointers of this type from the Other into this.
<P>---------------------------------------------------------------------------*/
void C2DLineBaseSet::operator<<(C2DBaseSet& Other)
{
	C2DBaseSet Temp;

	while (Other.size() > 0)
	{
		C2DBase* pLast = Other.ExtractLast();
		if (pLast->GetType() == C2DBase::StraightLine ||
			pLast->GetType() == C2DBase::ArcedLine)
		{
			Add( dynamic_cast<C2DLineBase*>(pLast) );
		}
		else
		{
			Temp << pLast;
		}
	}

	while (Temp.size() > 0)
	{
		Other << Temp.ExtractLast();
	}
}


/**--------------------------------------------------------------------------<BR>
CGeoLineSetBase::AddCopy
\brief Adds a copy of the line given by finding out what type it is.
<P>---------------------------------------------------------------------------*/
void C2DLineBaseSet::AddCopy(const C2DLineBase& Line)
{
	switch (Line.GetType())
	{
		case C2DBase::StraightLine:
			Add(new C2DLine( dynamic_cast< const C2DLine&>( Line)));
			break;

		case C2DBase::ArcedLine:
			Add(new C2DArc( dynamic_cast< const C2DArc&>( Line)));
			break;

		default:
			assert(0);
			break;
	}
}


/**--------------------------------------------------------------------------<BR>
C2DLineBaseSet::GetIntersections
\brief Gets the intersections within this set.
<P>---------------------------------------------------------------------------*/
void C2DLineBaseSet::GetIntersections(C2DPointSet* pPoints, 
		CIndexSet* pIndexes1, CIndexSet* pIndexes2) const
{
	// The structure to be used to store all the data
	struct sLineBaseRect
	{
		const C2DLineBase* pLine;
		C2DRect Rect;
		unsigned int usIndex;
	};

	// Set up an array of these structures and the left most points of the line rects
	std::vector<sLineBaseRect*> Lines;
	std::vector<double> xValues;

	for (unsigned int i = 0 ; i <  size() ; i++)
	{
		sLineBaseRect* pLineRect = new sLineBaseRect;
		pLineRect->pLine = GetAt(i);
		pLineRect->pLine->GetBoundingRect(pLineRect->Rect);
		pLineRect->usIndex = i;
		Lines.push_back(pLineRect);
		xValues.push_back(  pLineRect->Rect.GetLeft());
	}
	// Sort them all according to the left most point of the line rects.
	GeoSort::PQuickSort< std::vector<double>, double, std::vector<sLineBaseRect*>, sLineBaseRect*>( xValues, Lines);

	unsigned int j = 0;
	C2DPointSet IntPt;
	// For each line...
	while (j < Lines.size())
	{
		unsigned int r = j + 1;

		double dXLimit = Lines[j]->Rect.GetRight();
		// ...search forward untill the end or a line whose rect starts after this ends
		while (r < Lines.size() && Lines[r]->Rect.GetLeft() < dXLimit)
		{
			
			if ( Lines[j]->Rect.Overlaps(  Lines[r]->Rect) &&
				Lines[j]->pLine->Crosses(  *Lines[r]->pLine, &IntPt) )
			{
				while (IntPt.size() > 0)
				{
					if (pPoints != 0)
						pPoints->Add( IntPt.ExtractLast());

					if (pIndexes1)
					{
						pIndexes1->Add( Lines[j]->usIndex );
					}
					if (pIndexes2)
					{
						pIndexes2->Add( Lines[r]->usIndex );
					}	
				}
			}
			r++;
		}	
		j++;
	}

	unsigned int d = 0 ;
	while (d < Lines.size())
	{
		delete Lines.at(d);
		d++;
	}
}

/**--------------------------------------------------------------------------<BR>
C2DLineBaseSet::GetIntersections
\brief Gets the intersections between this set and the other.
<P>---------------------------------------------------------------------------*/
void C2DLineBaseSet::GetIntersections(const C2DLineBaseSet& Other, C2DPointSet* pPoints, 
			CIndexSet* pIndexesThis, CIndexSet* pIndexesOther,
			const C2DRect* pBoundingRectThis , const  C2DRect* pBoundingRectOther) const
{
	struct sLineBaseRect
	{
		const C2DLineBase* pLine;
		C2DRect Rect;
		unsigned int usIndex;
		bool bSetFlag;
	};

	std::vector<sLineBaseRect*> Lines;
	std::vector<double> xValues;

	for (unsigned int i = 0 ; i <  size() ; i++)
	{
		sLineBaseRect* pLineRect = new sLineBaseRect;
		pLineRect->pLine = GetAt(i);
		pLineRect->pLine->GetBoundingRect(pLineRect->Rect);
		pLineRect->usIndex = i;
		pLineRect->bSetFlag = true;

		if (pBoundingRectOther == 0 || pBoundingRectOther->Overlaps( pLineRect->Rect))
		{
			Lines.push_back(pLineRect);
			xValues.push_back(  pLineRect->Rect.GetLeft());
		}
		else
		{
			delete pLineRect;
		}
	}

	for (unsigned int d = 0 ; d <  Other.size() ; d++)
	{
		sLineBaseRect* pLineRect = new sLineBaseRect;
		pLineRect->pLine = Other.GetAt(d);
		pLineRect->pLine->GetBoundingRect(pLineRect->Rect);
		pLineRect->usIndex = d;
		pLineRect->bSetFlag = false;

		if (pBoundingRectThis == 0 || pBoundingRectThis->Overlaps( pLineRect->Rect))
		{
			Lines.push_back(pLineRect);
			xValues.push_back(  pLineRect->Rect.GetLeft());
		}
		else
		{
			delete pLineRect;
		}
	}

	GeoSort::PQuickSort< std::vector<double>, double, std::vector<sLineBaseRect*>, sLineBaseRect*>( xValues, Lines);


	unsigned int j = 0;
	C2DPointSet IntPt;
	while (j < Lines.size())
	{
		unsigned int r = j + 1;

		double dXLimit = Lines[j]->Rect.GetRight();

		while (r < Lines.size() && 
			   Lines[r]->Rect.GetLeft() < dXLimit)
		{
			
			if (  ( Lines[j]->bSetFlag ^ Lines[r]->bSetFlag  ) &&				
					Lines[j]->Rect.Overlaps(  Lines[r]->Rect) &&
					Lines[j]->pLine->Crosses(  *Lines[r]->pLine, &IntPt) )
			{
				while (IntPt.size() >0)
				{
					if (pPoints != 0)
						pPoints->Add( IntPt.ExtractLast());

					if (pIndexesThis)
					{
						if (Lines[j]->bSetFlag)
							pIndexesThis->Add( Lines[j]->usIndex );
						else
							pIndexesThis->Add( Lines[r]->usIndex );
					}
					if (pIndexesOther)
					{
						if (Lines[j]->bSetFlag)
							pIndexesOther->Add( Lines[r]->usIndex );
						else
							pIndexesOther->Add( Lines[j]->usIndex );
					}
				}
			}
			r++;
		}	
		j++;
	}


	unsigned int d = 0 ;
	while (d < Lines.size())
	{
		delete Lines.at(d);
		d++;
	}
}

/**--------------------------------------------------------------------------<BR>
C2DLineBaseSet::HasCrossingLines
\brief Returns true if there are any intersections in the set.
<P>---------------------------------------------------------------------------*/
bool C2DLineBaseSet::HasCrossingLines(void) const
{
	// The structure to be used to store all the data
	struct sLineBaseRect
	{
		const C2DLineBase* pLine;
		C2DRect Rect;
	};

	// Set up an array of these structures and the left most points of the line rects
	std::vector<sLineBaseRect*> Lines;
	std::vector<double> xValues;

	for (unsigned int i = 0 ; i <  size() ; i++)
	{
		sLineBaseRect* pLineRect = new sLineBaseRect;
		pLineRect->pLine = GetAt(i);
		pLineRect->pLine->GetBoundingRect(pLineRect->Rect);
		Lines.push_back(pLineRect);
		xValues.push_back(  pLineRect->Rect.GetLeft());
	}
	// Sort them all according to the left most point of the line rects.
	GeoSort::PQuickSort< std::vector<double>, double, std::vector<sLineBaseRect*>, sLineBaseRect*>( xValues, Lines);

	unsigned int j = 0;
	C2DPointSet IntPt;
	bool bIntersect = false;
	// For each line...
	while (j < Lines.size() && !bIntersect)
	{
		unsigned int r = j + 1;

		double dXLimit = Lines[j]->Rect.GetRight();
		// ...search forward untill the end or a line whose rect starts after this ends
		while ( !bIntersect && r < Lines.size() && Lines[r]->Rect.GetLeft() < dXLimit )
		{
			if ( Lines[j]->Rect.Overlaps(  Lines[r]->Rect) &&
				Lines[j]->pLine->Crosses(  *Lines[r]->pLine, &IntPt) )
			{
				bIntersect = true;
			}
			r++;
		}	
		j++;
	}

	unsigned int d = 0 ;
	while (d < Lines.size())
	{
		delete Lines.at(d);
		d++;
	}

	return bIntersect;
}

/**--------------------------------------------------------------------------<BR>
C2DLineBaseSet::IsClosed
\brief True if the set is completely closed to form a loop.
<P>---------------------------------------------------------------------------*/
bool C2DLineBaseSet::IsClosed(bool bEndsOnly) const
{
	unsigned int usSize = size();
	
	if (bEndsOnly)
	{
		if (GetAt(0)->GetPointFrom() == GetAt(usSize - 1)->GetPointTo())
			return true;
		else
			return false;
	}
	else
	{
		for (unsigned int i = 0; i < usSize; i++)
		{
			unsigned int usNext = (i + 1) % usSize;

			if (GetAt(i)->GetPointTo() != GetAt(usNext)->GetPointFrom() )
				return false;
		}

		return true;
	}
}


/**--------------------------------------------------------------------------<BR>
C2DLineBaseSet::AddCopy
\brief Adds a copy of the line.
<P>---------------------------------------------------------------------------*/
void C2DLineBaseSet::AddCopy(const C2DLineBaseSet& Other)
{
	for( unsigned int i = 0 ; i < Other.size(); i++)
	{
		const C2DLineBase& Line = Other[i];
		switch (Line.GetType())
		{
		case C2DBase::StraightLine:
			Add(new C2DLine( dynamic_cast<const C2DLine&>(Line)));
			break;
		case C2DBase::ArcedLine:
			Add(new C2DArc( dynamic_cast<const C2DArc&>(Line)));
			break;

		}
	}
}


/**--------------------------------------------------------------------------<BR>
C2DLineBaseSet::ReverseDirection
\brief Reverse direction.
<P>---------------------------------------------------------------------------*/
void C2DLineBaseSet::ReverseDirection(void )
{
	ReverseOrder();

	for (unsigned int i = 0; i < size() ; i++)
	{
		GetAt(i)->ReverseDirection();
	}	
}




/**--------------------------------------------------------------------------<BR>
C2DLineBaseSet::AddIfCommonEnd
\brief Adds the points of another to this if they have a common end which is either
the first or last point. Returns true if there was a match.
<P>---------------------------------------------------------------------------*/
bool C2DLineBaseSet::AddIfCommonEnd(C2DLineBaseSet& Other)
{
	assert(!IsClosed());
	assert(!Other.IsClosed());

	int nThisCount = size() ;
	if (nThisCount < 1) return false;

	int nOtherCount = Other.size();
	if (nOtherCount < 1) return false;

	if ( GetAt(0)->GetPointFrom() == Other.GetAt(0)->GetPointFrom())
	{
		ReverseDirection();

		*this << Other;

		return true;
	}
	else if( GetAt(0)->GetPointFrom() == Other.GetAt(nOtherCount - 1)->GetPointTo() )
	{
		ReverseDirection();

		Other.ReverseDirection();

		*this << Other;

		return true;
	}
	else if (GetAt(nThisCount - 1)->GetPointTo() == Other.GetAt(0)->GetPointFrom())
	{
		*this << Other;

		return true;
	}	
	else if (GetAt(nThisCount - 1)->GetPointTo() == Other.GetAt(nOtherCount - 1)->GetPointTo() )
	{
		Other.ReverseDirection();	

		*this << Other;

		return true;
	}	

	return false;
}



/**--------------------------------------------------------------------------<BR>
C2DLineBaseSet::Remove0Lines
\brief Removes lines smaller than the tolerance.
<P>---------------------------------------------------------------------------*/
void C2DLineBaseSet::Remove0Lines(double dTolerance)
{
	for (unsigned int i = 0 ; i < size(); i++)
	{
		double dLength = GetAt(i)->GetLength();

		if (dLength < dTolerance)
		{
			DeleteAt(i);
		}
	}
}