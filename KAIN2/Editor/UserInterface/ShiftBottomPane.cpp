#include "ShiftBottomPane.h"

Shift::BottomPane::BottomPane(QWidget* parent)
{
	m_consoleWidget = new QDockWidget(QObject::tr("Console"), parent);
	m_consoleWidget->setObjectName("console");
	m_consoleWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
	//m_consoleWidget->setStyleSheet("QDockWidget#" + consoleWidget->objectName() + " { background-color: rgb(51, 51, 51); color: rgb(194, 194, 194);} QDockWidget#" + consoleWidget->objectName() + "::title { border-style: solid; border-width: 1px; border-color: rgb(169, 169, 169); background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #4F4F4F, stop: 1.0 #343434);}");
}

Shift::BottomPane::~BottomPane()
{
	if (m_consoleWidget != nullptr)
	{
		delete m_consoleWidget;
	}
}

QDockWidget* Shift::BottomPane::getConsoleWidget()
{
	return m_consoleWidget;
}
