/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file Grid.h
\brief Declaration file for the CGrid class. 

\class CGrid
\brief Class which is used to hold information about the grid. 

The grid is simply a spacing between allowable points. When objects are "Snapped"
to the grid, all points must then lie on the grid lines. This is used within 
GeoLib to manage degenerate cases but has other applications. Also used to record
degenerate errors. All functions are static. 
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_CGRID_H 
#define _GEOLIB_CGRID_H

class C2DRect;

#ifdef _EXPORTING
	#define CLASS_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define CLASS_DECLSPEC
	#else
		#define CLASS_DECLSPEC		__declspec(dllimport)
	#endif
#endif

class CLASS_DECLSPEC CGrid
{
public:
	/// Enumeration for degenerate handling methods.
	enum eDegenerateHandling
	{
		None,
		RandomPerturbation,
		DynamicGrid,
		PreDefinedGrid,
		PreDefinedGridPreSnapped,
	};
	/// Constructor
	CGrid(void) {;}
	/// Destructor
	~CGrid(void) {;}
	/// Sets the size of the grid.
	static void SetGridSize(double dGridSize);
	/// Gets the grid size.
	static double GetGridSize(void);
	/// Finds a recommended minimum grid size to avoid point equality problems.
	static double GetMinGridSize(const C2DRect& cRect, 
		bool bRoundToNearestDecimalFactor = false);
	/// Sets to the minimum recommended size for degenerate handling.
	static void SetToMinGridSize(const C2DRect& cRect, 
		bool bRoundToNearestDecimalFactor = false);
	/// Resets the degenerate count.
	static void ResetDegenerateErrors(void);
	/// Gets the degenerate errors.
	static unsigned int GetDegenerateErrors(void);
	/// Used to log a degenerate error.
	static void LogDegenerateError(void);

};

#endif