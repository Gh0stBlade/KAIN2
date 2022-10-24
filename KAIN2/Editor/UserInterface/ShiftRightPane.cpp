#include "ShiftRightPane.h"
#include "ShiftEmptyPane.h"
#include "ShiftRenderablePropertiesPane.h"
#include "ShiftUnitPropertiesPane.h"

enum Shift::RightPane::PaneIndex g_activeRightPane = Shift::RightPane::PaneIndex::NONE;
enum Shift::RightPane::PaneIndex g_lastActiveRightPane = Shift::RightPane::PaneIndex::NONE;

struct Level* g_selectedUnit = nullptr;

Shift::RightPane::RightPane(QWidget* parent)
{
    m_shiftPropertyEditorWidget = new QDockWidget(QObject::tr("ShiftPropertyEditor"), parent);
    m_shiftPropertyEditorWidget->setObjectName("shiftpropertyeditor");
    m_shiftPropertyEditorWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    m_shiftPropertyEditorWidget->setFixedWidth(278);
    m_shiftPropertyEditorWidget->setFixedHeight(750);
    m_layout = nullptr;
    m_groupBox = new QGroupBox(m_shiftPropertyEditorWidget);

    m_timer = new QTimer(m_groupBox);
    parent->connect(m_timer, &QTimer::timeout, this, &Shift::RightPane::update);
    m_timer->setInterval(1000.0f / 10.0f);
    m_timer->start();

    populateEmpty();
}

Shift::RightPane::~RightPane()
{
    if (m_groupBox != nullptr)
    {
        delete m_groupBox;
    }

    if (m_shiftPropertyEditorWidget != nullptr)
    {
        delete m_shiftPropertyEditorWidget;
    }
}

QDockWidget* Shift::RightPane::getShiftPropertyEditorWidget()
{
	return m_shiftPropertyEditorWidget;
}

void Shift::RightPane::populateUnitProperties()
{
    if (m_layout != nullptr)
    {
        delete m_layout;
    }

    m_groupBox->setTitle("Properties:");
    m_layout = new Shift::UnitPropertiesPane();
    m_groupBox->setLayout(m_layout);
    m_groupBox->setGeometry(0, 0, 278, 725);
}

void Shift::RightPane::populateEmpty()
{
    if (m_layout != nullptr)
    {
        delete m_layout;
    }

    m_groupBox->setTitle("");
    m_layout = new Shift::Pane(m_shiftPropertyEditorWidget);
    m_groupBox->setLayout(m_layout);
    m_groupBox->setGeometry(0, 23, 278, 725);
}

void Shift::RightPane::update()
{
    if (g_lastActiveRightPane != g_activeRightPane)
    {
        switch (g_activeRightPane)
        {
        case Shift::RightPane::PaneIndex::NONE:
            populateEmpty();
            break;
        case Shift::RightPane::PaneIndex::UNIT_PROPERTIES:
            populateUnitProperties();
            break;
        }
    }

    g_lastActiveRightPane = g_activeRightPane;
}
