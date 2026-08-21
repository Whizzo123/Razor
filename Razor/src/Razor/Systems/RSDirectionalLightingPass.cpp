#include "RSDirectionalLightingPass.h"
#include "../Component.h"

namespace Razor
{

	void RSDirectionalLightingPass::Render(RenderPipelineData& data)
	{
		for (auto RenderingEntity : _mCurrentScene->GetEntitiesWithComponents<DirectionalLight, Transform>())
		{
			DirectionalLight& Light = _mCurrentScene->GetComponent<DirectionalLight>(RenderingEntity);
			Transform& transform = _mCurrentScene->GetComponent<Transform>(RenderingEntity);
			for (auto& Pair : data.mEntityRenderProperties.Properties)
			{
				ShaderProperty& Property = Pair.second;
				for (int i = 0; i < Property.GetNumberOfSlots(); i++)
				{
					ShaderPropertySlot& Slot = Property.GetPropertySlot(i);

					Slot.AddProperty<glm::vec3>("light.position", transform.Position);
					Slot.AddProperty<glm::vec3>("light.ambient", Light.Ambient);
					Slot.AddProperty<glm::vec3>("light.diffuse", Light.Diffuse);
					Slot.AddProperty<glm::vec3>("light.specular", Light.Specular);
					Slot.AddProperty<glm::vec3>("light.direction", Light.Direction);
					Slot.AddProperty<int>("light.lightType", 0);
				}
			}
		}
	}
}

