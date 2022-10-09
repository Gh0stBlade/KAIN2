#pragma once

#include <QGroupBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>

#if 0
namespace Shift
{
    class RenderablePropertiesPane : public QVBoxLayout
    {
        Q_OBJECT

    public:
        RenderablePropertiesPane(Engine::Resource::RenderableMesh* mesh, int instance, QWidget* parent = nullptr);
        ~RenderablePropertiesPane();

    private:
        QComboBox* m_animSelectComboBox;
        QLabel* m_animSelectLabel;
        Engine::Resource::RenderableMesh* m_mesh;
        int m_instance;

    private slots:
        void animChanged(int index);
    };
}
#endif