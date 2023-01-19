#pragma once

#include <QMenuBar>
#include <QLabel>
#include <QDialog>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>

namespace Shift
{
	class MenuBar : public QMenuBar
	{
		Q_OBJECT

	public:
		Shift::MenuBar(QWidget* parent = nullptr);
		

	private:
		QMenu* m_zoneMenu;
		QMenu* m_editMenu;
		QMenu* m_scriptsMenu;
		QMenu* m_windowsMenu;
		QMenu* m_unsupportedMenu;
		QMenu* m_helpMenu;

		QLabel* m_labelVersion;
		QLabel* m_labelWhatsNew;

		QDialog* m_zoneOpenDialog;
		QHBoxLayout* m_vbox;
		QGroupBox* m_groupBox;
		QLabel* m_labelZone;
		QComboBox* m_zoneComboBox;
		QPushButton* m_openButton;

	private slots:
		void DoOpenZone();
		void OpenZone();
	};
}