#ifndef HEADERS_DRZEWO_H_
#define HEADERS_DRZEWO_H_

#include "Wezel.h"

class Drzewo
{
public:
	Drzewo();
	~Drzewo();

	string toString(bool prec = false);	//wyswietlenie drzewa

	Wezel* zwrocWezel(uint*, uint);		//zwrocenie wezla, który jest ntym potomkiem korzenia
	Wezel* zwrocWezel(vector<uint>);	//zwrocenie wezla, który jest ntym potomkiem korzenia (za pomoc¹ wektora)

	Wezel* korzen;
private:
};

#endif