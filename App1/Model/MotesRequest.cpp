#include "pch.h"
#include "Model/MotesRequest.h"
#include "Model/Mote.h"
#include "conversionsMaison.h"
#include <iostream>
#include <string>
#include "VariablesGlobales.h"

using namespace Windows::Foundation;
using namespace Windows::Web::Http;
using namespace Concurrency;
using namespace Windows::Data::Json;
using namespace std;

MotesRequest::MotesRequest()
{
	
}

void MotesRequest::getAllMotes()
{
	//Construction de l'URL
	Platform::String^ requestUrl = urlIOTLab + "rest/info/motes";

	HttpClient^ httpClient = ref new HttpClient();
	Uri^ uri = ref new Uri(requestUrl);

	auto test = create_task(httpClient->GetStringAsync(uri))
		.then([&](task<Platform::String^> httpContent$) -> void {
		try {
			Platform::String^ httpContent = httpContent$.get();
			auto objJson = JsonObject::Parse(httpContent);
			JsonArray^ data = objJson->GetObject()->GetNamedArray("sender")->GetArray();
			int jsonArraySize = data->Size;

			VariablesGlobales::vectorMotes.clear();

			for (int j = 0; j < jsonArraySize; j++) {

				int id = (int)data->GetObjectAt(j)->GetNamedNumber("id");

				string ipv6 = ConversionsMaison::conversionPlatformStringToString(data->GetObjectAt(j)->GetNamedString("ipv6")->ToString());
				string mac = ConversionsMaison::conversionPlatformStringToString(data->GetObjectAt(j)->GetNamedString("mac")->ToString());
				
				double longitude = data->GetObjectAt(j)->GetNamedNumber("lon");
				double latitude = data->GetObjectAt(j)->GetNamedNumber("lat");

				Mote temp = Mote(id, latitude, longitude, ipv6, mac);

				VariablesGlobales::vectorMotes.push_back(temp);
			}
			
		}
		catch (Platform::COMException^ e) {
			OutputDebugString(e->Message->Data());
		}
		VariablesGlobales::VerrouMotes.unlock();
	});
}

void MotesRequest::updateMote(int idMote)
{
	VariablesGlobales::indiceMote = 0;

	for (VariablesGlobales::indiceMote; VariablesGlobales::indiceMote < VariablesGlobales::vectorMotes.size(); VariablesGlobales::indiceMote++)
	{
		if (VariablesGlobales::vectorMotes[VariablesGlobales::indiceMote].getId() == idMote)
			break;
	}

	Platform::String^ moteId = ConversionsMaison::conversionStringToPlatformString(VariablesGlobales::vectorMotes[VariablesGlobales::indiceMote].getIpv6());

	//Construction de l'URL
	Platform::String^ requestUrl = urlIOTLab + "rest/data/1/temperature-light2-light1-battery_indicator-humidity/1/" + moteId;

	HttpClient^ httpClient = ref new HttpClient();
	Uri^ uri = ref new Uri(requestUrl);

	auto test = create_task(httpClient->GetStringAsync(uri))
		.then([&](task<Platform::String^> httpContent$) -> void {
			try {
				Platform::String^ httpContent = httpContent$.get();
				Mote::parseMote(httpContent, &(VariablesGlobales::vectorMotes[VariablesGlobales::indiceMote]));
			}
			catch (Platform::COMException^ e) {
				OutputDebugString(e->Message->Data());
			}
			VariablesGlobales::VerrouMotes.unlock();
		});
}
