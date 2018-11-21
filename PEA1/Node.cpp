#include "stdafx.h"
#include "Node.h"

ComNode::ComNode()
{

}

ComNode::ComNode(const ComNode &wez)
{
	matrix = wez.matrix;
	index = wez.index;
	lowerBound = wez.lowerBound;
}

ComNode::ComNode(Matrix &m)
{
	matrix = m;
}

ComNode::ComNode(Matrix &m, uint idx)
{
	matrix = m;
	index = idx;
}

void ComNode::minimalizeCost()
{
	lowerBound = matrix.minimalizeCost();
}

ostream & operator<<(ostream& os, const ComNode& wez)
{
	os << "[#" << wez.index << ", " << wez.lowerBound << "]";
	return os;
}
