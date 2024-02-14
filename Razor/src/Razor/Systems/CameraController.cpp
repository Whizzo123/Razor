#include "CameraController.h"
#include "../Component.h"

namespace Razor
{
	void CameraController::Run(float dt)
	{
		for (Entity OurEntity : Entities)
		{
			Camera& EntityCamera = Coordinator->GetComponent<Camera>(OurEntity);
			
			const float CameraSpeed = 5.0f * dt; // adjust accordingly
			if (RazorIO::Get().GetStateForKey(RazorKey::W) == KEY_PRESSED)
			{
				EntityCamera.CameraPos += CameraSpeed * EntityCamera.CameraFront;
			}
			if (RazorIO::Get().GetStateForKey(RazorKey::S) == KEY_PRESSED)
			{
				EntityCamera.CameraPos -= CameraSpeed * EntityCamera.CameraFront;
			}
			if (RazorIO::Get().GetStateForKey(RazorKey::A) == KEY_PRESSED)
			{
				EntityCamera.CameraPos -= glm::normalize(glm::cross(EntityCamera.CameraFront, EntityCamera.CameraUp)) * CameraSpeed;
			}
			if (RazorIO::Get().GetStateForKey(RazorKey::D) == KEY_PRESSED)
			{
				EntityCamera.CameraPos += glm::normalize(glm::cross(EntityCamera.CameraFront, EntityCamera.CameraUp)) * CameraSpeed;
			}

			Vector2D CurrentMouseCoords = RazorIO::Get().CurrentMousePos;

			if (bIsFirstMouse)
			{
				MouseLastX = CurrentMouseCoords.X;
				MouseLastY = CurrentMouseCoords.Y;
				bIsFirstMouse = false;
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

				EntityCamera.CameraDirection.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
				EntityCamera.CameraDirection.y = sin(glm::radians(Pitch));
				EntityCamera.CameraDirection.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

				MouseLastX = RazorIO::Get().CurrentMousePos.X;
				MouseLastY = RazorIO::Get().CurrentMousePos.Y;
			}
			EntityCamera.CameraFront = EntityCamera.CameraDirection;
		}
	}
}
