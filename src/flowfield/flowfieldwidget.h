#pragma once

#include "cellfield.h"
#include "ui_flowfieldwidget.h"
#include <QPainter>
#include <QWidget>

class FlowFieldWidget : public QWidget
{
	Q_OBJECT

public:
	FlowFieldWidget(QWidget *parent = nullptr);
	~FlowFieldWidget();

public slots:
	void changeFlowFieldWidth(int width);
	void changeFlowFieldHeight(int height);

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	void drawCellValues();

	Ui::FlowFieldWidgetClass ui;

	QImage m_originalMap;
	QImage m_map;
	QPainter m_painter;
	CellField m_cellField;
};
