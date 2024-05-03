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

void Solver::solve(double dt)
{
   /* auto start = std::chrono::high_resolution_clock::now();*/

    // Dispatch work
    int endLoop = grid.size() / 8;
    std::vector<double> buff0(grid.size(), 0.0);
    for (int i = 0; i < grid.size(); i++)
        buff0[i] = grid[i];
    std::vector<double> k0(kernel.size(), 0.0);
    for (int i = 0; i < kernel.size(); i++)
        k0[i] = kernel[i];

    std::vector<double> buff1(grid.size(), 0.0);
    for (int i = 0; i < grid.size(); i++)
        buff1[i] = grid[i];
    std::vector<double> k1(kernel.size(), 0.0);
    for (int i = 0; i < kernel.size(); i++)
        k1[i] = kernel[i];
    
    std::vector<double> buff2(grid.size(), 0.0);
    for (int i = 0; i < grid.size(); i++)
        buff2[i] = grid[i];
    std::vector<double> k2(kernel.size(), 0.0);
    for (int i = 0; i < kernel.size(); i++)
        k2[i] = kernel[i];

    std::vector<double> buff3(grid.size(), 0.0);
    for (int i = 0; i < grid.size(); i++)
        buff3[i] = grid[i];
    std::vector<double> k3(kernel.size(), 0.0);
    for (int i = 0; i < kernel.size(); i++)
        k3[i] = kernel[i];

    std::vector<double> buff4(grid.size(), 0.0);
    for (int i = 0; i < grid.size(); i++)
        buff4[i] = grid[i];
    std::vector<double> k4(kernel.size(), 0.0);
    for (int i = 0; i < kernel.size(); i++)
        k4[i] = kernel[i];

    std::vector<double> buff5(grid.size(), 0.0);
    for (int i = 0; i < grid.size(); i++)
        buff5[i] = grid[i];
    std::vector<double> k5(kernel.size(), 0.0);
    for (int i = 0; i < kernel.size(); i++)
        k5[i] = kernel[i];

    std::vector<double> buff6(grid.size(), 0.0);
    for (int i = 0; i < grid.size(); i++)
        buff6[i] = grid[i];
    std::vector<double> k6(kernel.size(), 0.0);
    for (int i = 0; i < kernel.size(); i++)
        k6[i] = kernel[i];

    std::vector<double> buff7(grid.size(), 0.0);
    for (int i = 0; i < grid.size(); i++)
        buff7[i] = grid[i];
    std::vector<double> k7(kernel.size(), 0.0);
    for (int i = 0; i < kernel.size(); i++)
        k7[i] = kernel[i];
    
    std::future<std::vector<double>> fut0 = std::async(&Solver::work, this, buff0, k0, dt, 0, 7);
    std::future<std::vector<double>> fut1 = std::async(&Solver::work, this, buff1, k1, dt, 1, 7);
    std::future<std::vector<double>> fut2 = std::async(&Solver::work, this, buff2, k2, dt, 2, 7);
    std::future<std::vector<double>> fut3 = std::async(&Solver::work, this, buff3, k3, dt, 3, 7);
    std::future<std::vector<double>> fut4 = std::async(&Solver::work, this, buff4, k4, dt, 4, 7);
    std::future<std::vector<double>> fut5 = std::async(&Solver::work, this, buff5, k5, dt, 5, 7);
    std::future<std::vector<double>> fut6 = std::async(&Solver::work, this, buff6, k6, dt, 6, 7);
    std::future<std::vector<double>> fut7 = std::async(&Solver::work, this, buff7, k7, dt, 7, 7);
    std::vector<double> buffer0 = fut0.get();
    std::vector<double> buffer1 = fut1.get();
    std::vector<double> buffer2 = fut2.get();
    std::vector<double> buffer3 = fut3.get();
    std::vector<double> buffer4 = fut4.get();
    std::vector<double> buffer5 = fut5.get();
    std::vector<double> buffer6 = fut6.get();
    std::vector<double> buffer7 = fut7.get();

    // Combine work
    std::vector<double> newGrid(grid.size(), 0.0);
    for (int i = 0; i < buffer0.size(); i++)
    {
        newGrid[i                 + 7 * gridWidth * std::floor(i / gridWidth)] = buffer0[i];
        newGrid[i + 1 * gridWidth + 7 * gridWidth * std::floor(i / gridWidth)] = buffer1[i];
        newGrid[i + 2 * gridWidth + 7 * gridWidth * std::floor(i / gridWidth)] = buffer2[i];
        newGrid[i + 3 * gridWidth + 7 * gridWidth * std::floor(i / gridWidth)] = buffer3[i];
        newGrid[i + 4 * gridWidth + 7 * gridWidth * std::floor(i / gridWidth)] = buffer4[i];
        newGrid[i + 5 * gridWidth + 7 * gridWidth * std::floor(i / gridWidth)] = buffer5[i];
        newGrid[i + 6 * gridWidth + 7 * gridWidth * std::floor(i / gridWidth)] = buffer6[i];
        newGrid[i + 7 * gridWidth + 7 * gridWidth * std::floor(i / gridWidth)] = buffer7[i];
    }

    //auto stop = std::chrono::high_resolution_clock::now();
    //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    //std::cout << "Duration: " << duration.count() << "\n----------------------------------------------------" << std::endl;

    // Copy new value inside resulting grid
    for (int i = 0; i < grid.size(); i++)
        grid[i] = newGrid[i];
}

