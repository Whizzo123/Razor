#include "RSRenderPass.h"
#include "../Assets/AssetDirectory.h"
#include "../Engine.h"
#include "../Renderer/Shaders/DefaultMeshShader.h"
#include "../Renderer/Font/Text.h"
#include "../Renderer/Font/Font.h"

namespace Razor
{
	void RSRenderPass::Render(RenderPipelineData& data)
	{
		auto View = _mCurrentScene->GetEntitiesWithComponents<Mesh>();

		for (auto EntityToRender : View)
		{
			Mesh& EntityMesh = _mCurrentScene->GetComponent<Mesh>(EntityToRender);
			AssetWrapper<Model>* Model = Engine::Get().GetAssetDirectory()->ProcessRequest<Razor::Model>(EntityMesh.mKey);
			if (!Model)
			{
				continue;
			}
			Material& EntityMaterial = Model->asset.GetMaterial();
			for (const MeshData& Child : Model->asset.GetModelMeshData())
			{
				Renderer->UseShader(EntityMaterial.ShaderID);
				if (data.mEntityRenderProperties.Properties.find(EntityToRender) == data.mEntityRenderProperties.Properties.end())
				{
					continue;
				}
				ShaderPropertySlot& Slot = data.mEntityRenderProperties.Properties[EntityToRender].GetPropertySlot(Child.MaterialId);
				std::shared_ptr<Shader> MeshShader = ShaderMap[EntityMaterial.ShaderID];
				
				HandleProperties(Slot, MeshShader);

				Renderer->DrawMesh({ Child });
			}
			data.mEntityRenderProperties.Properties[EntityToRender].Clear();
		}

		auto TextView = _mCurrentScene->GetEntitiesWithComponents<Text>();

 		for (auto EntityToRender : TextView)
		{
			Text& EntityText = _mCurrentScene->GetComponent<Text>(EntityToRender);
			Transform& EntityTransform = _mCurrentScene->GetComponent<Transform>(EntityToRender);
			// TODO get rid of this raw pointer
			AssetWrapper<Font>* Font = Engine::Get().GetAssetDirectory()->ProcessRequest<Razor::Font>(EntityText.mFontKey);
			if(!Font)
			{
				RZ_CORE_WARN("No font for font key on text object");
				continue;
			}
			// TODO probably should also rely on a material
			Renderer->UseShader(EntityText.mShader.ID);
			if (data.mEntityRenderProperties.Properties.find(EntityToRender) == data.mEntityRenderProperties.Properties.end() || data.mEntityRenderProperties.Properties[EntityToRender].GetNumberOfSlots() == 0)
			{
				continue;
			}
			ShaderPropertySlot& Slot = data.mEntityRenderProperties.Properties[EntityToRender].GetPropertySlot(0);
			
			HandleProperties(Slot, CreateRef<Shader>(EntityText.mShader));

			EntityText.mPosition = {EntityTransform.Position.x, EntityTransform.Position.y};
			Renderer->RenderText(EntityText, Font->asset);
			
			data.mEntityRenderProperties.Properties[EntityToRender].Clear();
		}

		Ref<Shader> defaultShader = _mShaderNameMap[std::string(typeid(DefaultMeshShader).name())];
		if (data.mDebugLineProperties.size() > 0) {
			Renderer->UseShader(defaultShader->ID);
			HandleProperties(data.mDebugLineProperties[0].GetPropertySlot(0), defaultShader);
			for (int i = 0; i < data.mDebugLines.size(); i++)
			{
				Renderer->DrawLine(data.mDebugLines[i]);
			}
		}
		data.mDebugLineProperties.clear();
		
		for (int i = 0; i < data.mDebugTriangles.size(); i++)
		{
			Renderer->UseShader(defaultShader->ID);
			HandleProperties(data.mDebugTriangleProperties[i].GetPropertySlot(0), defaultShader);
			Renderer->DrawTriangle(data.mDebugTriangles[i]);
		}
		data.mDebugTriangleProperties.clear();
	}

	void RSRenderPass::HandleProperties(ShaderPropertySlot& slot, Ref<Shader> shader)
	{
		if (!shader)
		{
			RZ_CORE_WARN("RSRenderPass(Render) -> Shader for material is null id was {0}", shader->ID);
			return;
		}

		static const std::string floatTypeName = std::string(typeid(float).name());
		static const std::string vec3TypeName = std::string(typeid(glm::vec3).name());
		static const std::string mat4TypeName = std::string(typeid(glm::mat4).name());
		static const std::string boolTypeName = std::string(typeid(bool).name());
		static const std::string intTypeName = std::string(typeid(int).name());

		for (Scope<IProperty>& Prop : slot.GetProperties())
		{
			if (!Prop)
			{
				continue;
			}

			if (Prop->GetType() == floatTypeName)
			{
				if (Property<float>* FloatProperty = dynamic_cast<Property<float>*>(Prop.get()))
				{
					shader->SetFloat(FloatProperty->Name, FloatProperty->Value);
				}
			}

			if (Prop->GetType() == vec3TypeName)
			{
				if (Property<glm::vec3>* Vec3Property = dynamic_cast<Property<glm::vec3>*>(Prop.get()))
				{
					shader->SetVec3(Vec3Property->Name, Vec3Property->Value);
				}
			}

			if (Prop->GetType() == mat4TypeName)
			{
				if (Property<glm::mat4>* Mat4Property = dynamic_cast<Property<glm::mat4>*>(Prop.get()))
				{
					shader->SetMat4Float(Mat4Property->Name, Mat4Property->Value);
				}
			}

			if (Prop->GetType() == boolTypeName)
			{
				if (Property<bool>* BoolProperty = dynamic_cast<Property<bool>*>(Prop.get()))
				{
					shader->SetBool(BoolProperty->Name, BoolProperty->Value);
				}
			}

			if (Prop->GetType() == intTypeName)
			{
				if (Property<int>* IntProperty = dynamic_cast<Property<int>*>(Prop.get()))
				{
					shader->SetInt(IntProperty->Name, IntProperty->Value);
				}
			}
		}
	}
}