#pragma once

#include <vector>
#include "Model/Mote.h"
#include <shared_mutex>

using namespace std;

class VariablesGlobales
{
public :
	static std::vector<Mote> vectorMotes;
	static int indiceMote;

	static shared_mutex VerrouGPS;
	static shared_mutex VerrouAffichage; 
	static shared_mutex VerrouMotes; 
	static shared_mutex VerrouCoordonnees;

};

