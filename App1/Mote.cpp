#include "pch.h"
#include "Mote.h"
using namespace std;

Mote::Mote(string mote, float temperature, float ligth, float humidity) {
	this->humidity = humidity;
	this->temperature = temperature;
	this->ligth = ligth;
	this->mote = mote;
}
