#include "simulation.h"

Simulation::Simulation(QWidget* parent, unsigned int frameTime)
    : QSFMLCanvas(parent, frameTime)
{
}

Simulation::Simulation(QWidget* parent, const QPoint& position, const QSize& size, unsigned int frameTime)
    : QSFMLCanvas(parent, position, size, frameTime)
{
}

Simulation::~Simulation()
{
    delete solver;
}

void Simulation::onInit()
{
    sf::Vector2<float> size(width(), height());
    scaleWidth = simulatedWidth / width();
    scaleHeight = simulatedHeight / height();

    Background = sf::RectangleShape(size);
    Background.setFillColor(BackgroundColor);

    solver = new Solver(this);
    solver->width = width();
    solver->height = height();
    geometryDirty = true;
}

void Simulation::onScroll(const double& amt)
{
    if (amt < 0) // up Wheel
        zoomFactor += 1;
    else if (amt > 0 && zoomFactor > 1) //down Wheel
        zoomFactor -= 1;
}

void Simulation::mousePressEvent(QMouseEvent* event)
{
}

void Simulation::resizeEvent(QResizeEvent* event)
{
    // Resize grid
    if (solver) 
    {
        solver->width = width();
        solver->height = height();
        geometryDirty = true;
    }    
}

void Simulation::buildGeometry()
{
    // Init
    int w = width();
    int h = height();
    double stepX = ((double)width()) / ((double)solver->gridWidth);
    double stepY = ((double)height()) / ((double)solver->gridHeight);

    // Grid lines
    if (drawGridLines)
    {
        gridLines.clear();
        // Vertical lines
        for (int i = 0; i < solver->gridWidth - 1; i++)
        {
            sf::Vector2f size(gridLinesThickness, solver->height);
            sf::RectangleShape rec(size);
            rec.move(i * stepX + stepX, 0.0);
            gridLines.push_back(rec);
        }

        // Horizontal lines
        for (int i = 0; i < solver->gridHeight - 1; i++)
        {
            sf::Vector2f size(solver->width, gridLinesThickness);
            sf::RectangleShape rec(size);
            rec.move(0.0, i * stepY + stepY);
            gridLines.push_back(rec);
        }
    }
    
    // Cells
    cells.clear();
    int nbCells = solver->gridHeight * solver->gridWidth;
    for (size_t i = 0; i < nbCells; i++)
    {
        sf::Vector2f size(stepX, stepY);
        sf::RectangleShape rec(size);
        float x = (i % solver->gridWidth) * stepX;
        float y = std::floor(i / solver->gridHeight) * stepY;
        sf::Vector2f pos(x, y);
        rec.move(pos);
        cells.push_back(rec);
    }

    // Flag
    geometryDirty = false;
}

void Simulation::onUpdate()
{
    if (playing)
    {
        solver->solve(((double)myTimer.interval())/100);
    }
    render();
}

void Simulation::reset()
{
    pause();
}

void Simulation::render()
{
    // Clear canvas
    clear();

    // Draw background
    draw(Background);

    // Build geometry
    if (geometryDirty)
        buildGeometry();

    // Draw grid
    if (drawGridLines) // Draw lines-> add bool
    {
        for (auto& line : gridLines)
        {
            draw(line);
        }
    }

    // Draw cells
    int toto = std::sqrt(solver->kernel.size());
    int inc{ 0 };
    for (int i = 0; i < solver->grid.size(); i++)
    {
        Color c = linearGradient(solver->grid[i]);
        cells[i].setFillColor(sf::Color(c.r, c.g, c.b));
        draw(cells[i]);
    }
}

double Simulation::getScaleWidth()
{
    return scaleWidth;
}

double Simulation::getScaleHeight()
{
    return scaleHeight;
}

void Simulation::setScaleWidth(const double& amt)
{
    scaleWidth = amt;
}

void Simulation::setScaleHeight(const double& amt)
{
    scaleHeight = amt;
}