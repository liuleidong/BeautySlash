/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file RandomNumber.cpp
\brief Implementation file for the CRandomNumber Class.

Implementation file for CRandomNumber, a class which provides a simple mechanism
for generating random numbers.
<P>---------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "RandomNumber.h"

_MEMORY_POOL_IMPLEMENATION(CRandomNumber)

/**--------------------------------------------------------------------------<BR>
CRandomNumber::CRandomNumber
\brief Constructor.
<P>---------------------------------------------------------------------------*/
CRandomNumber::CRandomNumber(void)
{
	m_dMin = 0;
	m_dMax = 1;
}

/**--------------------------------------------------------------------------<BR>
CRandomNumber::~CRandomNumber
\brief Destructor.
<P>---------------------------------------------------------------------------*/
CRandomNumber::~CRandomNumber(void)
{
}

/**--------------------------------------------------------------------------<BR>
CRandomNumber::CRandomNumber
\brief Constructor, initialises to 2 double forming the bounds
<P>---------------------------------------------------------------------------*/
CRandomNumber::CRandomNumber(double dMin, double dMax)
{
	m_dMin = dMin;
	m_dMax = dMax;
}


/**--------------------------------------------------------------------------<BR>
CRandomNumber::Set
\brief Sets the bounds
<P>---------------------------------------------------------------------------*/
void CRandomNumber::Set(double dMin, double dMax)
{
	m_dMin = dMin;
	m_dMax = dMax;
}


/**--------------------------------------------------------------------------<BR>
CRandomNumber::Get
\brief Gets a random number between the bounds.
<P>---------------------------------------------------------------------------*/
double CRandomNumber::Get(void) const
{
	double dResult = m_dMin + (m_dMax - m_dMin) * CRandomNumber::GetFraction();
	return dResult;
}

/**--------------------------------------------------------------------------<BR>
CRandomNumber::GetInt
\brief Gets an integer based on the settings. Sets up temporary new boundaries so that an interval
of e.g. 0.8 to 3.7 will become 1.0 to 3.0 allowing integers 1 and 2 only.
<P>---------------------------------------------------------------------------*/
int CRandomNumber::GetInt(void) const
{
    CRandomNumber Num(ceil(m_dMin), floor(m_dMax) + 1.0);
	double dRes = Num.Get();
	if (dRes == (int)Num.GetMax())
		return (int) (dRes - 1);
	else 
		return (int) dRes;
}

/**--------------------------------------------------------------------------<BR>
CRandomNumber::GetFraction
\brief Gets a random number between 0 and 1 inclusive.
<P>---------------------------------------------------------------------------*/
double CRandomNumber::GetFraction(void)
{
	double dResult = (double) rand() / (double) ( RAND_MAX);
	return dResult;
}


/**--------------------------------------------------------------------------<BR>
CRandomNumber::GetBool
\brief Gets a boolean value.
<P>---------------------------------------------------------------------------*/
bool CRandomNumber::GetBool(void)
{
	return (CRandomNumber::GetFraction() > 0.5);
}
