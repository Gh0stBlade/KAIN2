#pragma once

#include <QDockWidget>
#include <QGroupBox>
#include <QLabel>
#include <QTabWidget>
#include <QObject>
#include <QComboBox>
#include <QVBoxLayout>
#include <QTimer>

namespace Shift
{
	class LeftPane : public QVBoxLayout
	{
		Q_OBJECT

	public:
		LeftPane(QWidget* parent);
		~LeftPane();
		QDockWidget* getZoneSliceManagerWidget();
		QDockWidget* getPlacementBrowserWidget();

		void update();

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
		QComboBox* m_comboBox;
		QTimer* m_timer;

	private slots:
		void zoneIndexChanged(int index);
	};
}