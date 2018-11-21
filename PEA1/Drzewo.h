#ifndef HEADERS_DRZEWO_H_
#define HEADERS_DRZEWO_H_

#include "Wezel.h"

class Drzewo
{
public:
	Drzewo();
	~Drzewo();

	string toString(bool prec = false);	//wyswietlenie drzewa

	Wezel* zwrocWezel(uint*, uint);		//zwrocenie wezla, kt�ry jest ntym potomkiem korzenia
	Wezel* zwrocWezel(vector<uint>);	//zwrocenie wezla, kt�ry jest ntym potomkiem korzenia (za pomoc� wektora)

	Wezel* korzen;
private:
};

#endif