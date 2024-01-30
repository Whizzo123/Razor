#include "RSRenderPass.h"

namespace Razor
{
	void RSRenderPass::Render(RenderPipelineEntityProperties& Properties)
	{
		for (const Entity& EntityToRender : Entities)
		{
			Mesh& EntityMesh = Coordinator->GetComponent<Mesh>(EntityToRender);
			Material& EntityMaterial = Coordinator->GetComponent<Material>(EntityToRender);
			for (const MeshData& Child : EntityMesh.Data)
			{
				glUseProgram(EntityMaterial.ShaderID);
				PropertySlot Slot = Properties.Properties[EntityToRender].GetPropertySlot(Child.MaterialId);
				std::shared_ptr<Shader> MeshShader = ShaderMap[EntityMaterial.ShaderID];
				for (const FProperty& FloatProperty : Slot.GetFloatProperties())
				{
					MeshShader->SetFloat(FloatProperty.Name, FloatProperty.Value);
				}
				for (const VProperty& Vec3Property : Slot.GetVec3Properties())
				{
					MeshShader->SetVec3(Vec3Property.Name, Vec3Property.Value);
				}
				for (const MProperty& Mat4Property : Slot.GetMat4Properties())
				{
					MeshShader->SetMat4Float(Mat4Property.Name, Mat4Property.Value);
				}
				for (const BProperty& BoolProperty : Slot.GetBoolProperties())
				{
					MeshShader->SetBool(BoolProperty.Name, BoolProperty.Value);
				}
				for (const IProperty& IntProperty : Slot.GetIntProperties())
				{
					MeshShader->SetInt(IntProperty.Name, IntProperty.Value);
				}
				Renderer->DrawMesh({ Child });
			}
			Properties.Properties[EntityToRender].Clear();
		}
	}
}