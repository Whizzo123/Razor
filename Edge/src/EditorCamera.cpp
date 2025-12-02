#include "EditorCamera.h"

namespace EdgeEditor
{
	void EditorCamera::ProcessInput(float dt)
	{
		const float CameraSpeed = 5.0f * dt; // adjust accordingly
		const glm::vec3 CameraFront = Camera.CameraFront;
		const glm::vec3 CameraUp = Camera.CameraUp;

		if (Razor::RazorIO::Get().GetStateForKey(Razor::RazorKey::W) == Razor::KEY_PRESSED || Razor::RazorIO::Get().GetStateForKey(Razor::RazorKey::W) == Razor::KEY_HELD)
		{
			Camera.CameraPos += CameraSpeed * CameraFront;
		}
		if (Razor::RazorIO::Get().GetStateForKey(Razor::RazorKey::S) == Razor::KEY_PRESSED || Razor::RazorIO::Get().GetStateForKey(Razor::RazorKey::S) == Razor::KEY_HELD)
		{
			Camera.CameraPos -= CameraSpeed * CameraFront;
		}
		if (Razor::RazorIO::Get().GetStateForKey(Razor::RazorKey::A) == Razor::KEY_PRESSED || Razor::RazorIO::Get().GetStateForKey(Razor::RazorKey::A) == Razor::KEY_HELD)
		{
			Camera.CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
		}
		if (Razor::RazorIO::Get().GetStateForKey(Razor::RazorKey::D) == Razor::KEY_PRESSED || Razor::RazorIO::Get().GetStateForKey(Razor::RazorKey::D) == Razor::KEY_HELD)
		{
			Camera.CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
		}

		const Razor::Vector2D CurrentMouseCoords = Razor::RazorIO::Get().CurrentMousePos;
		// Capture moment we click down
		// Move us each update by the distance we have moved
		// Capture moment we stop clicking
		if (Razor::RazorIO::Get().GetStateForMouseButton(Razor::RazorMouseButton::RIGHT) == Razor::RazorMouseState::MOUSE_DOWN)
		{
			if (bIsFirstFrameDown)
			{
				MouseLastX = CurrentMouseCoords.X;
				MouseLastY = CurrentMouseCoords.Y;
				bIsFirstFrameDown = false;
			}

			if (MouseLastX != CurrentMouseCoords.X || MouseLastY != CurrentMouseCoords.Y)
			{
				float Xoffset = Razor::RazorIO::Get().CurrentMousePos.X - MouseLastX;
				float Yoffset = Razor::RazorIO::Get().CurrentMousePos.Y - MouseLastY;

				const float MouseSensitivity = 0.1f;
				Xoffset *= MouseSensitivity;
				Yoffset *= MouseSensitivity;

				Yaw += Xoffset;
				Pitch += Yoffset;

				Pitch = Pitch > 89.0f ? 89.0f : ((Pitch < -89.0f) ? -89.0f : Pitch);

				Camera.CameraDirection.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
				Camera.CameraDirection.y = sin(glm::radians(Pitch));
				Camera.CameraDirection.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

				MouseLastX = Razor::RazorIO::Get().CurrentMousePos.X;
				MouseLastY = Razor::RazorIO::Get().CurrentMousePos.Y;
			}
			Camera.CameraFront = Camera.CameraDirection;
		}

		if (Razor::RazorIO::Get().GetStateForMouseButton(Razor::RazorMouseButton::RIGHT) == Razor::RazorMouseState::MOUSE_UP)
		{
			bIsFirstFrameDown = true;
		}
	}

	Razor::Camera& EditorCamera::GetCamera()
	{
		return Camera;
	}
}