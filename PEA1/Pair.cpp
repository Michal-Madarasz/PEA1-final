#include "stdafx.h"
#include "Pair.h"


Pair::Pair(uint r, uint c)
{
	row = r;
	column = c;
}

Pair::Pair(const Pair & pair)
{
	row = pair.row;
	column = pair.column;
}

Pair::~Pair()
{
}

Pair & Pair::operator=(const Pair & pair)
{
	row = pair.row;
	column = pair.column;
	return *this;
}

ostream & operator<<(ostream& os, const Pair&pair)
{
	os << "[" << pair.row << ", " << pair.column << "]";
	return os;
}
