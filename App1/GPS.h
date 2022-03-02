#pragma once
using namespace Windows::Devices::Geolocation;
class GPS
{
public:
	Platform::String^ GPS::getGPS();
	void GPS::setGPS();
	//void GPS::GetCloserMote(double lat, double lng, vector<Mote> motes);
	double GPS::deg2rad(double deg);
};

