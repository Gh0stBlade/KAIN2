#pragma once

#include <QDockWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>

#include "Game/STREAM.H"
#include "Game/GAMELOOP.H"

extern struct Level* g_selectedUnit;

namespace Shift
{
	class RightPane : public QVBoxLayout
	{
		Q_OBJECT

	public:
		enum PaneIndex
		{
			NONE,
			UNIT_PROPERTIES,
		};

		RightPane(QWidget* parent);
		~RightPane();
		QDockWidget* getShiftPropertyEditorWidget();
		//void populateRenderableProperties(Engine::Resource::RenderableMesh* mesh, int instance);
		void populateUnitProperties();
		void populateEmpty();
		void update();

	private:
		QDockWidget* m_shiftPropertyEditorWidget;
		QGroupBox* m_groupBox;
		QComboBox* m_comboBox;
		QVBoxLayout* m_layout;
		QTimer* m_timer;
	};
}