#include "ShiftD3D11Window.h"

#include "UserInterface/Editor_Window.h"
#include "UserInterface/Editor_UI.h"
#include <QtGui/private/qpaintdevicewindow_p.h>
#include <QTime>
#include <QPainter>
#include <QLabel>

#if 0

namespace Shift
{
    class D3D11WindowPrivate : public QPaintDeviceWindowPrivate
    {
        Q_DECLARE_PUBLIC(D3D11Window);

    public:
        D3D11WindowPrivate()
            : initialised(false)
        { }
        ~D3D11WindowPrivate() {};

        void beginPaint(const QRegion& region) Q_DECL_OVERRIDE;
        void flush(const QRegion& region) Q_DECL_OVERRIDE;

        void initialise();

        bool initialised;
    };
}

Shift::D3D11Window::D3D11Window(QWindow* parent) : QPaintDeviceWindow(*(new D3D11WindowPrivate), parent)
{
   // setSurfaceType(QSurface::Direct3DSurface);
}

void Shift::D3D11Window::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    paintD3D();
}

void Shift::D3D11Window::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    //Q_D(D3D11Window);

    if (!isExposed() || size().isEmpty())
        return;

    //d->resize();
    //resizeD3D(size());
    paintD3D();
    afterPresent();
}

void Shift::D3D11Window::paintD3D()
{
    static QElapsedTimer deltaTimer;
    static float deltaTime = 0.0f;

    //if (g_engine.m_initialised)
    {
        deltaTimer.start();

        //
       // g_engine.DrawScene(deltaTime, static_cast<float>(QTime::currentTime().msec()));

#if 0
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addRoundedRect(QRectF(10, 10, 100, 50), 10, 10);
        QPen pen(Qt::black, 10);
        p.setPen(pen);
        p.fillPath(path, Qt::red);
        p.drawPath(path);
        update();
#else
        QPainter painter(this);
        painter.beginNativePainting();
        painter.setPen(Qt::yellow);
        painter.drawText(0, 20, "One");
        painter.setPen(Qt::red);
        painter.drawText(QFontMetrics(painter.font()).size(Qt::TextSingleLine, "One ").width(), 20, "Two");
        painter.endNativePainting();
#endif
        deltaTime = static_cast<float>(deltaTimer.elapsed()) / 1000.0f;
        //qDebug() << "Dt:" << static_cast<float>(deltaTimer.elapsed()) / 1000.0f;
        deltaTimer.restart();
    }

    update();
}

void Shift::D3D11Window::afterPresent()
{
    QPainter painter(this);
    painter.setPen(Qt::yellow);
    painter.drawText(0, 20, "One");
    painter.setPen(Qt::red);
    painter.drawText(QFontMetrics(painter.font()).size(Qt::TextSingleLine, "One ").width(), 20, "Two");
}

void Shift::D3D11WindowPrivate::beginPaint(const QRegion& region)
{
    Q_UNUSED(region);

    initialise();
}

void Shift::D3D11WindowPrivate::flush(const QRegion& region)
{
    Q_Q(D3D11Window);
    Q_UNUSED(region);

    q->afterPresent();
}

void Shift::D3D11WindowPrivate::initialise()
{
    Q_Q(D3D11Window);

    if (initialised)
    {
        return;
    }

    //g_engine.getRenderer()->InitialiseD3D11(q->width(), q->height(), (HWND)q->winId());
    if (!Editor::InitScene()) {
        printf("Failed to initialise scene!\n");
    }

    //if (!g_engine.m_initialised) {
    //    g_engine.Initialise();
    //}
    initialised = true;
}
#endif