#pragma once

#include "ShiftToolButton.h"

#include <QPainter>

namespace Shift
{
	class ToolMenuButton : public Shift::ToolButton
	{
		enum State
		{
			NOT_PRESSED,
			PRESSED
		};

		Q_OBJECT

	public:
		Shift::ToolMenuButton(QWidget* parent = nullptr);
		~ToolMenuButton();

	protected:
		void paintEvent(QPaintEvent* event);

	private slots:
		void OnClick();

	private:
		State m_state;

	};
}