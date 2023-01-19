#pragma once

#include <QDockWidget>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>

namespace Shift
{
	class BottomPane
	{
	public:
		BottomPane(QWidget* parent);
		~BottomPane();
		QDockWidget* getConsoleWidget();

	private:
		QHBoxLayout* m_boxLayout;
		QDockWidget* m_consoleWidget;
		QGroupBox* m_groupBox;
		QLineEdit* m_textBox;
		QPushButton* m_button;
	};
}