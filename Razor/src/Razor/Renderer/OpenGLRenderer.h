#pragma once

#include "IRenderer.h"

namespace Razor
{

	class OpenGLRenderer : public IRenderer
	{

	public:
		CameraInfo GetCameraInfo() override;
		void DrawMesh(RendererInfo Info) override;
		void DrawCube(MeshData Data, Shader CubeShader) override;
	private:
		
	};

}