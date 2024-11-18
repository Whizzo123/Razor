#pragma once
#include "../Window.h"
#include "../IO/RazorIO.h"
#include "../Renderer/IRenderer.h"

namespace Razor
{
	// TODO API-CHANGE create seperate ImGUI opengl and dx11 api as shouldn't be having a Razor generic class containing a platform specific operation
	class RazorImGui
	{
	public:
		RazorImGui() {}
		~RazorImGui() {}
		void Setup(const std::shared_ptr<IWindowProvider>& WindowProvider);
		void BeginNewFrame();
		void EndFrame(const Window& RenderWindow, Ref<IRenderer> Renderer);
		void CreateDockspace();
	public:
		// Function to hook our on such and such events to GLFW key events
		void RegisterImGuiEvents();
		void OnMouseButtonPressedEvent(RazorMouseButton Button, RazorMouseState State);
		bool OnMouseButtonReleasedEvent(RazorMouseButton Button, RazorMouseState State);
		void OnMouseMovedEvent(double XPos, double YPos);
		bool OnMouseScrolledEvent();
		
	private:
		float m_Time = 0.0f;
	};

}

