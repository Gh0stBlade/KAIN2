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

#define MAX_GAME_INSTANCE_COUNT (6)

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
		
		void addNewViewport();

	private:
		QDockWidget* m_viewportWidget;
		QStackedLayout* m_stackedLayout;
		QTabWidget* m_tabWidget;

#if defined(D3D9)
		Shift::D3D9Frame* m_viewport[MAX_GAME_INSTANCE_COUNT];
#elif defined(D3D11)
		Shift::D3D11Frame* m_viewport[MAX_GAME_INSTANCE_COUNT];
#elif defined(OGL)
		Shift::OGLFrame* m_viewport[MAX_GAME_INSTANCE_COUNT];
#endif
		Shift::Label* m_labelItemsCount;
		QLabel* m_labelUntitled;
		QTimer* m_timer[MAX_GAME_INSTANCE_COUNT];

		Shift::Panes* m_panes;

		int m_numViewports;
	};
}