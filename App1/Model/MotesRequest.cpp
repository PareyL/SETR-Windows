#include "pch.h"
#include "Model/MotesRequest.h"
#include "Model/Mote.h"
#include "conversionsMaison.h"
#include <iostream>
#include <string>

using namespace Windows::Foundation;
using namespace Windows::Web::Http;
using namespace Concurrency;
using namespace Windows::Data::Json;
using namespace std;

std::vector<Mote> vectorMotes = std::vector<Mote>();

MotesRequest::MotesRequest()
{
	
}

void MotesRequest::getAllMotes()
{
	vectorMotes.clear();

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

			for (int j = 0; j < jsonArraySize; j++) {

				int id = (int)data->GetObjectAt(j)->GetNamedNumber("id");

				string ipv6 = ConversionsMaison::conversionPlatformStringToString(data->GetObjectAt(j)->GetNamedString("ipv6")->ToString());
				string mac = ConversionsMaison::conversionPlatformStringToString(data->GetObjectAt(j)->GetNamedString("mac")->ToString());
				
				double longitude = data->GetObjectAt(j)->GetNamedNumber("lon");
				double latitude = data->GetObjectAt(j)->GetNamedNumber("lat");

				Mote temp = Mote(id, latitude, longitude, ipv6, mac);

				vectorMotes.push_back(temp);
			}
			
		}
		catch (Platform::COMException^ e) {
			OutputDebugString(e->Message->Data());
		}

		//OutputDebugString(tabMotes[0].getId().ToString()->Data());
		
	});





}
