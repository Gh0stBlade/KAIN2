#pragma once

#include <Windows.h>
#include <d3d11.h>


extern unsigned int windowWidth;
extern unsigned int windowHeight;
extern float windowWidthf;
extern float windowHeightf;

namespace Editor
{
	extern bool IsRunning;
	//bool CreateEditorWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, HWND& hWnd);
	void ReleaseObjects();
	bool InitScene();
	int MessageLoop();

	namespace Graphics
	{
		extern ID3D11VertexShader* g_vertexShader;
		extern ID3D11PixelShader* g_pixelShader;
		extern ID3DBlob* g_vertexShaderBlob;
		extern ID3DBlob* g_pixelShaderBlob;
	}
}