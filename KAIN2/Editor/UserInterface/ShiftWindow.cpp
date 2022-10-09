#include "ShiftWindow.h"
#include "./ui_shiftwindow.h"

#include "UserInterface/Editor_Window.h"
#include "UserInterface/Editor_UI.h"
#include <qimagereader.h>


ShiftWindow::ShiftWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ShiftWindow)
{
    ui->setupUi(this);
    DoUserInterface();
}

void ShiftWindow::DoUserInterface()
{
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
    }

    if (m_panes.m_toolBar != nullptr)
    {
        delete m_panes.m_toolBar;
    }

    if (m_panes.m_leftPane != nullptr)
    {
        delete m_panes.m_leftPane;
    }

    if (m_panes.m_centerPane != nullptr)
    {
        delete m_panes.m_centerPane;
    }

    if (m_panes.m_rightPane != nullptr)
    {
        delete m_panes.m_rightPane;
    }

    if (m_panes.m_bottomPane != nullptr)
    {
        delete m_panes.m_bottomPane;
    }

    if (m_panes.m_toolBox != nullptr)
    {
        delete m_panes.m_toolBox;
    }
}

