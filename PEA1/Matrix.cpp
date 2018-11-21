#include "stdafx.h"
#include "Matrix.h"


bool Matrix::seed = false;
// PUBLIC

using kptree::print_tree_bracketed;

Matrix::Matrix(uint nCity)
{
	// Jeśli random seed nie został zainicjowany
	if (!Matrix::seed)
	{
		srand((unsigned int)time(NULL));
		Matrix::seed = true;
	}

	numberOfCities = 0;
	matrix = nullptr;

	// Jeśli ilość wierzchołków nie jest zerem
	if (nCity)
	{
		engage(nCity);
	}
		
}

Matrix::Matrix(const Matrix &m)
{
	matrix = nullptr;
	numberOfCities = 0;
	clone(m);
}

Matrix::Matrix(ifstream &input)
{
	numberOfCities = 0;
	matrix = nullptr;

	readFile(input);
}

Matrix::~Matrix()
{
	myClear();
}

Matrix & Matrix::operator =(const Matrix & m)
{
	clone(m);
	return *this;
}

void Matrix::clone(const Matrix &m)
{
	if (numberOfCities != m.numberOfCities)
		engage(m.numberOfCities);

	for (uint i = 0; i < numberOfCities; i++)
	{
		memcpy(matrix[i], m.matrix[i], sizeof(int) * numberOfCities);
	}
}

void Matrix::readFile(ifstream &input)
{
	uint vertexNum = 0;
	if (!input.is_open())
		input.open("input.txt", ios_base::in);

	input >> vertexNum;	// liczba wierzchołków

	if (engage(vertexNum))
	{
		for (uint i = 0; i < numberOfCities; ++i)
		{
			for (uint j = 0; j < numberOfCities; j++)
			{
				input >> matrix[i][j];
				if (i == j)
				{
					matrix[i][j] = -1;
				}
			}
		}
	}
}

void Matrix::list(bool noColor)
{
	const uint MaxKol = getMaxInColumn();
	const uint columnWidth = (uint)(ceil(log10(MaxKol)) + 1);
	const uint cityWidthIndex = (uint)(ceil(log10(numberOfCities - 1)));


	for (uint row = 0; row < numberOfCities; row++)
	{
		cout.width(cityWidthIndex);
		cout << row << ": ";
		for (uint column = 0; column < numberOfCities; column++)
		{
			cout.width(columnWidth);
			cout << matrix[row][column] << " ";
		}
		cout << endl;
	}
}

void Matrix::bruteForce(bool rysPostep)
{
	uint nCity = numberOfCities;
	if (!nCity)	//jezeli liczba miast jest rowna 0, to nie mozna wykonac algorytmu
		return;

	if (nCity >= 20)	//brak wypisywania postepu dla wiekszej ilosci miast od 20
		rysPostep = false;

	ulong endStep = 2;	//ilosc permutacji
	for (uint i = 3; i <= nCity; ++i)
		endStep *= i;

	ulong divide = endStep / 100;
	if (!divide)				//ograniczenie sprawdzanych permutacji do 100000000
		divide = 1;
	if (divide > 1000000)
		divide = 1000000;

	bool newDiv = false;
	uint maxValue = -1, actualStep = 0;
	uint *cityVector = new uint[nCity];	//tablica z numerami miast
	uint *bestPath = new uint[nCity];	//tablica przechowujaca najlepsza droge

	// Pierwsza permutacja
	for (uint i = 0; i < nCity; ++i)
		cityVector[i] = i;

	double permutation = 0.0;

	do
	{
		++actualStep;
		
		uint aktWaga = matrix[cityVector[nCity - 1]][cityVector[0]];	//waga permutacji
		for (uint i = 0; i < nCity - 1; ++i)
			aktWaga += matrix[cityVector[i]][cityVector[i + 1]];		//obliczanie wagi permutacji

		if (aktWaga < maxValue)									//jezeli aktualna waga permutacji mniejsza od najmniejszej to staje sie najmniejsza
		{
			maxValue = aktWaga;									//przypisanie najmniejszej wagi
			memcpy(bestPath, cityVector, nCity * sizeof(uint));	//skopiowanie najlepszej drogi
		}

	} while (nextPermutation(cityVector, nCity));				//wykonuj dopoki nie zostaly sprawdzone wszystkie permutacje

	if (rysPostep)												//wypisanie wynikow i czasu trwania
	{
		cout << "\nDroga:\n";
		for (uint i = 0; i < nCity; ++i)
		{
			if (i < nCity)
				cout << bestPath[i] << " -> ";
		}
		cout << bestPath[0] << endl;
		cout << "Koszt drogi: " << maxValue << endl;
	}

	delete[] bestPath;
	delete[] cityVector;
}

