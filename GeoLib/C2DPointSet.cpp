/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file 2DPointSet.cpp
\brief Implementation file for the C2DPointSet Class

Implementation file for C2DPointSet, a class which represents a set of points. 
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "C2DPointSet.h"
#include "Sort.h"
#include "C2DLine.h"
#include "C2DBaseSet.h"
#include "C2DRect.h"
#include "C2DCircle.h"


_MEMORY_POOL_IMPLEMENATION(C2DPointSet)

/**--------------------------------------------------------------------------<BR>
C2DPointSet::C2DPointSet<BR>
\brief Constructor does nothing.
<P>---------------------------------------------------------------------------*/
C2DPointSet::C2DPointSet(void)
{
	m_Type = PointSet;
}

/**--------------------------------------------------------------------------<BR>
C2DPointSet::~C2DPointSet<BR>
\brief Destructor does nothing. (Base deletes the points).
<P>---------------------------------------------------------------------------*/
C2DPointSet::~C2DPointSet(void)
{
}


/**--------------------------------------------------------------------------<BR>
C2DPointSet::operator<<
\brief Passes ONLY the pointers of this type from the Other into this.
<P>---------------------------------------------------------------------------*/
void C2DPointSet::operator<<(C2DBaseSet& Other)
{
	C2DBaseSet Temp;

	while (Other.size() > 0)
	{
		C2DBase* pLast = Other.ExtractLast();
		if (pLast->GetType() == C2DBase::Point)
		{
			Add( dynamic_cast<C2DPoint*>(pLast) );
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
C2DPointSet::~C2DPointSet<BR>
\brief Adds a new points defined by x and y.
<P>---------------------------------------------------------------------------*/
void C2DPointSet::AddCopy(double x, double y)
{
	Add(new C2DPoint(x, y));
}

/**--------------------------------------------------------------------------<BR>
C2DPointSet::AddCopy<BR>
\brief Adds a copy of the points provided.
<P>---------------------------------------------------------------------------*/
void C2DPointSet::AddCopy(const C2DPointSet& Other)
{
	for (unsigned int i = 0 ; i < Other.size(); i++)
	{
		Add(new C2DPoint(Other[i]));
	}
}
/**--------------------------------------------------------------------------<BR>
C2DPointSet::MakeCopy<BR>
\brief Makes a copy of the points provided.
<P>---------------------------------------------------------------------------*/
void C2DPointSet::MakeCopy(const C2DPointSet& Other)
{
	DeleteAll();
	AddCopy(Other);

}

/**--------------------------------------------------------------------------<BR>
C2DPointSet::SortByAngleFromNorth<BR>
\brief Sorts by the angle from the positive Y axis relative to the origin given.
<P>---------------------------------------------------------------------------*/
void C2DPointSet::SortByAngleFromNorth( const C2DPoint& Origin)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);

	std::vector<double> AngFromNorth;

	for (unsigned int i = 0 ; i < size(); i++)
	{
		C2DVector Vec(Origin, (*this)[i]);
		AngFromNorth.push_back(Vec.AngleFromNorth());
	}

	GeoSort::PQuickSort<std::vector<double>, double, C2DBaseData, C2DBase*>(AngFromNorth, Data);
}

/**--------------------------------------------------------------------------<BR>
C2DPointSet::ExtractConvexHull<BR>
\brief Extracts the convex hull from the points given. Uses Graham's scan.
<P>---------------------------------------------------------------------------*/
void C2DPointSet::ExtractConvexHull( C2DPointSet& Other)
{
	DeleteAll();

	if (Other.size() < 4)
	{
		*this << Other;
		return;
	}

	C2DPoint ptLeftMost = Other[0];
	int nLeftMost = 0;
	
	// Find left most
	for (unsigned int i = 1 ; i < Other.size(); i++)
	{
		C2DPoint pt = Other[i];
		if (pt.x < ptLeftMost.x)
		{
			ptLeftMost = pt;
			nLeftMost = i;
		}
	}

	this->Add(Other.ExtractAt(nLeftMost));

	Other.SortByAngleFromNorth( (*this)[0]);

	// Always add the left most and the first of the rest.
	Add(Other.ExtractAt(0));

	// Add others if needed.
	unsigned int nIndx = 0;

	C2DPointSet Unused;

	while (nIndx < Other.size())
	{
			unsigned int nLast = size() - 1;
			C2DLine LastLine( (*this)[nLast-1], (*this)[nLast]);

			C2DVector Test((*this)[nLast], Other[nIndx]);

			double dAng = Test.AngleFromNorth();

			if (dAng < LastLine.vector.AngleFromNorth())
			{
				Unused.Add( ExtractAt(nLast) );
			}
			else
			{
				Add(Other.ExtractAt(nIndx));
			}
	}

	Other << Unused;	
}

/**--------------------------------------------------------------------------<BR>
C2DPointSet::GetBoundingRect<BR>
\brief Gets the bounding rectangle.
<P>---------------------------------------------------------------------------*/
void C2DPointSet::GetBoundingRect(C2DRect& Rect) const
{
	if (size() == 0)
	{
		Rect.Clear();
		return;
	}
	else
	{
		Rect.Set( *GetAt(0));

		for (unsigned int i = 1 ; i < size(); i++)
		{
			Rect.ExpandToInclude( *GetAt(i));
		}
	}
}

/**--------------------------------------------------------------------------<BR>
C2DPointSet::GetBoundingCircle<BR>
\brief Returns the minimum bounding circle of the points.
<P>---------------------------------------------------------------------------*/
void C2DPointSet::GetBoundingCircle(C2DCircle& Circle) const
{
	if (size() < 3)
	{
		if (size() == 2)
		{
			Circle.SetMinimum( (*GetAt(0) ), (*GetAt(1)) );
		}
		else if (size() == 1)
		{
			Circle.Set( (*GetAt(0)), 0);
		}
		else
		{
			assert(0);
		}
		return;
	}

	unsigned int nIndx1;
	unsigned int nIndx2;
	unsigned int nIndx3;
	double dDist;

	// First get the points that are furthest away from each other.
	GetExtremePoints(nIndx1, nIndx2, dDist);
	// Set the circle to bound these.
	Circle.SetMinimum( (*GetAt(nIndx1)), (*GetAt(nIndx2) ));
	// Set up a flag to show if we are circumscibed. (Once we are, we always will be).
	bool bCircum = false;
	// Cycle through and if any points aren't in the circle, then set the circle to be circumscribed.
	for (unsigned int i = 0 ; i < size(); i++)
	{
		if ( i != nIndx1 && i != nIndx2)
		{
			if (!Circle.Contains(  (*GetAt(i)) ))
			{
				nIndx3 = i;
				Circle.SetCircumscribed(  (*GetAt(nIndx1)), (*GetAt(nIndx2)), (*GetAt(nIndx3))  );
				bCircum = true;
				// Break out and try again.
				break;
			}
		}
	}

	// If we didn't succeed first time then go through again setting it to be circumscribed every time.
	if (bCircum)
	{
		for (unsigned int i = 0 ; i < size(); i++)
		{
			if ( i != nIndx1 && i != nIndx2 && i != nIndx3)
			{
				if (!Circle.Contains(  (*GetAt(i)) ))
				{
					double Dist1 = GetAt(i)->Distance(  (*GetAt(nIndx1)) );
					double Dist2 = GetAt(i)->Distance(  (*GetAt(nIndx2)) );
					double Dist3 = GetAt(i)->Distance(  (*GetAt(nIndx3)) );
					if (Dist1 < Dist2 && Dist1 < Dist3)
					{
						// Closest to point 1 so elimitate this
						nIndx1 = i;
					}
					else if (Dist2 < Dist3)
					{
						// Closest to point 2 so elimitate this
						nIndx2 = i;
					}
					else
					{
						// Closest to point 3 so elimitate this
						nIndx3 = i;
					}
					Circle.SetCircumscribed(  (*GetAt(nIndx1)), (*GetAt(nIndx2)), (*GetAt(nIndx3))  );
				}
			}
		}
	}

}


/**--------------------------------------------------------------------------<BR>
C2DPointSet::GetExtremePoints<BR>
\brief Returns the indexes of the 2 points which are furthest from each other.
<P>---------------------------------------------------------------------------*/
void C2DPointSet::GetExtremePoints(unsigned int& nIndx1, unsigned int& nIndx2, 
		double& dDist) const
{
	// First take a guess at them.
	GetExtremePointsEst(nIndx1, nIndx2, dDist, 0);

	// Set up a circle to bound the 2 guesses.
	C2DVector Vec((*GetAt(nIndx1)), (*GetAt(nIndx2)));
	Vec *= 0.5;
	C2DCircle Circle( *GetAt(nIndx1) + Vec, dDist / 2);

	// Now, if the guess was wrong, there must be a point outside the circle which is part of
	// the right solution. Go through all these, check and reset the result each time.
	for (unsigned int i = 0 ; i < size(); i++)
	{
		if ( i != nIndx1 && i != nIndx2)
		{
			if ( !Circle.Contains( *GetAt(i) ) )
			{
				double dDistCheck;
				unsigned int nCheck1 = GetFurthestPoint(i,  dDistCheck);
				if (dDistCheck > dDist)
				{
					nIndx1 = i;
					nIndx2 = nCheck1;
					dDist = dDistCheck;
				}				
			}
		}
	}
}


/**--------------------------------------------------------------------------<BR>
C2DPointSet::GetExtremePointsEst<BR>
\brief Returns an estimate of the indexes of the 2 points which are furthest 
from each other. Takes an initial guess for one index which defaults to 0. Basically
converges to a usually accurate result.
<P>---------------------------------------------------------------------------*/
void C2DPointSet::GetExtremePointsEst(unsigned int& nIndx1, unsigned int& nIndx2, 
									 double& dDist, unsigned int nStartEst) const
{
	if (size() < 3)
	{
		if (size() == 2)
		{
			nIndx1 = 0;
			nIndx2 = 1;
		}
		else if (size() == 1)
		{
			nIndx1 = 0;
			nIndx2 = 0;
		}
		else
		{
			nIndx1 = 0;
			nIndx2 = 0;
			assert(0);
		}

		return;
	}

	// Index 1 is the provided starting guess (default to 0).
	nIndx1 = nStartEst;
	// Index 2 is the furthest point from this.
	nIndx2 = GetFurthestPoint(nIndx1, dDist);

	unsigned int nIndx3 = ~(unsigned int)0;

	while (1)
	{
		nIndx3 = GetFurthestPoint(nIndx2, dDist);
		if (nIndx3 == nIndx1)
		{
			return;
		}
		else
		{
			nIndx1 = nIndx2;
			nIndx2 = nIndx3;
		}
	}
}


/**--------------------------------------------------------------------------<BR>
C2DPointSet::GetFurthestPoint<BR>
\brief Returns the index of the furthest point from the point specified by the 
index given.
<P>---------------------------------------------------------------------------*/
unsigned int C2DPointSet::GetFurthestPoint(unsigned int nIndex, double& dDist) const
{
	if (size() < 2 || nIndex >= size())
	{
		assert(0);
		return 0;
	}
	
	unsigned int usResult;

	if (nIndex == 0)
	{
		dDist = GetAt(1)->Distance(*GetAt(nIndex));
		usResult = 1;
	}
	else
	{
		dDist = GetAt(0)->Distance(*GetAt(nIndex));
		usResult = 0;
	}

	for (unsigned int i = 1 ; i < size(); i++)
	{
		if (i != nIndex)
		{
			double dD = GetAt(i)->Distance(*GetAt(nIndex));
			if (dD > dDist)
			{
				dDist = dD;
				usResult = i;
			}
		}
	}

	return usResult;
}


/**--------------------------------------------------------------------------<BR>
C2DPointSet::RemoveRepeatedPoints<BR>
\brief Removes repeated points from the set.
<P>---------------------------------------------------------------------------*/
void C2DPointSet::RemoveRepeatedPoints(void)
{
	if (size() < 2)
		return;

	unsigned int i = 0;
	while (i < size())
	{
		unsigned int r = i + 1;
		while (r < size())
		{
			if ( (*GetAt(i)) == (*GetAt(r)))
			{
				DeleteAt(r);
			}
			else
			{
				r++;
			}
		}
		i++;
	}
}

/**--------------------------------------------------------------------------<BR>
C2DPointSet::SortByAngleToRight<BR>
\brief Sorts the points by the angle to the right of the line i.e. the angle 
from the lines vector and the vector defined by the lines start point and 
each other.
<P>---------------------------------------------------------------------------*/
void C2DPointSet::SortByAngleToRight( const C2DLine& Line)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);


	std::vector<double> Angs;

	for (unsigned int i = 0 ; i < size(); i++)
	{
		C2DVector vec(Line.point, *this->GetAt(i));
		Angs.push_back( Line.vector.AngleToRight( vec ));
	}

	GeoSort::PQuickSort< std::vector<double>, double, C2DBaseData, C2DBase*>(Angs, Data);
}


