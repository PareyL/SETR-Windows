//
// MainPage.xaml.cpp
// Implémentation de la classe MainPage.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <thread>
#include <shared_mutex>
#include "GPS.h"
#include "Model/Mote.h"
#include "Model/MotesRequest.h"
#include "VariablesGlobales.h"

using namespace App1;


using namespace std;

INT Cpt;
Platform::String^ stringGPS;
Platform::String^ strLat; Platform::String^ oldStrLat;
Platform::String^ strLng; Platform::String^ oldStrLng;
bool GPSHaveValue, firstPrint = false;
TextBlock^ *textLat, ^ *textLong, ^ *textTemperature, ^ *textMote, ^ *textHumidity, ^ *textLuminosity;
MotesRequest motesRequest;


// Permet d'afficher les coordonnées
void MainPage::PrintGPS(TextBlock^ textboxA, TextBlock^ textboxB) {
	
	VariablesGlobales::VerrouGPS.lock();
	textboxA->Text = VariablesGlobales::latitude.ToString();
	textboxB->Text = VariablesGlobales::longitude.ToString();
	VariablesGlobales::VerrouGPS.unlock();


}


// Thread de récupération des données GPS
static UINT ThreadGPS() {
	Sleep(1);

	VariablesGlobales::VerrouStatutGPS.lock();
	GPS().setGPS(); // active le GPS

	while (true) {
		VariablesGlobales::VerrouStatutGPS.lock();
		GPS().getGPS(); // On récupère les coordonnées une fois qu'on a des données
	}
	
	return 0;
}


// Décrémentation avec Vérou
static UINT ThreadMotes() {
	Sleep(1);

	VariablesGlobales::VerrouMotes.lock();
	motesRequest.getAllMotes();

	while (true) {
		GPS::GetCloserMote();
	}
	return 0;
}

// Pour plus d'informations sur le modèle d'élément Page vierge, consultez la page https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
	Cpt = rand();
	DispatcherTimer^ timer = ref new DispatcherTimer;
	TimeSpan ts;
	ts.Duration = 10000000;
	timer->Interval = ts;
	timer->Start();
	timer->Tick += ref new EventHandler<Object^>(this, &MainPage::OnTick);

	VariablesGlobales::VerrouAffichage.lock();

	thread Th_GPS(ThreadGPS);
	Th_GPS.detach();

	thread Th_Mot(ThreadMotes);
	Th_Mot.detach();

}


// Permet d'activer l'affichage quand on a des valeurs GPS
void MainPage::HasGPSValue() {
	GPSHaveValue = true;
}


void App1::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	VariablesGlobales::VerrouGPS.unlock();
}

void App1::MainPage::Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	motesRequest.updateMote(43);

    /*OutputDebugStringA("lest go\n");
	VerDec.unlock();
    
	string sstr = "{\"data\":[{\"timestamp\":1645717380000,\"label\":\"light1\",\"value\":305.71,\"mote\":\"9.138\"},{\"timestamp\":1645759199000,\"label\":\"light1\",\"value\":164.29,\"mote\":\"111.130\"},{\"timestamp\":1645816193000,\"label\":\"light1\",\"value\":188.57,\"mote\":\"53.105\"},{\"timestamp\":1645717380000,\"label\":\"temperature\",\"value\":88.36,\"mote\":\"9.138\"},{\"timestamp\":1645759199000,\"label\":\"temperature\",\"value\":-18.85,\"mote\":\"111.130\"},{\"timestamp\":1645816193000,\"label\":\"temperature\",\"value\":-30.94,\"mote\":\"53.105\"},{\"timestamp\":1645717380000,\"label\":\"humidity\",\"value\":1.43,\"mote\":\"9.138\"},{\"timestamp\":1645759199000,\"label\":\"humidity\",\"value\":-4.0,\"mote\":\"111.130\"},{\"timestamp\":1645816193000,\"label\":\"humidity\",\"value\":-4.0,\"mote\":\"53.105\"}]}";
	Platform::String^ myStr = convertFromString(sstr);
	Mote* mote = vectorM
	parseMote(myStr,mote);*/
}

void App1::MainPage::OnTick(Platform::Object^ sender, Platform::Object^ e)
{
	VariablesGlobales::VerrouGPS.lock();
	this->Lat->Text = VariablesGlobales::latitude.ToString();
	this->Long->Text = VariablesGlobales::longitude.ToString();
	VariablesGlobales::VerrouGPS.unlock();

	VariablesGlobales::VerrouMotes.lock();
	this->Hum->Text = VariablesGlobales::vectorMotes[VariablesGlobales::indiceMote].humidity.ToString();
	this->Mote->Text = VariablesGlobales::vectorMotes[VariablesGlobales::indiceMote].getId().ToString();
	this->Temp->Text = VariablesGlobales::vectorMotes[VariablesGlobales::indiceMote].temperature.ToString();
	this->Light->Text = VariablesGlobales::vectorMotes[VariablesGlobales::indiceMote].ligth.ToString();
	VariablesGlobales::VerrouMotes.unlock();

	VariablesGlobales::VerrouStatutGPS.unlock();
}