void Matrix::branchAndBound()
{
	bool ended = false;
	ComNode pom(*this, 0);								//pomocniczy wezel komiwojazera
	pom.minimalizeCost();								//minimalizacja kosztu wezla
	tree<ComNode> root(pom);							//drzewo wezlow o korzeniu pomocniczego wezla

	// Początkowa pozycja w drzewie
	tree <ComNode>::iterator actualNode = root.begin();	//iterator drzewa

	bool addChild = false;
	do
	{
		addChild = false;
		uint help;
		vector<bool> visited = buildVisited(actualNode, numberOfCities);	//wektor odwiedzonych miast
		vector<uint> path = buildPath(actualNode, help);			//wektor sciezki pokonanej dzieki algorytmowi
		for (uint i = 0; i < numberOfCities; i++)
		{
			// Jeśli wierzchołek nie został jeszcze odwiedzony
			if (!visited[i])												//jezeli miasto nie zostalo odwiedzone
			{
				ComNode pom(actualNode->matrix, i);								//utworzenie wezla								
				pom.matrix.setRow(actualNode->index);						//ustawienie wiersza indeksem
				pom.matrix.setColumn(i);									//ustawienie kolumny wartoscia i z petli

				for (uint j = 0; j < path.size(); j++)
				{
					pom.matrix.setValue(i, path[j]);					//ustawienie macierzy w wezle
				}

				// Minimalizacja kosztu
				pom.minimalizeCost();											//minimalizacja kosztu
				pom.lowerBound += actualNode->matrix.getValue(actualNode->index, i); //obliczanie dolnego ograniczenia
				pom.lowerBound += actualNode->lowerBound;

				// Dodanie grafu do drzewa
				root.append_child(actualNode, pom);							//dodanie dziecka do drzewa
				addChild = true;
			}
		}

		// Szukanie najmniejszego rozwiązania
		uint minimum = MAXUINT;
		for (tree<ComNode>::leaf_iterator iter = root.begin_leaf(); iter != root.end_leaf(); iter++)
		{
			//cout << iter->lowerBound << ", ";
			if (iter->lowerBound < minimum)
			{
				minimum = iter->lowerBound;
				actualNode = iter;
			}
		}

		//cout << endl;
	} while (addChild);

	uint lowerBound;
	vector<uint> sc = buildPath(actualNode, lowerBound);				//zbudowanie sciezki wyznaczonej w algorytmie
	cout << "Droga: ";														//wypisanie wynikow dzialania algorytmu
	for (uint i = 0; i < sc.size(); i++)
	{
		cout << sc[i];
		if (i < sc.size() - 1)
		{
			cout<< " -> ";
		}
		else
		{
			cout << " -> " << sc[0]<< endl;
		}
		
	}
	cout << "Koszt drogi: " << lowerBound << endl;
	
}

