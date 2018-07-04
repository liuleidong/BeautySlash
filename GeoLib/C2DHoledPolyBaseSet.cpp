/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------<BR>
\file 2DHoledPolyBaseSet.cpp
\brief Implementation file for the C2DHoledPolyBaseSet Class.

Implementation file for C2DHoledPolyBaseSet, a collection of holed polygons.
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "C2DHoledPolyBaseSet.h"
#include "C2DBaseSet.h"
#include "Grid.h"
#include "C2DHoledPolyBase.h"
#include "C2DPolyBase.h"
#include "C2DLineBase.h"


_MEMORY_POOL_IMPLEMENATION(C2DHoledPolyBaseSet)


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::C2DHoledPolyBaseSet
\brief Constructor
<P>---------------------------------------------------------------------------*/
C2DHoledPolyBaseSet::C2DHoledPolyBaseSet(void)
{
	m_Type = PolyHoledBaseSet;
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::~C2DHoledPolyBaseSet
\brief Destructor
<P>---------------------------------------------------------------------------*/
C2DHoledPolyBaseSet::~C2DHoledPolyBaseSet(void)
{
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::AddCopy
\brief Adds a copy of the other set
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBaseSet::AddCopy(const C2DHoledPolyBaseSet& Other)
{
	for(unsigned int i = 0 ; i < Other.size(); i++)
	{
		AddCopy(Other[i]);
	}

}
/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::MakeCopy
\brief Makes a copy of the other set
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBaseSet::MakeCopy( const C2DHoledPolyBaseSet& Other)
{
	DeleteAll();
	AddCopy( Other);

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::operator<<
\brief Passes ONLY the pointers of this type from the Other into this.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBaseSet::operator<<(C2DBaseSet& Other)
{
	C2DBaseSet Temp;

	while (Other.size() > 0)
	{
		C2DBase* pLast = Other.ExtractLast();
		C2DBase::E_TYPE eType = pLast->GetType();

		if (eType == C2DBase::PolyHoledBase ||
			eType == C2DBase::PolyArcHoled ||
			eType == C2DBase::PolyLineHoled)
		{
			C2DBaseSet::Add(pLast);
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
C2DHoledPolyBaseSet::operator<<
\brief Passes all the pointers from the Other into this.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBaseSet::operator<<(C2DPolyBaseSet& Other)
{
	C2DHoledPolyBaseSet TempSet;

	while (Other.size() > 0)
	{
		TempSet.Add(new C2DHoledPolyBase);
		TempSet.GetLast()->SetRimDirect( Other.ExtractLast());
	}

	while (TempSet.size() > 0)
	{
		this->Add( TempSet.ExtractLast());
	}
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::operator<<
\brief Adds a new pointer and takes responsibility for it.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBaseSet::operator<<(C2DPolyBase* NewItem) 
{
	C2DHoledPolyBase* pNew = new C2DHoledPolyBase;
	pNew->SetRimDirect( NewItem );
	Add(pNew);

}



/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::UnifyBasic
\brief Simple Unification.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBaseSet::UnifyBasic(void)
{
	C2DHoledPolyBaseSet TempSet;
	C2DHoledPolyBaseSet UnionSet;

	while (size() > 0)
	{
		C2DHoledPolyBase* pLast = this->ExtractLast();
	
		bool bIntersect = false;
		unsigned int i = 0;

		while ( i < size() && !bIntersect)
		{
			this->GetAt(i)->GetUnion( *pLast, UnionSet);

			if (UnionSet.size() == 1)
			{
				this->DeleteAndSet( i, UnionSet.ExtractAt(0));
				delete pLast;
				bIntersect = true;
			}
			else
			{
				assert(UnionSet.size() == 0);
				UnionSet.DeleteAll();
				i++;
			}
		}

		if (!bIntersect)
		{
			TempSet.Add(pLast);
		}
	}

	(*this) << TempSet;

}



/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::AddAndUnify
\brief This function adds the polygon set whilst unifying it with others in the
collection. Assumes that the both sets are distinct to start with.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBaseSet::AddAndUnify(C2DHoledPolyBaseSet& pOther)
{
	C2DHoledPolyBaseSet TempSet;

	while (pOther.size() >0)
	{
		C2DHoledPolyBase* pLast = pOther.ExtractLast();	
		
		if (!AddIfUnify(pLast))
			TempSet.Add(pLast);
	}

	(*this) << TempSet;

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::AddAndUnify
\brief This function adds the polygon whilst unifying it with others in the
collection. Assumes that the current set are distinct to start with.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBaseSet::AddAndUnify(C2DHoledPolyBase* pPoly)
{
	if (! AddIfUnify(pPoly))
		Add(pPoly);
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::AddIfUnify
\brief This function adds the polygon whilst unifying it with others in the
collection only if there is a unification possible. Assumes that the current 
set are distinct to start with.
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBaseSet::AddIfUnify(C2DHoledPolyBase* pPoly)
{
	C2DHoledPolyBaseSet TempSet;
	C2DHoledPolyBaseSet UnionSet;

	while (size() > 0 && pPoly != 0)
	{
		C2DHoledPolyBase* pLast = this->ExtractLast();	

		pLast->GetUnion( *pPoly, UnionSet);

		if (UnionSet.size() == 1)
		{
			delete pPoly;
			pPoly = 0;
			delete pLast;
			pLast = 0;

			AddAndUnify( UnionSet.ExtractAt(0));
		}
		else
		{
			assert(UnionSet.size() == 0);
			UnionSet.DeleteAll();
			TempSet.Add( pLast );
		}
	}

	(*this) << TempSet;

	return (pPoly == 0);
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::GetLineCount
\brief This function gets the line count for the entire set.
<P>---------------------------------------------------------------------------*/
unsigned int C2DHoledPolyBaseSet::GetLineCount(void)
{
	unsigned int nResult = 0 ;

	for (unsigned int n = 0; n < size() ; n ++)
	{
		nResult += GetAt(n)->GetLineCount();
	}

	return nResult;

}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::GetMinLineCount
\brief Returns the minimum line count of all polys in the set.
<P>---------------------------------------------------------------------------*/
unsigned int C2DHoledPolyBaseSet::GetMinLineCount(void)
{
	unsigned int nMin = ~(unsigned int)0;

	for(unsigned int i = 0 ; i < size(); i++)
	{
		unsigned int nCount = this->GetAt(i)->GetLineCount();
		if( nCount < nMin)
		{
			nMin = nCount;
		}
	}
	return nMin;
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::UnifyProgressive
\brief This function unifies the set by progressively allowing polygons with 
higher numbers of lines. This is to avoid doing the union of a large polygon
with several smaller ones as it is much better to unify the small ones then add 
to the big one. By far the quickest way for large numbers of shape with lots of
lines.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBaseSet::UnifyProgressive(CGrid::eDegenerateHandling eDegen) 
{
	switch( eDegen )
	{
	case CGrid::RandomPerturbation:
		for (unsigned int i = 0 ; i < size() ; i++)
		{
			GetAt(i)->RandomPerturb();
		}
		eDegen = CGrid::None;
		break;
	case CGrid::DynamicGrid:

		break;
	case CGrid::PreDefinedGrid:
		SnapToGrid();
		eDegen = CGrid::PreDefinedGridPreSnapped;
		break;
	case CGrid::PreDefinedGridPreSnapped:

		break;
	}


	C2DHoledPolyBaseSet NoUnionSet;
	C2DHoledPolyBaseSet PossUnionSet;
	C2DHoledPolyBaseSet SizeHoldSet;
	C2DHoledPolyBaseSet UnionSet;
	C2DHoledPolyBaseSet TempSet;

	unsigned int nThreshold = GetMinLineCount();

	if (nThreshold == 0)
		nThreshold = 10;	// avoid infinate loop.
	
	// Assumed all are size held to start
	SizeHoldSet << (*this);

	do
	{
		// double the threshold
		nThreshold *= 3;

		// Put all the possible intersects back in this.
		(*this) << PossUnionSet;

		// Put all the size held that are small enough back (or in to start with)
		while (SizeHoldSet.size() > 0)
		{
			C2DHoledPolyBase* pLast = SizeHoldSet.ExtractLast();
			if (pLast->GetLineCount() > nThreshold)
			{
				TempSet << pLast;
			}
			else
			{
				(*this) << pLast;
			}
		}
		SizeHoldSet << TempSet;


		// Cycle through all popping the last and finding a union
		while (size() > 0)
		{
			C2DHoledPolyBase* pLast = this->ExtractLast();
		
			bool bIntersect = false;

			unsigned int i = 0;
			while ( i < size() && !bIntersect )
			{
				GetAt(i)->GetUnion( *pLast, UnionSet, eDegen);

				if (UnionSet.size() == 1)
				{
					C2DHoledPolyBase* pUnion = UnionSet.ExtractLast();

					if (pUnion->GetLineCount() > nThreshold)
					{
						DeleteAt(i);
						SizeHoldSet << pUnion;
					}
					else
					{
						DeleteAndSet( i, pUnion);
						i++;
					}

					delete pLast;
					bIntersect = true;
				}
				else
				{
					assert(UnionSet.size() == 0);
					UnionSet.DeleteAll();
					i++;
				}
			}

			if (!bIntersect)
			{
				bool bPosInterSect = false;
				for (unsigned int j = 0 ; j <  SizeHoldSet.size(); j ++)
				{
					if (pLast->GetRim()->GetBoundingRect().Overlaps( 
								SizeHoldSet[j].GetRim()->GetBoundingRect()))
					{
						bPosInterSect = true;	
						break;
					}
				}

				if (bPosInterSect)
				{
					PossUnionSet << pLast;
				}
				else
				{
					NoUnionSet << pLast;
				}
			}
		}
	}
	while (SizeHoldSet.size() != 0);


	(*this) << NoUnionSet;
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::AddKnownHoles
\brief Adds the shapes provided assuming they are known holes - adds them as holes
to those already in the set.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBaseSet::AddKnownHoles( C2DPolyBaseSet& pOther )
{
	if (size() != 0)
	{
		while (pOther.size() > 0)
		{
			C2DPolyBase* pLast = pOther.ExtractLast();
			if (pLast->GetLineCount() > 0)
			{
				unsigned int i = size() - 1;
				bool bFound = false;
				while ( i > 0 && !bFound)
				{
					if ( GetAt(i)->Contains( pLast->GetLine(0)->GetPointFrom()))
					{
						GetAt(i)->AddHoleDirect( pLast);
						bFound = true;
					}
					i--;
				}

				if (!bFound)
				{
					GetAt(0)->AddHoleDirect( pLast);
				}
			}
			else
			{
				delete pLast;
			}
		}
	}
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::AddKnownHoles
\brief Makes all arc valid if not already by adjusting radius to minimum required.
<P>---------------------------------------------------------------------------*/
unsigned int C2DHoledPolyBaseSet::MakeValidArcs(void)
{
	unsigned int nResult = 0;

	for (unsigned int i = 0 ; i <  size(); i++)
	{
		nResult += this->GetAt(i)->MakeValidArcs();
	}

	return nResult;
}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::AddKnownHoles
\brief True if there are arcs in the shape.
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBaseSet::HasArcs(void) const
{
	for (unsigned int i = 0 ; i <  size(); i++)
	{
		if (this->GetAt(i)->HasArcs())
			return true;
	}
	return false;

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::AddKnownHoles
\brief True if all the arcs are valid
<P>---------------------------------------------------------------------------*/
bool C2DHoledPolyBaseSet::IsValidArcs(void) const
{
	for (unsigned int i = 0 ; i <  size(); i++)
	{
		if (!this->GetAt(i)->IsValidArcs())
			return false;
	}
	return true;

}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::AddKnownHoles
\brief Removes all lines whose end is the same is the start. Returns the number found.
<P>---------------------------------------------------------------------------*/
unsigned int C2DHoledPolyBaseSet::RemoveNullLines(void)
{
	unsigned int nResult = 0;

	for (unsigned int i = 0 ; i <  size(); i++)
	{
		nResult += this->GetAt(i)->RemoveNullLines();
	}

	return nResult;


}


/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::MakeClockwise
\brief Makes the order clockwise.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBaseSet::MakeClockwise(void)
{
	for (unsigned int i = 0 ; i <  size(); i++)
	{
		this->GetAt(i)->MakeClockwise();
	}
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::Transform
\brief Transform by the given operator.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBaseSet::Transform(CTransformation* pProject)
{
	for (unsigned int i = 0 ; i <  size(); i++)
	{
		this->GetAt(i)->Transform(pProject);
	}
}

/**--------------------------------------------------------------------------<BR>
C2DHoledPolyBaseSet::Transform
\brief Transform by the given operator.
<P>---------------------------------------------------------------------------*/
void C2DHoledPolyBaseSet::InverseTransform(CTransformation* pProject)
{
	for (unsigned int i = 0 ; i <  size(); i++)
	{
		this->GetAt(i)->InverseTransform(pProject);
	}
}