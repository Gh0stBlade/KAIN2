#pragma once

#include <QFrame>
#include <QElapsedTimer>
#include <QResizeEvent>
#include <QLabel>
#include <QtQuick/QQuickWindow>
#include <QTabWidget.h>

#include <DirectXMath.h>
#include "ShiftPanes.h"

namespace Shift
{
    class D3D11Frame : public QFrame
    {
        Q_OBJECT

    public:
        D3D11Frame(Shift::Panes* panes, QTabWidget* tabWidget, QWidget* parent = nullptr)
        {
            setAttribute(Qt::WA_PaintOnScreen, true);
            setAttribute(Qt::WA_NativeWindow, true);
            m_dragging = false;
            m_panes = panes;
            m_tabWidget = tabWidget;
        }

        virtual  QPaintEngine* paintEngine();
        virtual void initialiseHWND(HWND windowHandle, int width, int height);
        virtual void resizeEvent(QResizeEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void render();
        void renderWidget();

    protected:
        virtual void paintEvent(QPaintEvent* event) override;

    private:
        bool m_dragging;
        QPoint m_previousMousePosition;
        QLabel* m_labelItemsCount;
        void pickRayVector(float mouseX, float mouseY, float ClientWidth, float ClientHeight, DirectX::XMVECTOR& rayPos, DirectX::XMVECTOR& rayDir);
        //bool rayBoxIntersect(DirectX::XMVECTOR& rpos, DirectX::XMVECTOR& rdir, Engine::Vector3& boxMin, Engine::Vector3& boxMax, DirectX::XMMATRIX& transform);
        void CheckIfRayIntersectedWithAnySceneObject(DirectX::XMVECTOR& rayPosition, DirectX::XMVECTOR& rayDirection);
        QSize getRatio(QSize currentResolution);
        Shift::Panes* m_panes;
        QTabWidget* m_tabWidget;

        void setCameraSpeed(int speed);
    };
}