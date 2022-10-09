#include "ShiftRightPane.h"
#include "ShiftEmptyPane.h"
#include "ShiftRenderablePropertiesPane.h"

Shift::RightPane::RightPane(QWidget* parent)
{
    m_shiftPropertyEditorWidget = new QDockWidget(QObject::tr("ShiftPropertyEditor"), parent);
    m_shiftPropertyEditorWidget->setObjectName("shiftpropertyeditor");
    m_shiftPropertyEditorWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    m_shiftPropertyEditorWidget->setFixedWidth(278);
    m_shiftPropertyEditorWidget->setFixedHeight(750);
    m_layout = nullptr;
    m_groupBox = new QGroupBox(m_shiftPropertyEditorWidget);

    populateEmpty();
}

Shift::RightPane::~RightPane()
{
    if (m_groupBox != nullptr)
    {
        delete m_groupBox;
    }

    if (m_comboBox != nullptr)
    {
        delete m_comboBox;
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

#if 0
void Shift::RightPane::populateRenderableProperties(Engine::Resource::RenderableMesh* mesh, int instance)
{
    if (m_layout != nullptr)
    {
        delete m_layout;
    }

    m_groupBox->setTitle("Properties:");
    m_layout = new Shift::RenderablePropertiesPane(mesh, instance, m_shiftPropertyEditorWidget);
    m_groupBox->setLayout(m_layout);
    m_groupBox->setGeometry(0, 23, 278, 725);
}
#endif

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
