#pragma once

#include "../Core.h"
#include "../Component.h"
#include <memory>

namespace Razor
{

	struct RendererInfo
	{
		MeshData ObjMesh;
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

	struct CameraInfo
	{
		glm::mat4 CameraView;
		glm::mat4 CameraProjection;
		glm::vec3 CameraPosition;
	};

	class RAZOR_API IRenderer
	{
	public:
		virtual void DrawMesh(RendererInfo Info) = 0;
		virtual CameraInfo GetCameraInfo() = 0;
		Camera RendererCamera;
	};

}