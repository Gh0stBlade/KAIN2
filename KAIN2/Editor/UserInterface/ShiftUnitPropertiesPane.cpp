#include "ShiftUnitPropertiesPane.h"

#include "Game/STREAM.H"
#include "Game/GAMELOOP.H"

extern struct GameTracker gameTrackerX;

Shift::UnitPropertiesPane::UnitPropertiesPane(QWidget* parent) : QVBoxLayout(parent)
{
	m_backColourSpectralLabel = new QLabel;
	m_backColourSpectralLabel->setText("Unit Back Color (Spectral):");
	m_backColourSpectralLabel->setGeometry(32, 32, 100, 40);
	addWidget(m_backColourSpectralLabel);

	m_backColorSpectralR = new QSpinBox;
	m_backColorSpectralR->setMinimum(0);
	m_backColorSpectralR->setMaximum(255);
	m_backColorSpectralR->setValue(gameTrackerX.level->specturalColorR);
	addWidget(m_backColorSpectralR);
	connect(m_backColorSpectralR, SIGNAL(valueChanged(int)), this, SLOT(UpdateSpectralBackColorR()));

	m_backColorSpectralG = new QSpinBox;
	m_backColorSpectralG->setMinimum(0);
	m_backColorSpectralG->setMaximum(255);
	m_backColorSpectralG->setValue(gameTrackerX.level->specturalColorG);
	addWidget(m_backColorSpectralG);
	connect(m_backColorSpectralG, SIGNAL(valueChanged(int)), this, SLOT(UpdateSpectralBackColorG()));


	m_backColorSpectralB = new QSpinBox;
	m_backColorSpectralB->setMinimum(0);
	m_backColorSpectralB->setMaximum(255);
	m_backColorSpectralB->setValue(gameTrackerX.level->specturalColorB);
	addWidget(m_backColorSpectralB);
	connect(m_backColorSpectralB, SIGNAL(valueChanged(int)), this, SLOT(UpdateSpectralBackColorB()));

	m_backColourMaterialLabel = new QLabel;
	m_backColourMaterialLabel->setText("Unit Fog Color (Material):");
	m_backColourMaterialLabel->setGeometry(32, 32, 100, 40);
	addWidget(m_backColourMaterialLabel);

	m_backColorMaterialR = new QSpinBox;
	m_backColorMaterialR->setMinimum(0);
	m_backColorMaterialR->setMaximum(255);
	m_backColorMaterialR->setValue(gameTrackerX.level->backColorR);
	addWidget(m_backColorMaterialR);
	connect(m_backColorMaterialR, SIGNAL(valueChanged(int)), this, SLOT(UpdateMaterialBackColorR()));

	m_backColorMaterialG = new QSpinBox;
	m_backColorMaterialG->setMinimum(0);
	m_backColorMaterialG->setMaximum(255);
	m_backColorMaterialG->setValue(gameTrackerX.level->backColorG);
	addWidget(m_backColorMaterialG);
	connect(m_backColorMaterialG, SIGNAL(valueChanged(int)), this, SLOT(UpdateMaterialBackColorG()));

	m_backColorMaterialB = new QSpinBox;
	m_backColorMaterialB->setMinimum(0);
	m_backColorMaterialB->setMaximum(255);
	m_backColorMaterialB->setValue(gameTrackerX.level->backColorB);
	addWidget(m_backColorMaterialB);
	connect(m_backColorMaterialB, SIGNAL(valueChanged(int)), this, SLOT(UpdateMaterialBackColorB()));

	addStretch();
}

Shift::UnitPropertiesPane::~UnitPropertiesPane()
{
	if (m_backColourSpectralLabel != nullptr)
	{
		delete m_backColourSpectralLabel;
	}

	if (m_backColourMaterialLabel != nullptr)
	{
		delete m_backColourMaterialLabel;
	}

	if (m_backColorSpectralR != nullptr)
	{
		delete m_backColorSpectralR;
	}

	if (m_backColorSpectralG != nullptr)
	{
		delete m_backColorSpectralG;
	}

	if (m_backColorSpectralB != nullptr)
	{
		delete m_backColorSpectralB;
	}

	if (m_backColorMaterialR != nullptr)
	{
		delete m_backColorMaterialR;
	}

	if (m_backColorMaterialG != nullptr)
	{
		delete m_backColorMaterialG;
	}

	if (m_backColorMaterialB != nullptr)
	{
		delete m_backColorMaterialB;
	}
}

void Shift::UnitPropertiesPane::UpdateSpectralBackColorR()
{
	if (gameTrackerX.level != nullptr)
	{
		gameTrackerX.level->specturalColorR = m_backColorSpectralR->value();
	}
}

void Shift::UnitPropertiesPane::UpdateSpectralBackColorG()
{
	if (gameTrackerX.level != nullptr)
	{
		gameTrackerX.level->specturalColorG = m_backColorSpectralG->value();
	}
}

void Shift::UnitPropertiesPane::UpdateSpectralBackColorB()
{
	if (gameTrackerX.level != nullptr)
	{
		gameTrackerX.level->specturalColorB = m_backColorSpectralB->value();
	}
}

void Shift::UnitPropertiesPane::UpdateMaterialBackColorR()
{
	if (gameTrackerX.level != nullptr)
	{
		gameTrackerX.level->backColorR = m_backColorMaterialR->value();
	}
}

void Shift::UnitPropertiesPane::UpdateMaterialBackColorG()
{
	if (gameTrackerX.level != nullptr)
	{
		gameTrackerX.level->backColorG = m_backColorMaterialG->value();
	}
}

void Shift::UnitPropertiesPane::UpdateMaterialBackColorB()
{
	if (gameTrackerX.level != nullptr)
	{
		gameTrackerX.level->backColorB = m_backColorMaterialB->value();
	}
}
