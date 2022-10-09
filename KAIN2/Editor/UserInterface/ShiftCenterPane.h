#pragma once

#include <QDockWidget>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QTimer>
#include <QStackedLayout>

#include "ShiftD3D11Frame.h"
#include "ShiftD3D11Window.h"
#include "ShiftLabel.h"
#include "ShiftPanes.h"

namespace Shift
{
	class CenterPane
	{
	public:
		CenterPane(Shift::Panes* panes, QWidget* parent);
		~CenterPane();
		QWidget* getViewportWidget();
		QDockWidget* getViewportDockWidget();

	private:
		QDockWidget* m_viewportWidget;
		QHBoxLayout* m_boxLayout;
		QStackedLayout* m_stackedLayout;
		QTabWidget* m_tabWidget;
		Shift::D3D11Frame* m_d3d11Viewport;
		//Shift::D3D11Window* m_d3d11Window;
		Shift::Label* m_labelItemsCount;
		QLabel* m_labelUntitled;
		QTimer* m_timer;
	};
}