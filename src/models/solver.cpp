#include "solver.h"
#include "boost/json.hpp"
#include <random>

Solver::Solver(sf::RenderWindow* w)
    : renderWindow(w)
{
    init();
}

Solver::~Solver()
{
}

void Solver::init()
{
    // Configure solver
    readConfiguration();

    // Initialize objects
    std::mt19937 engine(42);
    std::normal_distribution<double> normal(0.5, 0.5);
   double size = gridWidth * gridHeight;
    double maxVal = (std::max(gridWidth, gridHeight) / 2.0);
    for (int i = 0; i < size; i++)
    {
        double row = -(gridHeight / 2) + std::floor(i / gridHeight);
        double col = -(gridWidth / 2) + (i % gridWidth);
        double r = std::sqrt(row * row + col * col);
        double val = 1 - r / maxVal;
        if (r > maxVal)
		    grid.push_back(0.0);
        else
		    grid.push_back(val);
    }
    // Initialize kernel
    kernel = buildDonutKernel(15, 2, 12);
    
}

void Solver::solve(double dt)
{
    // Game of life: first test
    std::vector<double> newGrid(grid.size(), 0.0);
    for (int i = 0; i < grid.size(); i++)
    {
        // Compute potential
        double potential = convolve(i);

        // Compute growth rate
        double tmp = -std::pow(potential - 0.15, 2) / (2 * 0.015);
        double growth = 2 * std::exp(tmp) - 1;
        newGrid[i] = std::max(0.0, std::min(1.0, grid[i] + growth * dt));
    }

    // Copy new value inside resulting grid
    for (int i = 0; i < grid.size(); i++)
        grid[i] = newGrid[i];
}

double Solver::computePotential(int& cellIdx)
{
    double potential{ 0 };
    std::vector<double> neighbours = findNeighbours(cellIdx, 3);
    for (int j = 0; j < neighbours.size(); j++)
    {
        potential += neighbours[j];
    }
    return potential;
}

double Solver::convolve(const int& cellIdx)
{
    double potential{ 0 };
    int kernelWidth = std::sqrt(kernel.size());
    std::vector<double> neighbours = findNeighbours(cellIdx, kernelWidth);
    for (int j = 0; j < neighbours.size(); j++)
    {
        potential += neighbours[j] * kernel[j];
    }
    return potential;
}

void Solver::readConfiguration()
{
    std::string path = PATH_RESOURCES"/config.json";
    std::ifstream is(path, std::ifstream::binary);
    if (is)
    {
        boost::json::value jv = boost::json::parse(is);

        // General configuration
        boost::json::object generalObj = jv.at("general").as_object();

        // Grid
        boost::json::object gridObj = generalObj.at("grid").as_object();
        gridWidth = gridObj.at("width").as_int64();
        gridHeight = gridObj.at("height").as_int64();
    }
}

std::vector<double> Solver::findNeighbours(const int& cellIdx, const int& kernelWidth)
{
    // Find the neighbours in a square around the cell
    // If out of bounds and the cell itself, the resulting value inside the output vector is 0
    // Else, the resulting value inside the output vector is the value of the cell
    int kernelHalf = std::floor(kernelWidth / 2);
    int kernelSize = kernelWidth * kernelWidth;
    std::vector<double> result;
    for (int i = 0; i < kernelSize; i++)
    {
        int row = -kernelHalf + std::floor(i / kernelWidth);
        int col = -kernelHalf + (i % kernelWidth);
        bool validCol = (cellIdx % gridWidth) + col >= 0 && (cellIdx % gridWidth) + col < gridWidth;
        bool validRow = std::floor(cellIdx / gridHeight) + row >= 0 
            && std::floor(cellIdx / gridHeight) + row < gridWidth;
        int idx = cellIdx + row * gridWidth + col;
        bool notHimself = idx != cellIdx;

        if (validRow && validCol && notHimself)
        {
            result.push_back(grid[idx]);
        }
        else
        {
            result.push_back(0.0);
        }
    }

    return result;
}

std::vector<double> Solver::buildDonutKernel(const int& width, const int& innerRadius, const int& outerRadius)
{
    std::vector<double> result;
    int size = width * width;
    double centerRadius = (outerRadius + innerRadius) / 2.0;
    for (int i = 0; i < size; i++)
    {
        double row = -(width / 2) + std::floor(i / width);
        double col = -(width / 2) + (i % width);
        double r = std::sqrt(row * row + col * col);
        double c = -.25 * (outerRadius - innerRadius) * (outerRadius - innerRadius);
        double val = std::max(0.0, ((r - innerRadius) * (r - outerRadius)) / c);
        if (r < innerRadius || r > outerRadius)
        {
            result.push_back(0.0);
        }
        else
        {
            result.push_back(val);
        }
    }
    return result;
}
