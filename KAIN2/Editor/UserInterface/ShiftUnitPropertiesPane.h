#pragma once

#include <QGroupBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>

namespace Shift
{
    class UnitPropertiesPane : public QVBoxLayout
    {
        Q_OBJECT

    public:
        UnitPropertiesPane(QWidget* parent = nullptr);
        ~UnitPropertiesPane();

    private:
        QLabel* m_backColourSpectralLabel;
        
        QLabel* m_backColourMaterialLabel;

        QSpinBox* m_backColorSpectralR;
        QSpinBox* m_backColorSpectralG;
        QSpinBox* m_backColorSpectralB;

        QSpinBox* m_backColorMaterialR;
        QSpinBox* m_backColorMaterialG;
        QSpinBox* m_backColorMaterialB;

    private slots:
        void UpdateSpectralBackColorR();
        void UpdateSpectralBackColorG();
        void UpdateSpectralBackColorB();

        void UpdateMaterialBackColorR();
        void UpdateMaterialBackColorG();
        void UpdateMaterialBackColorB();
    };
}
