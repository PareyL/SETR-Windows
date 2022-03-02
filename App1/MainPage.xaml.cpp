//
// MainPage.xaml.cpp
// Implémentation de la classe MainPage.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <thread>
#include <shared_mutex>
#include <GPS.h>
#include <Model\Mote.h>
#include "Model/MotesRequest.h"

using namespace App1;

using namespace Platform;
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

shared_mutex VerInc, VerDec;
INT Cpt;
Platform::String^ stringGPS;
Platform::String^ strLat; Platform::String^ oldStrLat;
Platform::String^ strLng; Platform::String^ oldStrLng;
bool GPSHaveValue, firstPrint = false;

// Incrémentation avec Vérou
static UINT Inc() {
	Sleep(1);
	while (true) {
		VerInc.lock();
		Cpt++;
	}
	return 0;
}

// Décrémentation avec Vérou
static UINT Dec() {
	Sleep(1);
	while (true) {
		VerDec.lock();
		Cpt--;
	}
	return 0;
}

// Pour plus d'informations sur le modèle d'élément Page vierge, consultez la page https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
	GPS().setGPS(); // active le GPS
	Cpt = rand();
	DispatcherTimer^ timer = ref new DispatcherTimer; 
	TimeSpan ts;
	ts.Duration = 10000000;
	timer->Interval = ts;
	timer->Start();
	timer->Tick += ref new EventHandler<Object^>(this, &MainPage::OnTick);

	thread Th_Inc(Inc);
	Th_Inc.detach();

	thread Th_Dec(Dec);
	Th_Dec.detach();

	//Motes Request
	MotesRequest test;
	test.getAllMotes();
}

// Découpage d'une Plateform::String, permet de récupérer la latitude et longitude
Platform::String^ strPos(const wchar_t* str, int start, int end) {
	Platform::String^ finalStr = "";
	for (int i = start; i < end; i++)
		finalStr += str[i].ToString();
	return finalStr;
}

// Permet d'activer l'affichage quand on a des valeurs GPS
void MainPage::HasGPSValue() {
	GPSHaveValue = true;
}

// Permet d'afficher les coordonnées
void PrintGPS(TextBox^ textboxA, TextBox^ textboxB) {
	if (stringGPS->Length() > 0) {
		if (oldStrLat != "" && oldStrLng != "") {
			if (strLat != oldStrLat || strLng != oldStrLng) {
				OutputDebugStringA("Change location\n");
				const wchar_t* charStrGPS = stringGPS->ToString()->Begin();
				strLat = strPos(charStrGPS, 0, 7);
				strLng = strPos(charStrGPS, 10, 17);
				textboxA->Text = strLat;
				textboxB->Text = strLng;
			}
		}
		else {
			OutputDebugStringA("First Print\n");
			const wchar_t* charStrGPS = stringGPS->ToString()->Begin();
			strLat = strPos(charStrGPS, 0, 7);
			strLng = strPos(charStrGPS, 10, 17);
			textboxA->Text = strLat;
			textboxB->Text = strLng;
			}
		oldStrLat = strLat;
		oldStrLng = strLng;
	}
}

Platform::String^ convertFromString(const std::string& input)
{
	std::wstring w_str = std::wstring(input.begin(), input.end());
	const wchar_t* w_chars = w_str.c_str();

	return (ref new Platform::String(w_chars));
}

void App1::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	VerInc.unlock();
}

void App1::MainPage::Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    /*OutputDebugStringA("lest go\n");
	VerDec.unlock();
    
	string sstr = "{\"data\":[{\"timestamp\":1645717380000,\"label\":\"light1\",\"value\":305.71,\"mote\":\"9.138\"},{\"timestamp\":1645759199000,\"label\":\"light1\",\"value\":164.29,\"mote\":\"111.130\"},{\"timestamp\":1645816193000,\"label\":\"light1\",\"value\":188.57,\"mote\":\"53.105\"},{\"timestamp\":1645717380000,\"label\":\"temperature\",\"value\":88.36,\"mote\":\"9.138\"},{\"timestamp\":1645759199000,\"label\":\"temperature\",\"value\":-18.85,\"mote\":\"111.130\"},{\"timestamp\":1645816193000,\"label\":\"temperature\",\"value\":-30.94,\"mote\":\"53.105\"},{\"timestamp\":1645717380000,\"label\":\"humidity\",\"value\":1.43,\"mote\":\"9.138\"},{\"timestamp\":1645759199000,\"label\":\"humidity\",\"value\":-4.0,\"mote\":\"111.130\"},{\"timestamp\":1645816193000,\"label\":\"humidity\",\"value\":-4.0,\"mote\":\"53.105\"}]}";
	Platform::String^ myStr = convertFromString(sstr);
	Mote* mote = vectorM
	parseMote(myStr,mote);*/
}

void App1::MainPage::OnTick(Platform::Object^ sender, Platform::Object^ e)
{
	Affich->Text = Cpt.ToString();
	if (GPSHaveValue) {
		stringGPS = GPS().getGPS(); // On récupère les coordonnées une fois qu'on a des données
		PrintGPS(Lat, Long);
	}
}

void App1::MainPage::GPS_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	stringGPS = GPS().getGPS();
}
