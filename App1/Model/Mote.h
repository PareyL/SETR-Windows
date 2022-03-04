#pragma once
#include <string>

class Mote
{
private :
	int id;
	float latitude, longitude;
	std::string ipv6;
	std::string mac;

public :
	Mote(int id, float latitude, float longitude, std::string i1, std::string i2);

	float temperature;
	float ligth;
	float humidity;
	float battery;
	double distance;

	int getId();
	std::string getIpv6();
	std::string getMac();
	float getLatitude();
	float getLongitude();

	void operator=(const Mote& mote);

	static void parseMote(Platform::String^ json, Mote* motes);
};

