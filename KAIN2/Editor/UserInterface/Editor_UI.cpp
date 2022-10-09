
#include "Editor_UI.h"
#include "Editor_Window.h"
#include "Editor_UI_Font.h"
#include "Editor_UI_Icons.h"
#if 0
#include "ArchiveFileSystem/Section/MeshSection.h"
#include "Material/MaterialNodeManager/MaterialNodeEditor.h"
#include "ActionGraph/ActionGraphManager/ActionGraphNodeEditor.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <examples/imgui_impl_win32.h>
#include <examples/imgui_impl_dx11.h>
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3d11.h>

///@FIXME should ideally fix this sometime.
#pragma warning( disable : 4100 )
#pragma warning( disable : 4239 )

struct ExampleAppLog
{
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
	bool                AutoScroll;     // Keep scrolling if already at the bottom

	ExampleAppLog()
	{
		AutoScroll = true;
		Clear();
	}

	void    Clear()
	{
		Buf.clear();
		LineOffsets.clear();
		LineOffsets.push_back(0);
	}

	void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		int old_size = Buf.size();
		va_list args;
		va_start(args, fmt);
		Buf.appendfv(fmt, args);
		va_end(args);
		for (int new_size = Buf.size(); old_size < new_size; old_size++)
			if (Buf[old_size] == '\n')
				LineOffsets.push_back(old_size + 1);
	}

	void    Draw(const char* title, bool* p_open = NULL)
	{
		if (!ImGui::Begin(title, p_open))
		{
			ImGui::End();
			return;
		}

		/*ImGui::SameLine();
		bool clear = ImGui::Button("Clear");
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
        */

		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		/*if (clear)
			Clear();
		if (copy)
			ImGui::LogToClipboard();
        */

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		const char* buf = Buf.begin();
		const char* buf_end = Buf.end();
		if (Filter.IsActive())
		{
			// In this example we don't use the clipper when Filter is enabled.
			// This is because we don't have a random access on the result on our filter.
			// A real application processing logs with ten of thousands of entries may want to store the result of search/filter.
			// especially if the filtering function is not trivial (e.g. reg-exp).
			for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
			{
				const char* line_start = buf + LineOffsets[line_no];
				const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
				if (Filter.PassFilter(line_start, line_end))
					ImGui::TextUnformatted(line_start, line_end);
			}
		}
		else
		{
			// The simplest and easy way to display the entire buffer:
			//   ImGui::TextUnformatted(buf_begin, buf_end);
			// And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward to skip non-visible lines.
			// Here we instead demonstrate using the clipper to only process lines that are within the visible area.
			// If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them on your side is recommended.
			// Using ImGuiListClipper requires A) random access into your data, and B) items all being the  same height,
			// both of which we can handle since we an array pointing to the beginning of each line of text.
			// When using the filter (in the block of code above) we don't have random access into the data to display anymore, which is why we don't use the clipper.
			// Storing or skimming through the search result would make it possible (and would be recommended if you want to search through tens of thousands of entries)
			ImGuiListClipper clipper;
			clipper.Begin(LineOffsets.Size);
			while (clipper.Step())
			{
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{
					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
					ImGui::TextUnformatted(line_start, line_end);
				}
			}
			clipper.End();
		}
		ImGui::PopStyleVar();

		if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
		ImGui::End();
	}
};

#endif

namespace Editor
{
	namespace UI
	{
		bool bInitialised = false;
		bool ImGuiInitialised = false;

		struct UISettings
		{
			const char* m_zoneListCurrentItem = nullptr;
			const char* m_animListCurrentItem = nullptr;
			const char* m_modelListCurrentItem = nullptr;
			bool m_zoneListSelectorOpen = false;
			bool m_modelListSelectorOpen = false;
		};

		struct UIIcons
		{
			ID3D11ShaderResourceView* mousePointerIconTexture = nullptr;
			ID3D11ShaderResourceView* mouseDragIconTexture = nullptr;
			ID3D11ShaderResourceView* undoIconTexture = nullptr;
			ID3D11ShaderResourceView* unk00IconTexture = nullptr;
			ID3D11ShaderResourceView* unk01IconTexture = nullptr;
			ID3D11ShaderResourceView* unk02IconTexture = nullptr;
			ID3D11ShaderResourceView* unk03IconTexture = nullptr;
			ID3D11ShaderResourceView* unk04IconTexture = nullptr;
			ID3D11ShaderResourceView* unk05IconTexture = nullptr;
			ID3D11ShaderResourceView* unk06IconTexture = nullptr;
			ID3D11ShaderResourceView* unk07IconTexture = nullptr;
			ID3D11ShaderResourceView* unk08IconTexture = nullptr;
			ID3D11ShaderResourceView* unk09IconTexture = nullptr;
			ID3D11ShaderResourceView* unk10IconTexture = nullptr;
		};

		Editor::UI::UISettings  Settings;
		Editor::UI::UIIcons  Icons;
	}
}

#if 0
bool Editor::UI::Initalise(HWND& hWnd)
{
    if (!ImGuiInitialised)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();

        io.ConfigFlags = ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

        if (!ImGui_ImplWin32_Init(hWnd))
        {
            printf("Failed to initialise Imgui window.");
            return false;
        }

        if (!ImGui_ImplDX11_Init(g_engine.getRenderer()->GetDevice(), g_engine.getRenderer()->GetDeviceContext()))
        {
            printf("Failed to initialise Imgui D3D11\n");
            return false;
        }


        ImFontConfig fontConfig;
        fontConfig.FontDataOwnedByAtlas = false;
		fontConfig.OversampleH = 3;
        Settings.m_fonts.push_back(io.Fonts->AddFontFromMemoryTTF((void*)font2, sizeof(font2), 14.0f, &fontConfig, ImGui::GetIO().Fonts->GetGlyphRangesDefault()));
		Settings.m_fonts.push_back(io.Fonts->AddFontFromMemoryTTF((void*)font2, sizeof(font2), 14.0f, &fontConfig, ImGui::GetIO().Fonts->GetGlyphRangesDefault()));

        LoadUIIcons();
        ImGuiInitialised = true;
    }

	return true;
}

void Editor::UI::DoMenuBar()
{
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Zone"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O")) 
			{
				Editor::UI::Settings.m_zoneListSelectorOpen = true;
			}

			if (ImGui::MenuItem("Open (Model)", "CTRL+O"))
			{
				Editor::UI::Settings.m_modelListSelectorOpen = true;
			}

			if (ImGui::MenuItem("Close", "CTRL+?"))
			{
				g_engine.m_scene.destroyScene();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Reset Window Layout", NULL))
			{
				bInitialised = false;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Unsupported"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About", nullptr, false, false)) {}  // Disabled item
			ImGui::EndMenu();
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::TextDisabled("V 0.0.1 (0x0)");

		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleColor();
}

