#include "ShiftToolButton.h"

Shift::ToolButton::ToolButton(QWidget* parent) : QToolButton(parent)
{
	m_state = NOT_PRESSED;
}

Shift::ToolButton::~ToolButton()
{
}

void Shift::ToolButton::OnClick()
{
	m_state = m_state == NOT_PRESSED ? PRESSED : NOT_PRESSED;
	setDown(m_state == PRESSED);
}
