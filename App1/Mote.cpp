#include "pch.h"
#include "Mote.h"
#include <windows.data.json.h>
using namespace std;
using namespace Windows::Data::Json;



Mote::Mote(string mote, float temperature, float ligth, float humidity, float battery) {
    this->humidity = humidity;
    this->temperature = temperature;
    this->ligth = ligth;
    this->mote = mote;
    this->battery = battery;
}

string converteString(Platform::String^ fooRT) {
    std::wstring fooW(fooRT->Begin());
    std::string fooA(fooW.begin(), fooW.end());
    return fooA.c_str();
}

void parseMote(Platform::String^ json, Mote* motes) {


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


