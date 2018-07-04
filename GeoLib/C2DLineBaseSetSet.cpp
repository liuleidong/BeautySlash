/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DLineBaseSetSet.cpp
\brief Implementation file for the C2DLineBaseSetSet Class.

Implementation file for C2DLineBaseSetSet, a collection of lines sets.
<P>---------------------------------------------------------------------------*/


#include "StdAfx.h"
#include "C2DLineBaseSetSet.h"
#include "C2DLineBaseSet.h"

_MEMORY_POOL_IMPLEMENATION(C2DLineBaseSetSet)

/**--------------------------------------------------------------------------<BR>
C2DLineBaseSetSet::C2DLineBaseSetSet
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DLineBaseSetSet::C2DLineBaseSetSet(void)
{
	m_Type = LineBaseSetSet;
}

/**--------------------------------------------------------------------------<BR>
C2DLineBaseSetSet::~C2DLineBaseSetSet
\brief Constructor.
<P>---------------------------------------------------------------------------*/
C2DLineBaseSetSet::~C2DLineBaseSetSet(void)
{

}


/**--------------------------------------------------------------------------<BR>
C2DLineBaseSetSet::MergeJoining
\brief Merges all joining routes.
<P>---------------------------------------------------------------------------*/
void C2DLineBaseSetSet::MergeJoining(void)
{
	C2DLineBaseSetSet Temp;

	while (size() > 0)
	{
		// pop the last one.
		C2DLineBaseSet* pLast = this->ExtractLast();

		if (pLast != 0 && !pLast->IsClosed())
		{
			unsigned int i = 0 ;
			while ( i < size() )
			{
				if ( ! this->GetAt(i)->IsClosed())
				{
					if (this->GetAt(i)->AddIfCommonEnd( *pLast))
					{
						delete pLast;
						pLast = 0;
						i += size();	// escape
					}
				}

				i++;
			}
		}

		if (pLast != 0)
		{
			Temp.Add( pLast);
		}
	}

	(*this) << Temp;

}

/**--------------------------------------------------------------------------<BR>
C2DLineBaseSetSet::AddJoining
\brief Adds all routes that join this. 
<P>---------------------------------------------------------------------------*/
void C2DLineBaseSetSet::AddJoining( C2DLineBaseSetSet& Other )
{
	C2DLineBaseSetSet Temp;
	
	while (Other.size() > 0 )
	{
		C2DLineBaseSet* pLast = Other.ExtractLast();

		unsigned int i = 0;
		while ( i < size())
		{
			if ( !GetAt(i)->IsClosed() && GetAt(i)->AddIfCommonEnd( *pLast))
			{
				delete pLast;
				pLast = 0;
				i += size();	// escape
			}

			i++;
		}

		if (pLast != 0)
		{
			Temp.Add(pLast);
		}
	}

	while (Temp.size() > 0)
	{
		Other.Add( Temp.ExtractLast());
	}
}

/**--------------------------------------------------------------------------<BR>
C2DLineBaseSetSet::AddClosed
\brief Adds all routes that are closed. Flag to indicate whether only the ends
of the route needs checking for closure.
<P>---------------------------------------------------------------------------*/
void C2DLineBaseSetSet::AddClosed(C2DLineBaseSetSet& Other , bool bEndsOnly)
{
	C2DLineBaseSetSet Temp;
	
	while (Other.size() > 0 )
	{
		C2DLineBaseSet* pLast = Other.ExtractLast();
		if (pLast->IsClosed(bEndsOnly))
		{
			this->Add( pLast );
		}
		else
		{
			Temp.Add( pLast );
		}
	}

	while (Temp.size() > 0)
	{
		Other.Add( Temp.ExtractLast());
	}
}