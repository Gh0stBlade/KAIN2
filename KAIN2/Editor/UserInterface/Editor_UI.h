#pragma once

#include<d3d11.h>

namespace Editor
{
	namespace UI
	{
		extern bool bInitialised;
		extern bool ImGuiInitialised;
		bool Initalise(HWND& hWnd);
		void Frame();
		void Render();
		void InitialiseCamera();
		void UpdateCamera();
		void Shutdown();
		void DoOpenZone();
		void DoOpenModel();
		void DoDockSpace();
		void DoMenuBar();
		void DoTabBar();
		void DoTopPane();
		void DoLeftPane();
		void DoRightPane();
		void DoCenterPane();
		void DoBottomPane();
		void ApplyTheme();
		void LoadUIIcons();
		bool LoadTexture(const unsigned char* imageData, ID3D11ShaderResourceView** outShaderResourceView, int width, int height);
	}
}
