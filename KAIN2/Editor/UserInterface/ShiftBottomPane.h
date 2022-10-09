#pragma once

#include <QDockWidget>

namespace Shift
{
	class BottomPane
	{
	public:
		BottomPane(QWidget* parent);
		~BottomPane();
		QDockWidget* getConsoleWidget();

	private:
		QDockWidget* m_consoleWidget;
	};
}