void Matrix::HeldKarp() 
{
	int minCost=INT_MAX;
	vector<vector<Subset> > point;
	point.resize(1 << (numberOfCities - 1), vector<Subset>(numberOfCities));
	int test = (1 << (numberOfCities - 1));
	for (auto visited = 1; visited < (1 << (numberOfCities - 1)); ++visited) {
		for (auto last = 0; last < (numberOfCities - 1); ++last) {

			// ostatnio odwiedzone miasto musi byc jednym z odwiedzonych miast
			if (!(visited & 1 << last)) continue;

			// sprawdzanie wszystkich mozliwosci dla poprzedniego miasta
			// wybor najlepszego z nich
			if (visited == 1 << last) {
				// poprzednie miasto musi byc mniejsze od N-1 (N=liczba miast)
				point[visited][last].cost = matrix[numberOfCities - 1][last];
				point[visited][last].pop = (numberOfCities - 1);
				point[visited][last].city = last;
			}
			else {
				// poprzednie miasto bylo jednym z innych odwiedzonych
				int prevVisited = visited ^ 1 << last;
				for (int prev = 0; prev < numberOfCities - 1; ++prev) {
					if (!(prevVisited & 1 << prev)) continue;
					if (point[visited][last].cost > (matrix[prev][last] + point[prevVisited][prev].cost))
					{
						point[visited][last].cost = matrix[prev][last] + point[prevVisited][prev].cost;
					}
					else
					{
						point[visited][last].cost = point[visited][last].cost;
					}

					if (point[visited][last].cost > (matrix[prev][last] + point[prevVisited][prev].cost))
					{
						point[visited][last].pop = visited;
						point[visited][last].city = last;
					}
					else
					{
						point[visited][last].pop = prevVisited;
						point[visited][last].city = last;
					}
				}
			}
		}
	}
	// wybor najlepszej sciezki
	for (int last = 0; last < numberOfCities - 1; ++last)
	{
		if (minCost > (matrix[last][numberOfCities - 1] + point[(1 << (numberOfCities - 1)) - 1][last].cost))
		{
			minCost = (matrix[last][numberOfCities - 1] + point[(1 << (numberOfCities - 1)) - 1][last].cost);
		}
	}
	for (int last = 0; last < numberOfCities - 1; ++last) {
		point[(1 << (numberOfCities - 1)) - 1][last].cost = (matrix[last][numberOfCities - 1] + point[(1 << (numberOfCities - 1)) - 1][last].cost);
	}

	int subsetId = (1 << (numberOfCities - 1)) - 1;
	vector<uint> path;
	path.push_back(numberOfCities - 1);
	while (path.size() != numberOfCities)
	{

		Subset temp_min;
		for (int i = 0; i < numberOfCities; ++i)
		{
			if (temp_min.cost > point[subsetId][i].cost)
			{
				temp_min.cost = point[subsetId][i].cost;
				temp_min.pop = point[subsetId][i].pop;
				temp_min.city = point[subsetId][i].city;
			}
		}
		path.push_back(temp_min.city);
		subsetId = temp_min.pop;
	}

	cout << "Droga: ";

	int zero = -1;
	for (int i = 0; i < numberOfCities; i++)
	{
		if (path[i] == 0)
		{
			zero = i;
		}
	}
	for (int i = zero; i > -1; i--)
	{
		cout << path[i] <<" -> ";
	}
	for (int i = numberOfCities-1 ; i > zero; i--)
	{
			if (i == zero)
			{
				cout << path[i] << endl;;
			}
			else
			{
				cout << path[i] << " -> ";
			}
	}
	cout << path[zero] << endl;

	cout << "Koszt drogi: " << minCost << endl;
	


}





int Matrix::getValue(uint row, uint column)
{
	return matrix[row][column];			//zwrocenie odleglosci z miasta do miasta
}

uint Matrix::countCities()
{
	return numberOfCities;						//zwrocenie calkowitej liczby miast
}

// PROTECTED

bool Matrix::engage(uint nCity)		//rezerwowanie macierzy w pamieci, gdzie beda zapisane odleglosci pomiedzy miastami
{
	// Jeśli ilość wierzchołków nie jest zerem
	if (nCity)
	{
		// Czyszczenie grafu
		myClear();

		numberOfCities = nCity;
		matrix = new int*[numberOfCities];

		int** end = matrix + numberOfCities;
		for (int** i = matrix; i < end; ++i)
		{
			*i = new int[nCity];
			memset(*i, 0, sizeof(int) * nCity);
		}

		return true;
	}

	return false;
}

