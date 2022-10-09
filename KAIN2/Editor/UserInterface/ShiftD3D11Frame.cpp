#include "ShiftD3D11Frame.h"

#include "UserInterface/Editor_Window.h"
#include "UserInterface/Editor_UI.h"

#include <QTime>
#include <QPainter>

#include "ShiftRightPane.h"

void Shift::D3D11Frame::initialiseD3D11(HWND windowHandle, int width, int height)
{
#if 1//OLD_RENDERER || 1
    //g_engine.getRenderer()->InitialiseD3D11(1920, 1080, windowHandle);

    if (!Editor::InitScene()) {
        printf("Failed to initialise scene!\n");
    }

    //if (!g_engine.m_initialised) {
    //    g_engine.Initialise();
   // }
#else
    Shift::Init init;
    Shift::PCSettings settings;

    init.PreInitialiseDX(true, &settings, false);

    bool firstTime[4];
    firstTime[0] = settings.ReadFromRegistry(L"SOFTWARE\\Crystal Dynamics\\Tomb Raider\\Graphics");

    Shift::GameWindowDX11::m_window = windowHandle;

    init.InitialiseDX(Shift::GameWindowDX11::m_window, &settings);
    Shift::CoreSystems coreSystems;
    coreSystems.Init();
#endif
}

void Shift::D3D11Frame::render()
{

}

void Shift::D3D11Frame::paintEvent(QPaintEvent* event)
{
    //QFrame::paintEvent(event);

    static QElapsedTimer deltaTimer;
    static float deltaTime = 0.0f;
    
    //if (g_engine.m_initialised)
    {
        deltaTimer.start();

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
        //QPainter painter(this);
       // painter.setPen(Qt::yellow);
       // painter.drawText(0, 20, "One");
       // painter.setPen(Qt::red);
       // painter.drawText(QFontMetrics(painter.font()).size(Qt::TextSingleLine, "One ").width(), 20, "Two");
#endif
        deltaTime = static_cast<float>(deltaTimer.elapsed()) / 1000.0f;
        //g_engine.setCurrentDeltaTime(deltaTime);
        //qDebug() << "Dt:" << static_cast<float>(deltaTimer.elapsed()) / 1000.0f;
        deltaTimer.restart();
    }
}

QPaintEngine* Shift::D3D11Frame::paintEngine()
{
    return NULL;
}

void Shift::D3D11Frame::resizeEvent(QResizeEvent* event)
{
#if 0
    if (g_engine.m_initialised)
    {
        int oldW = event->size().width();
        int oldH = event->size().height();

        int pixelRatio = devicePixelRatio();
        int newW = width() * pixelRatio + (width() * pixelRatio % 2);
        int newH = height() * devicePixelRatio();

        QSize currentSize = QSize(newW, newH);
        QSize newSize = getRatio(currentSize);
#if defined(_DEBUG)
        //newSize.setWidth(1920);
        //newSize.setHeight(1080);
#endif
        g_engine.getRenderer()->ResizeBuffers(newSize.width(), newSize.height());
    }
#endif
}

void Shift::D3D11Frame::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space)
    {
        //g_engine.loadZone("survival_den_puzzleroom");
        //g_engine.loadZone("qt_stalkerfight");
        //g_engine.loadZone("mb_readyroom");
        //g_engine.loadZone("oceanvista");
        //g_engine.loadZone("test_leveleditor1");
        //g_engine.loadZone("survival_den97");
        //g_engine.loadZone("main_menu_1");
        //g_engine.loadZone("tr_06_monastery");
        //g_engine.loadModel("v2_lara");
        Editor::UI::InitialiseCamera();
    }

    if (event->key() == Qt::Key_M)
    {
        //MenuManager menuManager;
        //menuManager.Init();
    }

    int flags = 0;

    //if (event->key() == Qt::Key_W) flags |= Engine::Camera::MOVE_FORWARD;
    //if (event->key() == Qt::Key_S) flags |= Engine::Camera::MOVE_BACK;
    //if (event->key() == Qt::Key_A) flags |= Engine::Camera::MOVE_LEFT;
    //if (event->key() == Qt::Key_D) flags |= Engine::Camera::MOVE_RIGHT;

