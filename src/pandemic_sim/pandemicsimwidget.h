#pragma once

#include <QWidget>
#include "ui_pandemicsimwidget.h"

class PandemicSimWidget : public QWidget
{
	Q_OBJECT

public:
	PandemicSimWidget(QWidget *parent = nullptr);
	~PandemicSimWidget() = default;

public slots:
	void onUpdateFPS(float fps);

private:
	Ui::PandemicSimWidgetClass ui;
};