void Matrix::myClear()						//czyszczenie macierzy odleglosci
{
	if (matrix)
	{
		int** koniec = matrix + numberOfCities;
		for (int** i = matrix; i < koniec; ++i)
		{
			if (*i)
			{
				delete[] * i;
				*i = nullptr;
			}
		}

		delete[] matrix;
		matrix = nullptr;
	}

	numberOfCities = 0;
}

// PRIVATE
uint Matrix::getMax()					//zwrocenie najwiekszej wartosci w macierzy
{
	int value = 0;

	int** koniecZP = matrix + numberOfCities;
	for (int** i = matrix; i < koniecZP; ++i)
	{
		int* koniecWP = *i + numberOfCities;
		for (int* j = *i; j < koniecWP; ++j)
		{
			if (abs(*j) > value)
				value = abs(*j);
		}
	}

	return value;
}

uint Matrix::getMaxInColumn()					//zwrocenie najwiekszej wartosci w kolumnie
{
	uint value = getMax() + 1;

	if (value < numberOfCities - 1)
		value = numberOfCities - 1;

	return value;
}

int Matrix::minRow(uint row)				//znalezienie najmniejszej wartosci w wierszu
{
	if (row >= numberOfCities)
	{
		return -1;
	}
	else
	{
		int min = MAXINT;
		for (uint column = 0; column < numberOfCities; column++)
		{
			if (matrix[row][column] < min && matrix[row][column] >= 0)
				min = matrix[row][column];
		}

		if (min == MAXINT)
			return -1;
		else
			return min;
	}
}

int Matrix::minColumn(uint column)			//znalezienie najmniejszej wartosci w kolumnie
{
	if (column >= numberOfCities)
	{
		return -1;
	}
	else
	{
		int min = MAXINT;
		for (uint row = 0; row < numberOfCities; row++)
		{
			if (matrix[row][column] < min && matrix[row][column] >= 0)
				min = matrix[row][column];
		}

		if (min == MAXINT)
			return -1;
		else
			return min;
	}
}	

int Matrix::minRow(uint row, int& secMin)	//znalezienie najmniejszej wartosci w wierszu
{
	if (row >= numberOfCities)
	{
		secMin = -1;
		return -1;
	}
	else
	{
		int min = MAXINT;
		secMin = MAXINT;
		uint positive = 0;
		for (uint column = 0; column < numberOfCities; column++)
		{
			if (matrix[row][column] >= 0)
			{
				++positive;
				if (matrix[row][column] <= min)
				{
					if (min < secMin)
						secMin = min;
					min = matrix[row][column];
				}
				else
				{
					if (matrix[row][column] < secMin)
						secMin = matrix[row][column];
				}
			}
		}
		if (secMin == MAXINT)
			secMin = -1;
		if (positive == 1)
			secMin = min;

		if (min == MAXINT)
			return -1;
		else
			return min;
	}
}

int Matrix::minColumn(uint column, int& secMin)	//znalezienie najmniejszej wartosci w kolumnie
{
	if (column >= numberOfCities)
	{
		secMin = -1;
		return -1;
	}
	else
	{
		int min = MAXINT;
		secMin = MAXINT;
		uint positive = 0;
		for (uint row = 0; row < numberOfCities; row++)
		{
			if (matrix[row][column] >= 0)
			{
				++positive;
				if (matrix[row][column] <= min)
				{
					if (min < secMin)
						secMin = min;
					min = matrix[row][column];
				}
				else
				{
					if (matrix[row][column] < secMin)
						secMin = matrix[row][column];
				}
			}
		}
		if (secMin == MAXINT)
			secMin = -1;
		if (positive == 1)
			secMin = min;

		if (min == MAXINT)
			return -1;
		else
			return min;
	}
}

int Matrix::secondMinRow(uint row)					//znalezienie prawie najmniejszej wartosci w wierszu
{
	if (row >= numberOfCities)
		return -1;
	else
	{
		int min = MAXINT;
		uint zeros = 0, positive = 0;
		for (uint column = 0; column < numberOfCities; column++)
		{
			if (matrix[row][column] < min && matrix[row][column] >= 0)
			{
				if (!matrix[row][column] && zeros++)
				{
					min = matrix[row][column];
				}
				else if (matrix[row][column])
				{
					min = matrix[row][column];
				}
			}
		}
		return min;
	}
}