/**--------------------------------------------------------------------------<BR>
C2DPointSet::SortByAngleToRight<BR>
\brief Sorts the points by the angle to the left of the line i.e. the angle 
from the lines vector and the vector defined by the lines start point and 
each other.
<P>---------------------------------------------------------------------------*/
void C2DPointSet::SortByAngleToLeft( const C2DLine& Line)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);

	std::vector<double> Angs;

	for (unsigned int i = 0 ; i < size(); i++)
	{
		C2DVector vec(Line.point, *this->GetAt(i));
		Angs.push_back( Line.vector.AngleToLeft( vec ));
	}

	GeoSort::PQuickSort< std::vector<double>, double, C2DBaseData, C2DBase*>(Angs, Data);

}

/**--------------------------------------------------------------------------<BR>
struct sPointIndex
\brief Structure to hold a point reference and its corresponding index.
<P>---------------------------------------------------------------------------*/
struct sPointIndex
{
	const C2DPoint* pt;
	unsigned int usIndex;
};

class PointIndexSet : public vector<sPointIndex*>
{
};


/**--------------------------------------------------------------------------<BR>
C2DPointSet::GetClosestPair<BR>
\brief Private function to help finding closest pair.
<P>---------------------------------------------------------------------------*/
double C2DPointSet::GetClosestPair(PointIndexSet& Pts, unsigned int& nIndex1, unsigned int& nIndex2)
{
	if (Pts.size() > 2)
	{
		PointIndexSet Pts1;
		PointIndexSet Pts2;
		
		unsigned int nHalfWay = Pts.size() / 2;
		double dHalfWay = Pts[ nHalfWay ]->pt->x;
		
		for ( unsigned int i = 0; i <= nHalfWay ; i++)
		{
			Pts1.push_back( Pts[i] );
		}
		for ( unsigned int i = nHalfWay; i < Pts.size() ; i++)
		{
			Pts2.push_back( Pts[i] );
		}
		unsigned int n1_1;
		unsigned int n1_2;	
		unsigned int n2_1;
		unsigned int n2_2;

		double d1 = C2DPointSet::GetClosestPair(Pts1, n1_1, n1_2);
		double d2 = C2DPointSet::GetClosestPair(Pts2, n2_1, n2_2);

		double dMinDist;
		if (d1 < d2)
		{
			dMinDist = d1;
			nIndex1 = n1_1;
			nIndex2 = n1_2;
		}
		else
		{
			dMinDist = d2;
			nIndex1 = n2_1 + nHalfWay;
			nIndex2 = n2_2 + nHalfWay;
		}

		double dMin = dHalfWay - dMinDist;
		double dMax = dHalfWay + dMinDist;
		unsigned int i = Pts1.size() - 2; // don't start on last as that was included in the other set
		while ( i >= 0 && Pts1[i]->pt->x > dMin)
		{
			unsigned int j = 1;	// don't start on last as that was included in the other set
			while ( j < Pts2.size() && Pts2[j]->pt->x < dMax)
			{
                if ( abs( Pts2[j]->pt->y - Pts1[i]->pt->y ) < dMinDist )
				{
					double dDist = Pts2[j]->pt->Distance( *Pts1[i]->pt );
					if (dDist < dMinDist)
					{
						dMinDist = dDist;
						dMin = dHalfWay - dMinDist;
						dMax = dHalfWay + dMinDist;
						nIndex1 = i;
						nIndex2 = j + nHalfWay;
					}
				}
				j++;
			}
			i--;
		}
		return dMinDist;

	}
	else if (Pts.size() == 2)
	{
		nIndex1 = 0;
		nIndex2 = 1;
		return Pts[0]->pt->Distance( *Pts[1]->pt );
	}
	else
	{
		assert(false);
		return 0;
	}
}