void Editor::UI::DoTabBar()
{
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Avocado"))
		{
			ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Broccoli"))
		{
			ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Cucumber"))
		{
			ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

void Editor::UI::DoTopPane()
{
#if 1
	ImGui::Columns(7, nullptr, false);
	ImGui::SetColumnWidth(0, 8.0f + 27.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12157f, 0.12157f, 0.12157f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30196f, 0.30196f, 0.30196f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.12157f, 0.12157f, 0.12157f, 1.0f));
	if (ImGui::ImageButton((ImTextureID)Icons.mousePointerIconTexture, ImVec2(Icons::DEFAULT_ICON_WIDTH, Icons::DEFAULT_ICON_HEIGHT), ImVec2(0.0f, 0.0f), ImVec2(1.0f, -1.0f), 2))
	{
		
	}

	ImGui::NextColumn();
	ImGui::SetColumnWidth(1, 5.0f + (27.0f * 3));
	ImGui::SameLine();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	if (ImGui::ImageButton((ImTextureID)Icons.mouseDragIconTexture, ImVec2(Icons::DEFAULT_ICON_WIDTH, Icons::DEFAULT_ICON_HEIGHT), ImVec2(0.0f, 0.0f), ImVec2(1.0f, -1.0f), 2))
	{

	}

	ImGui::SameLine();
	if (ImGui::ImageButton((ImTextureID)Icons.undoIconTexture, ImVec2(Icons::DEFAULT_ICON_WIDTH, Icons::DEFAULT_ICON_HEIGHT), ImVec2(0.0f, 0.0f), ImVec2(1.0f, -1.0f), 2))
	{

	}

	ImGui::SameLine();
	if (ImGui::ImageButton((ImTextureID)Icons.unk00IconTexture, ImVec2(Icons::DEFAULT_ICON_WIDTH, Icons::DEFAULT_ICON_HEIGHT), ImVec2(0.0f, 0.0f), ImVec2(1.0f, -1.0f), 2))
	{

	}
	ImGui::PopStyleVar();

	ImGui::NextColumn();
	ImGui::SetColumnWidth(2, 8.0f + 27.0f);
	if (ImGui::ImageButton((ImTextureID)Icons.unk01IconTexture, ImVec2(Icons::DEFAULT_ICON_WIDTH, Icons::DEFAULT_ICON_HEIGHT), ImVec2(0.0f, 0.0f), ImVec2(1.0f, -1.0f), 2))
	{

	}

	ImGui::NextColumn();
	ImGui::SetNextItemWidth(64.0f);
    ImGui::SetColumnWidth(3, 64.0f);
	ImGui::Spacing();
	if (ImGui::BeginCombo("", "World", ImGuiComboFlags_None))
	{
		
		ImGui::EndCombo();
	}

	ImGui::NextColumn();
	ImGui::SetColumnWidth(4, 5.0f + (27.0f * 3));
	if (ImGui::ImageButton((ImTextureID)Icons.unk02IconTexture, ImVec2(Icons::DEFAULT_ICON_WIDTH, Icons::DEFAULT_ICON_HEIGHT), ImVec2(0.0f, 0.0f), ImVec2(1.0f, -1.0f), 2))
	{

	}
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SameLine();
	if (ImGui::ImageButton((ImTextureID)Icons.unk03IconTexture, ImVec2(Icons::DEFAULT_ICON_WIDTH, Icons::DEFAULT_ICON_HEIGHT), ImVec2(0.0f, 0.0f), ImVec2(1.0f, -1.0f), 2))
	{

	}
	ImGui::PopStyleVar();

	ImGui::NextColumn();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SetColumnWidth(5, 5.0f + (27.0f * 3));
	if (ImGui::ImageButton((ImTextureID)Icons.unk04IconTexture, ImVec2(Icons::DEFAULT_ICON_WIDTH, Icons::DEFAULT_ICON_HEIGHT), ImVec2(0.0f, 0.0f), ImVec2(1.0f, -1.0f), 2))
	{

	}
	ImGui::SameLine();
	if (ImGui::ImageButton((ImTextureID)Icons.unk05IconTexture, ImVec2(Icons::DEFAULT_ICON_WIDTH, Icons::DEFAULT_ICON_HEIGHT), ImVec2(0.0f, 0.0f), ImVec2(1.0f, -1.0f), 2))
	{

	}
	ImGui::SameLine();
	if (ImGui::ImageButton((ImTextureID)Icons.unk06IconTexture, ImVec2(Icons::DEFAULT_ICON_WIDTH, Icons::DEFAULT_ICON_HEIGHT), ImVec2(0.0f, 0.0f), ImVec2(1.0f, -1.0f), 2))
	{

	}

    ImGui::PopStyleVar();

    ImGui::NextColumn();
    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
    //ImGui::SetColumnWidth(5, 10.0f + (27.0f * 2));
    if (ImGui::ImageButton((ImTextureID)Icons.unk07IconTexture, ImVec2(Icons::DEFAULT_ICON_WIDTH, Icons::DEFAULT_ICON_HEIGHT), ImVec2(0.0f, 0.0f), ImVec2(1.0f, -1.0f), 2))
    {

    }
    ImGui::SameLine();
    if (ImGui::ImageButton((ImTextureID)Icons.unk08IconTexture, ImVec2(Icons::DEFAULT_ICON_WIDTH, Icons::DEFAULT_ICON_HEIGHT), ImVec2(0.0f, 0.0f), ImVec2(1.0f, -1.0f), 2))
    {

    }

	ImGui::SameLine();
	if (ImGui::ImageButton((ImTextureID)Icons.unk09IconTexture, ImVec2(Icons::DEFAULT_ICON_WIDTH, Icons::DEFAULT_ICON_HEIGHT), ImVec2(0.0f, 0.0f), ImVec2(1.0f, -1.0f), 2))
	{

	}

	ImGui::SameLine();
	if (ImGui::ImageButton((ImTextureID)Icons.unk10IconTexture, ImVec2(Icons::DEFAULT_ICON_WIDTH, Icons::DEFAULT_ICON_HEIGHT), ImVec2(0.0f, 0.0f), ImVec2(1.0f, -1.0f), 2))
	{
		g_engine.initialiseActionGraph();
	}

	//ImGui::PopStyleVar();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	
#endif
}

void Editor::UI::DoRightPane()
{
	if (ImGui::Begin("SceneProperty"))
	{
		if (g_engine.m_scene.m_selectedRenderable != nullptr)
		{
			//Temporary print distance for lods
			ImGui::Text("%f", g_engine.m_scene.m_selectedRenderable->m_cameraDistance[g_engine.m_scene.m_selectedRenderableInstance]);
			
			//IF we have animations, show the collapsing header to switch them.
			if (g_engine.m_scene.m_selectedRenderable->m_animations.size() > 0)
			{
				if (ImGui::CollapsingHeader("Animations"))
				{
					if (Settings.m_animListCurrentItem == nullptr)
					{
						Settings.m_animListCurrentItem = g_engine.m_scene.m_selectedRenderable->m_animationNames[0];

					}

					if (ImGui::BeginCombo("AnimCombo", Settings.m_animListCurrentItem, ImGuiComboFlags_None))
					{
						for (int n = 0; n < g_engine.m_scene.m_selectedRenderable->m_animations.size(); n++)
						{
							bool is_selected = (Settings.m_animListCurrentItem == g_engine.m_scene.m_selectedRenderable->m_animationNames[n]);
							if (ImGui::Selectable(g_engine.m_scene.m_selectedRenderable->m_animationNames[n], is_selected))
							{
								Settings.m_animListCurrentItem = g_engine.m_scene.m_selectedRenderable->m_animationNames[n];
								g_engine.getAnimationManager()->setAnimation(g_engine.m_scene.m_selectedRenderable, n, g_engine.m_scene.m_selectedRenderableInstance);
							}

							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}

					ImGui::Checkbox("Play Animtion", &g_engine.m_scene.m_selectedRenderable->m_playAnimations[g_engine.m_scene.m_selectedRenderableInstance]);
				}
			}
			
			if (ImGui::CollapsingHeader("Transform"))
			{
#if 1
				DirectX::XMMATRIX transform;
				memcpy(&transform, &g_engine.m_scene.m_selectedRenderable->m_transform[g_engine.m_scene.m_selectedRenderableInstance], sizeof(DirectX::XMMATRIX));

				DirectX::XMVECTOR scale;
				DirectX::XMVECTOR rotation;
				DirectX::XMVECTOR translation;
				DirectX::XMMatrixDecompose(&scale, &rotation, &translation, transform);
				ImGui::SliderFloat3("Translation:", reinterpret_cast<float*>(&translation), -10000.0f, 10000.0f, "%.3f", 1.0f);
				ImGui::SliderFloat3("Scale:", reinterpret_cast<float*>(&scale), -1.0f, 1.0f, "%.3f", 1.0f);
				ImGui::SliderFloat3("Rotation:", reinterpret_cast<float*>(&rotation), -1.0f, 1.0f, "%.3f", 1.0f);
				DirectX::XMMATRIX transformation = DirectX::XMMatrixIdentity();
				DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(scale);
				DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationRollPitchYawFromVector(rotation);
				DirectX::XMMATRIX translationMat = DirectX::XMMatrixTranslation(translation.m128_f32[0], translation.m128_f32[1], translation.m128_f32[2]);
				transformation =  scaleMat * rotationMat * translationMat;

				memcpy(&g_engine.m_scene.m_selectedRenderable->m_transform[g_engine.m_scene.m_selectedRenderableInstance], &transformation, sizeof(DirectX::XMMATRIX));
#else
				ImGui::SliderFloat4("Transform[0]", (float*)&reinterpret_cast<DirectX::XMMATRIX*>(g_engine.m_scene.m_selectedRenderable->m_transform[g_engine.m_scene.m_selectedRenderableInstance])->r[0], -1.0f, 1.0f, "%.3f", 0.25f);
				ImGui::SliderFloat4("Transform[1]", (float*)&reinterpret_cast<DirectX::XMMATRIX*>(g_engine.m_scene.m_selectedRenderable->m_transform[g_engine.m_scene.m_selectedRenderableInstance])->r[1], -1.0f, 1.0f, "%.3f", 0.25f);
				ImGui::SliderFloat4("Transform[2]", (float*)&reinterpret_cast<DirectX::XMMATRIX*>(g_engine.m_scene.m_selectedRenderable->m_transform[g_engine.m_scene.m_selectedRenderableInstance])->r[2], -1.0f, 1.0f, "%.3f", 0.25f);
				ImGui::SliderFloat4("Transform[3]", (float*)&reinterpret_cast<DirectX::XMMATRIX*>(g_engine.m_scene.m_selectedRenderable->m_transform[g_engine.m_scene.m_selectedRenderableInstance])->r[3], -10000.0f, 10000.0f, "%.3f", 0.25f);
#endif
			}

			if (ImGui::CollapsingHeader("Materials"))
			{
				int materialIndex = 0;
				Engine::Resource::MaterialSection* selectedMaterial = nullptr;

				for (size_t i = 0; i < g_engine.m_scene.m_selectedRenderable->m_meshGroups.size(); i++)
				{
					Engine::Resource::MeshGroup* meshGroup = &g_engine.m_scene.m_selectedRenderable->m_meshGroups[i];

					for (size_t j = 0; j < meshGroup->m_faceGroups.size(); j++)
					{
						Engine::Resource::FaceGroup* faceGroup = &meshGroup->m_faceGroups[j];

						if (faceGroup->m_material != nullptr)
						{
							
							char buff[32];
							sprintf(&buff[0], "Material: %d", materialIndex++);
							if (ImGui::Button(&buff[0]))
							{
								selectedMaterial = faceGroup->m_material;
								break;
							}
						}
					}

					if (selectedMaterial != nullptr)
					{
						break;
					}
				}

				if (selectedMaterial != nullptr)
				{
					MaterialNodeEditor_Initialise(selectedMaterial);
					bDisplayMaterialNodeEditor = true;
				}
			}
		}
	}

	ImGui::End();
}

static void pickRayVector(float mouseX, float mouseY, float ClientWidth, float ClientHeight, DirectX::XMVECTOR& rayPos, DirectX::XMVECTOR& rayDir)
{
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
}

static bool rayBoxIntersect(DirectX::XMVECTOR& rpos, DirectX::XMVECTOR& rdir, Engine::Vector3& boxMin, Engine::Vector3& boxMax, DirectX::XMMATRIX& transform)
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

static bool IsPointInsideSquare(ImVec2& mousePosition, ImVec2& topLeft, ImVec2& bottomRight)
{
	bool inX = false;
	bool inY = false;
	if (mousePosition.x < bottomRight.x && mousePosition.x > topLeft.x)
		inX = true;

	if (mousePosition.y < bottomRight.y && mousePosition.y > topLeft.y)
		inY = true;

	return (inX && inY);
}

void DrawLine(DirectX::XMVECTOR& position, DirectX::XMVECTOR& endPosition, DirectX::XMVECTOR& direction)
{
	g_engine.getRenderer()->GetDeviceContext()->OMSetRenderTargets(1, &g_engine.getRenderer()->GetRenderPass(RenderPass::kBackBuffer)->m_RenderTargetView, nullptr);
	
	struct Point
	{
		float x;
		float y;
		float z;
	};

	struct Line
	{
		Point p1;
		Point p2;
	};

	struct Vertex
	{
		float x;
		float y;
		float z;
	};

	Line line;
	line.p1.x = position.m128_f32[0];
	line.p1.y = position.m128_f32[1];
	line.p1.z = position.m128_f32[2];

#if 1
	line.p2.x = endPosition.m128_f32[0];
	line.p2.y = endPosition.m128_f32[1];
	line.p2.z = endPosition.m128_f32[2];
#else
	line.p2.x = line.p1.x + (100.0f * cosf(direction.m128_f32[0]) * sinf(direction.m128_f32[1]));
	line.p2.z = line.p1.z + (100.0f * sinf(direction.m128_f32[0]) * sinf(direction.m128_f32[1]));
	line.p2.y = line.p1.y + (100.0f * cosf(direction.m128_f32[1]));
#endif
	Vertex vertices[] = {
		line.p1.x, line.p1.y, line.p1.z,
		line.p2.x, line.p2.y, line.p2.z
	};

	unsigned short indices[] = {
		0, 1
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	ID3D11Buffer* vertexBuffer = nullptr;
	g_engine.getRenderer()->GetDevice()->CreateBuffer(&bd, &InitData, &vertexBuffer);

	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.ByteWidth = sizeof(indices);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;
	ID3D11Buffer* indexBuffer = nullptr;
	g_engine.getRenderer()->GetDevice()->CreateBuffer(&bd, &InitData, &indexBuffer);

	g_engine.getRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	g_engine.getRenderer()->GetDeviceContext()->VSSetShader(Editor::Graphics::g_vertexShader, nullptr, 0);
	g_engine.getRenderer()->GetDeviceContext()->PSSetShader(Editor::Graphics::g_pixelShader, nullptr, 0);

	cbPerObject cb;
	cb.World = DirectX::XMMatrixTranslation(line.p1.x, line.p1.y, line.p1.z);
	cb.WVP = DirectX::XMMatrixTranspose(cb.World * Engine::g_camera.m_viewMatrix * Engine::g_camera.m_projectionMatrix);

	g_engine.getRenderer()->GetDeviceContext()->UpdateSubresource(Engine::g_camera.cbPerObjectBuffer, 0, nullptr, &cb, 0, 0);

	g_engine.getRenderer()->GetDeviceContext()->VSSetConstantBuffers(0, 1, &Engine::g_camera.cbPerObjectBuffer);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	g_engine.getRenderer()->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	g_engine.getRenderer()->GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	ID3D11InputLayout* inputLayout = nullptr;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R16_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R16_SINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	g_engine.getRenderer()->GetDevice()->CreateInputLayout(layout, 3, Editor::Graphics::g_vertexShaderBlob->GetBufferPointer(), Editor::Graphics::g_vertexShaderBlob->GetBufferSize(), &inputLayout);
	g_engine.getRenderer()->GetDeviceContext()->IASetInputLayout(inputLayout);
	g_engine.getRenderer()->GetDeviceContext()->DrawIndexed(2, 0, 0);

	indexBuffer->Release();
	vertexBuffer->Release();
	inputLayout->Release();
	g_engine.getRenderer()->GetSwapChain()->Present(1, 0);
}

void CheckIfRayIntersectedWithAnySceneObject(DirectX::XMVECTOR& rayPosition, DirectX::XMVECTOR& rayDirection)
{
	bool bSelected = false;

	Engine::Vector3* sceneTranslationOffset = g_engine.m_scene.getSceneOffset();
	DirectX::XMMATRIX sceneMatrix = DirectX::XMMatrixTranslation(-sceneTranslationOffset->x, -sceneTranslationOffset->y, -sceneTranslationOffset->z);
	
	//Deselect
	if (g_engine.m_scene.m_selectedRenderable != nullptr)
	{
		g_engine.m_scene.m_selectedRenderable->m_bDrawAABB[g_engine.m_scene.m_selectedRenderableInstance] = false;
		g_engine.m_scene.m_selectedRenderableInstance = -1;
		g_engine.m_scene.m_selectedRenderable = nullptr;
	}

	float closestDist = 100000000.0f;

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
	}

	return;
}

static void NoBlend(const ImDrawList* parent_list, const ImDrawCmd* pcmd)
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

#if 1
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
#endif

	FLOAT blendFactor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	g_engine.getRenderer()->SetBlendState(&blendDesc, blendFactor, UINT32_MAX);
}

void Editor::UI::DoCenterPane()
{
	if (ImGui::Begin(g_engine.m_scene.getSceneName(), nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
#if _DEBUG && 1
        ImGui::Text("Camera X: %f Y: %f Z: %f", Engine::g_camera.Position.m128_f32[0], Engine::g_camera.Position.m128_f32[1], Engine::g_camera.Position.m128_f32[2]);
		ImGui::Text("FPS: %.1f ms(%f) DT: %f", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().DeltaTime);
		ImGui::Text("Num Meshes: %d - Num Terrain: %d", g_engine.getRenderer()->m_numMeshesDrawn, g_engine.getRenderer()->m_numTerrainDrawn);
#endif

		ImGuiIO& io = ImGui::GetIO();
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		ImVec2 vOffset = ImGui::GetWindowPos();
		vMin.x += vOffset.x;
		vMin.y += vOffset.y;
		vMax.x += vOffset.x;
		vMax.y += vOffset.y;

		ImVec2 vDiff = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);

#if 0//Debug clip area
		ImGui::GetForegroundDrawList()->AddRect(vMin, vMax, IM_COL32(255, 255, 0, 255));
#endif
		ImGui::GetWindowDrawList()->AddCallback(NoBlend, nullptr);
		ImGui::Image((void*)g_engine.getRenderer()->GetRenderPass(g_engine.getRenderer()->getVisiblePassIndex())->m_ShaderResourceView, vDiff);
		ImGui::GetWindowDrawList()->AddCallback(ImDrawCallback_ResetRenderState, nullptr);

		return;
		//if (IsPointInsideSquare(ImGui::GetMousePos(), vMin, vMax))
		{
			float mouseX = (ImGui::GetMousePos().x - vMin.x) * ((windowWidthf) / vDiff.x);
			float mouseY = (ImGui::GetMousePos().y - vMin.y) * ((windowHeightf) / vDiff.y);

			static DirectX::XMVECTOR rayPosition, rayDirection;
			if(io.MouseClicked[0])///@TODO and scene loaded.
			{
				pickRayVector(mouseX, mouseY, windowWidthf, windowHeightf, rayPosition, rayDirection);
				CheckIfRayIntersectedWithAnySceneObject(rayPosition, rayDirection);
			}

#if 0//Note causes Y offset on scroll down.
			//ImGui::Text("Intersect? : %d", bDidIntersect);
			ImGui::Begin("Debug");

			ImGui::Text("MP: %f %f", mouseX, mouseY);
			ImGui::Text("RP: %f %f %f", rayPosition.m128_f32[0], rayPosition.m128_f32[1], rayPosition.m128_f32[2]);
			ImGui::Text("RD: %f %f %f", rayDirection.m128_f32[0], rayDirection.m128_f32[1], rayDirection.m128_f32[2]);
			ImGui::End();
#endif

		}
	}

	ImGui::End();
}

void Editor::UI::DoBottomPane()
{
	if (ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoTitleBar))
	{
		char buff[32];
		memset(&buff[0], 0, sizeof(buff));
		sprintf(&buff[0], "%s", "Enter Command");
		ImGui::PushItemWidth(375.0f);
		ImGui::InputText("##runcommand", buff, IM_ARRAYSIZE(buff));
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if (ImGui::Button("Run")) 
		{

		}
		ImGui::SameLine();
		ImGui::PushItemWidth(375.0f);
		ImGui::InputText("##log", buff, IM_ARRAYSIZE(buff));
		ImGui::PopItemWidth();
		ImGui::SameLine();
		
		if (ImGui::Button("Console"))
		{

		}
		
		ImGui::SameLine();
		ImGui::PushItemWidth(175.0f);
		ImGui::InputText("##console", buff, IM_ARRAYSIZE(buff));
		ImGui::PopItemWidth();
	
		static ExampleAppLog log;
		// For the demo: add a debug button _BEFORE_ the normal log window contents
		// We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
		// Most of the contents of the window will be added by the log.Draw() call.
		ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
		ImGui::Begin("Console", nullptr);
        log.AddLog("Debug: %d\n", 1);
        // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
        log.Draw("Console", nullptr);
		ImGui::End();

		
	}

	ImGui::End();
}

void Editor::UI::DoLeftPane()
{	
	//Create left pane
	if (ImGui::Begin("ZoneSliceManager"))
	{
	}
	ImGui::End();

	if (ImGui::Begin("PlacementBrowser"))
	{
		ImGui::PushFont(Settings.m_fonts[1]);
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("TabBar 0", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Objects"))
			{
				ImGui::EndTabItem();
			}
			
			if (ImGui::BeginTabItem("Meshes"))
			{
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Other"))
			{
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Units"))
			{
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("In Use"))
			{
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("*"))
			{
				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
		ImGui::PopFont();
	}
	ImGui::End();
}

void Editor::UI::ApplyTheme()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.21569f, 0.21569f, 0.21569f, 1.0f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.62745f, 0.62745f, 0.62745f, 1.0f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.13333f, 0.13333f, 0.13333f, 1.0f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.31373f, 0.31373f, 0.31373f, 1.0f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.62745f, 0.62745f, 0.62745f, 1.0f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0, 1.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.17647f, 0.17647f, 0.17647f, 1.0f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.17647f, 0.17647f, 0.17647f, 1.0f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(0.30980f, 0.30980f, 0.30980f, 1.0f);
	style->Colors[ImGuiCol_TabUnfocused] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_TabHovered] = ImVec4(0.22745f, 0.22745f, 0.22745f, 1.0f);
	style->Colors[ImGuiCol_Tab] = ImVec4(0.48235f, 0.48235f, 0.48235f, 1.0f);
	style->Colors[ImGuiCol_TabActive] = ImVec4(0.22745f, 0.22745f, 0.22745f, 1.0f);
	style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.22745f, 0.22745f, 0.22745f, 1.0f);
	style->Colors[ImGuiCol_Separator] = ImVec4(0.62353f, 0.62353f, 0.62353f, 1.0f);
	style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_DockingPreview] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.13333f, 0.13333f, 0.13333f, 1.0f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30196f, 0.30196f, 0.30196f, 1.0f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.30196f, 0.30196f, 0.30196f, 1.0f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.13333f, 0.13333f, 0.13333f, 1.0f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.13333f, 0.13333f, 0.13333f, 1.0f);
	//style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.30196f, 0.30196f, 0.30196f, 1.0f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.13333f, 0.13333f, 0.13333f, 1.0f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.13333f, 0.13333f, 0.13333f, 1.0f);

	//Disable window rounding.
	style->WindowRounding = 0.0f;
	style->ChildRounding = 0.0f;
	style->FrameRounding = 0.0f;
	style->GrabRounding = 0.0f;
	style->PopupRounding = 0.0f;
	style->ScrollbarRounding = 0.0f;
	style->TabRounding = 0.0f;
	style->FrameRounding = 8.0f;
}

void Editor::UI::LoadUIIcons()
{
	LoadTexture(iconOne, &Editor::UI::Icons.mousePointerIconTexture, Editor::Icons::DEFAULT_ICON_WIDTH, Editor::Icons::DEFAULT_ICON_HEIGHT);
	LoadTexture(iconTwo, &Editor::UI::Icons.mouseDragIconTexture, Editor::Icons::DEFAULT_ICON_WIDTH, Editor::Icons::DEFAULT_ICON_HEIGHT);
	LoadTexture(iconThree, &Editor::UI::Icons.undoIconTexture, Editor::Icons::DEFAULT_ICON_WIDTH, Editor::Icons::DEFAULT_ICON_HEIGHT);
	LoadTexture(iconFour, &Editor::UI::Icons.unk00IconTexture, Editor::Icons::DEFAULT_ICON_WIDTH, Editor::Icons::DEFAULT_ICON_HEIGHT);
	LoadTexture(iconFive, &Editor::UI::Icons.unk01IconTexture, Editor::Icons::DEFAULT_ICON_WIDTH, Editor::Icons::DEFAULT_ICON_HEIGHT);
	LoadTexture(iconSix, &Editor::UI::Icons.unk02IconTexture, Editor::Icons::DEFAULT_ICON_WIDTH, Editor::Icons::DEFAULT_ICON_HEIGHT);
	LoadTexture(iconSeven, &Editor::UI::Icons.unk03IconTexture, Editor::Icons::DEFAULT_ICON_WIDTH, Editor::Icons::DEFAULT_ICON_HEIGHT);
	LoadTexture(iconEight, &Editor::UI::Icons.unk04IconTexture, Editor::Icons::DEFAULT_ICON_WIDTH, Editor::Icons::DEFAULT_ICON_HEIGHT);
	LoadTexture(iconNine, &Editor::UI::Icons.unk05IconTexture, Editor::Icons::DEFAULT_ICON_WIDTH, Editor::Icons::DEFAULT_ICON_HEIGHT);
	LoadTexture(iconTen, &Editor::UI::Icons.unk06IconTexture, Editor::Icons::DEFAULT_ICON_WIDTH, Editor::Icons::DEFAULT_ICON_HEIGHT);
	LoadTexture(iconEleven, &Editor::UI::Icons.unk07IconTexture, Editor::Icons::DEFAULT_ICON_WIDTH, Editor::Icons::DEFAULT_ICON_HEIGHT);
	LoadTexture(iconTwelve, &Editor::UI::Icons.unk08IconTexture, Editor::Icons::DEFAULT_ICON_WIDTH, Editor::Icons::DEFAULT_ICON_HEIGHT);
	LoadTexture(iconThirteen, &Editor::UI::Icons.unk09IconTexture, Editor::Icons::DEFAULT_ICON_WIDTH, Editor::Icons::DEFAULT_ICON_HEIGHT);
	LoadTexture(iconFourteen, &Editor::UI::Icons.unk10IconTexture, Editor::Icons::DEFAULT_ICON_WIDTH, Editor::Icons::DEFAULT_ICON_HEIGHT);
}

void Editor::UI::Frame()
{
#if !defined(NO_IMGUI)
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	
	ImGui::NewFrame();
	
	Editor::UI::DoOpenZone();
	Editor::UI::DoOpenModel();
	Editor::UI::DoMenuBar();
	Editor::UI::DoDockSpace();///@FIXME library has a bug where calling this and minimising crashes the app

#if 0
	ImGui::ShowDemoWindow();
#endif
	
	bInitialised = true;

	if (bDisplayMaterialNodeEditor)
	{
		ImGui::Begin("Testing25");
		MaterialNodeEditor_Frame();
		ImGui::End();
	}

	if (bDisplayActionGraphNodeEditor)
	{
		ImGui::Begin("ActionGraphNodeEditor");
		ActionGraphNodeEditor_Frame();
		ImGui::End();
	}

#endif
}

void Editor::UI::Render()
{
#if !defined(NO_IMGUI)
	//Define clear colour.
	float clearColour[] = { 0.21f, 0.21f, 0.21f, 1.0f };

	//Set editor ui render target view
	g_engine.getRenderer()->GetDeviceContext()->OMSetRenderTargets(1, &g_engine.getRenderer()->GetRenderPass(RenderPass::kBackBuffer)->m_RenderTargetView, nullptr);

	//Clear the editor render target view.
	g_engine.getRenderer()->GetDeviceContext()->ClearRenderTargetView(g_engine.getRenderer()->GetRenderPass(RenderPass::kBackBuffer)->m_RenderTargetView, clearColour);

	ImGui::Render();

	//Change blend state
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	FLOAT blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_engine.getRenderer()->SetBlendState(&blendDesc, blendFactor, UINT32_MAX);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
}
#endif
size_t renderableIndex = 0;

void Editor::UI::InitialiseCamera()
{
#if 0
	/*if (g_engine.m_scene.m_renderableTerrain.size() > 0)
	{
		Engine::g_camera.Position = DirectX::XMVectorSet(g_engine.m_scene.m_renderableTerrain[renderableIndex].m_bbox.m_center.x, g_engine.m_scene.m_renderableTerrain[renderableIndex].m_bbox.m_center.y, g_engine.m_scene.m_renderableTerrain[renderableIndex].m_bbox.m_center.z, 1.0f);
	}*/
    if (g_engine.m_scene.m_renderables.size() > 0)
    {
        Engine::g_camera.Position = DirectX::XMVectorSet(-g_engine.m_scene.m_renderables[renderableIndex].m_transform[0].col3.x, g_engine.m_scene.m_renderables[renderableIndex].m_transform[0].col3.y, -g_engine.m_scene.m_renderables[renderableIndex].m_transform[0].col3.z, 1.0f);
		//Engine::Vector3* sceneTranslationOffset = g_engine.m_scene.getSceneOffset();
		
		//Engine::g_camera.Position = DirectX::XMVectorSet(sceneTranslationOffset->x, sceneTranslationOffset->z, -sceneTranslationOffset->y, 1.0f);
    }
	else
	{
		Engine::g_camera.Position = DirectX::XMVectorSet(0.0f, 0.0f, 180.0f, 0.0f);
		Engine::g_camera.Target = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		Engine::g_camera.Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	}

	Editor::UI::UpdateCamera();
#endif
}

void Editor::UI::UpdateCamera()
{
#if 0
	DirectX::XMVECTOR DefaultForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR DefaultUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX camRotationMatrix = Engine::g_camera.getRotationMatrix();
	Engine::g_camera.Target = DirectX::XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	Engine::g_camera.Target = DirectX::XMVectorAdd(Engine::g_camera.Position, Engine::g_camera.Target);
	Engine::g_camera.Up = DirectX::XMVector3TransformCoord(DefaultUp, camRotationMatrix);

	float fovDegrees = 60.0f;
	float fovRadians = (fovDegrees * DirectX::XM_PI) / 180.0f;
	//DirectX::XMMATRIX mirror = DirectX::XMMatrixReflect(DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));
	Engine::g_camera.m_viewMatrix = DirectX::XMMatrixLookAtLH(Engine::g_camera.Position, Engine::g_camera.Target, Engine::g_camera.Up) /** mirror*/;
	Engine::g_camera.m_viewMatrixZero = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), Engine::g_camera.Target, Engine::g_camera.Up) /** mirror*/;
	Engine::g_camera.m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 218000000.0f);

	Engine::g_camera.m_aspectRatio = windowWidthf / windowHeightf;
	Engine::g_camera.m_fov = fovRadians;

