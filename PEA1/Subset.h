#pragma once
class Subset				//element podzbioru wykorzystywany w algorytmie Helda-Karpa (programowanie dynamiczne)
{
public:
	int pop = -1;
	int city;
	int cost = INT_MAX;
};