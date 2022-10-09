#pragma once

#include "ShiftToolButton.h"
#include <QTabWidget>
#include <QLabel>
#include <QHBoxLayout>

namespace Shift
{
	class ToolBox : public QTabWidget
	{
		Q_OBJECT

	public:
		Shift::ToolBox(QWidget* parent = nullptr);
		~ToolBox();

	private:
		QHBoxLayout* m_generalLayout;
		QWidget* m_generalWidget;
		Shift::ToolButton* m_toolButton28;
		Shift::ToolButton* m_toolButton29;
		Shift::ToolButton* m_toolButton30;
		Shift::ToolButton* m_toolButton31;
		QLabel* m_labelGeneral;
		QLabel* m_labelObjects;
		QLabel* m_labelDesign;
		QLabel* m_labelPhysics;
		QLabel* m_labelAI;
		QLabel* m_labelVFX;
		QLabel* m_labelCamera;
		QLabel* m_labelArt;
		QLabel* m_labelLighting;
		QLabel* m_labelAudio;

	private slots:
		void DoScreenShot();
	};
}