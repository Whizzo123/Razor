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
		/**
		* Function to read pixels from a frame buffer out to a float ptr
		* 
		* @param X - the x-coord we are looking for in the buffer
		* @param Y - the y-coord we are looking for in the buffer
		* @param Width - the width of pixels we are reading 
		* @param Height - the height of pixels we are reading
		* @param OutPixels - the pixels resulting from the read
		* @param Buffer - int corresponding to buffer we are reading from
		*/
		virtual void ReadPixels(unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height, float* OutPixels, unsigned int Buffer) = 0;
		virtual void BackupContext() = 0;
		virtual void ResetCurrentContext() = 0;
		virtual void* GetSceneRenderedToTexture() = 0;
		virtual void SetFrameBufferTextureForScene(unsigned int SceneTexture) = 0;
		virtual void ResizeFramebuffers(unsigned int Width, unsigned int Height) = 0;
		/**
		* Function to set the viewport
		* 
		* @param X - lower left corner x-coord of viewport rectangle (in pixels)
		* @param Y - lower left corner y-coord of viewport rectangle (in pixels)
		* @param Width - width of viewport
		* @param Height - height of viewport
		*/
		virtual void SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height) = 0;
		/**
		* Function to set shader to use while rendering
		* 
		* @param ShaderProgramId - id for shader program to use
		*/
		virtual void UseShader(uint32_t ShaderProgramId) = 0;
		//TODO remove this
		Camera RendererCamera;
	};

}