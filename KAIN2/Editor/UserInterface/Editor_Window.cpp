#include "Editor_Window.h"
#include "Editor_UI.h"

#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <time.h>

unsigned int windowWidth = 1920;
unsigned int windowHeight = 1080;
float windowWidthf = static_cast<float>(windowWidth);
float windowHeightf = static_cast<float>(windowHeight);

namespace Editor {

	bool IsRunning = true;

	namespace GraphicsDevice {
		ID3D11Device* g_device = nullptr;
		ID3D11DeviceContext* g_deviceContext = nullptr;
		HWND g_hWnd = nullptr;
	}

	namespace Graphics {
		//RenderPass renderPasses[renderPassCount];

		ID3D11RenderTargetView* g_uiRenderTargetView = nullptr;
		ID3D11VertexShader* g_vertexShader = nullptr;
		ID3D11PixelShader* g_pixelShader = nullptr;
		ID3DBlob* g_vertexShaderBlob = nullptr;
		ID3DBlob* g_pixelShaderBlob = nullptr;
		ID3D11Buffer* cbPerObj = nullptr;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
	//	return true;
	//}

	static bool s_in_sizemove = false;
	static bool s_minimized = false;

	switch (uMsg)
	{
	case WM_CLOSE:
		Editor::IsRunning = false;
		return 0;
	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED)
		{
			if (!s_minimized)
			{
				s_minimized = true;
				// Your app should probably suspend
			}
		}
		else if (s_minimized)
		{
			s_minimized = false;
			// Your app should resume
		}
		else
		{
			windowWidth = LOWORD(lParam);
			windowHeight = HIWORD(lParam);
			windowWidthf = (float)windowWidth;
			windowHeightf = (float)windowHeight;
			
			//g_engine.getRenderer()->ResizeBuffers(windowWidth, windowHeight);
		}
	}
	break;
	case WM_ENTERSIZEMOVE:
		s_in_sizemove = true;
		break;

	case WM_EXITSIZEMOVE:
		s_in_sizemove = false;
		break;
	case WM_KEYDOWN:

		if (wParam == VK_F1)
		{
			//Engine::g_camera.Position = DirectX::XMVectorSet(15.05712, 306.4111f, -170.00f, 1.0f);
		}

		if (wParam == VK_HOME)
		{
			//Engine::g_camera.Position = DirectX::XMVectorAdd(Engine::g_camera.Position, DirectX::XMVectorSet(0.0f, 0.0f, -8.0f, 0.0f));
		}

		if (wParam == VK_END)
		{
			//Engine::g_camera.Position = DirectX::XMVectorAdd(Engine::g_camera.Position, DirectX::XMVectorSet(0.0f, 0.0f, 8.0f, 0.0f));
		}
		extern size_t renderableIndex;
		if (wParam == VK_INSERT)
		{
			++renderableIndex;
			//if (renderableIndex >= g_engine.m_scene.m_renderables.size())
			{
				//renderableIndex = g_engine.m_scene.m_renderables.size() - 1;
			}
			Editor::UI::InitialiseCamera();
		}

		if (wParam == VK_DELETE)
		{
			--renderableIndex;
			if (renderableIndex < 0)
			{
				renderableIndex = 0;
			}
			
			Editor::UI::InitialiseCamera();
		}

		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}

		if (wParam == VK_SPACE)
		{
			//g_engine.loadZone("sb_16");
			///g_engine.loadZone("rc_15_camp");
			//g_engine.loadZone("qt_scale_the_ziggurat");
			///g_engine.loadZone("rc_01_marsh");
			//g_engine.loadZone("vh_main");
			//g_engine.loadZone("tr_02_ww2_beach");
			//g_engine.loadZone("mb_readyroom");
			//g_engine.loadZone("oceanvista");
			//g_engine.loadZone("survival_den_puzzleroom");
			//g_engine.loadZone("ac_forest");
			//g_engine.loadZone("test_leveleditor1");
			//g_engine.loadZone("tr_03_japanese_shrine");
			//g_engine.loadZone("tr_04_chasm_bridge");
			//g_engine.loadZone("tr_05_submarine");
			//g_engine.loadZone("tr_06_monastery");
			//g_engine.loadZone("survival_den97");
			//g_engine.loadZone("survival_den04");
			//g_engine.loadZone("main_menu_1");
			//g_engine.loadZone("test_leveleditor1");
			//g_engine.loadModel("extras_lara_innocent");
			//g_engine.loadModel("laracroft_clean");
			//g_engine.loadModel("v1_lara");
			//g_engine.loadModel("v2_lara");
			//g_engine.loadModel("lara_mp");
			//g_engine.loadModel("extras_lara_innocent");
			//g_engine.loadModel("extras_grim");
			//g_engine.loadModel("extras_jonah");
			//g_engine.loadModel("extras_ceremonial_sam");
			//g_engine.loadModel("laracroft");
			//g_engine.loadModel("wolf_alpha.drm");
			//So we see the first renderable mesh
			Editor::UI::InitialiseCamera();
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

#if 0
bool Editor::CreateEditorWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, HWND& hWnd)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(nullptr , IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = L"Tomb Raider";
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Error registering class", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	hWnd = CreateWindowEx(0, L"Tomb Raider", L"Shift Editor (D3D11)", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);

	if (hWnd == nullptr) {
		return false;
	}

	ShowWindow(hWnd, ShowWnd);
	UpdateWindow(hWnd);

	return true;
}
#endif

void Editor::ReleaseObjects()
{
	
}

bool Editor::InitScene()
{
	Editor::UI::InitialiseCamera();

	return true;
}

int Editor::MessageLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (Editor::IsRunning)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
#if !defined(NO_IMGUI)
			//Editor::UI::Frame();
#endif
			//DrawScene();
#if !defined(NO_IMGUI)
			//ImGui::UpdatePlatformWindows();
			//ImGui::RenderPlatformWindowsDefault();
#endif
		}
	}
	return msg.wParam;
}