std::vector<double> Solver::work(std::vector<double> grid, std::vector<double> kernel, double dt, int start, int skip)
{
    std::vector<double> res;
    int gridWidth = std::sqrt(grid.size());
    int kernelWidth = std::sqrt(kernel.size());
    int firstIdx = start * gridWidth;
    for (int i = firstIdx; i < grid.size(); i++)
    {
        // Compute potential
        double potential = convolve(grid, kernel, i, kernelWidth);

        // Compute growth rate
        double tmp = -std::pow(potential - 0.15, 2) / (2 * 0.015);
        double growth = 2 * std::exp(tmp) - 1;
        res.push_back(std::max(0.0, std::min(1.0, grid[i] + growth * dt)));

        if ((i + 1) % gridWidth == 0)
        {
            i += skip * gridWidth;
        }
    }
    return res;
}

double Solver::computePotential(std::vector<double>& grid, int& cellIdx)
{
    double potential{ 0 };
    std::vector<double> neighbours = findNeighbours(grid, cellIdx, 3);
    for (int j = 0; j < neighbours.size(); j++)
    {
        potential += neighbours[j];
    }
    return potential;
}

double Solver::convolve(std::vector<double>& grid, std::vector<double>& kernel, const int& cellIdx, const int& kernelWidth)
{
    double potential{ 0 };
    std::vector<double> neighbours = findNeighbours(grid, cellIdx, kernelWidth);
    for (int j = 0; j < neighbours.size(); j++)
    {
        potential += neighbours[j] * kernel[j];
    }
    return potential;
}

std::vector<double> Solver::findNeighbours(std::vector<double>& grid,
    const int& cellIdx,
    const int& kernelWidth)
{
    // Find the neighbours in a square around the cell
    // If out of bounds, the resulting value inside the output vector is 0
    // Else, the resulting value inside the output vector is the value of the cell
    int kernelHalf = std::floor(kernelWidth / 2);
    int kernelSize = kernelWidth * kernelWidth;
    int gridSize = std::sqrt(grid.size());
    std::vector<double> result;
    for (int i = 0; i < kernelSize; i++)
    {
        int row = -kernelHalf + std::floor(i / kernelWidth);
        int col = -kernelHalf + (i % kernelWidth);
        bool validCol = (cellIdx % gridSize) + col >= 0 && (cellIdx % gridSize) + col < gridSize;
        bool validRow = std::floor(cellIdx / gridSize) + row >= 0
            && std::floor(cellIdx / gridSize) + row < gridSize;
        int idx = cellIdx + row * gridSize + col;

        if (validRow && validCol)
            result.push_back(grid[idx]);
        else
            result.push_back(0.0);
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
