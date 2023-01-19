#pragma once

#include <QPaintDeviceWindow.h>
#if 0
namespace Shift
{
	class D3D11WindowPrivate;

	class D3D11Window : public QPaintDeviceWindow
	{
		Q_DECLARE_PRIVATE(D3D11Window)
		Q_OBJECT

	public:
		D3D11Window(QWindow* parent = Q_NULLPTR);

		virtual void paintD3D();
		virtual void afterPresent();

	protected:
		void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
		void resizeEvent(QResizeEvent*) Q_DECL_OVERRIDE;

	private:
		Q_DISABLE_COPY(D3D11Window)
	};
}
#endif