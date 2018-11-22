#pragma once
#include "Matrix.h"

class ComNode
{
	
public:
	ComNode();
	ComNode(const ComNode &wez);
	ComNode(Matrix &m);
	ComNode(Matrix &m, uint idx);

	void minimalizeCost();			//minimalizacja kosztu (za pomoca metody macierzy)

	Matrix matrix;
	uint index, lowerBound;

	friend ostream & operator<<(ostream&, const ComNode&);
};
