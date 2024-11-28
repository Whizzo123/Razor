#pragma once

#include "IRenderer.h"

namespace Razor
{

	class OpenGLRenderer : public IRenderer
	{

	public:
		void InitRendererAPI() override;
		void PollForEvents() override;
		void TerminateRendererAPI() override;
		void EnableDepthTesting(bool bEnable) override;
		void EnableStencilTesting(bool bEnable) override;
		void SetStencilOperation(EStencilAction StencilFailAction, EStencilAction DepthFailAction, EStencilAction SDPassAction) override;
		void SetStencilFunction(EStencilFunc Function, unsigned int Ref, unsigned int Mask) override;
		void EnableStencilWriting(bool bEnable) override;
		CameraInfo GetCameraInfo() override;
		void DrawMesh(RendererInfo Info) override;
		void ClearBuffer() override;
		void BindFrameBuffer(uint32_t BufferIndex = 0) override;
		Ref<Framebuffer> CreateFrameBuffer(uint32_t Width, uint32_t Height) override;
		void ReadPixels(unsigned int X, unsigned int Y, unsigned int I1, unsigned int I2, unsigned char* OutPixels, unsigned int Buffer) override;
		void SwapBuffer(Window& window) override;
		void BackupContext() override;
		void ResetCurrentContext() override;
		void* GetSceneRenderedToTexture() override;
		void SetFrameBufferTextureForScene(unsigned int SceneTexture) override;
		void ResizeFramebuffers(unsigned int Width, unsigned int Height) override;
	private:
		GLenum Translate(EStencilAction StencilAction);
		GLenum Translate(EStencilFunc StencilFunction);

		GLFWwindow* GLFWBackupContext;

		std::unordered_map<EStencilAction, GLenum> StencilActionTranslation;
		std::unordered_map<EStencilFunc, GLenum> StencilFuncTranslation;

		GLuint SceneTexture;
	};

}