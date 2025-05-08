#pragma once

#include "Razor.h"

namespace EdgeEditor
{
	class EditorCamera
	{
	public:
		void ProcessInput(float dt);

		Razor::Camera& GetCamera();
	private:
		Razor::Camera Camera;

		float MouseLastX = 400;
		float MouseLastY = 300;
		float Yaw = -90.0f;
		float Pitch = 0.0f;
		bool bIsFirstFrameDown = true;
	};
}

