#include "ShiftLeftPane.h"

#include "ShiftRightPane.h"

#include "Game/STREAM.H"
#include "Game/GAMELOOP.H"

extern struct GameTracker gameTrackerX;

extern enum Shift::RightPane::PaneIndex g_activeRightPane;
extern enum Shift::RightPane::PaneIndex g_lastActiveRightPane;

Shift::LeftPane::LeftPane(QWidget* parent)
{
    m_zoneSliceManagerWidget = new QDockWidget(QObject::tr("ZoneSliceManager"), parent);

    m_groupBox = new QGroupBox;
    m_zoneSliceManagerWidget->setWidget(m_groupBox);
    m_groupBox->setGeometry(0, 23, 278, 700);
    m_groupBox->setTitle("Zones");

    m_comboBox = new QComboBox(m_groupBox);
    m_comboBox->setGeometry(23, 23, 75, 20);
    //m_groupBox->setLayout(this);

    m_zoneSliceManagerWidget->setObjectName("zoneslicemanager");
    m_zoneSliceManagerWidget->setObjectName("zoneslicemanager");
    m_zoneSliceManagerWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    m_zoneSliceManagerWidget->setFixedWidth(278);

    m_placementBrowserWidget = new QDockWidget(QObject::tr("PlacementBrowser"), parent);
    m_placementBrowserWidget->setObjectName("placementbrowser");
    m_placementBrowserWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    m_placementBrowserWidget->setFixedWidth(278);

    m_labelObjects = new QLabel;
    m_labelObjects->setText("Objects");
    m_labelMeshes = new QLabel;
    m_labelMeshes->setText("Meshes");
    m_labelOther = new QLabel;
    m_labelMeshes->setText("Other");
    m_labelUnits = new QLabel;
    m_labelMeshes->setText("Units");
    m_labelInUse = new QLabel;
    m_labelMeshes->setText("In Use");
    m_labelFav = new QLabel;
    m_labelMeshes->setText("*");

    m_placementBrowserTabWidget = new QTabWidget(m_placementBrowserWidget);
    m_placementBrowserTabWidget->setObjectName(QObject::tr("placementbrowsertab"));
    m_placementBrowserTabWidget->addTab(m_labelObjects, QObject::tr("Objects"));
    m_placementBrowserTabWidget->addTab(m_labelMeshes, QObject::tr("Meshes"));
    m_placementBrowserTabWidget->addTab(m_labelOther, QObject::tr("Other"));
    m_placementBrowserTabWidget->addTab(m_labelUnits, QObject::tr("Units"));
    m_placementBrowserTabWidget->addTab(m_labelInUse, QObject::tr("In Use"));
    m_placementBrowserTabWidget->addTab(m_labelFav, QObject::tr("*"));
    m_placementBrowserWidget->setWidget(m_placementBrowserTabWidget);

    m_timer = new QTimer(m_groupBox);
    parent->connect(m_timer, &QTimer::timeout, this, &Shift::LeftPane::update);
    m_timer->setInterval(1000.0f / 10.0f);
    m_timer->start();

    connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(zoneIndexChanged(int)));
}

Shift::LeftPane::~LeftPane()
{
    if (m_labelObjects != nullptr)
    {
        delete m_labelObjects;
        m_labelObjects = nullptr;
    }

    if (m_labelMeshes != nullptr)
    {
        delete m_labelMeshes;
        m_labelMeshes = nullptr;
    }

    if (m_labelOther != nullptr)
    {
        delete m_labelOther;
        m_labelOther = nullptr;
    }

    if (m_labelUnits != nullptr)
    {
        delete m_labelUnits;
        m_labelUnits = nullptr;
    }

    if (m_labelInUse != nullptr)
    {
        delete m_labelInUse;
        m_labelInUse = nullptr;
    }

    if (m_labelFav != nullptr)
    {
        delete m_labelFav;
        m_labelFav = nullptr;
    }

    if (m_timer != nullptr)
    {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }

    if (m_comboBox != nullptr)
    {
        delete m_comboBox;
        m_comboBox = nullptr;
    }

    if (m_groupBox != nullptr)
    {
        delete m_groupBox;
        m_groupBox = nullptr;
    }

    if (m_zoneSliceManagerWidget != nullptr)
    {
        delete m_zoneSliceManagerWidget;
        m_zoneSliceManagerWidget = nullptr;
    }

    if (m_placementBrowserWidget != nullptr)
    {
        delete m_placementBrowserWidget;
        m_placementBrowserWidget = nullptr;
    }
}

QDockWidget* Shift::LeftPane::getZoneSliceManagerWidget()
{
    return m_zoneSliceManagerWidget;
}

QDockWidget* Shift::LeftPane::getPlacementBrowserWidget()
{
    return m_placementBrowserWidget;
}

void Shift::LeftPane::update()
{
    int numUsedStreams = 0;
    for (int i = 0; i < 16; i++)
    {
        if (StreamTracker.StreamList[i].used == 2)
        {
            numUsedStreams++;
        }
    }

    if (gameTrackerX.gameMode == 0 && gameTrackerX.level != nullptr && m_comboBox->count() < numUsedStreams)
    {
        if (m_comboBox->count() != 0)
        {
            m_comboBox->clear();
        }

        for (int i = 0; i < 16; i++)
        {
            if (StreamTracker.StreamList[i].used == 2)
            {
                m_comboBox->addItem(StreamTracker.StreamList[i].level->worldName);
            }
        }
    }
}

void Shift::LeftPane::zoneIndexChanged(int index)
{
    g_lastActiveRightPane = Shift::RightPane::NONE;
    g_activeRightPane = Shift::RightPane::UNIT_PROPERTIES;

    int numUsedStreams = 0;

    for (int i = 0; i < 16; i++)
    {
        if (StreamTracker.StreamList[i].used == 2)
        {
            if (numUsedStreams == index)
            {
                g_selectedUnit = StreamTracker.StreamList[i].level;

                for (int j = 0; j < g_selectedUnit->numIntros; j++)
                {
                    struct Intro* intro = &g_selectedUnit->introList[j];

                    if (!(strcmp(intro->name, "raziel")))
                    {
                        extern struct _Position overrideEditorPosition;
                        extern struct _Rotation overrideEditorRotation;

                        overrideEditorPosition.x = 0x07C9;
                        overrideEditorPosition.y = 0xEDB0;
                        overrideEditorPosition.z = 0xEED8;

                        overrideEditorRotation.x = 0x0FC7;
                        overrideEditorRotation.y = 0x0000;
                        overrideEditorRotation.z = 0x005E;
                    }
                }
            }

            numUsedStreams++;
        }
    }
}