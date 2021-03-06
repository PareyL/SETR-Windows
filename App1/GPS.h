#pragma once

#ifndef GPSCLASS
#define GPSCLASS


#include "Model/Mote.h"
#include "Model/MotesRequest.h"

using namespace Windows::Devices::Geolocation;

class GPS
{
public:
	void GPS::getGPS();
	void GPS::setGPS();
	static void GPS::GetCloserMote();
	static double GPS::deg2rad(double deg);
};

#endif // !GPSCLASS