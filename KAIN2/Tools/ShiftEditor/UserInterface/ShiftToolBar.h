#pragma once

#include <QToolBar>
#include <QLabel>
#include <QToolButton>
#include <QMenu>

#include "ShiftToolButton.h"
#include "ShiftToolMenuButton.h"

namespace Shift
{
	class ToolBar : public QToolBar
	{
		Q_OBJECT

	public:
		Shift::ToolBar(QWidget* parent = nullptr);
		~ToolBar();

	private:
		Shift::ToolButton* m_mouseButton;
		Shift::ToolButton* m_translateButton;
		Shift::ToolButton* m_rotateButton;
		Shift::ToolButton* m_scaleButton;
		Shift::ToolButton* m_toolButton5;
		Shift::ToolButton* m_toolButton6;
		Shift::ToolButton* m_toolButton7;
		Shift::ToolButton* m_toolButton8;
		Shift::ToolButton* m_toolButton9;
		Shift::ToolButton* m_toolButton10;
		Shift::ToolMenuButton* m_toolButton11;
		Shift::ToolMenuButton* m_toolButton12;
		Shift::ToolMenuButton* m_toolButton13;
		Shift::ToolMenuButton* m_toolButton14;
		Shift::ToolMenuButton* m_toolButton15;
		Shift::ToolMenuButton* m_toolButton16;
		Shift::ToolMenuButton* m_toolButton17;
		Shift::ToolMenuButton* m_toolButton18;
		Shift::ToolMenuButton* m_toolButton19;
		Shift::ToolMenuButton* m_toolButton20;
		Shift::ToolMenuButton* m_toolButton21;
		Shift::ToolMenuButton* m_toolButton22;
		Shift::ToolMenuButton* m_toolButton23;
		Shift::ToolMenuButton* m_toolButton24;
		Shift::ToolMenuButton* m_toolButton25;
		Shift::ToolMenuButton* m_toolButton26;
		Shift::ToolMenuButton* m_toolButton27;

		QMenu* m_menu1;
		QMenu* m_menu2;
		QMenu* m_menu3;
		QMenu* m_menu4;
		QMenu* m_menu5;
		QMenu* m_menu6;
		QMenu* m_menu7;
		QMenu* m_menu8;
		QMenu* m_menu9;
		QMenu* m_menu10;
		QMenu* m_menu11;
		QMenu* m_menu12;
		QMenu* m_menu13;
		QMenu* m_menu14;
		QMenu* m_menu15;
		QMenu* m_menu16;
		QMenu* m_menu17;

	private slots:
		void ToggleGrid();
	};
}