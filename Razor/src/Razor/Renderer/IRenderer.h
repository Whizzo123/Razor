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
		virtual void ClearBuffer() = 0;;
		virtual CameraInfo GetCameraInfo() = 0;
		Camera RendererCamera;
	};

}