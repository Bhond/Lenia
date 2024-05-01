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
	std::vector<double> kernel;

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
	double computePotential(int& i);
	double convolve(const int& cellIdx);
	std::vector<double> findNeighbours(const int& cellIdx, const int& kernelWidth);
	std::vector<double> buildDonutKernel(const int& width, const int& innerRadius, const int& outerRadius);

};