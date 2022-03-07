#include "pch.h"
#include "GPS.h"
#include "MainPage.xaml.h"
#include "VariablesGlobales.h"
#include <string>
#include <thread>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <math.h>
#include <cmath>
#include <sstream>
#include <iterator>
#include <locale> 
#include <codecvt>
#include <stdio.h>
#include <array>


using namespace App1;

using namespace Platform;
using namespace Concurrency;
using namespace Windows::Devices::Geolocation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

using namespace std;

TimeSpan period;
Platform::String^ error = "";
char* test;
Platform::String^ coordStr = "";
bool statusOK = false;
double earthRadiusKm = 6371.0;
double M_PI = 2 * acos(0.0);
int closer;

// Permet de mettre en place le GPS et derécupérer les droits
void GPS::setGPS() {
	Geolocator^ geolocator = ref new Geolocator;
	create_task(geolocator->RequestAccessAsync()).then([&](task<GeolocationAccessStatus> geo$) -> void { // Vérifie les droits GPS
		if (geo$.get() == GeolocationAccessStatus::Allowed) {
			statusOK = true;
		}
		else if (geo$.get() == GeolocationAccessStatus::Denied) {
			OutputDebugStringA("Denied");
			// Permet d'ouvrir les droits GPS
			auto uri = ref new Uri("ms-settings:privacy-location");
			auto success = create_task(Windows::System::Launcher::LaunchUriAsync(uri));
		}
		});
}

// Récupère les Coordonnées GPS
void GPS::getGPS() {
	Geolocator^ geolocator = ref new Geolocator;

	if(statusOK) {
		period.Duration = 30 * 10000000;
		Geolocator^ geolocator = ref new Geolocator;		
		auto positionToken = create_task(geolocator->GetGeopositionAsync(period, period))
			.then([&](task<Geoposition^> position$) -> void {
				auto position = position$.get();
				auto coordinate = position->Coordinate;
				OutputDebugStringA("Position: ");
				OutputDebugString(coordinate->Latitude.ToString()->Data());
				OutputDebugStringA("  |  ");
				OutputDebugString(coordinate->Longitude.ToString()->Data());
				OutputDebugStringA("\n");
				coordStr = coordinate->Latitude.ToString() + " / " + coordinate->Longitude.ToString();
				if (coordStr->Length() > 0)
					MainPage::HasGPSValue();

				VariablesGlobales::coordonneesGPS = coordStr;
				VariablesGlobales::VerrouGPS.unlock();
			});		
		}
}

double GPS::deg2rad(double deg) {
	return (deg * M_PI / 180);
}

int GPS::GetCloserMote(float lat, float lng)
{
	VariablesGlobales::VerrouMotes.lock();

	double distance = INFINITE;
	double lat2r = GPS::deg2rad(lat);
	double lon2r = GPS::deg2rad(lng);

	for (unsigned i = 0; i < VariablesGlobales::vectorMotes.size(); i++)
	{
		double lat1r = VariablesGlobales::vectorMotes[i].getLatitude();
		double lon1r = VariablesGlobales::vectorMotes[i].getLongitude();
		double u = sin((lat2r - lat1r) / 2);
		double v = sin((lon2r - lon1r) / 2);
		VariablesGlobales::vectorMotes[i].distance = 2.0 * earthRadiusKm * asin(sqrt(pow(u,2) + cos(lat1r) * cos(lat2r) * pow(v,2)));
		if (VariablesGlobales::vectorMotes[i].distance < distance)
		{
			distance = VariablesGlobales::vectorMotes[i].distance;
			closer = i;
		}
	}
	int id = VariablesGlobales::vectorMotes[closer].getId();

	VariablesGlobales::VerrouMotes.unlock();

	return id;
}

