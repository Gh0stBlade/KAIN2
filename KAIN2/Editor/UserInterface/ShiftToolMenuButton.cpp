#include "ShiftToolMenuButton.h"
#include <QDebug>
#include <qpaintdevicewindow>
Shift::ToolMenuButton::ToolMenuButton(QWidget* parent) : Shift::ToolButton(parent)
{
	m_state = NOT_PRESSED;
}

Shift::ToolMenuButton::~ToolMenuButton()
{
}

void Shift::ToolMenuButton::paintEvent(QPaintEvent* event)
{
	Shift::ToolButton::paintEvent(event);

	QPainter painter(this);
	QPolygon p;
	QPen pen;
	pen.setColor(QColor(148, 148, 148));
	p.append(QPoint(24, 18));
	p.append(QPoint(19, 24));
	p.append(QPoint(24, 24));

	QLinearGradient gradient(0, 0, 0, 30);
	gradient.setColorAt(0, QColor(197, 197, 197));
	gradient.setColorAt(1, QColor(188, 188, 188));

	painter.setBrush(gradient);
	painter.setPen(pen);

	painter.drawPolygon(p);
}

void Shift::ToolMenuButton::OnClick()
{
	m_state = m_state == NOT_PRESSED ? PRESSED : NOT_PRESSED;
	setDown(m_state == PRESSED);
}