#if LIGHT_PASS_TEST || 0//oceanvista

	//cdc::CommonScene::GetViewMatrix
	const unsigned char viewMatrix[] = { 0x0B,0x5D,0x6D,0x3F,0x88,0xC6,0x4F,0x3D,0x94,0x02,0xBE,0x3E,0x00,0x00,0x00,0x00,0xE2,0xC6,0xBF,0xBE,0x20,0x95,0x00,0x3E,0x3A,0x2D,0x6B,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xB1,0x3B,0xA4,0x7D,0x3F,0x9C,0xAD,0x0A,0xBE,0x00,0x00,0x00,0x00,0x82,0x0C,0x28,0x47,0x52,0x53,0x5A,0xC4,0xF4,0x66,0x8C,0x46,0x00,0x00,0x80,0x3F };
	memcpy(&Engine::g_camera.m_viewMatrix, viewMatrix, sizeof(Engine::Matrix));

	//cdc::CommonScene::GetProjectionMatrix
	const unsigned char projectionMatrix[] = { 0x8B,0x4F,0x8A,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xBE,0xE2,0xF5,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2C,0x01,0x80,0x3F,0x00,0x00,0x80,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x77,0x01,0xA0,0xC0,0x00,0x00,0x00,0x00 };

	memcpy(&Engine::g_camera.m_projectionMatrix, projectionMatrix, sizeof(Engine::Matrix));

	Engine::g_camera.Position = Engine::g_camera.m_viewMatrix.r[3];
