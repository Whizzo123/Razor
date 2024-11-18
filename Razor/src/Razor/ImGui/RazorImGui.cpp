#pragma once

#include "RazorImGui.h"
#include "../Utils/Delegates.h"

#include <imgui.h>
#include "../../Platform/OpenGL/imgui_impl_opengl3.h"
#include "../../Platform/OpenGL/imgui_impl_glfw.h"
#include "../IO/RazorIO.h"
#include "../../Platform/OpenGL/OpenGLWindowProvider.h"
#include "../Log.h"



namespace Razor
{
	void RazorImGui::Setup(const std::shared_ptr<IWindowProvider>& WindowProvider)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& IO = ImGui::GetIO();
		IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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
		//RZ_CORE_INFO("Mouse Pos: {0},{1}", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
	}

	void RazorImGui::CreateDockspace()
	{
		bool bIsOpen;
		bool bIsDockspaceOpen;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		ImGui::Begin("Dockspace", &bIsDockspaceOpen, window_flags);
		ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
	}

	void RazorImGui::RegisterImGuiEvents()
	{
		RazorIO& EngineIO = RazorIO::Get();
		EngineIO.OnMouseButtonPressed().AddRaw(this, &RazorImGui::OnMouseButtonPressedEvent);
		EngineIO.OnMousePosChanged().AddRaw(this, &RazorImGui::OnMouseMovedEvent);
	}
	void RazorImGui::OnMouseButtonPressedEvent(RazorMouseButton Button, RazorMouseState State)
	{
		if (ImGui::GetIO().WantCaptureMouse)
		{
			if (State == RazorMouseState::MOUSE_DOWN)
			{

				ImGui::GetIO().MouseDown[(int)Button] = true;
			}
			else
			{
				ImGui::GetIO().MouseDown[(int)Button] = false;
			}
		}
	}
	void RazorImGui::OnMouseMovedEvent(double XPos, double YPos)
	{
		//ImGui requires full monitor space for coords when using docking branch
		ImGui::GetIO().MousePos = ImVec2(XPos, YPos);
	}
}