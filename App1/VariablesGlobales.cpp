#include "pch.h"
#include "VariablesGlobales.h"

std::vector<Mote> VariablesGlobales::vectorMotes = std::vector<Mote>();
int VariablesGlobales::indiceMote = 0;

shared_mutex VariablesGlobales::VerrouGPS;
shared_mutex VariablesGlobales::VerrouAffichage;
shared_mutex VariablesGlobales::VerrouMotes;
shared_mutex VariablesGlobales::VerrouCoordonnees;

VariablesGlobales::VariablesGlobales()
{


}
