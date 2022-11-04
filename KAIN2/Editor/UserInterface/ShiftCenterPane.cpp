#include "ShiftCenterPane.h"

#include <QPainter>
#include <QFile>

extern char g_lastAreaName[32];

Shift::CenterPane::CenterPane(Shift::Panes* panes, QWidget* parent)
{
	m_viewportWidget = new QDockWidget(QObject::tr(""));
	m_viewportWidget->setObjectName("viewport");

	m_boxLayout = new QHBoxLayout;
	m_viewportWidget->setLayout(m_boxLayout);
	m_viewportWidget->show();
	
	m_tabWidget = new QTabWidget(m_viewportWidget);
	m_tabWidget->setObjectName("ShiftViewportTabWidget");
	m_viewportWidget->setWidget(m_tabWidget);

#if defined(D3D11)
	m_viewport = new Shift::D3D11Frame(panes, m_tabWidget);
	m_viewport->setObjectName("ShiftViewport");
	m_viewport->setFocusPolicy(Qt::ClickFocus);
	m_viewport->initialiseHWND((HWND)m_viewport->winId(), m_viewport->width(), m_viewport->height());
#elif defined(OGL)
	m_viewport = new Shift::OGLFrame(panes, m_tabWidget);
	m_viewport->setObjectName("ShiftViewport");
	m_viewport->setFocusPolicy(Qt::ClickFocus);
	m_viewport->initialiseHWND((HWND)m_viewport->winId(), m_viewport->width(), m_viewport->height());
#endif

	m_labelUntitled = new QLabel;
	m_labelUntitled->setText("Untitled*");
	m_tabWidget->addTab(m_viewport, g_lastAreaName);

	m_timer = new QTimer(m_viewport);
#if defined(D3D11)
	parent->connect(m_timer, &QTimer::timeout, m_viewport, &Shift::D3D11Frame::render);
#else
	parent->connect(m_timer, &QTimer::timeout, m_viewport, &Shift::OGLFrame::render);
#endif
	m_timer->setInterval(1000.0f / 60.0f);
	m_timer->start();
}

Shift::CenterPane::~CenterPane()
{
	if (m_timer != nullptr)
	{
		delete m_timer;
	}

	if (m_viewport != nullptr)
	{
		delete m_viewport;
	}

	if (m_boxLayout != nullptr)
	{
		delete m_boxLayout;
	}

	if (m_labelItemsCount != nullptr)
	{
 		//delete m_labelItemsCount;
	}

	if (m_labelUntitled != nullptr)
	{
		delete m_labelUntitled;
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
