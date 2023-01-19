#include "ShiftToolBox.h"

#include "ShiftWindow.h"

#include <thread>

extern bool g_wipeScreen;
extern int g_gameInstanceCount;

Shift::ToolBox::ToolBox(QWidget* parent) : QTabWidget(parent)
{
	setObjectName(tr("ShiftToolBox"));
	m_generalWidget = new QWidget;

	m_toolButton28 = new Shift::ToolButton();
	m_toolButton28->setObjectName(tr("toolbar28"));
	m_toolButton28->setIcon(QIcon(":/Dark/Toolbar28.png"));
	m_toolButton28->setMinimumSize(26, 26);
	m_toolButton28->setMaximumSize(26, 26);

	m_toolButton29 = new Shift::ToolButton();
	m_toolButton29->setObjectName(tr("toolbar29"));
	m_toolButton29->setIcon(QIcon(":/Dark/Toolbar29.png"));
	m_toolButton29->setMinimumSize(26, 26);
	m_toolButton29->setMaximumSize(26, 26);

	m_toolButton30 = new Shift::ToolButton();
	m_toolButton30->setObjectName(tr("toolbar30"));
	m_toolButton30->setIcon(QIcon(":/Dark/Toolbar30.png"));
	m_toolButton30->setMinimumSize(26, 26);
	m_toolButton30->setMaximumSize(26, 26);
	connect(m_toolButton30, SIGNAL(pressed()), this, SLOT(DoScreenShot()));

	m_toolButton31 = new Shift::ToolButton();
	m_toolButton31->setObjectName(tr("toolbar31"));
	m_toolButton31->setIcon(QIcon(":/Dark/Toolbar31.png"));
	m_toolButton31->setMinimumSize(26, 26);
	m_toolButton31->setMaximumSize(26, 26);
	connect(m_toolButton31, SIGNAL(pressed()), this, SLOT(DoDebugGame()));

	m_toolButton32 = new Shift::ToolButton();
	m_toolButton32->setObjectName(tr("toolbar32"));
	m_toolButton32->setIcon(QIcon(":/Dark/Toolbar32.png"));
	m_toolButton32->setMinimumSize(26, 26);
	m_toolButton32->setMaximumSize(26, 26);
	connect(m_toolButton32, SIGNAL(pressed()), this, SLOT(DoPlayGame()));

	m_toolButton33 = new Shift::ToolButton();
	m_toolButton33->setObjectName(tr("toolbar33"));
	m_toolButton33->setIcon(QIcon(":/Dark/Toolbar33.png"));
	m_toolButton33->setMinimumSize(26, 26);
	m_toolButton33->setMaximumSize(26, 26);
	connect(m_toolButton33, SIGNAL(pressed()), this, SLOT(DoStopGame()));

	m_toolButton34 = new Shift::ToolButton();
	m_toolButton34->setObjectName(tr("toolbar34"));
	m_toolButton34->setIcon(QIcon(":/Dark/Toolbar34.png"));
	m_toolButton34->setMinimumSize(26, 26);
	m_toolButton34->setMaximumSize(26, 26);
	connect(m_toolButton34, SIGNAL(pressed()), this, SLOT(DoMorph()));

	m_generalLayout = new QHBoxLayout;
	m_generalLayout->setSpacing(0);
	m_generalLayout->setContentsMargins(0, 0, 0, 0);
	m_generalLayout->setSizeConstraint(QLayout::SetMaximumSize);
	m_generalLayout->addWidget(m_toolButton28);
	m_generalLayout->addWidget(m_toolButton29);
	m_generalLayout->addWidget(m_toolButton30);
	m_generalLayout->addWidget(m_toolButton31);
	m_generalLayout->addWidget(m_toolButton32);
	m_generalLayout->addWidget(m_toolButton33);
	m_generalLayout->addWidget(m_toolButton34);
	m_generalWidget->setLayout(m_generalLayout);

	m_labelObjects = new QLabel;
	m_labelDesign = new QLabel;
	m_labelPhysics = new QLabel;
	m_labelAI = new QLabel;
	m_labelVFX = new QLabel;
	m_labelCamera = new QLabel;
	m_labelArt = new QLabel;
	m_labelLighting = new QLabel;
	m_labelAudio = new QLabel;

	m_labelObjects->setText("Objects");
	m_labelDesign->setText("Design");
	m_labelPhysics->setText("Physics");
	m_labelAI->setText("AI");
	m_labelVFX->setText("VFX");
	m_labelCamera->setText("Camera");
	m_labelArt->setText("Art");
	m_labelLighting->setText("Lighting");
	m_labelAudio->setText("Audio");

	addTab(m_generalWidget, QObject::tr("General"));
	addTab(m_labelObjects, QObject::tr("Objects"));
	addTab(m_labelDesign, QObject::tr("Design"));
	addTab(m_labelPhysics, QObject::tr("Physics"));
	addTab(m_labelAI, QObject::tr("AI"));
	addTab(m_labelVFX, QObject::tr("VFX"));
	addTab(m_labelCamera, QObject::tr("Camera"));
	addTab(m_labelArt, QObject::tr("Art"));
	addTab(m_labelLighting, QObject::tr("Lighting"));
	addTab(m_labelAudio, QObject::tr("Audio"));
	setMaximumSize(550, 50);
	setMinimumSize(550, 50);
	move(820, 4);
}

