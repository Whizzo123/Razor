#include "RSPointLightingPass.h"
#include "../Component.h"

namespace Razor
{
	void RSPointLightingPass::Render(RenderPipelineEntityProperties& Properties)
	{
		auto View = CurrentScene->GetEntitiesWithComponents<PointLight>();

		for (auto RenderingEntity : View)
		{
			PointLight& Light = CurrentScene->GetComponent<PointLight>(RenderingEntity);
			for (auto& Pair : Properties.Properties)
			{
				EntityRenderProperty& Property = Pair.second;
				for (int i = 0; i < Property.GetNumberOfSlots(); i++)
				{
					PropertySlot& Slot = Property.GetPropertySlot(i);

					Slot.AddProperty<glm::vec3>("light.position", Light.Position);
					Slot.AddProperty<glm::vec3>("light.ambient", Light.Ambient);
					Slot.AddProperty<glm::vec3>("light.diffuse", Light.Diffuse);
					Slot.AddProperty<glm::vec3>("light.specular", Light.Specular);
					Slot.AddProperty<float>("light.constant", Light.Constant);
					Slot.AddProperty<float>("light.linear", Light.Linear);
					Slot.AddProperty<float>("light.quadratic", Light.Quadratic);
					Slot.AddProperty<int>("light.lightType", 1);
				}
			}
		}
	}
}