int Matrix::secondMinColumn(uint column)					//znalezienie prawie najmniejszej wartosci w kolumnie
{
	if (column >= numberOfCities)
		return -1;
	else
	{
		int min = MAXINT;
		uint zeros = 0;
		for (uint row = 0; row < numberOfCities; row++)
		{
			if (matrix[row][column] < min && matrix[row][column] >= 0)
			{
				if (!matrix[row][column] && zeros++)
				{
					min = matrix[row][column];
				}
				else if (matrix[row][column])
				{
					min = matrix[row][column];
				}
			}
		}
		return min;
	}
}

void Matrix::subtractRow(uint row, int from)			//odjecie wartosci najmniejszej w wierszach
{
	if (from > 0)
	{
		for (uint column = 0; column < numberOfCities; column++)
		{
			if (matrix[row][column] >= 0)
				matrix[row][column] -= from;
		}
	}
}

void Matrix::subtractColumn(uint column, int from)			//odjecie wartosci najmniejszej w kolumnach
{
	if (from > 0)
	{
		for (uint row = 0; row < numberOfCities; row++)
		{
			if (matrix[row][column] >= 0)
				matrix[row][column] -= from;
		}
	}
}

void Matrix::setRow(uint row, int value)			//ustawienie wszystkich wartosci w wierszu
{
	for (uint column = 0; column < numberOfCities; column++)
	{
		matrix[row][column] = value;
	}
}

void Matrix::setColumn(uint column, int value)		//ustawienie wszystkich wartosci w kolumnie
{
	for (uint row = 0; row < numberOfCities; row++)
	{
		matrix[row][column] = value;
	}
}

void Matrix::setValue(uint row, uint column, int value)	//ustawienie wartosci o podanym wierszu i kolumnie
{
	if (row < numberOfCities && column < numberOfCities)
		matrix[row][column] = value;
}

uint Matrix::minimalizeCost()							//fukncja minimalizacji kosztu (bnb)
{
	uint lowerBound = 0;
	// Minimalizacja kosztów w wierszach
	for (uint row = 0; row < numberOfCities; row++)
	{
		int pom = minRow(row);
		if (pom > 0)
		{
			lowerBound += pom;
			subtractRow(row, pom);
		}
	}

	// Minimalizacja kosztów w kolumnach
	for (uint column = 0; column < numberOfCities; column++)
	{
		int pom = minColumn(column);
		if (pom > 0)
		{
			lowerBound += pom;
			subtractColumn(column, pom);
		}
	}

	return lowerBound;
}

bool Matrix::reduce(vector<Pair>& path, uint& lowerBound)	//redukcja macierzy (bnb)
{
	vector<int> minWie(numberOfCities, -1);
	vector<int> minKol(numberOfCities, -1);
	bool reducible = false;

	// Wyszukiwanie drugich najmniejszych wartości
	for (uint i = 0; i < numberOfCities; i++)
	{
		int drugMin;
		// Jeśli pierwsza jest zerem, wtedy druga jest istotna
		if (!minRow(i, drugMin))
		{
			minWie[i] = drugMin;
			reducible = true;
		}

		if (!minColumn(i, drugMin))
		{
			minKol[i] = drugMin;
			reducible = true;
		}
	}

	if (reducible)
	{

		cout << "minWiersz: ";
		for (uint i = 0; i < numberOfCities; i++)
			cout << minWie[i] << " ";

		cout << "\nminKol: ";
		for (uint i = 0; i < numberOfCities; i++)
			cout << minKol[i] << " ";
		cout << endl;

		Pair pair;
		int max = INT_MIN;
		// Szukanie pary wartości która po zsumowaniu jest największa
		for (uint row = 0; row < numberOfCities; row++)
		{
			for (uint column = 0; column < numberOfCities; column++)
			{
				if (minWie[row] >= 0 && minKol[column] >= 0 && !matrix[row][column])
				{
					int value = minWie[row] + minKol[column];
					if (value > max)
					{
						max = value;
						pair.row = row;
						pair.column = column;
					}
				}
			}
		}

		cout << "Pair: " << pair << ", rowMin: " << minWie[pair.row]
			<< ", colMin: " << minKol[pair.column] << ", sum: " << max << endl;

		// "Zmniejszanie" rozmiaru macierzy 
		setRow(pair.row);
		setColumn(pair.column);
		// Zakazanie powrotu
		setValue(pair.column, pair.row);

		path.push_back(pair);

		vector<Path> paths = Path::createPath(path);

		for (uint i = 0; i < paths.size(); i++)
		{
			//cout << "\tPath #" << i << ": " << paths[i] << endl;
			// Jeśli któryś koniec równa się któremuś początkowi - zabroń dojścia na krańce
			setValue(paths[i].end().column, paths[i].start().row);
			//cout << "[" << paths[i].back().kolumna << ", " << paths[i].front().wiersz << "] marked as -1\n";
		}
	}
	else
	{
		cout << "NO ZEROS PRESENT - ADDING LAST PAIRS\n";
		lowerBound += addLastPair(path);
	}

	cout << "Matrix: \n";
	list();

	return reducible;
}

