#include "RSRenderPass.h"
#include "../Assets/AssetDirectory.h"
#include "../Engine.h"

namespace Razor
{
	void RSRenderPass::Render(RenderPipelineEntityProperties& Properties)
	{
		auto View = CurrentScene->GetEntitiesWithComponents<Mesh>();

		for (auto EntityToRender : View)
		{
			Mesh& EntityMesh = CurrentScene->GetComponent<Mesh>(EntityToRender);
			AssetWrapper<Model>* Model = Engine::Get().GetAssetDirectory()->ProcessRequest<Razor::Model>(EntityMesh.mKey);
			if (!Model)
			{
				continue;
			}
			Material& EntityMaterial = Model->asset.GetMaterial();
			for (const MeshData& Child : Model->asset.GetModelMeshData())
			{
				Renderer->UseShader(EntityMaterial.ShaderID);
				if (Properties.Properties.find(EntityToRender) == Properties.Properties.end())
				{
					continue;
				}
				PropertySlot& Slot = Properties.Properties[EntityToRender].GetPropertySlot(Child.MaterialId);
				std::shared_ptr<Shader> MeshShader = ShaderMap[EntityMaterial.ShaderID];
				if (!MeshShader)
				{
					RZ_CORE_WARN("RSRenderPass(Render) -> Shader for material is null id was {0}", EntityMaterial.ShaderID);
					continue;
				}
				
				for (Scope<IProperty>& Prop : Slot.GetProperties())
				{
					std::string propTypeName(Prop->GetType());
					if (!Prop)
					{
						continue;
					}
					
					if (propTypeName == std::string(typeid(float).name()))
					{
						if (Property<float>* FloatProperty = dynamic_cast<Property<float>*>(Prop.get()))
						{
							MeshShader->SetFloat(FloatProperty->Name, FloatProperty->Value);
						}
					}

					if (propTypeName == std::string(typeid(glm::vec3).name()))
					{
						if (Property<glm::vec3>* Vec3Property = dynamic_cast<Property<glm::vec3>*>(Prop.get()))
						{
							MeshShader->SetVec3(Vec3Property->Name, Vec3Property->Value);
						}
					}

					if (propTypeName == std::string(typeid(glm::mat4).name()))
					{
						if (Property<glm::mat4>* Mat4Property = dynamic_cast<Property<glm::mat4>*>(Prop.get()))
						{
							MeshShader->SetMat4Float(Mat4Property->Name, Mat4Property->Value);
						}
					}

					if (propTypeName == std::string(typeid(bool).name()))
					{
						if (Property<bool>* BoolProperty = dynamic_cast<Property<bool>*>(Prop.get()))
						{
							MeshShader->SetBool(BoolProperty->Name, BoolProperty->Value);
						}
					}

					if (propTypeName == std::string(typeid(int).name()))
					{
						if (Property<int>* IntProperty = dynamic_cast<Property<int>*>(Prop.get()))
						{
							MeshShader->SetInt(IntProperty->Name, IntProperty->Value);
						}
					}
				}

				Renderer->DrawMesh({ Child });
			}
			Properties.Properties[EntityToRender].Clear();
		}
	}
}