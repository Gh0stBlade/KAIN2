#pragma once

#include <QDockWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>

namespace Shift
{
	class RightPane
	{
	public:
		RightPane(QWidget* parent);
		~RightPane();
		QDockWidget* getShiftPropertyEditorWidget();
		//void populateRenderableProperties(Engine::Resource::RenderableMesh* mesh, int instance);
		void populateEmpty();

	private:
		QDockWidget* m_shiftPropertyEditorWidget;
		QGroupBox* m_groupBox;
		QComboBox* m_comboBox;
		QVBoxLayout* m_layout;
	};
}