#include "stdafx.h"
#include "Path.h"

Path::Path()
{
}

Path::Path(const Path&sc)
{
	path = sc.path;
}

Path::~Path()
{
}

void Path::addP(Pair & pair)
{
	path.insert(path.begin(), pair);
}

void Path::addK(Pair & pair)
{
	path.push_back(pair);
}

void Path::mergeP(Path & sc)
{
	path.insert(path.begin(), sc.path.begin(), sc.path.end());

}

void Path::mergeK(Path & sc)
{
	path.insert(path.end(), sc.path.begin(), sc.path.end());
}

Path & Path::operator=(const Path & sc)
{
	path = sc.path;
	return *this;
}

const Pair & Path::operator[](uint x) const
{
	return path[x];
}

ostream & operator<<(ostream & os, const Path & sc)
{
	for (uint i = 0; i < sc.path.size(); i++)
	{
		if (i == 0)
		{
			os << sc[i].row << " => " << sc[i].column << " => ";
		}
		else if (i < sc.path.size() - 1)
		{
			os << sc[i].column << " => ";
		}
		else
		{
			os << sc[i].column;
		}
	}
	return os;
}

Pair & Path::start()
{
	return path.front();
}

Pair & Path::end()
{
	return path.back();
}

void Path::display()
{
	for (uint i = 0; i < path.size(); i++)
	{
		cout << path[i];
		if (i < path.size() - 1)
		{
			cout << ", ";
		}
	}
}


vector<Path> Path::createPath(vector<Pair>& pairs)
{
	vector <bool> used(pairs.size(), false);
	vector<Path>paths;
	for (uint i = 0; i < pairs.size(); i++)
	{
		// Iterowanie po œcie¿kach
		for (uint j = 0; j < paths.size(); j++)
		{
			if (!used[i])
			{
				// Jeœli œcie¿ka jest niepusta
				if (paths[j].path.size())
				{
					// Jeœli ³¹czy siê z pocz¹tkiem lub koñcem œcie¿ki
					if (pairs[i].row == paths[j].end().column)
					{
						paths[j].addK(pairs[i]);
						used[i] = true;
					}
					else if (pairs[i].column == paths[j].start().row)
					{
						paths[j].addP(pairs[i]);
						used[i] = true;
					}
				}
			}
		}
		// Jeœli nie dodano do istniej¹cej œcie¿ki - utwórz now¹ i tam dodaj
		if (!used[i])
		{
			paths.push_back(Path());
			paths.back().addK(pairs[i]);
			used[i] = true;
		}
	}
	mergePaths(paths);
	return paths;
}

vector<Path> Path::mergePaths(vector<Path> & paths)
{
	bool merged = true;
	while (merged&&paths.size() > 1)
	{
		merged = false;
		for (uint i = 0; i < paths.size(); i++)
		{
			for (uint j = i + 1; j < paths.size(); j++)
			{
				if (paths[i].start().row == paths[j].end().column)
				{
					paths[i].mergeP(paths[j]);
					paths.erase(paths.begin() + j);
					merged = true;
				}
				else if (paths[j].start().row == paths[i].end().column)
				{
					paths[j].mergeP(paths[i]);
					paths.erase(paths.begin() + i);
					merged = true;
				}
			}
		}
	}
	return paths;
}
