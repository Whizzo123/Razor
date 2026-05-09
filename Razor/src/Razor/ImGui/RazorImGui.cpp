#pragma once

#include "RazorImGui.h"
#include <cstdlib>
#include "../Utils/Delegates.h"

#include <imgui.h>
#include "../../Platform/OpenGL/imgui_impl_opengl3.h"
#include "../../Platform/OpenGL/imgui_impl_glfw.h"
#include "../../Platform/OpenGL/OpenGLWindowProvider.h"
#include "../Log.h"
#include "../Renderer/IRenderer.h"
#include "../Window.h"

inline ImVec2 ToImVec2(const Razor::Vector2& Vec)
{
	return ImVec2(Vec.X, Vec.Y);
}

inline ImVec4 ToImVec4(const Razor::Vector4& Vec)
{
	return ImVec4(Vec.X, Vec.Y, Vec.Z, Vec.W);
}

namespace Razor
{
	void RazorImGui::Setup(const std::shared_ptr<IWindowProvider>& WindowProvider)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& IO = ImGui::GetIO();
		IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#if defined(RZ_PLATFORM_LINUX)
		// glfwGetWindowPos emits GLFW_FEATURE_UNAVAILABLE on Wayland (glfw3.h:3461) and
		// always returns (0,0). Multi-viewport requires global monitor coordinates to
		// position floating OS windows and compute drag hit-tests — disable it on Wayland.
		// Docking within the main window still works normally.
		if (getenv("WAYLAND_DISPLAY") == nullptr)
			IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#else
		IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif

		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		std::shared_ptr<OpenGLWindowProvider> OpenGLWindow = std::dynamic_pointer_cast<OpenGLWindowProvider>(WindowProvider);
		ImGui_ImplGlfw_InitForOpenGL(OpenGLWindow->GetPlatformWindowPtr(), true);
		ImGui_ImplOpenGL3_Init();
	}

	void RazorImGui::BeginNewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void RazorImGui::EndFrame(const Window& RenderWindow, Ref<IRenderer> Renderer)
	{
		ImGuiIO& io = ImGui::GetIO();
		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : 1.0f / 60.0f;
		m_Time = time;
		io.DisplaySize = ImVec2(RenderWindow.GetWidth(), RenderWindow.GetHeight());
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			Renderer->BackupContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			Renderer->ResetCurrentContext();
		}
	}

	void RazorImGui::RegisterImGuiEvents()
	{
	}

	ImGuiViewport& RazorImGui::GetViewport(unsigned int ID)
	{
		if (ID < 0)
		{
			RZ_CORE_ERROR("Invalid ImGui viewport ID: {0}. Cannot retrieve viewport.", ID);
			return *(new ImGuiViewport());
		}
		if (ImGuiViewport* Viewport = ImGui::FindViewportByID(ID))
		{
			return *Viewport;
		}
		else
		{
			RZ_CORE_ERROR("ImGui viewport with ID {0} not found.", ID);
			return *(new ImGuiViewport());
		}
	}

	void RazorImGui::ShowMetricsWindow(bool* pOpen)
	{
		ImGui::ShowMetricsWindow(pOpen);
	}

	void RazorImGui::Begin(char const* Name, bool* pOpen, ImGuiWindowFlags Flags)
	{
		ImGui::Begin(Name, pOpen, Flags);
	}

	void RazorImGui::End()
	{
		ImGui::End();
	}

	void RazorImGui::SetNextWindowPos(Vector2 Pos)
	{
		ImGui::SetNextWindowPos(ToImVec2(Pos));
	}

	void RazorImGui::SetNextWindowSize(Vector2 Size)
	{
		ImGui::SetNextWindowSize(ToImVec2(Size));
	}

	void RazorImGui::SetNextWindowViewport(ImGuiID Id)
	{
		ImGui::SetNextWindowViewport(Id);
	}

	Vector2 RazorImGui::GetContentRegionAvail()
	{
		ImVec2 Region = ImGui::GetContentRegionAvail();
		return Vector2(Region.x, Region.y);
	}

	unsigned int RazorImGui::GetID(const char* Title)
	{
		return ImGui::GetID(Title);
	}

	void RazorImGui::Image(void* Texture, Vector2 const& ViewportSize, Vector2 const& UV0, Vector2 const& UV1, Vector4 const& TintColor, Vector4 const& BorderColor)
	{
		ImGui::Image(Texture, ToImVec2(ViewportSize), ToImVec2(UV0), ToImVec2(UV1), ToImVec4(TintColor), ToImVec4(BorderColor));
	}

	bool RazorImGui::BeginMainMenuBar()
	{
		return ImGui::BeginMainMenuBar();
	}

	void RazorImGui::EndMainMenuBar()
	{
		return ImGui::EndMainMenuBar();
	}

	bool RazorImGui::BeginMenuBar()
	{
		return ImGui::BeginMenuBar();
	}

	void RazorImGui::EndMenuBar()
	{
		ImGui::EndMenuBar();
	}

	bool RazorImGui::BeginMenu(const char* Name, bool Enabled)
	{
		return ImGui::BeginMenu(Name, Enabled);
	}

	void RazorImGui::EndMenu()
	{
		ImGui::EndMenu();
	}

	bool RazorImGui::MenuItem(const char* Name, const char* Shortcut, bool Selected, bool Enabled)
	{
		return ImGui::MenuItem(Name, Shortcut, Selected, Enabled);
	}

	unsigned int RazorImGui::DockSpace(unsigned int ID, Vector2 Pos, ImGuiDockNodeFlags Flags)
	{
		return ImGui::DockSpace(ID, ToImVec2(Pos), Flags);
	}

	Vector2 RazorImGui::GetItemRectMin()
	{
		ImVec2 Min = ImGui::GetItemRectMin();
		return Vector2(Min.x, Min.y);
	}

	unsigned int RazorImGui::GetMainViewport()
	{
		if (ImGui::GetMainViewport() == nullptr)
		{
			RZ_CORE_ERROR("ImGui main viewport is null, cannot get ID.");
			return -1;
		}
		return ImGui::GetMainViewport()->ID;
	}

	bool RazorImGui::InputFloat(const char* Label, float* Value, float Step, float StepFast, const char* Format, ImGuiInputTextFlags Flags)
	{
		return ImGui::InputFloat(Label, Value, Step, StepFast, Format, Flags);
	}

	bool RazorImGui::CollapsingHeader(const char* Label, ImGuiTreeNodeFlags Flags)
	{
		return ImGui::CollapsingHeader(Label, Flags);
	}

	void RazorImGui::SameLine(float OffsetFromStartX, float Spacing)
	{
		ImGui::SameLine(OffsetFromStartX, Spacing);
	}

	bool RazorImGui::Button(const char* Label, const Vector2& Size)
	{
		return ImGui::Button(Label, ToImVec2(Size));
	}

	bool RazorImGui::BeginPopupModal(const char* Name, bool* pOpen, ImGuiWindowFlags Flags)
	{
		return ImGui::BeginPopupModal(Name, pOpen, Flags);
	}

	void RazorImGui::EndPopup()
	{
		ImGui::EndPopup();
	}

	void RazorImGui::OpenPopup(const char* Name, ImGuiPopupFlags Flags)
	{
		ImGui::OpenPopup(Name, Flags);
	}

	void RazorImGui::CloseCurrentPopup()
	{
		ImGui::CloseCurrentPopup();
	}

	void RazorImGui::Text(const char* Text, ImGuiTextFlags Flags)
	{
		ImGui::Text(Text, Flags);
	}

	bool RazorImGui::TreeNode(const char* Label)
	{
		return ImGui::TreeNode(Label);
	}

	void RazorImGui::TreePop()
	{
		ImGui::TreePop();
	}

	bool RazorImGui::BeginPopup(const char* Name, ImGuiWindowFlags Flags)
	{
		return ImGui::BeginPopup(Name, Flags);
	}

	bool RazorImGui::BeginTable(const char* Name, int Columns, ImGuiTableFlags Flags, const Vector2& OuterSize, int RowBgColor)
	{
		return ImGui::BeginTable(Name, Columns, Flags, ToImVec2(OuterSize), RowBgColor);
	}

	void RazorImGui::EndTable()
	{
		ImGui::EndTable();
	}

	bool RazorImGui::TableNextColumn()
	{
		return ImGui::TableNextColumn();
	}

	bool RazorImGui::ImageButton(void* TextureID, const Vector2& Size, const Vector2& UV0, const Vector2& UV1, int Framepadding, const Vector4& BgColor, const Vector4& TintColor)
	{
		return ImGui::ImageButton(TextureID, ToImVec2(Size), ToImVec2(UV0), ToImVec2(UV1), Framepadding, ToImVec4(BgColor), ToImVec4(TintColor));
	}
	Vector2 RazorImGui::GetViewportPos(unsigned int Id)
	{
		if (ImGuiViewport* Viewport = ImGui::FindViewportByID(Id))
		{
			return Vector2(Viewport->Pos.x, Viewport->Pos.y);
		}
		else
		{
			RZ_CORE_ERROR("ImGui viewport with ID {0} not found.", Id);
			return Vector2();
		}
	}
	Vector2 RazorImGui::GetViewportSize(unsigned int Id)
	{
		if (ImGuiViewport* Viewport = ImGui::FindViewportByID(Id))
		{
			return Vector2(Viewport->Size.x, Viewport->Size.y);
		}
		else
		{
			RZ_CORE_ERROR("ImGui viewport with ID {0} not found.", Id);
			return Vector2();
		}
	}
	void RazorImGui::SetWindowSize(const Vector2& Size)
	{
		ImGui::SetWindowSize(ToImVec2(Size));
	}

	bool RazorImGui::Combo(const char* label, const char** data, int dataLength, int* selectedIdx)
	{
		const char* combo_preview_value = data[*selectedIdx];
		if (ImGui::BeginCombo(label, combo_preview_value, 0))
		{
			for (int n = 0; n < dataLength; n++)
			{
				const bool is_selected = (*selectedIdx == n);
				if (ImGui::Selectable(data[n], is_selected))
					*selectedIdx = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
			return true;
		}
		return false;
	}

	bool RazorImGui::InputText(const char* label, char* buf, size_t bufSize)
	{
		if (ImGui::InputText(label, buf, bufSize))
		{
			return true;
		}
		return false;
	}

	bool RazorImGui::CheckBox(const char* label, bool* value)
	{
		if (ImGui::Checkbox(label, value))
		{
			return true;
		}
		return false;
	}
}