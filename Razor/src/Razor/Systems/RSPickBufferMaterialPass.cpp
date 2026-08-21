#include "RSPickBufferMaterialPass.h"
#include "../Component.h"
#include "../Engine.h"

namespace Razor
{
	void RSPickBufferMaterialPass::Render(RenderPipelineData& data)
	{
		auto View = _mCurrentScene->GetEntitiesWithComponents<Mesh>();
		for (auto RenderingEntity : View)
		{
			Mesh& EntityMesh = _mCurrentScene->GetComponent<Mesh>(RenderingEntity);
			AssetWrapper<Model>* Model = Engine::Get().GetAssetDirectory()->ProcessRequest<Razor::Model>(EntityMesh.mKey);
			if (!Model)
			{
				continue;
			}
			Material& EntityMat = Model->asset.GetMaterial();
			auto View = _mCurrentScene->GetEntitiesWithComponents<Material>();

			if (data.mEntityRenderProperties.Properties.find(RenderingEntity) == data.mEntityRenderProperties.Properties.end())
			{
				continue;
			}
			ShaderProperty& Property = data.mEntityRenderProperties.Properties[RenderingEntity];
			Property.GeneratePropertySlots(EntityMat.Materials.size());
			for (int i = 0; i < EntityMat.Materials.size(); i++)
			{
				ShaderPropertySlot& Slot = Property.GetPropertySlot(i);
				//Red, Green, Blue
				std::uint32_t Id = (std::uint32_t)RenderingEntity;
				glm::vec3 EntityColor = glm::vec3((Id & 0x000000FF) / 255.0f,
					((Id & 0x0000FF00) >> 8) / 255.0f,
					((Id & 0x00FF0000) >> 16) / 255.0f);
				Slot.AddProperty<glm::vec3>("entitycolor", EntityColor);
			}
		}
	}
}