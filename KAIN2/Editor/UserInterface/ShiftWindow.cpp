#include "ShiftWindow.h"
#include "./ui_shiftwindow.h"

#include "UserInterface/Editor_Window.h"
#include "UserInterface/Editor_UI.h"
#include <qimagereader.h>

class ShiftWindow* g_ShiftWindow;

ShiftWindow::ShiftWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ShiftWindow)
{
    ui->setupUi(this);
    DoUserInterface();
    g_ShiftWindow = this;
}

extern const char* renderBackendName;

void ShiftWindow::DoUserInterface()
{
    char windowNameBuff[128];
    sprintf(windowNameBuff, "Shift Editor (%s)", renderBackendName);
    setWindowTitle(windowNameBuff);

    DoMenuBar();
    DoToolBar();
    DoToolBox();
    DoLeftPane();
    DoCenterPane();
    DoRightPane();
    DoBottomPane();
}

void ShiftWindow::DoMenuBar()
{
    m_panes.m_menuBar = new Shift::MenuBar(this);
    setMenuBar(m_panes.m_menuBar);
}

void ShiftWindow::DoToolBar()
{
    m_panes.m_toolBar = new Shift::ToolBar(this);
    addToolBar(m_panes.m_toolBar);
    m_panes.m_toolBar->lower();
}

void ShiftWindow::DoToolBox()
{
    m_panes.m_toolBox = new Shift::ToolBox(this);
}

void ShiftWindow::DoLeftPane()
{
    m_panes.m_leftPane = new Shift::LeftPane(this);
    addDockWidget(Qt::LeftDockWidgetArea, m_panes.m_leftPane->getZoneSliceManagerWidget());
    addDockWidget(Qt::LeftDockWidgetArea, m_panes.m_leftPane->getPlacementBrowserWidget());
}

void ShiftWindow::DoCenterPane()
{
    m_panes.m_centerPane = new Shift::CenterPane(&m_panes, this);
    setCentralWidget(m_panes.m_centerPane->getViewportDockWidget());
}

void ShiftWindow::DoRightPane()
{
    m_panes.m_rightPane = new Shift::RightPane(this);
    addDockWidget(Qt::RightDockWidgetArea, m_panes.m_rightPane->getShiftPropertyEditorWidget());
}

void ShiftWindow::DoBottomPane()
{
    m_panes.m_bottomPane = new Shift::BottomPane(this);
    addDockWidget(Qt::BottomDockWidgetArea, m_panes.m_bottomPane->getConsoleWidget());
}

ShiftWindow::~ShiftWindow()
{
    if (m_panes.m_menuBar != nullptr)
    {
        delete m_panes.m_menuBar;
        m_panes.m_menuBar = nullptr;
    }

    if (m_panes.m_toolBar != nullptr)
    {
        delete m_panes.m_toolBar;
        m_panes.m_toolBar = nullptr;
    }

    if (m_panes.m_leftPane != nullptr)
    {
        delete m_panes.m_leftPane;
        m_panes.m_leftPane = nullptr;
    }

    if (m_panes.m_centerPane != nullptr)
    {
        delete m_panes.m_centerPane;
        m_panes.m_centerPane = nullptr;
    }

    if (m_panes.m_rightPane != nullptr)
    {
        delete m_panes.m_rightPane;
        m_panes.m_rightPane = nullptr;
    }

    if (m_panes.m_bottomPane != nullptr)
    {
        delete m_panes.m_bottomPane;
        m_panes.m_bottomPane = nullptr;
    }

    if (m_panes.m_toolBox != nullptr)
    {
        delete m_panes.m_toolBox;
        m_panes.m_toolBox = nullptr;
    }
}

