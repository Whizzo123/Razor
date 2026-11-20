#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	struct Camera;

	class CameraController : public System
	{
	public:
		CameraController(Ref<Razor::Scene> Scene) : System(Scene) {}
		void Run(float dt) override;

	private:
		float MouseLastX = 400;
		float MouseLastY = 300;
		float Yaw = -90.0f;
		float Pitch = 0.0f;
		bool bIsFirstFrameDown = true;
	};
}

