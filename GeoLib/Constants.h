/*---------------------------------------------------------------------------
Copyright (C) GeoLib.
This code is used under license from GeoLib (www.geolib.co.uk). This or
any modified versions of this cannot be resold to any other party.
---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------<BR>
\file Constants.h
\brief File containing useful constants and conversion factors.
<P>---------------------------------------------------------------------------*/

#ifndef _GEOLIB_CONSTANTS_H 
#define _GEOLIB_CONSTANTS_H

const double conRadiansPerDegree = 0.017453292519943295769236907684886;

const double conDegreesPerRadian = 57.295779513082320876798154814105;

const double conMetresPerNauticalMile = 1852.0;

const double conEARTH_RADIUS_METRES = 6370999.0; // Volumetric mean

const double conSecondsPerDegree = 3600.0;

const double conMinutesPerDegree = 60.0;

const double conMetersPerFoot = 0.3048;
/// This defined how close 2 doubles need to be to each other in order to be considered
/// Equal. If the difference between the 2 divided by 1 of them is less than this they are
/// equal.
const double conEqualityTolerance = 0.0000000001;
/// Random number perturbation seed.
const double coniPerturbationFactor = 0.0568412;
/// Random number perturbation seed.
const double conjPerturbationFactor = 0.0345687;

const double conPI = 3.14159265358979323846;

const double conTWOPI = 6.283185307179586476925286766559;

const double conSIXTHPI = conPI / 6.0;		// 30 degrees

const double conQUARTPI = conPI / 4.0;		// 30 degrees

const double conTHIRDPI = conPI / 3.0;		// 60 degrees

const double conTWOTHIRDPI = conTWOPI / 3.0;	// 120 degrees

const double conHALFPI = conPI / 2.0;

const double conMetresPerDataMile = 1828.80;

const double conDataMilesPerNauticalMile = 1.0111666666666666666666666666667;

// Earth semi-major axis of ellipsoid in meters
const double conGeocent_Major = 6378137.0;
// Earth semi-minor axis of ellipsoid in meters
const double conGeocent_Minor = 6356752.3142;
// Earth axis mean in meters
const double conGeocent_Mean = 6367444.6571;
// Earth axis eccentricity squared
const double conGeocent_e2 = 0.0066943799901413800;
// Earth axis 2nd eccentricity squared
const double conGeocent_ep2 = 0.00673949675658690300;

const double conLbsPerKilogram = 2.2;

const unsigned long conSecondsPerDay = 86400;

const double conSecondsPerHour = 3600.0;

const double conSecondsPerMinute = 60.0;

const double conE   =     2.71828182845904523536;

const double conRoot2 = 1.4142135623731;

const double conRoot3 = 1.73205080756888;


#endif