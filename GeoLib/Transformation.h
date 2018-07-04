/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------<BR>
\file Transformation.h
\brief Header file for a CTransformation class.

\class CTransformation
\brief Abstract class representing a transformation of some sort.
The purpose of this class is to allow transformations to be defined outside the 
GeoLib main geometry library which can still be performed on GeoLib
shapes. For example a polygon will be capable of taking an object derived
from this and calling transform on all its points.
<P>---------------------------------------------------------------------------*/


#pragma once

//class CGeoLatLong;

#ifdef _EXPORTING
	#define CLASS_DECLSPEC		__declspec(dllexport)
#else 
	#ifdef _STATIC
		#define CLASS_DECLSPEC
	#else
		#define CLASS_DECLSPEC		__declspec(dllimport)
	#endif
#endif



class CLASS_DECLSPEC CTransformation
{
public:
	/// Destructor.
	virtual ~CTransformation(void) {};
	/// Transform the given point.
	virtual void Transform(double& dx, double& dy) const = 0;
	/// Inverse transform the given point.
	virtual void InverseTransform(double& dx, double& dy) const = 0;

};
