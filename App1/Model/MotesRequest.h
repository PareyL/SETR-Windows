#pragma once
#include <list>
#include <vector>

class MotesRequest
{
private :
	Platform::String^ urlIOTLab = "http://iotlab.telecomnancy.eu:8080/iotlab/";

public :

	MotesRequest();


	void getAllMotes();
	void updateMote(int idMote);

};

