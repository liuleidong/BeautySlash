/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file TravellingSalesman.cpp
\brief Implementation file for the CTravellingSalesman Class.

Implementation file for CTravellingSalesman, a class which uses heuristic methods
to help minimise routes between points. Used by the polygon for reordering points.
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "Travellingsalesman.h"
#include "Sort.h"
#include "C2DPoint.h"
#include "C2DPointSet.h"
#include <list>


const unsigned int conRefineProximity = 10;
const unsigned int conMaxIterations = 5;


class CPointList : public std::list<C2DPoint*>
{

};


_MEMORY_POOL_IMPLEMENATION(CTravellingSalesman)


/**--------------------------------------------------------------------------<BR>
CTravellingSalesman::CTravellingSalesman
\brief Constructor.
<P>---------------------------------------------------------------------------*/
CTravellingSalesman::CTravellingSalesman(void)
{
	m_Points = new CPointList;
}


/**--------------------------------------------------------------------------<BR>
CTravellingSalesman::CTravellingSalesman
\brief Destructor.
<P>---------------------------------------------------------------------------*/
CTravellingSalesman::~CTravellingSalesman(void)
{
	DeleteAll();
	delete m_Points;
}


/**--------------------------------------------------------------------------<BR>
CTravellingSalesman::DeleteAll
\brief Deletes all the points in the collection.
<P>---------------------------------------------------------------------------*/
void CTravellingSalesman::DeleteAll(void)
{
	while(m_Points->size() > 0)
	{
		delete m_Points->back();
		m_Points->pop_back();
	}
}

/**--------------------------------------------------------------------------<BR>
CTravellingSalesman::SetPointsDirect
\brief Allocates all the points in the point set to this.
<P>---------------------------------------------------------------------------*/
void CTravellingSalesman::SetPointsDirect(C2DPointSet& Points)
{
	DeleteAll();

	while (Points.size() > 0)
		m_Points->push_front(Points.ExtractLast());

}


/**--------------------------------------------------------------------------<BR>
CTravellingSalesman::InsertOptimally
\brief Inserts a point in a position which minimises the route from the first to 
the last point. Simply tests every possible place.
<P>---------------------------------------------------------------------------*/
void CTravellingSalesman::InsertOptimally(C2DPoint* pt)
{
	assert(pt);

	// Special cases if there are less than 3 points
	if (m_Points->size() < 2)
	{
		if (m_Points->size() == 1)
			m_Points->push_back(pt);
		return;
	}

	// a pointer to a point and the point after it
	const C2DPoint* ptH1;
	const C2DPoint* ptH2;
	// Set up an iterator and the 2 points.
	std::list<C2DPoint*>::iterator Iter = m_Points->begin();
	ptH1 = *Iter;
	Iter++;
	ptH2 = *Iter;
	std::list<C2DPoint*>::iterator IterInsert = Iter;

	// Find the assumed minimum distance expansion. i.e. if we insert the point
	// between the first and second points what is the increase in the route.
	double dMinExp = ptH1->Distance(*pt) + pt->Distance(*ptH2) - ptH1->Distance(*ptH2);

	// Now go through all the other positions and do the same test, recording the
	// optimal position.
	Iter++;
	while (Iter != m_Points->end())
	{
		ptH1 = ptH2;
		ptH2 = *Iter;

		double dExp = ptH1->Distance(*pt) + pt->Distance(*ptH2) - ptH1->Distance(*ptH2);
		if (dExp < dMinExp)
		{
			dMinExp = dExp;
			IterInsert = Iter;
		}
		Iter++;
	}
	// Finally just insert it in the list at the best place.
	m_Points->insert( IterInsert, pt );

}


