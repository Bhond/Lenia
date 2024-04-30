#include "solver.h"
#include "boost/json.hpp"

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
}

void Solver::init()
{
    // Configure solver
    readConfiguration();

    // Initialize objects
    grid.resize(gridWidth * gridHeight, 0.0);
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
        gridWidth = gridObj.at("width").as_double();
        gridHeight = gridObj.at("height").as_double();
    }

}