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

Platform::String^ ConversionsMaison::conversionStringToPlatformString(const std::string& input)
{
    std::wstring w_str = std::wstring(input.begin(), input.end());
    const wchar_t* w_chars = w_str.c_str();

    return (ref new Platform::String(w_chars));
}
