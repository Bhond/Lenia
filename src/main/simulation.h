#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <random>

#include "SFML/Graphics.hpp"
#include <qevent.h>

#include "../main/misc.h"
#include "../graphics/qsfmlcanvas.h"
#include "../models/solver.h"

class Simulation : public QSFMLCanvas
{
	Q_OBJECT

public:
	Solver* solver{ nullptr };

private:
	const double simulatedWidth  { 1e10 };
	const double simulatedHeight { 1e10 };
	const std::string Name { "Lenia" };
	double scaleWidth { 1 };
	double scaleHeight { 1 };
	double zoomFactor { 1 };

private:
	const sf::Color BackgroundColor = sf::Color(0, 0, 0, 255);
	sf::RectangleShape Background;
	bool geometryDirty{ false };
	bool drawGridLines{ false };
	double gridLinesThickness{ 1.0 };
	std::vector<sf::RectangleShape> gridLines;
	std::vector<sf::RectangleShape> cells;

public:
	Simulation(QWidget* parent, unsigned int frameTime = 0);
	Simulation(QWidget* parent, const QPoint& position, const QSize& size, unsigned int frameTime = 0);
	~Simulation();
	
public:
	void onInit() override;
	void reset() override;
	double getScaleWidth();
	double getScaleHeight();
	void setScaleWidth(const double& amt);
	void setScaleHeight(const double& amt);

protected:
	void onUpdate() override;
	void onScroll(const double& amt) override;
	void mousePressEvent(QMouseEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	void buildGeometry();

public slots:
	void render();
};