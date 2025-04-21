#include "RSSpotLightingPass.h"
#include "../Component.h"

namespace Razor
{
	void RSSpotLightingPass::Render(RenderPipelineEntityProperties& Properties)
	{
		auto View = CurrentScene->GetEntitiesWithComponents<SpotLight>();
		for (auto RenderingEntity : View)
		{
			SpotLight& Light = CurrentScene->GetComponent<SpotLight>(RenderingEntity);
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
					Slot.AddProperty<glm::vec3>("light.direction", Light.Direction);
					Slot.AddProperty<float>("light.cutoff", Light.Cutoff);
					Slot.AddProperty<float>("light.outerCutoff", Light.OuterCutoff);
					Slot.AddProperty<int>("light.lightType", 2);
				}
			}
		}
	}
}