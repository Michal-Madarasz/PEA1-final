#pragma once
class Pair							//para wspolrzednych macierzy(wiersz+kolumna), miasto poczatkowe i koncowe w podrozy pomiedzy tymi dwoma miastami
{
public:
	unsigned int row;
	unsigned int column;
	Pair(uint wie = 0, uint kol = 0);
	Pair(const Pair &);
	~Pair();
	Pair & operator =(const Pair &);
	friend ostream & operator<<(ostream&, const Pair&);
};