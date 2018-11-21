#pragma once
#include "stdafx.h"
#include "menu.h"

void start()
{
	srand(time(NULL));
	setlocale(LC_ALL, "polish");
	Matrix* mac = new Matrix();

	const char title[] =
	{ "Problem komiwojazera" };

	firstMenu(mac, title);

	delete mac;
}

void firstMenu(Matrix* mac, const char* title)
{
	char op = '0';

	string firstMenu[] =
	{ "Wczytaj graf z pliku", "Wyswietl", "Algorytm B&B", "Algorytm BruteForce","Alorytm DP" , "Wyjscie" };

	
	do {
		system("cls");
		for (int i = 0; i < 6; i++) {
			cout << i << ". " << firstMenu[i] << "\n";
		}
		cin >> op;

		switch (op)
		{
		case '0':
			loadMatrix(mac);
			system("pause");
			break;
		case '1':
			showMatrix(mac);
			system("pause");
			break;
		case '2':
			branchAndBound(mac);
			system("pause");
			break;
		case '3':
			bruteForce(mac);
			system("pause");
			break;
		case '4':
			dynamicProgramming(mac);
			system("pause");
			break;
		case '5':
			break;
		default:
			break;
		}
	} while (op != '5');
}

void loadMatrix(Matrix* mac)
{
	string path;
	ifstream ifs;
	bool open = true;
	do
	{
		cout << endl;
		system("cls");
		if (!open)
			cout << "Nie udalo sie otworzyc" << endl;
		cout << "0. Powrot\n" << "Wprowadz sciezke: ";
		cin >> path;

		if (path == "0")
		{
			open = false;
			break;
		}

		ifs.open(path.c_str(), ios_base::in);
		open = ifs.is_open();
	} while (!open);

	if (open)
	{
		if (ifs.is_open())
		{
			cout << "Wczytuje macierz..." << endl;
			mac->readFile(ifs);
			cout << "Wczytano" << endl;
		}

		if (ifs.is_open())
			ifs.close();
	}
}

void showMatrix(Matrix* mac)
{
	cout << endl;
	system("cls");

	if (mac->countCities())
		mac->list(true);
	else
		cout << "Graf jest pusty\n";
}

void branchAndBound(Matrix* mac)
{
	cout << endl;
	system("cls");

	if (mac->countCities())
		mac->branchAndBound();
	else
		cout << "Graf jest pusty\n";
}

void bruteForce(Matrix* mac)
{
	cout << endl;
	system("cls");

	if (mac->countCities())
		mac->bruteForce(true);
	else
		cout << "Graf jest pusty\n";
}

void dynamicProgramming(Matrix * mac)
{
	cout << endl;
	system("cls");

	if (mac->countCities())
		mac->HeldKarp();
	else
		cout << "Graf jest pusty\n";
}