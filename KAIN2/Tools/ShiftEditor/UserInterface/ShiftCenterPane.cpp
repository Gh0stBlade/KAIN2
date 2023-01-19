#include "ShiftCenterPane.h"

#include <QPainter>
#include <QFile>

extern char g_lastAreaName[32];

int g_gameInstanceCount = 0;

Shift::CenterPane::CenterPane(Shift::Panes* panes, QWidget* parent)
{
	m_viewportWidget = new QDockWidget(QObject::tr(""));
	m_viewportWidget->setObjectName("viewport");
	m_viewportWidget->show();
	
	m_tabWidget = new QTabWidget(m_viewportWidget);
	m_tabWidget->setObjectName("ShiftViewportTabWidget");
	m_viewportWidget->setWidget(m_tabWidget);

	m_panes = panes;

	g_gameInstanceCount = 0;

	for (int i = 0; i < MAX_GAME_INSTANCE_COUNT; i++)
	{
		this->m_viewport[i] = nullptr;
		this->m_timer[i] = nullptr;
	}
}

Shift::CenterPane::~CenterPane()
{
	for (int i = 0; i < MAX_GAME_INSTANCE_COUNT; i++)
	{
		if (m_timer[i] != nullptr)
		{
			delete m_timer[i];
		}

		if (m_viewport[i] != nullptr)
		{
			delete m_viewport[i];
		}
	}

	if (m_labelItemsCount != nullptr)
	{
 		//delete m_labelItemsCount;
	}

	if (m_labelUntitled != nullptr)
	{
		//delete m_labelUntitled;
	}

	if (m_tabWidget != nullptr)
	{
		delete m_tabWidget;
	}

	if (m_viewportWidget != nullptr)
	{
		delete m_viewportWidget;
	}
}

QWidget* Shift::CenterPane::getViewportWidget()
{
	return m_viewportWidget;
}

QDockWidget* Shift::CenterPane::getViewportDockWidget()
{
	return m_viewportWidget;
}

void Shift::CenterPane::addNewViewport()
{
#if defined(D3D9)
	m_viewport[g_gameInstanceCount] = new Shift::D3D9Frame(m_panes, m_tabWidget);
	m_viewport[g_gameInstanceCount]->setObjectName("ShiftViewport");
	m_viewport[g_gameInstanceCount]->setFocusPolicy(Qt::ClickFocus);
	m_viewport[g_gameInstanceCount]->initialiseHWND((HWND)m_viewport[g_gameInstanceCount]->winId(), m_viewport[g_gameInstanceCount]->width(), m_viewport[g_gameInstanceCount]->height(), g_gameInstanceCount);
#elif defined(D3D11)
	m_viewport[g_gameInstanceCount] = new Shift::D3D11Frame(m_panes, m_tabWidget);
	m_viewport[g_gameInstanceCount]->setObjectName("ShiftViewport");
	m_viewport[g_gameInstanceCount]->setFocusPolicy(Qt::ClickFocus);
	m_viewport[g_gameInstanceCount]->initialiseHWND((HWND)m_viewport[g_gameInstanceCount]->winId(), m_viewport[g_gameInstanceCount]->width(), m_viewport[g_gameInstanceCount]->height(), g_gameInstanceCount);
#elif defined(OGL)
	m_viewport[g_gameInstanceCount] = new Shift::OGLFrame(m_panes, m_tabWidget);
	m_viewport[g_gameInstanceCount]->setObjectName("ShiftViewport");
	m_viewport[g_gameInstanceCount]->setFocusPolicy(Qt::ClickFocus);
	m_viewport[g_gameInstanceCount]->initialiseHWND((HWND)m_viewport[g_gameInstanceCount]->winId(), m_viewport[g_gameInstanceCount]->width(), m_viewport[g_gameInstanceCount]->height(), g_gameInstanceCount);
#endif

	char tabName[32];
	int tabIndex = this->m_tabWidget->currentIndex();

	if (g_gameInstanceCount == 0)
	{
		sprintf(tabName, "Untitled*");
}
	else
	{
		sprintf(tabName, "Untitled*(%d)", g_gameInstanceCount);
	}

	m_tabWidget->addTab(m_viewport[g_gameInstanceCount], tabName);

	m_tabWidget->setCurrentIndex(m_tabWidget->count()-1);
	tabIndex = this->m_tabWidget->currentIndex();

	m_timer[tabIndex] = new QTimer(m_viewport[tabIndex]);
#if defined(D3D9)
	m_viewport[tabIndex]->connect(m_timer[tabIndex], &QTimer::timeout, m_viewport[tabIndex], &Shift::D3D9Frame::render);
#elif defined(D3D11)
	m_viewport[tabIndex]->connect(m_timer[tabIndex], &QTimer::timeout, m_viewport[tabIndex], &Shift::D3D11Frame::render);
#elif defined(OGL)
	m_viewport[tabIndex]->connect(m_timer[tabIndex], &QTimer::timeout, m_viewport[tabIndex], &Shift::OGLFrame::render);
#endif
	m_timer[tabIndex]->setInterval(1000.0f / 60.0f);
	m_timer[tabIndex]->start();
}
