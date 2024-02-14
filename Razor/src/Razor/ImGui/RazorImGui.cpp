#pragma once

#include "RazorImGui.h"
#include "../Utils/Delegates.h"

#include <imgui.h>
#include "../../Platform/OpenGL/imgui_impl_opengl3.h"
#include "../../Platform/OpenGL/imgui_impl_glfw.h"
#include "../IO/RazorIO.h"

namespace Razor
{
	void RazorImGui::Setup(GLFWwindow* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& IO = ImGui::GetIO();
		IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		IO.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		IO.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
		// imgui_impl_glfw for io.keymap :) -> switch to own
	}

	void RazorImGui::Render(const Window& RenderWindow)
	{
		// Will need an update tick for ImGui to handle mouse and keyboard
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(RenderWindow.GetWidth(), RenderWindow.GetHeight());
		ImGui::NewFrame();
		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : 1.0f / 60.0f;
		m_Time = time;

		CreateImGuiWindow();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void RazorImGui::CreateImGuiWindow()
	{
		bool bIsOpen;
		ImGui::Begin("Inspector", &bIsOpen, ImGuiWindowFlags_MenuBar);
		ImGui::SetWindowSize(ImVec2(100.0f, 200.0f));
		ImGui::Text("Hello Inspector, %d", 123);
		ImGui::Button("Button", ImVec2(50.f, 25.f));
		ImGui::End();
	}

	void RazorImGui::RegisterImGuiEvents()
	{
		RazorIO& EngineIO = RazorIO::Get();
		//EngineIO.OnMouseButtonPressed().BindRaw(this, &RazorImGui::OnMouseButtonPressedEvent);
		//EngineIO.OnMouseButtonReleased().BindRaw(this, &RazorImGui::OnMouseButtonReleasedEvent);
	}

	bool RazorImGui::OnMouseButtonPressedEvent(int MouseButton)
	{
		ImGuiIO& IO = ImGui::GetIO();
		if (IO.WantCaptureMouse)
		{
			// Pass in index of mouse button
			IO.MouseDown[MouseButton] = true;
		}
		return false;
	}
	bool RazorImGui::OnMouseButtonReleasedEvent(int MouseButton)
	{
		ImGuiIO& IO = ImGui::GetIO();
		if (IO.WantCaptureMouse)
		{
			IO.MouseDown[MouseButton] = false;
		}
		return false;
	}
}