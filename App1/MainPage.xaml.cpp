//
// MainPage.xaml.cpp
// Implémentation de la classe MainPage.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <thread>
#include <shared_mutex>

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
}