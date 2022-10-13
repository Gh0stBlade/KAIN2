#include "ShiftBottomPane.h"

Shift::BottomPane::BottomPane(QWidget* parent)
{
	m_consoleWidget = new QDockWidget(QObject::tr("Console"), parent);
	m_consoleWidget->setObjectName("ShiftConsole");
	m_consoleWidget->setAllowedAreas(Qt::BottomDockWidgetArea);

	m_groupBox = new QGroupBox;
	m_consoleWidget->setWidget(m_groupBox);

	m_boxLayout = new QHBoxLayout(m_groupBox);

	m_textBox = new QLineEdit;
	m_textBox->setText(QObject::tr("(Enter Command)"));
	m_textBox->setObjectName("ShiftConsoleTextBox");

	m_button = new QPushButton;
	m_button->setText(QObject::tr("Run"));
	m_button->setObjectName("ShiftConsoleButton");

	m_boxLayout->addWidget(m_textBox);
	m_boxLayout->addWidget(m_button);
}

Shift::BottomPane::~BottomPane()
{
	if (m_consoleWidget != nullptr)
	{
		delete m_consoleWidget;
	}

	if (m_groupBox != nullptr)
	{
		delete m_groupBox;
	}

	if (m_textBox != nullptr)
	{
		delete m_textBox;
	}

	if (m_button != nullptr)
	{
		delete m_button;
	}
}

QDockWidget* Shift::BottomPane::getConsoleWidget()
{
	return m_consoleWidget;
}
