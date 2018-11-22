#pragma once
#include "Path.h"
#include "Subset.h"

class ComNode;

class Matrix
{
public:
	Matrix(uint lMiast = 0);
	Matrix(const Matrix &);
	Matrix(ifstream &);					//utworzenie macierzy z pliku
	virtual ~Matrix();

	Matrix & operator =(const Matrix &);
	void clone(const Matrix &);			//skopiowanie macierzy
	void readFile(ifstream &);			//odczytanie macierzy z pliku
	void list(bool noColor = false);		//wypisanie macierzy odleglosci
	void bruteForce(bool);					//przeglad zupelny
	void branchAndBound();					//bnb
	void HeldKarp();					//programowanie dynamiczne

	int getValue(uint, uint);			//odleglosc z miasta do miasta

	uint countCities();				//liczba miast
	uint minimalizeCost();			//

protected:
	bool engage(uint vNumber = 0);
	void myClear();

private:
	//Macierz w postaci[wierzcholki][krawedzie]
	uint numberOfCities;
	int ** matrix;//macierz

	// Zwraca maksymalną wartość w macierzy
	uint getMax();
	// Zwraca maskymalną wartość jaka jest zapisana w kolumnie
	// (macierz, waga lub numer kolumny)
	uint getMaxInColumn();
	int minRow(uint);			//minimalna wartosc w wierszu
	int minColumn(uint);		//minimalna wartosc w kolumnie
	int minRow(uint, int&);		//przeciazone
	int minColumn(uint, int&);
	int secondMinRow(uint);		
	int secondMinColumn(uint);
	void subtractRow(uint, int);
	void subtractColumn(uint, int);
	void setRow(uint wiersz, int wartosc = -1);
	void setColumn(uint kolumna, int wartosc = -1);
	void setValue(uint wiersz, uint kolumna, int wartosc = -1);
	bool reduce(vector<Pair>&, uint&);
	uint addLastPair(vector<Pair>&);

	long drawWithoutRepeat(bool*, uint);

	static bool nextPermutation(uint *, uint);
	static vector<bool> buildVisited(tree <ComNode>::iterator, uint);
	static vector<uint> buildPath(tree <ComNode>::iterator, uint &);
};
