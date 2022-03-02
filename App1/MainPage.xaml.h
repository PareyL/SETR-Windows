//
// MainPage.xaml.h
// Déclaration de la classe MainPage.
//

#pragma once

#include "MainPage.g.h"

namespace App1
{
	/// <summary>
	/// Une page vide peut être utilisée seule ou constituer une page de destination au sein d'un frame.
	/// </summary>

	public ref class MainPage sealed
	{
	public:
		MainPage();
		static void HasGPSValue();

	private:
		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnTick(Platform::Object^ sender, Platform::Object^ e);
	};
}
