#include "CameraController.h"
#include "../Component.h"
#include "../Engine.h"

namespace Razor
{
	void CameraController::Run(float dt)
	{
		if (!Engine::Get().IsGameInputEnabled())
			return;

		auto View = _mCurrentScene->GetEntitiesWithComponents<Camera>();

		for (auto Entity : View)
		{
			Camera& EntityCamera = _mCurrentScene->GetComponent<Camera>(Entity);
			
			const float CameraSpeed = 5.0f * dt; // adjust accordingly
			const Vector3 CameraFront = EntityCamera.CameraFront;
			const Vector3 CameraUp = EntityCamera.CameraUp;

			if (RazorIO::Get().GetStateForKey(RazorKey::W) == KEY_PRESSED)
			{
				EntityCamera.CameraPos += CameraFront * CameraSpeed;
			}
			if (RazorIO::Get().GetStateForKey(RazorKey::S) == KEY_PRESSED)
			{
				EntityCamera.CameraPos -= CameraFront * CameraSpeed;
			}
			if (RazorIO::Get().GetStateForKey(RazorKey::A) == KEY_PRESSED)
			{
				EntityCamera.CameraPos -= Vector3::Normalise(Vector3::Cross(CameraFront, CameraUp)) * CameraSpeed;
			}
			if (RazorIO::Get().GetStateForKey(RazorKey::D) == KEY_PRESSED)
			{
				EntityCamera.CameraPos += Vector3::Normalise(Vector3::Cross(CameraFront, CameraUp)) * CameraSpeed;
			}

			const Vector2D CurrentMouseCoords = RazorIO::Get().CurrentMousePos;
			// Capture moment we click down
			// Move us each update by the distance we have moved
			// Capture moment we stop clicking
			if (RazorIO::Get().GetStateForMouseButton(RazorMouseButton::RIGHT) == RazorMouseState::MOUSE_DOWN)
			{
				if (bIsFirstFrameDown)
				{
					MouseLastX = CurrentMouseCoords.X;
					MouseLastY = CurrentMouseCoords.Y;
					bIsFirstFrameDown = false;
				}

				if (MouseLastX != CurrentMouseCoords.X || MouseLastY != CurrentMouseCoords.Y)
				{
					RZ_CORE_INFO("MouseCoords: {0},{1}", CurrentMouseCoords.X, CurrentMouseCoords.Y);
					RZ_CORE_INFO("LastMouseCoords: {0},{1}", MouseLastX, MouseLastY);
					float Xoffset = RazorIO::Get().CurrentMousePos.X - MouseLastX;
					float Yoffset = RazorIO::Get().CurrentMousePos.Y - MouseLastY;

					const float MouseSensitivity = 0.1f;
					Xoffset *= MouseSensitivity;
					Yoffset *= MouseSensitivity;

					Yaw += Xoffset;
					Pitch += Yoffset;

					Pitch = Pitch > 89.0f ? 89.0f : ((Pitch < -89.0f) ? -89.0f : Pitch);

					EntityCamera.CameraDirection.X = cos(RazorMaths::Radians(Yaw)) * cos(RazorMaths::Radians(Pitch));
					EntityCamera.CameraDirection.Y = sin(RazorMaths::Radians(Pitch));
					EntityCamera.CameraDirection.Z = sin(RazorMaths::Radians(Yaw)) * cos(RazorMaths::Radians(Pitch));

					MouseLastX = RazorIO::Get().CurrentMousePos.X;
					MouseLastY = RazorIO::Get().CurrentMousePos.Y;
				}
				EntityCamera.CameraFront = EntityCamera.CameraDirection;
			}

			if (RazorIO::Get().GetStateForMouseButton(RazorMouseButton::RIGHT) == RazorMouseState::MOUSE_UP)
			{
				bIsFirstFrameDown = true;
			}
		}
	}
}
