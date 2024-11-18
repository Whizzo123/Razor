#pragma once

#include "../Core.h"
#include "../Component.h"
#include "../Window.h"
#include "Framebuffer.h"

namespace Razor
{

	struct RendererInfo
	{
		MeshData ObjMesh;
	};

	enum class EStencilAction
	{
		KEEP,
		REPLACE
	};

	enum class EStencilFunc
	{
		ALWAYS
	};

	class IRenderer
	{
	public:
		virtual void InitRendererAPI() = 0;
		virtual void PollForEvents() = 0;
		virtual void TerminateRendererAPI() = 0;
		virtual void EnableDepthTesting(bool bEnable) = 0;
		virtual void EnableStencilTesting(bool bEnable) = 0;
		virtual void SetStencilOperation(EStencilAction StencilFailAction, EStencilAction DepthFailAction, EStencilAction SDPassAction) = 0;
		virtual void SetStencilFunction(EStencilFunc Function, unsigned int Ref, unsigned int Mask) = 0;
		virtual void EnableStencilWriting(bool bEnable) = 0;
		virtual void DrawMesh(RendererInfo Info) = 0;
		virtual void SwapBuffer(Window& RenderWindow) = 0;
		virtual void ClearBuffer() = 0;
		virtual void BindFrameBuffer(uint32_t = 0) = 0;
		virtual Ref<Framebuffer> CreateFrameBuffer(uint32_t Width, uint32_t Height) = 0;
		virtual CameraInfo GetCameraInfo() = 0;
		virtual void ReadPixels(unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height, unsigned char* Pixels, unsigned int Buffer) = 0;
		virtual void BackupContext() = 0;
		virtual void ResetCurrentContext() = 0;
		virtual void* GetSceneRenderedToTexture() = 0;
		virtual void SetFrameBufferTextureForScene(unsigned int SceneTexture) = 0;
		virtual void ResizeFramebuffers(unsigned int Width, unsigned int Height) = 0;
		Camera RendererCamera;
	};

}