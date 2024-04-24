#include "debugoutputdialog.h"

#include <QTime>
#include <QScrollBar>

QTextEdit* DebugOutputDialog::s_displayText = nullptr;

DebugOutputDialog::DebugOutputDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setModal(false);

	s_displayText = ui.textEdit;
	m_originalHandler = qInstallMessageHandler(&DebugOutputDialog::messageHandler);
}

DebugOutputDialog::~DebugOutputDialog()
{
	qInstallMessageHandler(m_originalHandler);
	s_displayText = nullptr;
}

void DebugOutputDialog::messageHandler(QtMsgType type, const QMessageLogContext & context, const QString & msg)
{
	if (!s_displayText)
		return;

	QTextCharFormat format;
	switch (type)
	{
	case QtDebugMsg:
		format.setForeground(QColor(Qt::black));
		break;
	case QtWarningMsg:
		format.setForeground(QColor(Qt::darkYellow));
		break;
	case QtCriticalMsg:
		format.setForeground(QColor(Qt::red));
		break;
	case QtFatalMsg:
		format.setForeground(QColor(Qt::red));
		break;
	case QtInfoMsg:
		format.setForeground(QColor(Qt::black));
		break;
	default:
		format.setForeground(QColor(Qt::black));
		break;
	}

	QTextCursor cursor = s_displayText->textCursor();
	cursor.setCharFormat(format);
	s_displayText->setCurrentCharFormat(format);

	QString text = QString("%1: %2\n").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(msg);
	s_displayText->insertPlainText(text);
	s_displayText->verticalScrollBar()->setValue(s_displayText->verticalScrollBar()->maximum());
}
