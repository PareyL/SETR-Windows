#pragma once
#include <string>
#include <map>

using namespace Platform;
using namespace Windows::Data::Json;
using namespace std;

class Mote
{
private :

public:
    //Create a static map key is String and value is Mote*
    //static std::map<Platform::String^, Mote*> map;
    float temperature;
    float ligth;
    float humidity;
    float battery;
    std::string mote;
    Mote(string mote ,float temperature,float ligth,float humidity, float battery);
};

//string converteString(Platform::String^ fooRT);
void parseMote(Platform::String^ json,Mote* motes);
