#pragma once

#include <QDialog>
#include "ui_debugoutputdialog.h"

class DebugOutputDialog : public QDialog
{
	Q_OBJECT

public:
	DebugOutputDialog(QWidget* parent = nullptr);
	~DebugOutputDialog();

	static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

private:
	Ui::DebugOutputDialogClass ui;

	static QTextEdit* s_displayText;

	QtMessageHandler m_originalHandler = nullptr;
};
