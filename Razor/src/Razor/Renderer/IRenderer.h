#pragma once

#include "../Core.h"
#include "../Component.h"
#include "Shader.h"
#include <memory>
struct RendererInfo
{
	Mesh ObjMesh;
	Shader ObjShader;
	Transform ObjTransform;
	bool bHasIndices;
};

struct Camera
{
	glm::vec3 CameraPos;
	glm::vec3 CameraTarget;
	glm::vec3 CameraDirection;

	glm::vec3 Up;
	glm::vec3 CameraRight;
	glm::vec3 CameraUp;
	glm::vec3 CameraFront;
};

class RAZOR_API IRenderer
{
public:
	virtual void Draw(RendererInfo Info) = 0;
	Camera RendererCamera;
	
};

