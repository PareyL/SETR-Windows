#pragma once

#ifndef GPSCLASS
#define GPSCLASS


#include "Model/Mote.h"
#include "Model/MotesRequest.h"

using namespace Windows::Devices::Geolocation;

class GPS
{
public:
	Platform::String^ GPS::getGPS();
	void GPS::setGPS();
	int GPS::GetCloserMote(float lat, float lng);
	double GPS::deg2rad(double deg);
};

#endif // !GPSCLASS