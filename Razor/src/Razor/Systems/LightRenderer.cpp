#include "LightRenderer.h"
#include "../Component.h"

namespace Razor
{

	void LightRenderer::Init()
	{
		for (const Entity& Entity : Entities)
		{
			std::shared_ptr<Light> EntityLight = std::make_shared<Light>(Coordinator->GetComponent<Light>(Entity));
			if (EntityLight)
			{
				SceneLights->push_back(EntityLight.get());
			}
			std::shared_ptr<DirectionalLight> EntityDirectionalLight = std::make_shared<DirectionalLight>(Coordinator->GetComponent<DirectionalLight>(Entity));
			if (EntityDirectionalLight)
			{
				SceneLights->push_back(EntityDirectionalLight.get());
			}
		}
	}

	void LightRenderer::Run(float dt)
	{
		/*CameraInfo CamInfo = Renderer->GetCameraInfo();
		RZ_CORE_INFO("Running light renderer");
		for (const Entity& Entity : Entities)
		{
			RZ_CORE_INFO("Processing light component holding entity"); 
			Mesh EntityMesh = Coordinator->GetComponent<Mesh>(Entity);
			Transform EntityTransform = Coordinator->GetComponent<Transform>(Entity);
			glUseProgram(LightShader.ID);
			LightShader.SetMat4Float("projection", CamInfo.CameraProjection);
			LightShader.SetMat4Float("view", CamInfo.CameraView);
			LightShader.SetMat4Float("model", EntityTransform.Get());

			for (MeshData Child : EntityMesh.Data)
			{
				LightShader.SetTextures(Child.Textures);
				Renderer->DrawMesh({ Child, LightShader });
			*///}
		//}
	}

}
