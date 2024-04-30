#pragma once

#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../main/common.h"
#include "../main/mymath.h"

class Solver
{

public:
	double width{ 200 };
	double height{ 200 };
	int gridWidth{ 10 };
	int gridHeight{ 10 };
	std::vector<double> grid;

private:
	sf::RenderWindow* renderWindow{ nullptr };

public:
	Solver(sf::RenderWindow* w);
	~Solver();

public:
	void solve(double dt);

private:
	void init();
	void readConfiguration();
	std::vector<int> findNeighbours(const int& cellIdx);

};