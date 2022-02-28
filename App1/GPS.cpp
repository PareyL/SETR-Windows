#include "pch.h"
#include "GPS.h"
#include "MainPage.xaml.h"
#include <string>
#include <thread>
#include <shared_mutex>
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
Platform::String^ GPS::getGPS() {
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
			});		
		}	
	return coordStr;
}
