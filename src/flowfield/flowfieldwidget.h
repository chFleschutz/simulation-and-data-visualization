#pragma once

#include "cellfield.h"
#include "ui_flowfieldwidget.h"
#include <QPainter>
#include <QWidget>
#include "imagerenderer.h"

class FlowFieldWidget : public QWidget
{
	Q_OBJECT

public:
	FlowFieldWidget(QWidget *parent = nullptr);
	~FlowFieldWidget() = default;

public slots:
	void changeFlowFieldWidth(int width);
	void changeFlowFieldHeight(int height);
	void showCells(bool show);
	void setCellSize(int size);
	void setMap(int mapIndex);

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	void drawCellValues(QPainter& painter);

	Ui::FlowFieldWidgetClass ui;

	ImageRenderer m_imageRenderer;

	CellField m_cellField;

	int m_fieldWidth = 50;
	int m_fieldHeight = 50;

	bool m_showCells = false;
	float m_cellSize = 1.0f;
};
