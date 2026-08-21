#include "RSMaterialPass.h"
#include "../Component.h"
#include "../Engine.h"
#include "../Renderer/Debug/DebugLine.h"
#include "../Renderer/Debug/DebugTriangle.h"

namespace Razor
{

	void RSMaterialPass::Render(RenderPipelineData& data)
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
			if (data.mEntityRenderProperties.Properties.find(RenderingEntity) == data.mEntityRenderProperties.Properties.end())
			{
				continue;
			}
			ShaderProperty& Property = data.mEntityRenderProperties.Properties[RenderingEntity];
			Property.GeneratePropertySlots(EntityMat.Materials.size());
			for (int i = 0; i < EntityMat.Materials.size(); i++)
			{
				ShaderPropertySlot& Slot = Property.GetPropertySlot(i);
				Slot.AddProperty<int>("material.texture_diffuse", 0);
				Slot.AddProperty<int>("material.texture_normal", 0);
				Slot.AddProperty<int>("material.texture_specular", 0);
				Slot.AddProperty<int>("material.texture_height", 0);
				Slot.AddProperty<glm::vec3>("material.diffuse", EntityMat.Materials[i].Diffuse);
				Slot.AddProperty<glm::vec3>("material.ambient", EntityMat.Materials[i].Ambient);
				Slot.AddProperty<glm::vec3>("material.specular", EntityMat.Materials[i].Specular);
				Slot.AddProperty<float>("material.shininess", EntityMat.Materials[i].Shininess);
				Slot.AddProperty<int>("material.bUseTextures", 0);
			}
		}
		glm::vec3 zero = glm::vec3(0.0f, 0.0f, 0.0f);

		for (const auto& line : data.mDebugLines)
		{
			glm::vec3 color = glm::vec3(line.color.X, line.color.Y, line.color.Z);
			
			ShaderProperty debugLineProperty;
			debugLineProperty.GeneratePropertySlots(1);
			ShaderPropertySlot& slot = debugLineProperty.GetPropertySlot(0);
			slot.AddProperty<int>("material.texture_diffuse", 0);
			slot.AddProperty<int>("material.texture_normal", 0);
			slot.AddProperty<int>("material.texture_specular", 0);
			slot.AddProperty<int>("material.texture_height", 0);
			slot.AddProperty<glm::vec3>("material.diffuse", color);
			slot.AddProperty<glm::vec3>("material.ambient", zero);
			slot.AddProperty<glm::vec3>("material.specular", zero);
			slot.AddProperty<float>("material.shininess", 0.0f);
			slot.AddProperty<int>("material.bUseTextures", 0);
			data.mDebugLineProperties.push_back(std::move(debugLineProperty));
		}

		for (const auto& triangle : data.mDebugTriangles)
		{
			glm::vec3 color = glm::vec3(triangle.color.X, triangle.color.Y, triangle.color.Z);
			ShaderProperty property;
			property.GeneratePropertySlots(1);
			ShaderPropertySlot& slot = property.GetPropertySlot(0);
			slot.AddProperty<int>("material.texture_diffuse", 0);
			slot.AddProperty<int>("material.texture_normal", 0);
			slot.AddProperty<int>("material.texture_specular", 0);
			slot.AddProperty<int>("material.texture_height", 0);
			slot.AddProperty<glm::vec3>("material.diffuse", color);
			slot.AddProperty<glm::vec3>("material.ambient", zero);
			slot.AddProperty<glm::vec3>("material.specular", zero);
			slot.AddProperty<float>("material.shininess", 0.0f);
			slot.AddProperty<int>("material.bUseTextures", 0);
			data.mDebugTriangleProperties.push_back(std::move(property));
		}
	}

}