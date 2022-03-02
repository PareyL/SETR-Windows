#include "pch.h"
#include "Model/ConversionsMaison.h"
#include <string>

using namespace std;

string ConversionsMaison::conversionPlatformStringToString(Platform::String^ chaine)
{
    wstring wstring(chaine->Begin());
    string string(wstring.begin(), wstring.end());
    return string;
}

int ConversionsMaison::conversionPlatformStringToInt(Platform::String^ chaine)
{
    string string = conversionPlatformStringToString(chaine);
    int res = stoi(string);
    return res;
}
