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


void Solver::solve(double dt)
{
    // Game of life: first test
    std::vector<double> newGrid(grid.size(), 0.0);
    for (int i = 0; i < grid.size(); i++)
    {
        std::vector<int> neighbours = findNeighbours(i);
        if (grid[i] == 0)
        {
            if (neighbours.size() == 3)
            {
                newGrid[i] = 1;
            }
        }
        else
        {
            if (neighbours.size() == 3 || neighbours.size() == 2)
            {
                newGrid[i] = 1;
            }
        }
    }

    for (int i = 0; i < grid.size(); i++)
        grid[i] = newGrid[i];
}

void Solver::init()
{
    // Configure solver
    readConfiguration();

    // Initialize objects
    std::mt19937 engine(42);
    std::normal_distribution<double> normal(0.5, 0.5);
    for (int i = 0; i < gridWidth * gridHeight; i++) {
        double val = std::min(1.0, std::max(0.0, normal(engine)));
        if (val <= 0.5)
            grid.push_back(0);
        else
            grid.push_back(1);
    }
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

std::vector<int> Solver::findNeighbours(const int& cellIdx)
{
    // First, let's take the 8 neighbours around the cell
    int kernelWidth = 3;
    int kernelHalf = std::floor(kernelWidth / 2);
    int kernelSize = kernelWidth * kernelWidth;
    std::vector<int> result;
    for (int i = 0; i < kernelSize; i++)
    {
        int row = -kernelHalf + std::floor(i / kernelWidth);
        int col = -kernelHalf + (i % kernelWidth);
        bool validCol = (cellIdx % gridWidth) + col >= 0 && (cellIdx % gridWidth) + col < gridWidth;
        bool validRow = std::floor(cellIdx / gridHeight) + row >= 0 
            && std::floor(cellIdx / gridHeight) + row < gridWidth;
        int idx = cellIdx + row * gridWidth + col;
        bool notHimself = idx != cellIdx;

        if (validRow && validCol && notHimself && grid[idx] == 1.0)
        {
            result.push_back(idx);
        }
    }

    return result;
}