/**--------------------------------------------------------------------------<BR>
C2DPointSet::GetClosestPair<BR>
\brief Gets the closest pair of points.
<P>---------------------------------------------------------------------------*/
double C2DPointSet::GetClosestPair(unsigned int& nIndex1, unsigned int& nIndex2) const
{
	PointIndexSet Pts;
	std::vector<double> xValues;

	for (unsigned int i = 0 ; i < size() ; i++)
	{
		sPointIndex* pPointIndex = new sPointIndex;
		pPointIndex->pt = GetAt(i);
		pPointIndex->usIndex = i;
		Pts.push_back( pPointIndex );
		xValues.push_back(pPointIndex->pt->x);
	}

	// Sort them all according to the left most point of the line rects.
	GeoSort::PQuickSort< std::vector<double>, double, std::vector<sPointIndex*>, sPointIndex*>( xValues, Pts);

	double dResult = C2DPointSet::GetClosestPair(Pts, nIndex1, nIndex2);

	for (unsigned int i = 0 ; i < Pts.size() ; i++)
	{
		delete Pts[i];
	}

	return dResult;

}
/**--------------------------------------------------------------------------<BR>
C2DPointSet::SortLeftToRight<BR>
\brief Sorts left to right.
<P>---------------------------------------------------------------------------*/
void C2DPointSet::SortLeftToRight(void)
{
	C2DBaseData& Data = *reinterpret_cast<C2DBaseData*>(m_Data);

	std::vector<double> dLefts;

	for (unsigned int i = 0 ; i < size(); i++)
	{
		dLefts.push_back( this->GetAt(i)->x);
	}

	GeoSort::PQuickSort<std::vector<double>, double, C2DBaseData, C2DBase*>(dLefts, Data);

}
