
#include "Razor.h"
#include <memory>

class Sandbox : public Razor::Application
{
public:
	Sandbox() 
	{
		Razor::Engine& GEngine = Razor::Engine::Get();
		Razor::Entity entity = GEngine.CreateEntity();
		Razor::Transform EntityTransform;
		EntityTransform.Position = glm::vec3(0.0f, 0.0f, -20.0f);
		EntityTransform.Rotate(glm::vec3(0.0f, 10.0f, 0.0f));
		EntityTransform.Scale = glm::vec3(0.05f, 0.05f, 0.05f);

		Razor::Shader NewShader;

		
		
		//// TODO include resources/model path in ProcessModel so we only supply name
		Razor::ModelInfo WormModel = GEngine.ProcessModel("resources/models/Articulated_Worm.obj");
		Razor::Mesh TestMesh = WormModel.ModelMesh;
		Razor::Material WormMat = WormModel.ModelMaterial;
		WormMat.ShaderID = GEngine.GetShaderForType(typeid(Razor::DefaultMeshShader).name())->ID;
		GEngine.AddComponentToEntity<Razor::Mesh>(entity, TestMesh);
		GEngine.AddComponentToEntity<Razor::Transform>(entity, EntityTransform);
		GEngine.AddComponentToEntity<Razor::Material>(entity, WormMat);
		// LIGHT
		Razor::ModelInfo LightModel = GEngine.ProcessModel("resources/models/Cube.obj");
		Razor::Mesh LightMesh = LightModel.ModelMesh;
		Razor::Material LightMeshMaterial = LightModel.ModelMaterial;
		LightMeshMaterial.ShaderID = GEngine.GetShaderForType(typeid(Razor::DebugLightShader).name())->ID;
		Razor::Transform LightTransform;
		LightTransform.Position = glm::vec3(-15.2f, 1.0f, -1.0f);
		LightTransform.Rotate(glm::vec3(0.0f, 0.0f, 0.0f));
		LightTransform.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		Razor::DirectionalLight LightComp = Razor::DirectionalLight();
		LightComp.Position = LightTransform.Position;
		LightComp.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		LightComp.Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		LightComp.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
		LightComp.Direction = glm::vec3(-1.0f, 0.0f, 0.0f);
		Razor::Entity LightEntity = GEngine.CreateEntity();
		GEngine.AddComponentToEntity<Razor::Mesh>(LightEntity, LightMesh);
		GEngine.AddComponentToEntity<Razor::Material>(LightEntity, LightMeshMaterial);
		GEngine.AddComponentToEntity<Razor::Transform>(LightEntity, LightTransform);
		GEngine.AddComponentToEntity<Razor::DirectionalLight>(LightEntity, LightComp);
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
	Razor::Engine::Get().Run();
}