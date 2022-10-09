#pragma once

#include <QToolButton>

namespace Shift
{
	class ToolButton : public QToolButton
	{
		enum State
		{
			NOT_PRESSED,
			PRESSED
		};

		Q_OBJECT

	public:
		Shift::ToolButton(QWidget* parent = nullptr);
		~ToolButton();

	private slots:
		void OnClick();

	private:
		State m_state;

	};
}