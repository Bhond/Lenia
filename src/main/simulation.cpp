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
    solver->width = width();
    solver->height = height();
    gridLinesDirty = true;
}

void Simulation::buildGridLines()
{
    // Reset
    gridLines.clear();

    // Vertical lines
    double step = width() / ((double)solver->gridWidth);
    for (int i = 0; i < solver->gridWidth - 1; i++)
    {
        sf::Vector2f pos(gridLinesThickness, solver->height);
        sf::RectangleShape rec(pos);
        rec.move(i * step + step, 0.0);
        gridLines.push_back(rec);
    }
    
    // Horizontal lines
    step = height() / ((double)solver->gridHeight);
    for (int i = 0; i < solver->gridHeight - 1; i++)
    {
        sf::Vector2f pos(solver->height, gridLinesThickness);
        sf::RectangleShape rec(pos);
        rec.move(0.0, i * step + step);
        gridLines.push_back(rec);
    }
}

void Simulation::onUpdate()
{
    if (playing)
    {
        solver->solve(((double)myTimer.interval()) / 1000);
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

    // Draw grid
    if (drawGridLines) // Draw lines-> add bool
    {
        if (gridLinesDirty)
            buildGridLines();

        for (auto& line : gridLines)
        {
            draw(line);
        }
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