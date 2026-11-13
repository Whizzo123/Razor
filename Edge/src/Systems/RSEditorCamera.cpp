#include "RSEditorCamera.h"

namespace EdgeEditor
{

	void RSEditorCamera::Render(Razor::RenderPipelineData& data)
	{
		const float AspectRatio = ViewportHeight > 0 ? (float)ViewportWidth / (float)ViewportHeight : 1.0f;
		Razor::Matrix4 CameraProjection = Razor::Matrix4::Perspective(Razor::RazorMaths::Radians(45.0f), AspectRatio, 0.1f, 100.0f);
		Razor::Matrix4 CameraView = Razor::Matrix4::LookAt(EditorCamera.CameraPos, EditorCamera.CameraPos + EditorCamera.CameraFront, EditorCamera.CameraUp);
		for (auto& Pair : data.mEntityRenderProperties.Properties)
		{
			Razor::ShaderProperty& Property = Pair.second;
			for (int j = 0; j < Property.GetNumberOfSlots(); j++)
			{
				Razor::ShaderPropertySlot& Slot = Property.GetPropertySlot(j);

				Slot.AddProperty<Razor::Matrix4>("projection", CameraProjection);
				Slot.AddProperty<Razor::Matrix4>("view", CameraView);
				Slot.AddProperty<Razor::Vector3>("viewPos", EditorCamera.CameraPos);
			}
		}

	}

}