/**--------------------------------------------------------------------------<BR>
CTravellingSalesman::Refine
\brief Since repeatedly inserting points optimally does not gaurantee an overall
optimal route between them, this function aims to go through and find a better
arrangement. This takes a point and looks at positions before it and in front
of it (defined by conRefineProximity), looking for a better place. Repeats untill
it goes all the way through without finding a refinement.
<P>---------------------------------------------------------------------------*/
void CTravellingSalesman::Refine(void)
{
	// CHECK FOR LESS THAN 2 ITEMS.
	if (m_Points->size() < 2)
		return;

	bool bRepeat = true;

	int nIt = 0;

	while (bRepeat && nIt < conMaxIterations)
	{
		nIt++;
		std::list<C2DPoint*>::iterator IterRemove = m_Points->begin(); // The item considered for removal.
		IterRemove++; // We don't want to remove the first point
		std::list<C2DPoint*>::iterator IterRemoveLimit = m_Points->end(); // The last item considered for removal.
		IterRemoveLimit--; // We don't want to remove the end. (This IS the end but we won't go that far)

		bRepeat = false;
		while (IterRemove != IterRemoveLimit)
		{
			// Move back along the list untill we find a better place. Only go so far.
			std::list<C2DPoint*>::iterator IterInsert = IterRemove; // The item considered for a new place.
			IterInsert--;
			int nCountBack = 1;
			bool bFound = false;
			std::list<C2DPoint*>::iterator IterRemoveBefore;
			std::list<C2DPoint*>::iterator IterRemoveAfter;
			std::list<C2DPoint*>::iterator IterInsertBefore;
			while (nCountBack < conRefineProximity && IterInsert != m_Points->begin())
			{
				IterRemoveBefore = IterRemove;
				IterRemoveBefore--; // This is the point before the removal
				IterRemoveAfter = IterRemove;
				IterRemoveAfter++; // This is the point after the removal.
				IterInsertBefore = IterInsert;
				IterInsertBefore--;// This is the point before the potential insertion point.

	//			assert((**IterRemove) != (**IterInsert));

				double dCurrentPerimPart = (*IterRemoveBefore)->Distance(**IterRemove) + 
										(*IterRemove)->Distance(**IterRemoveAfter) +
										(*IterInsertBefore)->Distance(**IterInsert);
				double dNewPerimPart = (*IterRemoveBefore)->Distance(**IterRemoveAfter) +
										(*IterInsertBefore)->Distance(**IterRemove) +
										(*IterRemove)->Distance(**IterInsert);
				if (dNewPerimPart < dCurrentPerimPart)
				{
					C2DPoint* ptRemove = *IterRemove;
					m_Points->erase(IterRemove);
					m_Points->insert( IterInsert, ptRemove );
					bFound = true;
					IterRemove = IterRemoveAfter;//IterInsert; // WE HAVE GONE BACK SO PUT THE REMOVAL POINT BACK HERE AND SEARCH AGAIN.
					break;
				}
				IterInsert--;
				nCountBack++;
			}
			
			// Now go forward along the list untill we find a better place. Only go so far.
			int nCountForward = 2;
			IterInsert = IterRemove; // The item considered for a new place.
			IterInsert++;
			if (IterInsert != m_Points->end())
				IterInsert++; // Go forward 2 to avoid taking it out and putting it back in the same place.
			else
				nCountForward = conRefineProximity; // get out

			while (!bFound && nCountForward < conRefineProximity && IterInsert != m_Points->end())
			{
				IterRemoveBefore = IterRemove;
				IterRemoveBefore--; // This is the point before the removal
				IterRemoveAfter = IterRemove;
				IterRemoveAfter++; // This is the point after the removal.
				IterInsertBefore = IterInsert;
				IterInsertBefore -- ;// This is the point before the potential insertion point.

//				assert((**IterRemove) != (**IterInsert));

				double dCurrentPerimPart = (*IterRemoveBefore)->Distance(**IterRemove) + 
										(*IterRemove)->Distance(**IterRemoveAfter) +
										(*IterInsertBefore)->Distance(**IterInsert);
				double dNewPerimPart = (*IterRemoveBefore)->Distance(**IterRemoveAfter) +
										(*IterInsertBefore)->Distance(**IterRemove) +
										(*IterRemove)->Distance(**IterInsert);
				if (dNewPerimPart  < dCurrentPerimPart)
				{
					C2DPoint* ptRemove = *IterRemove;
					m_Points->erase(IterRemove);
					m_Points->insert( IterInsert, ptRemove );
					IterRemove = IterRemoveAfter; 
				//	IterRemove ++;
					bFound = true; 
					break;
				}
				IterInsert++;
				nCountForward++;
			}
			
			if (bFound) bRepeat = true;

			if (!bFound)
				IterRemove++;
		}	// while Remove.
	} // while bRepeat.
}


