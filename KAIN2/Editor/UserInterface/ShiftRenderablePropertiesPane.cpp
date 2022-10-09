#include "ShiftRenderablePropertiesPane.h"

#if 0
Shift::RenderablePropertiesPane::RenderablePropertiesPane(Engine::Resource::RenderableMesh* mesh, int instance, QWidget* parent) : QVBoxLayout(parent)
{
    m_mesh = mesh;
    m_instance = instance;
    m_animSelectComboBox = new QComboBox;
    m_animSelectLabel = new QLabel;

    m_animSelectLabel->setText("DEBUGGING");
    addWidget(m_animSelectLabel);
    addWidget(m_animSelectComboBox);
    addStretch();

    for (int i = 0; i < mesh->m_animations.size(); i++)
    {
        m_animSelectComboBox->addItem(mesh->m_animationNames[i]);
    }

    connect(m_animSelectComboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(animChanged(int)));
}

Shift::RenderablePropertiesPane::~RenderablePropertiesPane()
{
    if (m_animSelectComboBox != nullptr)
    {
        delete m_animSelectComboBox;
    }

    if (m_animSelectLabel != nullptr)
    {
        delete m_animSelectLabel;
    }
}

void Shift::RenderablePropertiesPane::animChanged(int index)
{
    if (m_mesh != nullptr)
    {
        m_mesh->m_currentAnimation[m_instance] = m_mesh->m_animations[index]->mAnimID;
    }
}
#endif