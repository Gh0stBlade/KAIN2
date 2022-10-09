#include "ShiftCenterPane.h"

#include <QPainter>
#include <QFile>

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
#if 0
	m_d3d11Window = new Shift::D3D11Window;
	QWidget* ww = QWidget::createWindowContainer(m_d3d11Window);
	ww->setGeometry(0, 0, 400, 400);
	ww->setAutoFillBackground(false);
	m_d3d11Window->setObjectName("ShiftViewport");
	m_boxLayout->setGeometry(QRect(0,0,400,400));
#else
	m_d3d11Viewport = new Shift::D3D11Frame(panes, m_tabWidget);
	m_d3d11Viewport->setObjectName("ShiftViewport");
	m_d3d11Viewport->setFocusPolicy(Qt::ClickFocus);
	m_d3d11Viewport->initialiseHWND((HWND)m_d3d11Viewport->winId(), m_d3d11Viewport->width(), m_d3d11Viewport->height());
#endif
	

#if 0
	m_labelItemsCount = new Shift::Label(m_tabWidget);
	//m_labelItemsCount->setAutoFillBackground(false);
	//m_labelItemsCount->setText("1 Items");
	//m_labelItemsCount->move(30, 390);
	//m_labelItemsCount->raise();
	//m_labelItemsCount->setAutoFillBackground(true);
	//m_labelItemsCount->repaint();
	//m_labelItemsCount->setAttribute(Qt::WA_TranslucentBackground, true);
	//m_labelItemsCount->setAutoFillBackground(false);


	QPalette palette = m_labelItemsCount->palette();
	palette.setColor(m_labelItemsCount->backgroundRole(), Qt::yellow);
	palette.setColor(m_labelItemsCount->foregroundRole(), Qt::yellow);
	m_labelItemsCount->setPalette(palette);
#endif

	m_labelUntitled = new QLabel;
	m_labelUntitled->setText("Untitled*");
	m_tabWidget->addTab(m_d3d11Viewport, QObject::tr("Untitled*"));

	m_timer = new QTimer(m_d3d11Viewport);
	parent->connect(m_timer, &QTimer::timeout, m_d3d11Viewport, &Shift::D3D11Frame::renderWidget);
	m_timer->setInterval(1000.0f / 60.0f);
	m_timer->start();
}

Shift::CenterPane::~CenterPane()
{
	if (m_timer != nullptr)
	{
		delete m_timer;
	}

	if (m_d3d11Viewport != nullptr)
	{
		delete m_d3d11Viewport;
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
