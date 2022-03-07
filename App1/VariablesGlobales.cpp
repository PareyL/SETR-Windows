#include "pch.h"
#include "VariablesGlobales.h"

std::vector<Mote> VariablesGlobales::vectorMotes = std::vector<Mote>();
int VariablesGlobales::indiceMote = 0;
float VariablesGlobales::latitude = 0.0;
float VariablesGlobales::longitude = 0.0;

shared_mutex VariablesGlobales::VerrouGPS;
shared_mutex VariablesGlobales::VerrouStatutGPS;
shared_mutex VariablesGlobales::VerrouMotes;
shared_mutex VariablesGlobales::VerrouAffichage;

VariablesGlobales::VariablesGlobales()
{
	
}