#endif
	//float fov = atan(1.0f / Engine::g_camera.m_projectionMatrix.r[1].m128_f32[1]) * 2.0 * (180.0f / DirectX::XM_PI);

	DirectX::XMMATRIX temp = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX invViewMatrix = DirectX::XMMatrixInverse(nullptr, Engine::g_camera.m_viewMatrix);
	Engine::OrthonormalInverse3x3((Engine::Matrix*)&temp, (Engine::Matrix*)&invViewMatrix);
	Engine::Mul4x4((Engine::Matrix*)&Engine::g_camera.m_viewMatrixAnim, (Engine::Matrix*)&Engine::g_camera.m_projectionMatrix, (Engine::Matrix*)&temp);

	Engine::g_camera.updateFrustum();
#endif
}
#if 0
void Editor::UI::Shutdown()
{
	Editor::UI::Icons.mousePointerIconTexture->Release();
	Editor::UI::Icons.mouseDragIconTexture->Release();
	Editor::UI::Icons.undoIconTexture->Release();
	Editor::UI::Icons.unk00IconTexture->Release();
	Editor::UI::Icons.unk01IconTexture->Release();
	Editor::UI::Icons.unk02IconTexture->Release();
	Editor::UI::Icons.unk03IconTexture->Release();
	Editor::UI::Icons.unk04IconTexture->Release();
	Editor::UI::Icons.unk05IconTexture->Release();
	Editor::UI::Icons.unk06IconTexture->Release();
	Editor::UI::Icons.unk07IconTexture->Release();
	Editor::UI::Icons.unk08IconTexture->Release();
	Editor::UI::Icons.unk09IconTexture->Release();
	Editor::UI::Icons.unk10IconTexture->Release();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

	Engine::g_camera.cbPerObjectBuffer->Release();
	Engine::g_camera.g_drawableBuffer->Release();
	Engine::g_camera.g_irradianceBuffer->Release();
	Engine::g_camera.g_instanceBufferPS->Release();
	Engine::g_camera.g_instanceBufferVS->Release();
	Engine::g_camera.g_lightBuffer->Release();
	Engine::g_camera.g_sceneBuffer->Release();
	Engine::g_camera.g_streamDeclBuffer->Release();
	Engine::g_camera.g_worldBuffer->Release();
}

