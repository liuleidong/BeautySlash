/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------<BR>
\file StdAfx.h
\brief File used for all commonly used includes within the library.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_STDAFX_H 
#define _GEOLIB_STDAFX_H


#ifndef WINVER				
#define WINVER 0x0400		// 0x0400 = Windows NT 4.0.   0x0501 = XP
#endif

#include <assert.h>
#include <math.h>
#include <vector>
#include <QtGlobal>
#include "Constants.h"

using namespace std;

#ifdef __BORLANDC__
	#define sprintf_s snprintf  // borland does not support sprintf_s
#endif

class C2DBase;
typedef std::vector<C2DBase*> C2DBaseData;

#endif
