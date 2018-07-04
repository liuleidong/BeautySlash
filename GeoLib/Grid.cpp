/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file Grid.cpp
\brief Implementation file for the CGrid class.
<P>---------------------------------------------------------------------------*/


#include "StdAfx.h"

#include "Grid.h"
#include "C2DRect.h"


static double ms_dGridSize = 0.0001;
static unsigned int ms_nDegenerateErrors = 0;

const double const_dEqualityAvoidanceFactor = 1000.0;

/**--------------------------------------------------------------------------<BR>
CGeoLatLong::SetGridSize <BR>
\brief Sets the grid size.
<P>---------------------------------------------------------------------------*/
void CGrid::SetGridSize(double dGridSize)
{
	if (dGridSize != 0)
	{
		ms_dGridSize = fabs(dGridSize);
	}
}


/**--------------------------------------------------------------------------<BR>
CGeoLatLong::ResetDegenerateErrors <BR>
\brief Resets the degenerate error log.
<P>---------------------------------------------------------------------------*/
void CGrid::ResetDegenerateErrors(void)
{
	ms_nDegenerateErrors = 0 ;
}


/**--------------------------------------------------------------------------<BR>
CGeoLatLong::GetDegenerateErrors <BR>
\brief Gets the Degenerate Errors.
<P>---------------------------------------------------------------------------*/
unsigned int CGrid::GetDegenerateErrors(void) 
{
	return ms_nDegenerateErrors;
}

/**--------------------------------------------------------------------------<BR>
CGeoLatLong::GetDegenerateErrors <BR>
\brief Logs a Degenerate Errors.
<P>---------------------------------------------------------------------------*/
void CGrid::LogDegenerateError(void) 
{
	ms_nDegenerateErrors ++;
}


/**--------------------------------------------------------------------------<BR>
CGeoLatLong::GetGridSize <BR>
\brief Gets the grid size.
<P>---------------------------------------------------------------------------*/
double CGrid::GetGridSize(void)
{
	return ms_dGridSize;
}

/**--------------------------------------------------------------------------<BR>
CGeoLatLong::GetMinGridSize <BR>
\brief Gets the minimum recommended grid size.
<P>---------------------------------------------------------------------------*/
double CGrid::GetMinGridSize(const C2DRect& cRect, 
							 bool bRoundToNearestDecimalFactor)
{
	// Find the furthest possible linear distance from the origin.
	C2DPoint pt = cRect.GetPointFurthestFromOrigin();

	double dRes = fabs(max(pt.x ,  pt.y));
	// Now multiply this by the eq tol. Now, 2 points which are this far apart from each other
	// (in x and y) and at the edge of the rect would be considered only just not equal.
	dRes *= conEqualityTolerance;
	// Now multiple this by an avoidance factor.
	dRes *= const_dEqualityAvoidanceFactor;
	
	if (dRes == 0)
		dRes = const_dEqualityAvoidanceFactor;

	if (bRoundToNearestDecimalFactor)
	{
		double dRound = 0.0001;

		while(dRound >= dRes)
			dRound /= 10.0;

		while(dRound < dRes)
			dRound *= 10.0;

		dRes = dRound;
	}


	return dRes;
}

/**--------------------------------------------------------------------------<BR>
CGeoLatLong::SetToMinGridSize <BR>
\brief Sets to the minimum recommended grid size.
<P>---------------------------------------------------------------------------*/
void CGrid::SetToMinGridSize(const C2DRect& cRect,
							 bool bRoundToNearestDecimalFactor)
{
	CGrid::SetGridSize( GetMinGridSize(cRect, bRoundToNearestDecimalFactor));

}