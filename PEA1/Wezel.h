#ifndef HEADERS_WEZEL_H_
#define HEADERS_WEZEL_H_

#include "Macierz.h"
#include <vector>

class Wezel
{
public:
	Wezel(Wezel* rodzic = nullptr);
	~Wezel();

	string toString(bool prec = false);
	string wyswietlSiebieISynow(bool prec = false);

	Wezel* zwrocSyna(uint);
	uint zwrocIloscSynow();
	Wezel* zwroOjca();
	Macierz& zwrocMacierz();
	uint zwrocDolnaGranica();

	void dodajSyna(Wezel*);
	void ustawGraf(Macierz&);
private:
	Wezel *ojciec;
	vector<Wezel*> synowie;
	Macierz macierz;
	uint dolnaGranica;
	bool czyDG;
	string wyswietlSiebieISynow(bool, int);
};

#endif