#include "RSCameraPass.h"
#include "../Utils/Vector3.h"
#include "../Utils/Matrix4.h"
#include "../Renderer/Font/Text.h"

namespace Razor
{
	void RSCameraPass::Render(RenderPipelineData& data)
	{
		const float AspectRatio = ViewportHeight > 0 ? (float)ViewportWidth / (float)ViewportHeight : 1.0f;

		auto View = _mCurrentScene->GetEntitiesWithComponents<Camera>();

		Matrix4 CameraProjection = Matrix4(1.0f);
		Matrix4 CameraProjectionOrtho = Matrix4(1.0f);
		Matrix4 CameraView = Matrix4(1.0f);
		Vector3 CameraPos = Vector3(0.0f);

		for(auto CameraEntity : View)
		{
			Camera& GameCamera = _mCurrentScene->GetComponent<Camera>(CameraEntity);
			Transform& CameraTransform = _mCurrentScene->GetComponent<Transform>(CameraEntity);
			CameraProjection = Matrix4::Perspective(RazorMaths::Radians(45.0f), AspectRatio, 0.1f, 100.0f);
			CameraProjectionOrtho = Matrix4::Ortho(0.0f, static_cast<float>(ViewportWidth), 0.0f, static_cast<float>(ViewportHeight));
			CameraView = Matrix4::LookAt(CameraTransform.Position, CameraTransform.Position + GameCamera.CameraFront, GameCamera.CameraUp);
			CameraPos = CameraTransform.Position;

			for (auto& Pair : data.mEntityRenderProperties.Properties)
			{
				ShaderProperty& Property = Pair.second;
				for (int j = 0; j < Property.GetNumberOfSlots(); j++)
				{
					ShaderPropertySlot& Slot = Property.GetPropertySlot(j);

					Slot.AddProperty<Matrix4>("projection", CameraProjection);
					Slot.AddProperty<Matrix4>("view", CameraView);
					Slot.AddProperty<Vector3>("viewPos", CameraTransform.Position);
				}
			}
		}

		auto TextView = _mCurrentScene->GetEntitiesWithComponents<Text>();

		for(auto TextEntity : TextView)
		{
			if (data.mEntityRenderProperties.Properties.find(TextEntity) == data.mEntityRenderProperties.Properties.end())
			{
				data.mEntityRenderProperties.Properties.insert(std::pair(TextEntity, ShaderProperty()));
			}
			ShaderProperty& Property = data.mEntityRenderProperties.Properties[TextEntity];
			for (int i = 0; i < 1; i++)
			{
				ShaderPropertySlot& Slot = Property.GetPropertySlot(i);
				Slot.AddProperty<Matrix4>("projection", CameraProjectionOrtho);
			}
		}

		for (auto& property : data.mDebugLineProperties)
		{
			for (int j = 0; j < property.GetNumberOfSlots(); j++)
			{
				ShaderPropertySlot& slot = property.GetPropertySlot(j);

				slot.AddProperty<Matrix4>("projection", CameraProjection);
				slot.AddProperty<Matrix4>("view", CameraView);
				slot.AddProperty<Vector3>("viewPos", CameraPos);
			}
		}

	}
}