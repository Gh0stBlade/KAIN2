#include "ShiftUnitPropertiesPane.h"

#include "Game/STREAM.H"
#include "Game/GAMELOOP.H"

extern struct GameTracker thread_local gameTrackerX;

extern struct Level* g_selectedUnit;

Shift::UnitPropertiesPane::UnitPropertiesPane(QWidget* parent) : QVBoxLayout(parent)
{
	m_backColourSpectralLabel = nullptr;

	m_backColourMaterialLabel = nullptr;
	
	m_backColorSpectralR = nullptr;

	m_backColorSpectralG = nullptr;

	m_backColorSpectralB = nullptr;

	m_backColorMaterialR = nullptr;

	m_backColorMaterialG = nullptr;

	m_backColorMaterialB = nullptr;

	m_backColourSpectralLabel = new QLabel;
	m_backColourSpectralLabel->setText("Unit Back Color (Spectral):");
	m_backColourSpectralLabel->setGeometry(32, 32, 100, 40);
	addWidget(m_backColourSpectralLabel);

	m_backColorSpectralR = new QSpinBox;
	m_backColorSpectralR->setMinimum(0);
	m_backColorSpectralR->setMaximum(255);
	m_backColorSpectralR->setValue(g_selectedUnit->specturalColorR);
	addWidget(m_backColorSpectralR);
	connect(m_backColorSpectralR, SIGNAL(valueChanged(int)), this, SLOT(UpdateSpectralBackColorR()));

	m_backColorSpectralG = new QSpinBox;
	m_backColorSpectralG->setMinimum(0);
	m_backColorSpectralG->setMaximum(255);
	m_backColorSpectralG->setValue(g_selectedUnit->specturalColorG);
	addWidget(m_backColorSpectralG);
	connect(m_backColorSpectralG, SIGNAL(valueChanged(int)), this, SLOT(UpdateSpectralBackColorG()));


	m_backColorSpectralB = new QSpinBox;
	m_backColorSpectralB->setMinimum(0);
	m_backColorSpectralB->setMaximum(255);
	m_backColorSpectralB->setValue(g_selectedUnit->specturalColorB);
	addWidget(m_backColorSpectralB);
	connect(m_backColorSpectralB, SIGNAL(valueChanged(int)), this, SLOT(UpdateSpectralBackColorB()));

	m_backColourMaterialLabel = new QLabel;
	m_backColourMaterialLabel->setText("Unit Fog Color (Material):");
	m_backColourMaterialLabel->setGeometry(32, 32, 100, 40);
	addWidget(m_backColourMaterialLabel);

	m_backColorMaterialR = new QSpinBox;
	m_backColorMaterialR->setMinimum(0);
	m_backColorMaterialR->setMaximum(255);
	m_backColorMaterialR->setValue(g_selectedUnit->backColorR);
	addWidget(m_backColorMaterialR);
	connect(m_backColorMaterialR, SIGNAL(valueChanged(int)), this, SLOT(UpdateMaterialBackColorR()));

	m_backColorMaterialG = new QSpinBox;
	m_backColorMaterialG->setMinimum(0);
	m_backColorMaterialG->setMaximum(255);
	m_backColorMaterialG->setValue(g_selectedUnit->backColorG);
	addWidget(m_backColorMaterialG);
	connect(m_backColorMaterialG, SIGNAL(valueChanged(int)), this, SLOT(UpdateMaterialBackColorG()));

	m_backColorMaterialB = new QSpinBox;
	m_backColorMaterialB->setMinimum(0);
	m_backColorMaterialB->setMaximum(255);
	m_backColorMaterialB->setValue(g_selectedUnit->backColorB);
	addWidget(m_backColorMaterialB);
	connect(m_backColorMaterialB, SIGNAL(valueChanged(int)), this, SLOT(UpdateMaterialBackColorB()));

	addStretch();
}

void Shift::UnitPropertiesPane::update()
{
	
}

void Shift::UnitPropertiesPane::destroy()
{
	if (m_backColourSpectralLabel != nullptr)
	{
		delete m_backColourSpectralLabel;
		m_backColourSpectralLabel = nullptr;
	}

	if (m_backColourMaterialLabel != nullptr)
	{
		delete m_backColourMaterialLabel;
		m_backColourMaterialLabel = nullptr;
	}

	if (m_backColorSpectralR != nullptr)
	{
		delete m_backColorSpectralR;
		m_backColorSpectralR = nullptr;
	}

	if (m_backColorSpectralG != nullptr)
	{
		delete m_backColorSpectralG;
		m_backColorSpectralG = nullptr;
	}

	if (m_backColorSpectralB != nullptr)
	{
		delete m_backColorSpectralB;
		m_backColorSpectralB = nullptr;
	}

	if (m_backColorMaterialR != nullptr)
	{
		delete m_backColorMaterialR;
		m_backColorMaterialR = nullptr;
	}

	if (m_backColorMaterialG != nullptr)
	{
		delete m_backColorMaterialG;
		m_backColorMaterialG = nullptr;
	}

	if (m_backColorMaterialB != nullptr)
	{
		delete m_backColorMaterialB;
		m_backColorMaterialB = nullptr;
	}
}

Shift::UnitPropertiesPane::~UnitPropertiesPane()
{
	this->destroy();
}

void Shift::UnitPropertiesPane::UpdateSpectralBackColorR()
{
	if (g_selectedUnit != nullptr)
	{
		g_selectedUnit->specturalColorR = m_backColorSpectralR->value();
	}
}

void Shift::UnitPropertiesPane::UpdateSpectralBackColorG()
{
	if (g_selectedUnit != nullptr)
	{
		g_selectedUnit->specturalColorG = m_backColorSpectralG->value();
	}
}

void Shift::UnitPropertiesPane::UpdateSpectralBackColorB()
{
	if (g_selectedUnit != nullptr)
	{
		g_selectedUnit->specturalColorB = m_backColorSpectralB->value();
	}
}

void Shift::UnitPropertiesPane::UpdateMaterialBackColorR()
{
	if (g_selectedUnit != nullptr)
	{
		g_selectedUnit->backColorR = m_backColorMaterialR->value();
	}
}

void Shift::UnitPropertiesPane::UpdateMaterialBackColorG()
{
	if (g_selectedUnit != nullptr)
	{
		g_selectedUnit->backColorG = m_backColorMaterialG->value();
	}
}

void Shift::UnitPropertiesPane::UpdateMaterialBackColorB()
{
	if (g_selectedUnit != nullptr)
	{
		g_selectedUnit->backColorB = m_backColorMaterialB->value();
	}
}
