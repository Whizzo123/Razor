#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	class CameraController : public System
	{
	public:

		CameraController()
		{
			Coordinator = Razor::Coordinator::GetInstance();
		}
		void Run(float dt) override;

	private:
		std::shared_ptr<Razor::Coordinator> Coordinator;
		float MouseLastX = 400;
		float MouseLastY = 300;
		float Yaw = -90.0f;
		float Pitch = 0.0f;
		bool bIsFirstMouse = true;
	};
}

