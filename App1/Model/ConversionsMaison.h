#pragma once

using namespace std;

class ConversionsMaison
{
public:
	static string conversionPlatformStringToString(Platform::String^ chaine);
	static int conversionPlatformStringToInt(Platform::String^ chaine);
	static Platform::String^ conversionStringToPlatformString(const std::string& input);

};

