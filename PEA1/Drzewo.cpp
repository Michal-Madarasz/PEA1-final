#include "stdafx.h"
#include "Drzewo.h"

Drzewo::Drzewo()
{
	korzen = new Wezel();
}

Drzewo::~Drzewo()
{
	delete korzen;
}

string Drzewo::toString(bool prec)
{
	return korzen->wyswietlSiebieISynow(prec);	//wyswietlenie drzewa
}

Wezel* Drzewo::zwrocWezel(uint* trasa, uint wielkoscTrasy)
{
	Wezel* zwrot = korzen;

	for (uint i = 1; i < wielkoscTrasy; i++)
	{
		zwrot = zwrot->zwrocSyna(trasa[i]);
	}

	return zwrot;
}

Wezel* Drzewo::zwrocWezel(vector<uint> tab)
{
	Wezel* zwrot = korzen;

	for (uint i = 1; i < tab.size(); i++)
	{
		zwrot = zwrot->zwrocSyna(tab[i]);
	}

	return zwrot;
}