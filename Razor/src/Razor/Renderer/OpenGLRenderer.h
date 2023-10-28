#pragma once

#include "IRenderer.h"

class OpenGLRenderer : public IRenderer
{

public:
	void Draw(RendererInfo Info) override;

private:
	void DrawMesh(Mesh& MeshToDraw, Shader& MeshShader, Transform& MeshTransform);
};

