#include "ShiftLabel.h"

Shift::Label::Label(QWidget* parent) : QLabel(parent)
{
	
}

Shift::Label::~Label()
{
	
}

void Shift::Label::paintEvent(QPaintEvent* event)
{
    QLabel::paintEvent(event);

   /* QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(QRectF(0, 0, width(), height()), 10, 10);
    QPen pen(Qt::black, 10);
    p.setPen(pen);
    p.fillPath(path, QBrush(QColor(128, 128, 255, 40)));
    p.drawPath(path);*/
}
