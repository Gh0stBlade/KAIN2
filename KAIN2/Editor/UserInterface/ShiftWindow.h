#pragma once

#include <QMainWindow>
#include <QFrame>
#include <QTimer>

#include "ShiftMenuBar.h"
#include "ShiftToolBar.h"
#include "ShiftToolBox.h"
#include "ShiftLeftPane.h"
#include "ShiftCenterPane.h"
#include "ShiftRightPane.h"
#include "ShiftBottomPane.h"
#include "ShiftPanes.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ShiftWindow; }
QT_END_NAMESPACE

class ShiftWindow : public QMainWindow
{
    Q_OBJECT

public:
    ShiftWindow(QWidget* parent = nullptr);
    ~ShiftWindow();

    Ui::ShiftWindow* getUI()
    {
        return ui;
    }

    Shift::Panes* getPanes()
    {
        return &m_panes;
    }

private:
    Ui::ShiftWindow* ui;
    void DoUserInterface();
    void DoMenuBar();
    void DoToolBar();
    void DoToolBox();
    void DoLeftPane();
    void DoCenterPane();
    void DoRightPane();
    void DoBottomPane();

    Shift::Panes m_panes;
};

extern class ShiftWindow* g_ShiftWindow;