uint Matrix::addLastPair(vector<Pair>& trasa)			//dodanie ostatniej pary miast trasy do ogrnaiczenia
{
	uint lowerBound = 0;
	for (uint row = 0; row < numberOfCities; row++)
	{
		for (uint column = 0; column < numberOfCities; column++)
		{
			if (matrix[row][column] >= 0)
			{
				trasa.push_back(Pair(row, column));
				lowerBound += matrix[row][column];
			}
		}
	}

	return lowerBound;
}

long Matrix::drawWithoutRepeat(bool* drawn, uint length)		//tworzenie losowej macierzy 
{
	uint rys = 0, notDrawn = 0, ret = 0;

	// Ile liczb jest niewylosowanych
	for (uint i = 0; i < length; ++i)
	{
		if (!drawn[i])
			++notDrawn;
	}

	// Która liczba z niewylosowanych ma być wylosowana
	if (notDrawn)
		rys = rand() % notDrawn;
	else
		return -1;

	for (uint i = 0; i <= rys; ++ret)
	{
		if (!drawn[ret])
			++i;
	}

	drawn[--ret] = true;
	return ret;
}

bool Matrix::nextPermutation(uint *tab, uint length)		//sprawdzanie czy to ostatnia permutacja (przeglad zupelny)
{
	// Znajduje nierosnący ciąg
	if (length == 0)
		return false;
	int i = length - 1;
	while (i > 0 && tab[i - 1] >= tab[i])
		i--;
	if (i == 0)
		return false;

	// Znajdź następnik pivota
	int j = length - 1;
	while (tab[j] <= tab[i - 1])
		j--;
	int pom = tab[i - 1];
	tab[i - 1] = tab[j];
	tab[j] = pom;

	// Odwróć ciąg
	j = length - 1;
	while (i < j)
	{
		pom = tab[i];
		tab[i] = tab[j];
		tab[j] = pom;
		i++;
		j--;
	}
	return true;
}

vector<bool> Matrix::buildVisited(tree <ComNode>::iterator actual, uint nCity)	//budowanie wektora odwiedzonych miast
{
	vector<bool> visited(nCity, false);
	tree<ComNode>::iterator parent;

	while (actual != nullptr)
	{
		parent = tree<ComNode>::parent(actual);
		visited[actual->index] = true;
		actual = parent;
	}

	return visited;
}

vector<uint> Matrix::buildPath(tree <ComNode>::iterator actual, uint &lowerBound)	//budowanie pokonanej sciezki miast
{
	vector<uint> path;
	tree<ComNode>::iterator parent;
	
	if (actual != nullptr)
		lowerBound = actual->lowerBound;


	while (actual != nullptr)
	{
		parent = tree<ComNode>::parent(actual);
		path.insert(path.begin(), actual->index);
		actual = parent;
	}
	


	return path;
}
