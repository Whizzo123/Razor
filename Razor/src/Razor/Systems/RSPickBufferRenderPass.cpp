#include "RSPickBufferRenderPass.h"
#include "../Renderer/Shaders/PickBufferShader.h"
#include "../Engine.h"

namespace Razor
{
	void RSPickBufferRenderPass::Render(RenderPipelineData& data)
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
			for (const MeshData& Child : Model->asset.GetModelMeshData())
			{
				Renderer->UseShader(Engine::Get().GetShaderForType(typeid(PickBufferShader).name())->ID);
				if (data.mEntityRenderProperties.Properties.find(EntityToRender) == data.mEntityRenderProperties.Properties.end())
				{
					continue;
				}
				ShaderPropertySlot& Slot = data.mEntityRenderProperties.Properties[EntityToRender].GetPropertySlot(Child.MaterialId);
				std::shared_ptr<Shader> MeshShader = ShaderMap[Engine::Get().GetShaderForType(typeid(PickBufferShader).name())->ID];
				
				static const std::string floatTypeName = std::string(typeid(float).name());
				static const std::string vec3TypeName = std::string(typeid(glm::vec3).name());
				static const std::string mat4TypeName = std::string(typeid(glm::mat4).name());
				static const std::string boolTypeName = std::string(typeid(bool).name());
				static const std::string intTypeName = std::string(typeid(int).name());

				for (Scope<IProperty>& Prop : Slot.GetProperties())
				{
					if (!Prop)
					{
						continue;
					}
					
					if (Prop->GetType() == floatTypeName)
					{
						if (Property<float>* FloatProperty = dynamic_cast<Property<float>*>(Prop.get()))
						{
							MeshShader->SetFloat(FloatProperty->Name, FloatProperty->Value);
						}
					}

					if (Prop->GetType() == vec3TypeName)
					{
						if (Property<glm::vec3>* Vec3Property = dynamic_cast<Property<glm::vec3>*>(Prop.get()))
						{
							MeshShader->SetVec3(Vec3Property->Name, Vec3Property->Value);
						}
					}

					if (Prop->GetType() == mat4TypeName)
					{
						if (Property<glm::mat4>* Mat4Property = dynamic_cast<Property<glm::mat4>*>(Prop.get()))
						{
							MeshShader->SetMat4Float(Mat4Property->Name, Mat4Property->Value);
						}
					}

					if (Prop->GetType() == boolTypeName)
					{
						if (Property<bool>* BoolProperty = dynamic_cast<Property<bool>*>(Prop.get()))
						{
							MeshShader->SetBool(BoolProperty->Name, BoolProperty->Value);
						}
					}

					if (Prop->GetType() == intTypeName)
					{
						if (Property<int>* IntProperty = dynamic_cast<Property<int>*>(Prop.get()))
						{
							MeshShader->SetInt(IntProperty->Name, IntProperty->Value);
						}
					}
				}
				Renderer->DrawMesh({ Child });
			}
			data.mEntityRenderProperties.Properties[EntityToRender].Clear();
		}
	}
}