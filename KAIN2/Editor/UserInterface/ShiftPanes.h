#pragma once

namespace Shift
{
    class MenuBar;
    class ToolBar;
    class ToolBox;
    class LeftPane;
    class CenterPane;
    class RightPane;
    class BottomPane;

    struct Panes
    {
        MenuBar* m_menuBar;
        ToolBar* m_toolBar;
        ToolBox* m_toolBox;
        LeftPane* m_leftPane;
        CenterPane* m_centerPane;
        RightPane* m_rightPane;
        BottomPane* m_bottomPane;
    };
}