#pragma once
#include "../Window.h"

namespace Razor
{

	class RazorImGui
	{
	public:
		RazorImGui() {}
		~RazorImGui() {}
		void Setup();
		void Render(const Window& RenderWindow);
		void CreateImGuiWindow();
	public:
		// Function to hook our on such and such events to GLFW key events
		void RegisterImGuiEvents();
		bool OnMouseButtonPressedEvent(int MouseButton);
		bool OnMouseButtonReleasedEvent();
		bool OnMouseMovedEvent();
		bool OnMouseScrolledEvent();

	private:
		float m_Time = 0.0f;
	};

}

