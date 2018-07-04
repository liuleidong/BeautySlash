/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file RandomNumber.h
\brief Declaration file for the CRandomNumber Class.

Declaration file for CRandomNumber, a class which provides a simple mechanism
for generating random numbers.

\class CRandomNumber
\brief A class which provides a simple mechanism for generating random numbers.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_CRANDOMNUMBER_H 
#define _GEOLIB_CRANDOMNUMBER_H


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

class CLASS_DECLSPEC CRandomNumber
{
public:
	_MEMORY_POOL_DECLARATION

	/// Constructor
	CRandomNumber(void);
	/// Constructor, initialises to 2 double forming the bounds
	CRandomNumber(double dMin, double dMax);
	/// Destructor.
	~CRandomNumber(void);
	/// Sets the random number bound to 2 doubles
	void Set(double dMin, double dMax);
	/// Sets the max
	void SetMax(double dMax) {m_dMax = dMax;}
	/// Sets the min
	void SetMin(double dMin) {m_dMin = dMin;}
	/// Access to the min
	double GetMin(void) const {return m_dMin;}
	/// Access to the max
	double GetMax(void)  const {return m_dMax;}
	/// Gets a random number based on the settings
	double Get(void) const;
	/// Gets an integer based on the settings. Sets up temporary new boundaries so that an interval
	/// of e.g. 0.8 to 3.7 will become 1.0 to 3.0 allowing integers 1 and 2 only.
	int GetInt(void) const;
	/// Returns a random number from 0 to 1 inclusive.
	static double GetFraction(void);
	/// Returns true or false;
	static bool GetBool(void);

private:
	/// The minimum possible value
	double m_dMin;
	/// The maximum possible value
	double m_dMax;
};

#endif