//
// MainPage.xaml.cpp
// Implémentation de la classe MainPage.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <thread>
#include <shared_mutex>
#include <GPS.h>

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
TextBox ^Lat, ^Long;
Platform::String^ strLat;
Platform::String^ strLng;
bool GPSHaveValue, printedGPSValue = false;

static UINT Inc() {
	Sleep(1);
	while (true) {
		VerInc.lock();
		Cpt++;
	}
	return 0;
}

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
	GPS().setGPS();
	Cpt = rand();
	DispatcherTimer^ timer = ref new DispatcherTimer; 
	TimeSpan ts;
	ts.Duration = 500000;
	timer->Start();
	timer->Tick += ref new EventHandler<Object^>(this, &MainPage::OnTick);

	thread Th_Inc(Inc);
	Th_Inc.detach();

	thread Th_Dec(Dec);
	Th_Dec.detach();
}

void MainPage::AsGPSValue() {
	GPSHaveValue = true;
}

Platform::String^ strPos(const wchar_t* str, int start, int end) {
	Platform::String^ finalStr = "";
	for (int i = start; i < end; i++)
		finalStr += str[i].ToString();
	return finalStr;
}

void App1::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	VerInc.unlock();
}

void App1::MainPage::Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	VerDec.unlock();
}

void App1::MainPage::OnTick(Platform::Object^ sender, Platform::Object^ e)
{
	Affich->Text = Cpt.ToString();
	if (GPSHaveValue && !printedGPSValue) {
		stringGPS = GPS().getGPS();
		if (stringGPS->Length() > 0) {
			const wchar_t* charStrGPS = stringGPS->ToString()->Begin();
			Platform::String^ strLat = strPos(charStrGPS, 0, 7);
			Platform::String^ strLng = strPos(charStrGPS, 9, 16);
			Lat->Text = strLat;
			Long->Text = strLng;
			printedGPSValue = true;
		}
	}
}

void App1::MainPage::GPS_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	stringGPS = GPS().getGPS();
	if (stringGPS->Length() > 0) {
		const wchar_t* charStrGPS = stringGPS->ToString()->Begin();
		Platform::String^ strLat = strPos(charStrGPS, 0, 7);
		Platform::String^ strLng = strPos(charStrGPS, 9, 16);
		Lat->Text = strLat;
		Long->Text = strLng;
	}
}
