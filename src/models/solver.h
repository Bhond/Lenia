#pragma once

#include <iostream>
#include <fstream>
#include <functional>
#include <thread>
#include <future>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../main/common.h"
#include "../main/mymath.h"

#include <chrono>

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
	std::vector<double> work(std::vector<double> grid, std::vector<double> kernel, double dt, int start, int skip);
	double computePotential(std::vector<double>& grid, int& i);
	double convolve(std::vector<double>& grid, std::vector<double>& kernel, const int& cellIdx, const int& kernelWidth);
	std::vector<double> findNeighbours(std::vector<double>& grid, const int& cellIdx, const int& kernelWidth);
	std::vector<double> buildDonutKernel(const int& width, const int& innerRadius, const int& outerRadius);

};