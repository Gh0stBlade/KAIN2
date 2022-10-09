#pragma once

#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QPixMap>
#include <QPainterPath>

namespace Shift
{
	class Label : public QLabel
	{
		Q_OBJECT

	public:
		Shift::Label(QWidget* parent = nullptr);
		~Label();

	protected:
		virtual void paintEvent(QPaintEvent* event) override;
	};
}