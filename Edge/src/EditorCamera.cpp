#include "EditorCamera.h"

namespace EdgeEditor
{
	void EditorCamera::ProcessInput(float dt)
	{
		const float CameraSpeed = 5.0f * dt; // adjust accordingly
		const glm::vec3 CameraFront = Camera.CameraFront;
		const glm::vec3 CameraUp = Camera.CameraUp;

		if (RazorIO::Get().GetStateForKey(RazorKey::W) == KEY_PRESSED || RazorIO::Get().GetStateForKey(RazorKey::W) == KEY_HELD)
		{
			Camera.CameraPos += CameraSpeed * CameraFront;
		}
		if (RazorIO::Get().GetStateForKey(RazorKey::S) == KEY_PRESSED || RazorIO::Get().GetStateForKey(RazorKey::S) == KEY_HELD)
		{
			Camera.CameraPos -= CameraSpeed * CameraFront;
		}
		if (RazorIO::Get().GetStateForKey(RazorKey::A) == KEY_PRESSED || RazorIO::Get().GetStateForKey(RazorKey::A) == KEY_HELD)
		{
			Camera.CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
		}
		if (RazorIO::Get().GetStateForKey(RazorKey::D) == KEY_PRESSED || RazorIO::Get().GetStateForKey(RazorKey::D) == KEY_HELD)
		{
			Camera.CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
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

				Camera.CameraDirection.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
				Camera.CameraDirection.y = sin(glm::radians(Pitch));
				Camera.CameraDirection.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

				MouseLastX = RazorIO::Get().CurrentMousePos.X;
				MouseLastY = RazorIO::Get().CurrentMousePos.Y;
			}
			Camera.CameraFront = Camera.CameraDirection;
		}

		if (RazorIO::Get().GetStateForMouseButton(RazorMouseButton::RIGHT) == RazorMouseState::MOUSE_UP)
		{
			bIsFirstFrameDown = true;
		}
	}

	Razor::Camera& EditorCamera::GetCamera()
	{
		return Camera;
	}
}