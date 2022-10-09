#pragma once

#include <QDockWidget>
#include <QGroupBox>
#include <QLabel>
#include <QTabWidget>
#include <QObject>

namespace Shift
{
	class LeftPane
	{
	public:
		LeftPane(QWidget* parent);
		~LeftPane();
		QDockWidget* getZoneSliceManagerWidget();
		QDockWidget* getPlacementBrowserWidget();

	private:
		QDockWidget* m_zoneSliceManagerWidget;
		QDockWidget* m_placementBrowserWidget;
		QTabWidget* m_placementBrowserTabWidget;
        QLabel* m_labelObjects;
        QLabel* m_labelMeshes;
        QLabel* m_labelOther;
        QLabel* m_labelUnits;
        QLabel* m_labelInUse;
        QLabel* m_labelFav;
		QGroupBox* m_groupBox;
	};
}