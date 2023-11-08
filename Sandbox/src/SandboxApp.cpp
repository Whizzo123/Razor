
#include "Razor.h"
#include <memory>

class Sandbox : public Razor::Application
{
public:
	Sandbox() 
	{
		Razor::Entity entity = Razor::Engine::CreateEntity();
		Razor::Transform EntityTransform;
		EntityTransform.Position = glm::vec3(0.0f, 0.0f, -20.0f);
		EntityTransform.Rotate(glm::vec3(0.0f, 10.0f, 0.0f));
		EntityTransform.Scale = glm::vec3(0.05f, 0.05f, 0.05f);

		//// TODO include resources/model path in ProcessModel so we only supply name
		Razor::ModelInfo WormModel = Razor::Engine::ProcessModel("resources/models/Articulated_Worm.obj");
		Razor::Mesh TestMesh = WormModel.ModelMesh;
		Razor::Material WormMat = WormModel.ModelMaterial;
		Razor::Engine::AddComponentToEntity<Razor::Mesh>(entity, TestMesh);
		Razor::Engine::AddComponentToEntity<Razor::Transform>(entity, EntityTransform);
		Razor::Engine::AddComponentToEntity<Razor::Material>(entity, WormMat);
		// LIGHT
		/*Razor::ModelInfo LightModel = Razor::Engine::ProcessModel("resources/models/Cube.obj");
		Razor::Mesh LightMesh = LightModel.ModelMesh;
		Razor::Material LightMeshMaterial = LightModel.ModelMaterial;
		LightMesh.MeshShader = std::make_shared<Razor::DebugLightShader>();
		Razor::Transform LightTransform;
		LightTransform.Position = glm::vec3(1.2f, 1.0f, -1.0f);
		LightTransform.Rotate(glm::vec3(0.0f, 0.0f, 0.0f));
		LightTransform.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		Razor::Light LightComp = Razor::Light();
		Razor::Entity LightEntity = Razor::Engine::CreateEntity();
		Razor::Engine::AddComponentToEntity<Razor::Mesh>(LightEntity, LightMesh);
		Razor::Engine::AddComponentToEntity<Razor::Material>(LightEntity, LightMeshMaterial);
		Razor::Engine::AddComponentToEntity<Razor::Transform>(LightEntity, LightTransform);
		Razor::Engine::AddComponentToEntity<Razor::Light>(LightEntity, LightComp);*/
	}
	~Sandbox() {}
	void Run() override;
};

Razor::Application* Razor::CreateApplication()
{
	return new Sandbox();
}

void Sandbox::Run()
{
	Razor::Engine::Run();
}