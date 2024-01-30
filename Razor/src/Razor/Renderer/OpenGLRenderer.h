#pragma once

#include "IRenderer.h"

namespace Razor
{

	class OpenGLRenderer : public IRenderer
	{

	public:
		CameraInfo GetCameraInfo() override;
		void DrawMesh(RendererInfo Info) override;
		void ClearBuffer() override;
		void SwapBuffer(Window& window) override;
	private:
		
	};

}