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
		void BindFrameBuffer(unsigned int BufferIndex = 0) override;
		unsigned int CreateFrameBuffer() override;
		unsigned int CreateTextureForFrameBuffer(unsigned int Framebuffer) override;
		void ReadPixels(unsigned int X, unsigned int Y, unsigned int I1, unsigned int I2, unsigned char* Pixels, unsigned int Buffer) override;
		void SwapBuffer(Window& window) override;
	private:
	};

}