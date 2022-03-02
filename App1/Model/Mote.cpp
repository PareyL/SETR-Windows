#include "pch.h"
#include "Model/Mote.h"

using namespace Windows::Data::Json;

Mote::Mote(int id, float latitude, float longitude, std::string i1, std::string i2) :
    id(id),
    latitude(latitude),
    longitude(longitude),
    ipv6(i1),
    mac(i2),
    temperature(0.0),
    ligth(0.0),
    humidity(0.0),
    battery(0.0)
{}

int Mote::getId()
{
	return id;
}

std::string Mote::getIpv6()
{
	return ipv6;
}

std::string Mote::getMac()
{
	return mac;
}

float Mote::getLatitude()
{
	return latitude;
}

float Mote::getLongitude()
{
	return longitude;
}

void Mote::operator=(const Mote& mote)
{
	this->id = mote.id;
	this->latitude = mote.latitude;
	this->longitude = mote.longitude;
	this->ipv6 = mote.ipv6;
	this->mac = mote.mac;
}

void Mote::parseMote(Platform::String^ json, Mote* motes) {

    auto objJson = JsonObject::Parse(json);
    JsonArray^ data = objJson->GetObject()->GetNamedArray("data")->GetArray();
    int jsonArraySize = data->Size;
    for (int j = 0; j < jsonArraySize; j++) {
        if (data->GetObjectAt(j)->GetNamedString("label")->Equals("temperature")) {
            motes->temperature = _wtof(data->GetObjectAt(j)->GetNamedValue("value")->ToString()->Data());
        }
        if (data->GetObjectAt(j)->GetNamedString("label")->Equals("battery_indicator")) {
            motes->battery = _wtof(data->GetObjectAt(j)->GetNamedValue("value")->ToString()->Data());
        }
        if (data->GetObjectAt(j)->GetNamedString("label")->Equals("light1")) {
            motes->ligth = _wtof(data->GetObjectAt(j)->GetNamedValue("value")->ToString()->Data());
        }
        if (data->GetObjectAt(j)->GetNamedString("label")->Equals("humidity")) {
            motes->humidity = _wtof(data->GetObjectAt(j)->GetNamedValue("value")->ToString()->Data());
        }
    }
}