#define CAMERA_MAX_SPEED 10000.0f

    float speed = 0.0f;// (g_engine.getCurrentDeltaTime())* CAMERA_MAX_SPEED;

    if (speed != 0.0f)
    {
        int testing = 0;
        testing++;
    }

    if (event->modifiers() & Qt::ControlModifier)  speed /= 4.0f;
    if (event->modifiers() & Qt::ShiftModifier) speed *= 4.0f;

    //Engine::g_camera.move(flags, speed);
}

void Shift::D3D11Frame::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        ShowCursor(false);
        m_previousMousePosition = event->pos();
        m_dragging = true;
    }
}

void Shift::D3D11Frame::pickRayVector(float mouseX, float mouseY, float ClientWidth, float ClientHeight, DirectX::XMVECTOR& rayPos, DirectX::XMVECTOR& rayDir)
{
#if 0
    //Normalized device coordinates
    DirectX::XMVECTOR pickRayViewSpace = DirectX::XMVectorSet(
        mouseX = (((2.0f * mouseX) / ClientWidth) - 1) / Engine::g_camera.m_projectionMatrix.r[0].m128_f32[0],
        mouseY = -(((2.0f * mouseY) / ClientHeight) - 1) / Engine::g_camera.m_projectionMatrix.r[1].m128_f32[1],
        1.0f,
        0.0f);

    DirectX::XMVECTOR pickRayViewSpacePos = DirectX::XMVectorZero();
    DirectX::XMVECTOR pickRayViewSpaceDir = pickRayViewSpace;
    DirectX::XMMATRIX pickRayWorldSpace = DirectX::XMMatrixInverse(nullptr, Engine::g_camera.m_viewMatrix);

    pickRayViewSpacePos = DirectX::XMVector3TransformCoord(pickRayViewSpacePos, pickRayWorldSpace);
    pickRayViewSpaceDir = DirectX::XMVector3TransformNormal(pickRayViewSpaceDir, pickRayWorldSpace);

    rayPos = pickRayViewSpacePos;
    rayDir = pickRayViewSpaceDir;
#endif
}

