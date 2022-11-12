#pragma once

#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_INCLUDES.H"

#include <QFrame>
#include <QElapsedTimer>
#include <QResizeEvent>
#include <QLabel>
#include <QtQuick/QQuickWindow>
#include <QTabWidget.h>

#include <DirectXMath.h>
#include "ShiftPanes.h"
#include "Game/CORE.H"

namespace Shift
{
    class OGLFrame : public QFrame
    {
        Q_OBJECT

    public:
        OGLFrame(Shift::Panes* panes, QTabWidget* tabWidget, QWidget* parent = nullptr)
        {
            setAttribute(Qt::WA_NativeWindow, true);
            m_dragging = false;
            m_panes = panes;
            m_tabWidget = tabWidget;
        }

        virtual void initialiseHWND(HWND windowHandle, int width, int height);
        virtual void resizeEvent(QResizeEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void render();
        void renderWidget();

    private:
        bool m_dragging;
        QPoint m_previousMousePosition;
        QLabel* m_labelItemsCount;
        void pickRayVector(int mouseX, int mouseY, int ClientWidth, int ClientHeight, VECTOR* rayPos, VECTOR* rayDir);
        //bool rayBoxIntersect(DirectX::XMVECTOR& rpos, DirectX::XMVECTOR& rdir, Engine::Vector3& boxMin, Engine::Vector3& boxMax, DirectX::XMMATRIX& transform);
        void CheckIfRayIntersectedWithAnySceneObject(DirectX::XMVECTOR& rayPosition, DirectX::XMVECTOR& rayDirection);
        QSize getRatio(QSize currentResolution);
        Shift::Panes* m_panes;
        QTabWidget* m_tabWidget;

        void setCameraSpeed(int speed);
    };
}