void Editor::UI::DoOpenZone()
{
	ImGui::SetNextItemWidth(64.0f);

	if (Editor::UI::Settings.m_zoneListSelectorOpen)
	{
		if (ImGui::Begin("ZoneList", &Editor::UI::Settings.m_zoneListSelectorOpen, ImGuiWindowFlags_NoDocking))
		{
			const char* zoneNames[] = {
				"ac_bunker",
"ac_challenge_tomb",
"ac_dummy_tomb",
"ac_forest",
"ac_main",
"ac_main_to_swamp_connector",
"bh_beach_hub",
"bh_sidetomb_connector_01",
"bh_sidetomb_connector_02",
"bi_altar_room",
"bi_catacombs",
"bi_ceremony",
"bi_entrance",
"bi_exit",
"bi_pit",
"bi_puzzle",
"ch_cine_transformation",
"ch_hubtochasm",
"chasm_bridge",
"chasm_entrance",
"chasm_streamhall_01",
"chasm_streamhall_02",
"cine_chaos_beach",
"co_op_ww2_sos_01",
"co_op_ww2_sos_02",
"co_op_ww2_sos_03",
"co_op_ww2_sos_04",
"co_op_ww2_sos_gas_puzzle",
"co_op_ww2_sos_map_room",
"co_op_ww2_sos_pipes",
"connector_acmain_to_mountainclimb_a",
"coop_01_catacombs_pit",
"coop_01_village",
"coop_02_catacombs",
"coop_03_catacombs_altar",
"coop_04_catacombs_puzzle",
"coop_04_temple",
"coop_05_marsh",
"ct_batcave",
"ct_fortress_of_solitude",
"ct_windchasm",
"ct_windchasm_connector",
"de_descent",
"de_descent_to_scav_hub_connector",
"ge_01",
"ge_02",
"ge_02_a",
"ge_03",
"ge_04",
"ge_05",
"ge_06",
"ge_07",
"ge_08",
"ge_incense_burner_destructable_collection",
"ma_chasm_to_hub_connector",
"ma_chasm_vista",
"ma_monastery_interior",
"ma_puzzle",
"ma_run_out",
"main_menu_1",
"mb_candlehall_combat",
"mb_eatery",
"mb_readyroom",
"mountain_climb",
"mountain_climb_to_village_hub_connector",
"net_test_collectible_ahan",
"pdlc_30d_bunker",
"pdlc_30d_forest",
"pdlc_60d_lost_fleet",
"pdlc_60d_ritual",
"pedestal_bat",
"pedestal_chicken",
"pedestal_crab",
"pedestal_crow",
"pedestal_fish",
"pedestal_frog",
"pedestal_grim",
"pedestal_jonah",
"pedestal_lara",
"pedestal_oni_soldier",
"pedestal_oni_stalker",
"pedestal_rabbit",
"pedestal_rat",
"pedestal_roth",
"pedestal_sam",
"pedestal_scavenger_archer",
"pedestal_scavenger_melee",
"pedestal_scavenger_priest",
"pedestal_scavenger_shield",
"pedestal_scavenger_tank",
"ptboat_cine",
"qt_hall_of_queens",
"qt_pre_stalker_arena",
"qt_scale_the_ziggurat",
"qt_stalkerfight",
"qt_the_ritual",
"qt_trial_by_fire",
"qt_zig_to_ritual_connector",
"rc_01_marsh",
"rc_15_camp",
"rc_20_wolfden",
"rc_95_sound",
"rc_sidetomb_connector",
"sb_01",
"sb_05",
"sb_15",
"sb_16",
"sb_17",
"sb_20",
"sb_21",
"sb_water_death",
"sh_scavenger_hub",
"sh_scavenger_hub_2",
"sh_scavenger_hub_to_chopper_connector",
"sh_scavenger_hub_to_geothermal_connector",
"sh_scavenger_hub_to_well_connector",
"shipendcinematic",
"si_05_bunker_to_research_connector",
"si_10_research",
"si_15_elevator",
"si_20_machinery",
"si_25_tomb",
"si_30_tomb_to_bh_connector",
"si_35_bh_to_restroom_connector",
"si_55_submarine_to_bh_connector",
"si_95_sound",
"si_elevator",
"si_onigeneral_tomb",
"si_research",
"so_vistaview_global",
"survival_den_puzzleroom",
"survival_den_rambotunnel",
"tb_skub_to_kick_the_bucket",
"tb_to_beach",
"tb_to_beach_to_beach_hub_connector",
"tedcampsite",
"tedpersistentdata",
"tedscratchpad",
"testlevel1",
"testmap",
"tr_01_scavhub",
"tr_02_ww2_beach",
"tr_03_japanese_shrine",
"tr_04_chasm_bridge",
"tr_05_submarine",
"tr_06_monastery",
"tr_07_great_escape",
"tr_08_mountain_village",
"tr_09_marsh",
"tr_11_lost_fleet",
"tr_12_forest",
"tr_13_caves",
"tr_14_yousai",
"tr_16_the_cove",
"tr_17_scavhub_c",
"tt_connector_to_rc_01_marsh",
"tt_two_towers",
"unknown",
"vc_fishery",
"vc_plane_chopshop",
"vc_shockpond",
"vc_well_outsource",
"vc_wolf_paddock",
"vh_chasm_to_hub_connector",
"vh_chopshop_connector",
"vh_cliffs_to_hub_connector_a",
"vh_cliffs_to_hub_connector_b",
"vh_fisheries_connector",
"vh_hub_to_chasm_connector",
"vh_main",
"vh_vhmain_to_descent_connector",
"vh_vhmain_to_ww2_sos_01_connector",
"vh_wolf_paddock_connector",
"ww2_sos_01",
"ww2_sos_02",
"ww2sos_03",
"ww2sos_03_to_04_connector",
"ww2sos_04",
"ww2sos_05",
"ww2sos_gas_puzzle",
"ww2sos_map_room",
"cliffs_of_insanity",
"oceanvista",
"slide_of_insanity",
"cinefx",
"container1",
"test_leveleditor1",
"bridge_onslaught_start",
"survival_den03",
"survival_den04",
"survival_den97",
"test"
			};

			if (ImGui::BeginCombo("##custom combo", Settings.m_zoneListCurrentItem, ImGuiComboFlags_None))
			{
				for (int n = 0; n < IM_ARRAYSIZE(zoneNames); n++)
				{
					bool is_selected = (Settings.m_zoneListCurrentItem == zoneNames[n]);
					if (ImGui::Selectable(zoneNames[n], is_selected))
						Settings.m_zoneListCurrentItem = zoneNames[n];
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}


			if (ImGui::Button("Open"))
			{
				//Close current window.
				Editor::UI::Settings.m_zoneListSelectorOpen = false;
				g_engine.loadZone(Settings.m_zoneListCurrentItem);

				//So we see the first renderable mesh
				Editor::UI::InitialiseCamera();
			}
		}

		ImGui::End();
	}
}

void Editor::UI::DoOpenModel()
{
	ImGui::SetNextItemWidth(64.0f);

	if (Editor::UI::Settings.m_modelListSelectorOpen)
	{
		if (ImGui::Begin("ModelList", &Editor::UI::Settings.m_modelListSelectorOpen, ImGuiWindowFlags_NoDocking))
		{
			const char* modelNames[] = {
				"lara_mp_duplica",
				"zach_mp_duplica",
				"oni_archer_mp",
				"crewman_mp",
				"grim_mp",
				"samantha_ceremonial_mp",
				"alex_mp",
				"jonah_mp",
				"samantha_mp",
				"ascension_logo",
				"deer",
				"g_torch",
				"v5_lara",
				"lara_mp_menu",
				"v3_lara",
				"v2_lara",
				"fx_energy_beam",
				"fx_fire",
				"v1_lara"
			};

			if (ImGui::BeginCombo("##custom combo2", Settings.m_modelListCurrentItem, ImGuiComboFlags_None))
			{
				for (int n = 0; n < IM_ARRAYSIZE(modelNames); n++)
				{
					bool is_selected = (Settings.m_modelListCurrentItem == modelNames[n]);
					if (ImGui::Selectable(modelNames[n], is_selected))
						Settings.m_modelListCurrentItem = modelNames[n];
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}


			if (ImGui::Button("Open2"))
			{
				//Close current window.
				Editor::UI::Settings.m_modelListSelectorOpen = false;
				g_engine.loadModel(Settings.m_modelListCurrentItem);

				//So we see the first renderable mesh
				Editor::UI::InitialiseCamera();
			}
		}

		ImGui::End();
	}
}

void Editor::UI::DoDockSpace()
{
    const float menuBarHeight = 24.0f;
	const float toolsWindowHeight = 42.0f;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + menuBarHeight));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, toolsWindowHeight));
	ImGui::SetNextWindowViewport(viewport->ID);
    
	//Do the tools window first.
	if (ImGui::Begin("Tools Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		Editor::UI::DoTopPane();
		ImGui::End();
	}
	

	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + (toolsWindowHeight + menuBarHeight)));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - (toolsWindowHeight + menuBarHeight)));
	ImGui::SetNextWindowViewport(viewport->ID);
	//Do the dock window which is a window that the docked layout is constrained to.
	if (ImGui::Begin("Dock Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus))
	{
		
	}

	ImGuiID dockSpaceId = ImGui::GetID("MyDockspace");
	
    if (!ImGui::DockBuilderGetNode(dockSpaceId) || !bInitialised)
    {
        ImGui::DockBuilderRemoveNode(dockSpaceId);
        ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_None);
        ImGui::DockBuilderSetNodeSize(dockSpaceId, ImGui::GetMainViewport()->Size);

        ImGuiID mainId = dockSpaceId;
        ImGuiID dockIdTop = ImGui::DockBuilderSplitNode(mainId, ImGuiDir_Up, 0.05f, nullptr, &mainId);
        ImGuiID dockIdBottom = ImGui::DockBuilderSplitNode(mainId, ImGuiDir_Down, 0.11f, nullptr, &mainId);
        ImGuiID dockIdLeftTop = ImGui::DockBuilderSplitNode(mainId, ImGuiDir_Left, 0.20f, nullptr, &mainId);
        ImGuiID dockIdLeftBottom = ImGui::DockBuilderSplitNode(dockIdLeftTop, ImGuiDir_Down, 0.50f, nullptr, &dockIdLeftTop);
        ImGuiID dockIdCenter = ImGui::DockBuilderSplitNode(mainId, ImGuiDir_Left, 0.779f, nullptr, &mainId);
        ImGuiID dockIdRight = ImGui::DockBuilderSplitNode(mainId, ImGuiDir_Left, 0.175f, nullptr, &mainId);

        ImGui::DockBuilderDockWindow("Controls", dockIdTop);
		ImGui::DockBuilderDockWindow("ZoneSliceManager", dockIdLeftTop);
		ImGui::DockBuilderDockWindow("PlacementBrowser", dockIdLeftBottom);
        ImGui::DockBuilderDockWindow(g_engine.m_scene.getSceneName(), dockIdCenter);
        ImGui::DockBuilderDockWindow("SceneProperty", dockIdRight);
        ImGui::DockBuilderDockWindow("Console", dockIdBottom);

        ImGui::DockBuilderFinish(dockSpaceId);
    }

    ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();

	Editor::UI::DoLeftPane();
	Editor::UI::DoCenterPane();
	Editor::UI::DoRightPane();
	Editor::UI::DoBottomPane();
}

bool Editor::UI::LoadTexture(const unsigned char* imageData, ID3D11ShaderResourceView** outShaderResourceView, int width, int height)
{
	//Create the texture desc
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = imageData;
	subResource.SysMemPitch = textureDesc.Width * sizeof(unsigned int);
	subResource.SysMemSlicePitch = 0;
	g_engine.getRenderer()->GetDevice()->CreateTexture2D(&textureDesc, &subResource, &pTexture);

	//Create the texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	g_engine.getRenderer()->GetDevice()->CreateShaderResourceView(pTexture, &srvDesc, outShaderResourceView);
	pTexture->Release();

	return true;
}
#endif