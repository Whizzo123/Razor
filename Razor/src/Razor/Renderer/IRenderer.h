#pragma once

#include "../Core.h"
#include "../Component.h"
#include <memory>
#include "../Window.h"

namespace Razor
{

	struct RendererInfo
	{
		MeshData ObjMesh;
	};

	class RAZOR_API IRenderer
	{
	public:
		virtual void DrawMesh(RendererInfo Info) = 0;
		virtual void SwapBuffer(Window& RenderWindow) = 0;
		virtual void ClearBuffer() = 0;
		virtual void BindFrameBuffer(unsigned int BufferIndex = 0) = 0;
		virtual unsigned int CreateFrameBuffer() = 0;
		virtual unsigned int CreateTextureForFrameBuffer(unsigned int Framebuffer) = 0;
		virtual CameraInfo GetCameraInfo() = 0;
		virtual void ReadPixels(unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height, unsigned char* Pixels, unsigned int Buffer) = 0;
		Camera RendererCamera;
	};

}