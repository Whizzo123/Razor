#include "RSPickBufferRenderPass.h"
#include "../Renderer/Shaders/PickBufferShader.h"
#include "../Engine.h"

namespace Razor
{
	void RSPickBufferRenderPass::Render(RenderPipelineEntityProperties& Properties)
	{
		auto View = CurrentScene->GetEntitiesWithComponents<Mesh, Material>();

		for (auto EntityToRender : View)
		{
			Mesh& EntityMesh = CurrentScene->GetComponent<Mesh>(EntityToRender);
			Material& EntityMaterial = CurrentScene->GetComponent<Material>(EntityToRender);
			for (const MeshData& Child : EntityMesh.Data)
			{
				//TODO remove in place of IRenderer alternative
				glUseProgram(Engine::Get().GetShaderForType(typeid(PickBufferShader).name())->ID);
				if (Properties.Properties.find(EntityToRender) == Properties.Properties.end())
				{
					continue;
				}
				PropertySlot& Slot = Properties.Properties[EntityToRender].GetPropertySlot(Child.MaterialId);
				std::shared_ptr<Shader> MeshShader = ShaderMap[Engine::Get().GetShaderForType(typeid(PickBufferShader).name())->ID];
				
				for (Scope<IProperty>& Prop : Slot.GetProperties())
				{
					if (!Prop)
					{
						continue;
					}
					
					if (Prop->GetType() == typeid(float).name())
					{
						if (Property<float>* FloatProperty = dynamic_cast<Property<float>*>(Prop.get()))
						{
							MeshShader->SetFloat(FloatProperty->Name, FloatProperty->Value);
						}
					}

					if (Prop->GetType() == typeid(glm::vec3).name())
					{
						if (Property<glm::vec3>* Vec3Property = dynamic_cast<Property<glm::vec3>*>(Prop.get()))
						{
							MeshShader->SetVec3(Vec3Property->Name, Vec3Property->Value);
						}
					}

					if (Prop->GetType() == typeid(glm::mat4).name())
					{
						if (Property<glm::mat4>* Mat4Property = dynamic_cast<Property<glm::mat4>*>(Prop.get()))
						{
							MeshShader->SetMat4Float(Mat4Property->Name, Mat4Property->Value);
						}
					}

					if (Prop->GetType() == typeid(bool).name())
					{
						if (Property<bool>* BoolProperty = dynamic_cast<Property<bool>*>(Prop.get()))
						{
							MeshShader->SetBool(BoolProperty->Name, BoolProperty->Value);
						}
					}

					if (Prop->GetType() == typeid(int).name())
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