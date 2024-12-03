#pragma once
#include "System.h"
#include "../Coordinator.h"



namespace Razor
{
	struct Camera;

	class CameraController : public System
	{
	public:
		// TODO add a static member variable to the Scene called CurrentScene which is what we will reference inside of the Systems instead of the coordinator
		CameraController()
		{
			Coordinator = Razor::Coordinator::GetInstance();
			Signature = { Coordinator->GetComponentType<Camera>() };
		}
		void Run(float dt) override;

	private:
		std::shared_ptr<Razor::Coordinator> Coordinator;
		float MouseLastX = 400;
		float MouseLastY = 300;
		float Yaw = -90.0f;
		float Pitch = 0.0f;
		bool bIsFirstFrameDown = true;
	};
}

