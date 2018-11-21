#include "stdafx.h"
#include "Wezel.h"

//PUBLIC

Wezel::Wezel(Wezel* rodzic)
{
	ojciec = rodzic;
	dolnaGranica = 0;
	czyDG = false;
}

Wezel::~Wezel()
{
	for (uint i = 0; i < synowie.size(); i++)
	{
		delete synowie[i];
	}
}

string Wezel::toString(bool precise)					//wypisanie siebie i synow(jezeli precise==true), wypisanie dolnego ograniczenia
{
	stringstream zwrot;
	if (precise)
	{
		zwrot << "ojciec: " + (int)ojciec;
		for (uint i = 0; i < synowie.size(); i++)
		{
			zwrot << i + "# syn: " + (int)synowie[i];
			zwrot << ", ";
		}
		zwrot << "dolna granica: ";
	}

	zwrot << dolnaGranica;
	return zwrot.str();
}

string Wezel::wyswietlSiebieISynow(bool prec)
{
	return wyswietlSiebieISynow(prec, 0);
}

Wezel* Wezel::zwrocSyna(uint syn)
{
	if (syn < synowie.size())
		return synowie[syn];
	return nullptr;
}

uint Wezel::zwrocIloscSynow()
{
	return synowie.size();
}

Wezel* Wezel::zwroOjca()
{
	return ojciec;
}

Macierz& Wezel::zwrocMacierz()
{
	return macierz;
}

uint Wezel::zwrocDolnaGranica()
{
	if (!czyDG)
	{
		dolnaGranica = macierz.zminimalizujKoszt();
		czyDG = true;
	}

	return dolnaGranica;
}

void Wezel::dodajSyna(Wezel* syn)
{
	if (syn)
	{
		syn->ojciec = this;
		synowie.push_back(syn);
	}
}

void Wezel::ustawGraf(Macierz& mac)						//utworzenie grafu z macierzy odleglosci
{
	macierz = mac;
	czyDG = false;
	dolnaGranica = 0;
}

//PRIVATE
string Wezel::wyswietlSiebieISynow(bool prec, int glebokosc)
{
	stringstream zwrot;
	zwrot << glebokosc;
	zwrot << ": " + toString(prec);

	if (synowie.size())
		zwrot << ", ";

	for (uint i = 0; i < synowie.size(); i++)
	{
		zwrot << synowie[i]->wyswietlSiebieISynow(prec, glebokosc + 1) + ", ";
	}
	return zwrot.str();
}