#if 0
bool Shift::D3D11Frame::rayBoxIntersect(DirectX::XMVECTOR& rpos, DirectX::XMVECTOR& rdir, Engine::Vector3& boxMin, Engine::Vector3& boxMax, DirectX::XMMATRIX& transform)
{
    Engine::Vector3 dirfrac;
    // r.dir is unit direction vector of ray
    dirfrac.x = 1.0f / rdir.m128_f32[0];
    dirfrac.y = 1.0f / rdir.m128_f32[1];
    dirfrac.z = 1.0f / rdir.m128_f32[2];

    DirectX::XMVECTOR vMin = DirectX::XMVectorSet(boxMin.x, boxMin.y, boxMin.z, 0.0f);
    DirectX::XMVECTOR vMax = DirectX::XMVectorSet(boxMax.x, boxMax.y, boxMax.z, 0.0f);
    DirectX::XMVECTOR vNewMin = DirectX::XMVectorZero();
    DirectX::XMVECTOR vNewMax = DirectX::XMVectorZero();

    vNewMin = DirectX::XMVector3TransformCoord(vMin, transform);
    vNewMax = DirectX::XMVector3TransformCoord(vMax, transform);

    vMin = DirectX::XMVectorMin(vNewMin, vNewMax);
    vMax = DirectX::XMVectorMax(vNewMin, vNewMax);

    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
    // r.org is origin of ray
    float t1 = (vMin.m128_f32[0] - rpos.m128_f32[0]) * dirfrac.x;
    float t2 = (vMax.m128_f32[0] - rpos.m128_f32[0]) * dirfrac.x;
    float t3 = (vMin.m128_f32[1] - rpos.m128_f32[1]) * dirfrac.y;
    float t4 = (vMax.m128_f32[1] - rpos.m128_f32[1]) * dirfrac.y;
    float t5 = (vMin.m128_f32[2] - rpos.m128_f32[2]) * dirfrac.z;
    float t6 = (vMax.m128_f32[2] - rpos.m128_f32[2]) * dirfrac.z;

    float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
    float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
    if (tmax < 0)
    {
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
    {
        return false;
    }

    return true;
}
#endif

void Shift::D3D11Frame::CheckIfRayIntersectedWithAnySceneObject(DirectX::XMVECTOR& rayPosition, DirectX::XMVECTOR& rayDirection)
{
    bool bSelected = false;

    //Engine::Vector3* sceneTranslationOffset = g_engine.m_scene.getSceneOffset();
    //DirectX::XMMATRIX sceneMatrix = DirectX::XMMatrixTranslation(-sceneTranslationOffset->x, -sceneTranslationOffset->y, -sceneTranslationOffset->z);

    //Deselect
    //if (g_engine.m_scene.m_selectedRenderable != nullptr)
    {
        //g_engine.m_scene.m_selectedRenderable->m_bDrawAABB[g_engine.m_scene.m_selectedRenderableInstance] = false;
        //g_engine.m_scene.m_selectedRenderableInstance = -1;
        //g_engine.m_scene.m_selectedRenderable = nullptr;
    }

    float closestDist = 100000000.0f;

#if 0

    for (size_t i = 0; i < g_engine.m_scene.m_renderables.size(); i++)
    {
        Engine::Resource::RenderableMesh* renderableMesh = &g_engine.m_scene.m_renderables[i];

        for (unsigned int j = 0; j < renderableMesh->m_instanceCount; j++)
        {
            DirectX::XMMATRIX transform;
            memcpy(&transform, &renderableMesh->m_transform[j], sizeof(DirectX::XMMATRIX));
            transform *= sceneMatrix;

            if (Engine::g_camera.cullAABB(renderableMesh->m_boxMin, renderableMesh->m_boxMax, transform))
            {
                continue;
            }

            float objectDistanceToCamera = Engine::GetDistanceToCamera(transform.r[3]);

            //If aabb intersected with ray and object distance to camera is < closest distance so far.
            if (rayBoxIntersect(rayPosition, rayDirection, renderableMesh->m_boxMin, renderableMesh->m_boxMax, transform) && objectDistanceToCamera < closestDist)
            {
                g_engine.m_scene.m_selectedRenderable = renderableMesh;
                g_engine.m_scene.m_selectedRenderableInstance = j;
                closestDist = objectDistanceToCamera;
            }
            else
            {
                renderableMesh->m_bDrawAABB[j] = false;
            }
        }
    }

    if (g_engine.m_scene.m_selectedRenderable != nullptr)
    {
        g_engine.m_scene.m_selectedRenderable->m_bDrawAABB[g_engine.m_scene.m_selectedRenderableInstance] = true;
        m_panes->m_rightPane->populateRenderableProperties(g_engine.m_scene.m_selectedRenderable, g_engine.m_scene.m_selectedRenderableInstance);
    }
    else
    {
        m_panes->m_rightPane->populateEmpty();
    }
#endif

    return;
}

QSize Shift::D3D11Frame::getRatio(QSize currentResolution)
{
    int w = currentResolution.width();
    int h = currentResolution.height();

    if ((w / h) < (16 / 9))
    {
        w = (w / 16) * 16;
        h = (w / 16) * 9;
    }
    else
    {
        h = (h / 9) * 9;
        w = (h / 9) * 16;
    }

    return QSize(w, h);
}

void Shift::D3D11Frame::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        ShowCursor(true);
        m_dragging = false;
    }
    else if (event->button() == Qt::LeftButton)
    {
        static DirectX::XMVECTOR rayPosition, rayDirection;
        pickRayVector(event->localPos().x(), event->localPos().y(), windowWidthf, windowHeightf, rayPosition, rayDirection);
        CheckIfRayIntersectedWithAnySceneObject(rayPosition, rayDirection);
    }
}

void Shift::D3D11Frame::mouseMoveEvent(QMouseEvent* event)
{
    QPoint delta = event->pos() - m_previousMousePosition;

    if (m_dragging)
    {
#define CAM_MOUSE_SENSITIVITY 0.003f
        //Engine::g_camera.rotate(delta.y() * CAM_MOUSE_SENSITIVITY, -delta.x() * CAM_MOUSE_SENSITIVITY);
        m_previousMousePosition = event->pos();
        return;
    }
}

void Shift::D3D11Frame::renderWidget()
{
    //setUpdatesEnabled(true);
    if (m_tabWidget != nullptr)
    {
        //m_tabWidget->setTabText(0, g_engine.m_scene.getSceneName());
    }

    update();
}