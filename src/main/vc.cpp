#include "vc.h"

VC::VC()
	: QMainWindow()
{
}

VC::~VC()
{
	delete simulation;
	delete simulationFrame;
	delete playButton;
	delete pauseButton;
	delete resetButton;
	delete scaleWidthField;
	delete scaleHeightField;
}

void VC::onInit()
{
	// Parent
	resize(QSize(Width, Height));

	// Init sizes
	float anchorX = 0;
	float anchorY = 0;
	float margin = 10;

	float controlContainerWidth = Width / 4.0;
	float controlContainerHeight = Height - 2.0 * anchorY;

	// Position
	float posX = anchorX + controlContainerWidth + margin;
	float posY = anchorY;
	float simWidth = Width - posX - anchorX;
	float simHeight = controlContainerHeight;

	// Simulation
	simulation = new Simulation(this, QPoint(posX, posY), QSize(simWidth, simHeight), 1);
	simulation->show();

	// Controls
	float controlsX = 0.5 * controlContainerWidth;
	float controlsY = margin;
	float controlsStep = 10;

	playButton = new QPushButton("Play", this);
	float playButtonX = controlsX - playButton->size().width() * 0.5;
	playButton->move(QPoint(playButtonX, controlsY));

	pauseButton = new QPushButton("Pause", this);
	float pauseButtonX = controlsX - pauseButton->size().width() * 0.5;
	controlsY += controlsStep + playButton->size().height();
	pauseButton->move(QPoint(pauseButtonX, controlsY));

	/*resetButton = new QPushButton("Reset", this);
	float resetButtonX = controlsX - resetButton->size().width() * 0.5;
	controlsY += controlsStep + pauseButton->size().height();
	resetButton->move(QPoint(resetButtonX, controlsY));
	connect(resetButton, SIGNAL(clicked()), simulation, SLOT(reset()));*/

	/*scaleWidthField = new LabelValuePair("Scale width", 1.0, this);
	scaleWidthField->resize(QSize(std::min(controlContainerWidth - 2 * margin, 300.0f), 70.0));
	float scaleWidthFieldX = controlsX - scaleWidthField->size().width() * 0.5;
	controlsY += controlsStep + resetButton->size().height();
	scaleWidthField->move(QPoint(scaleWidthFieldX, controlsY));
	connect(scaleWidthField, SIGNAL(valueChanged()), this, SLOT(onScaleWidthFieldValueChanged()));

	scaleHeightField = new LabelValuePair("Scale height", 1.0, this);
	scaleHeightField->resize(QSize(std::min(controlContainerHeight - 2 * margin, 300.0f), 70.0));
	float scaleHeightFieldX = controlsX - scaleHeightField->size().width() * 0.5;
	controlsY += controlsStep + scaleWidthField->size().height();
	scaleHeightField->move(QPoint(scaleHeightFieldX, controlsY));
	connect(scaleHeightField, SIGNAL(valueChanged()), this, SLOT(onScaleHeightFieldValueChanged()));*/

	// Connect the buttons
	connect(playButton, SIGNAL(clicked()), simulation, SLOT(play()));
	connect(pauseButton, SIGNAL(clicked()), simulation, SLOT(pause()));
}

void VC::display()
{
	show();
}

void VC::onScaleWidthFieldValueChanged()
{
	simulation->setScaleWidth(scaleWidthField->getValue());
}

void VC::onScaleHeightFieldValueChanged()
{
	simulation->setScaleHeight(scaleHeightField->getValue());
}