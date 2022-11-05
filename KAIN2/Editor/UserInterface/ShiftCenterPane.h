#pragma once

#include <QDockWidget>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QTimer>
#include <QStackedLayout>

#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"

#if defined(D3D9)
#include "ShiftD3D9Frame.h"
#elif defined(D3D11)
#include "ShiftD3D11Frame.h"
#elif defined(OGL)
#include "ShiftOGLFrame.h"
#endif
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

		QTabWidget* getTabWidget()
		{
			return m_tabWidget;
		}

	private:
		QDockWidget* m_viewportWidget;
		QHBoxLayout* m_boxLayout;
		QStackedLayout* m_stackedLayout;
		QTabWidget* m_tabWidget;

#if defined(D3D9)
		Shift::D3D9Frame* m_viewport;
#elif defined(D3D11)
		Shift::D3D11Frame* m_viewport;
#elif defined(OGL)
		Shift::OGLFrame* m_viewport;
#endif
		Shift::Label* m_labelItemsCount;
		QLabel* m_labelUntitled;
		QTimer* m_timer;
	};
}