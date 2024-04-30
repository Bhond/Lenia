#pragma once

#include <qmainwindow.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qtabwidget.h>
#include <qframe.h>

#include "simulation.h"
#include "misc.h"
#include "../graphics/labelvaluepair.h"
#include "../graphics/labelprogressbarpair.h"

class VC : public QMainWindow
{
	Q_OBJECT

private:
	const float Width = 1500;
	const float Height = 900;

	Simulation* simulation{ nullptr };
	QFrame* simulationFrame{ nullptr };
	QPushButton* playButton{ nullptr };
	QPushButton* pauseButton{ nullptr };
	QPushButton* resetButton{ nullptr };
	LabelValuePair* scaleWidthField{ nullptr };
	LabelValuePair* scaleHeightField{ nullptr };

public:
	VC();
	~VC();

	void onInit();
	void display();

private slots:
	void onScaleWidthFieldValueChanged();
	void onScaleHeightFieldValueChanged();
};
