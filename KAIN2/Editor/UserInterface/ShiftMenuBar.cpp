#include "ShiftMenuBar.h"

#include "Engine_Version.h"
#include "UserInterface/Editor_UI.h"
#include "Game/GAMELOOP.H"

Shift::MenuBar::MenuBar(QWidget* parent) : QMenuBar(parent)
{
    //Set the object name (used for custom styling).
    setObjectName("ShiftMenuBar");

    //Add menus.
	m_zoneMenu = addMenu(tr("Zone"));
    m_editMenu = addMenu(tr("Edit"));
    m_scriptsMenu = addMenu(tr("Scripts"));
    m_windowsMenu = addMenu(tr("Windows"));
    m_unsupportedMenu = addMenu(tr("Unsupported"));
    m_helpMenu = addMenu(tr("Help"));

    //Add actions.
    QAction* currentAction = m_zoneMenu->addAction(tr("Open"));
    connect(currentAction, SIGNAL(triggered()), this, SLOT(DoOpenZone()));
    currentAction = m_zoneMenu->addAction(tr("Close"));
    currentAction = m_zoneMenu->addAction(tr("Save"));

    m_labelVersion = new QLabel(this);
    char versionBuff[32];
    sprintf(versionBuff, "V 1.0.%d (0x%x)", g_GIT_COMMIT_COUNT, g_GIT_COMMIT_COUNT);
    m_labelVersion->setText(versionBuff);
    m_labelVersion->move(340, 4);
    m_labelVersion->setStyleSheet("color:rgb(91, 91, 91);");

    m_labelWhatsNew = new QLabel(this);
    m_labelWhatsNew->setText("What's new");
    m_labelWhatsNew->move(460, 4);
    m_labelWhatsNew->setStyleSheet("color:rgb(91, 91, 91); text-decoration: underline;");
}

void Shift::MenuBar::DoOpenZone()
{
    m_zoneOpenDialog = new QDialog;
    m_zoneOpenDialog->setWindowTitle("Open");

    m_vbox = new QHBoxLayout;
    m_groupBox = new QGroupBox;
    m_groupBox->setTitle("Open Zone");
    m_zoneOpenDialog->show();

    m_labelZone = new QLabel(m_groupBox);
    m_labelZone->setText("Zone:");
    m_zoneComboBox = new QComboBox(m_groupBox);

    const char* zoneNames[] = {
                "train6",
                "train5",
                "under4",
                "under2",
                "under3",
                "pillars9",
                "stone4",
                "stone5",
                "fire3",
                "train9",
                "push1",
    };

    for (int i = 0; i < sizeof(zoneNames) / sizeof(uintptr_t); i++)
    {
        m_zoneComboBox->addItem(tr(zoneNames[i]));
    }
    QPushButton* openButton = new QPushButton(tr("Open"), m_groupBox);
    m_labelZone->setGeometry(30, 30, 200, 26);
    m_zoneComboBox->setGeometry(90, 30, 200, 26);
    openButton->setGeometry(320, 30, 32, 32);
    m_vbox->addWidget(m_groupBox);
    m_zoneOpenDialog->setLayout(m_vbox);
    m_zoneComboBox->show();
    connect(openButton, SIGNAL(clicked()), this, SLOT(OpenZone()));
}

void Shift::MenuBar::OpenZone()
{
    char unitName[16];
    int unitNameIdx = 0;
    char unitNumber[4];
    int unitNumberIdx = 0;

    for (int i = 0; i < strlen(m_zoneComboBox->currentText().toLocal8Bit().data()); i++)
    {
        if (m_zoneComboBox->currentText().toLocal8Bit().data()[i] >= 0x30 && m_zoneComboBox->currentText().toLocal8Bit().data()[i] <= 0x39)
        {
            unitNumber[unitNumberIdx++] = m_zoneComboBox->currentText().toLocal8Bit().data()[i];
        }
        else
        {
            unitName[unitNameIdx++] = m_zoneComboBox->currentText().toLocal8Bit().data()[i];
        }
    }

    unitNumber[unitNumberIdx] = 0;
    unitName[unitNameIdx] = 0;
    
    GAMELOOP_RequestLevelChange(unitName, atoi(unitNumber), &gameTrackerX);
}