Shift::ToolBox::~ToolBox()
{
	if (m_toolButton28 != nullptr)
	{
		delete m_toolButton28;
	}

	if (m_toolButton29 != nullptr)
	{
		delete m_toolButton29;
	}

	if (m_toolButton30 != nullptr)
	{
		delete m_toolButton30;
	}

	if (m_toolButton31 != nullptr)
	{
		delete m_toolButton31;
	}

	if (m_toolButton32 != nullptr)
	{
		delete m_toolButton32;
	}

	if (m_toolButton33 != nullptr)
	{
		delete m_toolButton33;
	}

	if (m_toolButton34 != nullptr)
	{
		delete m_toolButton34;
	}

	if (m_generalLayout != nullptr)
	{
		delete m_generalLayout;
	}

	if (m_generalWidget != nullptr)
	{
		delete m_generalWidget;
	}
}

std::thread* gameThread[MAX_GAME_INSTANCE_COUNT] = {};
struct _G2AppDataVM_Type
{
	int argc;//We hack this to the instance count.
	int* argv;
};
extern struct _G2AppDataVM_Type _appDataVM;
extern int MainG2(void* appData);
extern int stopGameThread;
extern int g_DisableTouchUI;

void Shift::ToolBox::DoDebugGame()
{
	g_DisableTouchUI = 1;

	int oldTabIndex = g_ShiftWindow->getPanes()->m_centerPane->getTabWidget()->currentIndex();

	if (oldTabIndex == -1)
	{
		g_ShiftWindow->getPanes()->m_centerPane->addNewViewport();
	}

	int tabIndex = g_ShiftWindow->getPanes()->m_centerPane->getTabWidget()->currentIndex();

	if (gameThread[tabIndex] != NULL)
	{
		DoStopGame();
	}

	if (tabIndex < MAX_GAME_INSTANCE_COUNT)
	{
		gameThread[tabIndex] = new std::thread(MainG2, &_appDataVM);
	}
}

void Shift::ToolBox::DoPlayGame()
{
	g_DisableTouchUI = 1;

	int oldTabIndex = g_ShiftWindow->getPanes()->m_centerPane->getTabWidget()->currentIndex();

	if (oldTabIndex == -1)
	{
		g_ShiftWindow->getPanes()->m_centerPane->addNewViewport();
	}

	int tabIndex = g_ShiftWindow->getPanes()->m_centerPane->getTabWidget()->currentIndex();

	if (gameThread[tabIndex] != NULL)
	{
		DoStopGame();
	}

	if (tabIndex < MAX_GAME_INSTANCE_COUNT)
	{
		gameThread[tabIndex] = new std::thread(MainG2, &_appDataVM);
	}
}

void Shift::ToolBox::DoStopGame()
{
	int tabIndex = g_ShiftWindow->getPanes()->m_centerPane->getTabWidget()->currentIndex();

	if (tabIndex != -1 && gameThread[tabIndex] != NULL)
	{
		stopGameThread = 1;

		while (stopGameThread)
		{
			_sleep(100);
		}

		if (gameThread[tabIndex]->joinable())
		{
			gameThread[tabIndex]->join();
		}

		delete gameThread[tabIndex];
		gameThread[tabIndex] = NULL;

		g_wipeScreen = true;
	}
}
extern "C" {
	extern void MORPH_ToggleMorph();
}

extern void Emulator_SaveBountyList();

void Shift::ToolBox::DoMorph()
{
	if (gameThread != NULL)
	{
		MORPH_ToggleMorph();

		Emulator_SaveBountyList();
	}
}

extern void Emulator_TakeScreenshot(int mode);

void Shift::ToolBox::DoScreenShot()
{
	Emulator_TakeScreenshot(0);
}