/**--------------------------------------------------------------------------<BR>
CTravellingSalesman::SimpleReorder
\brief This simply goes through all the points apart from the start and end and
finds the best place for each one. Taks a long time O(n2).
<P>---------------------------------------------------------------------------*/
void CTravellingSalesman::SimpleReorder(void)
{
	// CHECK FOR LESS THAN 2 ITEMS.
	if (m_Points->size() < 2)
		return;

	std::list<C2DPoint*>::iterator IterRemove = m_Points->begin(); // The item considered for removal.
	IterRemove++; // We don't want to remove the first point
	std::list<C2DPoint*>::iterator IterRemoveLimit = m_Points->end(); // The last item considered for removal.
	IterRemoveLimit--; // We don't want to remove the end. (This IS the end but we won't go that far)
		

	while (IterRemove != IterRemoveLimit)
	{
		// Cycle through the rest to find a new place.
		std::list<C2DPoint*>::iterator IterInsert = m_Points->begin(); // The item considered for a new place.
		IterInsert++; // We don't want to insert at the start.
		std::list<C2DPoint*>::iterator IterInsertLimit = m_Points->end(); // The limit for a new place.

		while (IterInsert != IterInsertLimit)
		{
			if (IterInsert == IterRemove)
			{
				IterInsert++;
				continue; // No point removing it and putting back in the same place.
			}

			std::list<C2DPoint*>::iterator IterRemoveBefore = IterRemove;
			IterRemoveBefore--; // This is the point before the removal
			std::list<C2DPoint*>::iterator IterRemoveAfter = IterRemove;
			IterRemoveAfter++; // This is the point after the removal.
			std::list<C2DPoint*>::iterator IterInsertBefore = IterInsert;
			IterInsertBefore -- ;// This is the point before the potential insertion point.

			double dCurrentPerimPart = (*IterRemoveBefore)->Distance(**IterRemove) + 
									   (*IterRemove)->Distance(**IterRemoveAfter) +
									   (*IterInsertBefore)->Distance(**IterInsert);
			double dNewPerimPart = (*IterRemoveBefore)->Distance(**IterRemoveAfter) +
									(*IterInsertBefore)->Distance(**IterRemove) +
									(*IterRemove)->Distance(**IterInsert);
			if (dNewPerimPart < dCurrentPerimPart)
			{
				C2DPoint* ptRemove = *IterRemove;
				m_Points->erase(IterRemove);
				m_Points->insert( IterInsert, ptRemove );
				break;
			}
			IterInsert++;

		}

		IterRemove++;
	}
}


/**--------------------------------------------------------------------------<BR>
CTravellingSalesman::CTravellingSalesman
\brief Removes all the points and puts them in the point set.
<P>---------------------------------------------------------------------------*/
void CTravellingSalesman::ExtractPoints(C2DPointSet& Points)
{
	while (m_Points->size() > 0)
	{
		Points.Add( m_Points->front());
		m_Points->pop_front();
	}
}

/**--------------------------------------------------------------------------<BR>
CTravellingSalesman::CTravellingSalesman
\brief This function takes out all the points apart from the first and last and 
then sorts them all by their distance from the line between the first and second
points and then inserts them all optimally.
<P>---------------------------------------------------------------------------*/
void CTravellingSalesman::Optimize(void)
{
	if (m_Points->size() < 4)
		return;

	// Take out the start.
	C2DPoint* pStart = m_Points->front();
	m_Points->pop_front();

	// Take out the end.
	C2DPoint* pEnd = m_Points->back();
	m_Points->pop_back();
	
	// Take all the rest out.
	C2DPointSet Points;
	this->ExtractPoints(Points);

	// Put the ends back in.
	m_Points->push_front(pStart);
	m_Points->push_back(pEnd);

	// Sort the rest by approx distance from the line in reverse order so we can get them off the end.
	Points.SortByDistance(pStart->GetMidPoint(*pEnd), false);

	// Add them all in the most sensible place (not gauranteed).
	while (Points.size() > 0)
	{
		this->InsertOptimally(Points.ExtractLast());
	}
}




/**--------------------------------------------------------------------------<BR>
CTravellingSalesman::CTravellingSalesman
\brief This function has the same aim as the other refine function but this one
takes each point and searches forward from it to find a better place for it.
<P>---------------------------------------------------------------------------*/
void CTravellingSalesman::Refine2(void)
{

	unsigned int nSize = m_Points->size();
	if (nSize < 4)
		return;

	std::list<C2DPoint*>::iterator Iter = m_Points->begin(); 
	std::list<C2DPoint*>::iterator Iter1;
	std::list<C2DPoint*>::iterator Iter2;
	std::list<C2DPoint*>::iterator Iter3;
	unsigned int nIndex = 0;
	unsigned int nIndexLimit = nSize - 3;

	bool bRepeat = true;

	int nIt = 0;

	while (bRepeat && nIt < conMaxIterations)
	{
		nIt++;
		bRepeat = false;
		while (nIndex < nIndexLimit)
		{
			Iter1 = Iter;
			Iter1++;
			Iter2 = Iter;	
			Iter2++;
			Iter2++;
			Iter3 = Iter;		
			Iter3++;
			Iter3++;
			Iter3++;

			double dCurrentPerimPart = (*Iter)->Distance(**Iter1) + 
										(*Iter1)->Distance(**Iter2) +
										(*Iter2)->Distance(**Iter3);

			double dNewPerimPart = (*Iter)->Distance(**Iter2) +
										(*Iter2)->Distance(**Iter1) +
										(*Iter1)->Distance(**Iter3);

			if (dCurrentPerimPart > dNewPerimPart)
			{
				C2DPoint* pRem = *Iter2;
				m_Points->erase(Iter2);
				m_Points->insert( Iter1, pRem );
				bRepeat = true;
			}


			Iter++;
			nIndex++;
		}	

	}
}