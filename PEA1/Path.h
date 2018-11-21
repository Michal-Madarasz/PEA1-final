#pragma once
#include "Pair.h"

class Path
{
private:
	vector<Pair> path;
public:
	Path();
	Path(const Path &);
	~Path();
	void addP(Pair &);		//dodanie pary na poczatek trasy
	void addK(Pair &);		//dodanie pary na koniec trasy
	void mergeP(Path &);		//doklejenie sciezki na poczatek aktualnej 
	void mergeK(Path &);		//doklejenie sciezki na koniec aktualnej 
	Path & operator=(const Path &);
	const Pair & operator[](uint)const;
	friend ostream & operator<<(ostream &, const Path&);
	Pair & start();			//zwrocenie poczatku sciezki
	Pair & end();			//zwrocenie konca sciezki
	void display();			//wyswietlenie sciezki
	static vector<Path> createPath(vector<Pair> &);	//utworzenie sciezek z wektoru par
	static vector<Path> mergePaths(vector<Path> &);	//scalenie sciezek z wektorem sciezek


};