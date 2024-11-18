#include "Razor.h"


class Edge : public Razor::Application
{
public:
	Edge() {};
	~Edge() {};
	void Run() override;
	void RenderInspector();
	void RenderSceneViewport(Razor::Ref<Razor::Framebuffer> SceneBuffer, ImVec2 ViewportSize);
};

Razor::Application* Razor::CreateApplication()
{
	return new Edge();
}

void Edge::Run()
{
	ImVec2 ViewportSize;
	Razor::Ref<Razor::Framebuffer> PickBuffer;
	Razor::Ref<Razor::Framebuffer> SceneBuffer;

	Razor::Engine& Engine = Razor::Engine::Get();

	Razor::RenderPipelineConfig PipelineConfig;
	PipelineConfig.Configuration.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_MATERIAL_PASS, std::vector<const char*> { typeid(Razor::RSMaterialPass).name() } });
	PipelineConfig.Configuration.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_LIGHTING_PASS, std::vector<const char*>
	{
		typeid(Razor::RSDirectionalLightingPass).name(),
		typeid(Razor::RSPointLightingPass).name(),
		typeid(Razor::RSSpotLightingPass).name()
	} });
	PipelineConfig.Configuration.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_TRANSFORMATION_PASS, std::vector<const char*> { typeid(Razor::RSTransformationsPass).name() } });
	PipelineConfig.Configuration.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_CAMERA_PASS, std::vector<const char*> { typeid(Razor::RSCameraPass).name() } });
	PipelineConfig.Configuration.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_RENDER, std::vector<const char*> { typeid(Razor::RSRenderPass).name() } });

	Razor::RenderPipelineConfig PickPipelineConfig;
	PickPipelineConfig.Configuration.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_MATERIAL_PASS, std::vector<const char*> { typeid(Razor::RSPickBufferMaterialPass).name() } });
	PickPipelineConfig.Configuration.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_LIGHTING_PASS, std::vector<const char*> { typeid(Razor::RSDirectionalLightingPass).name() } });
	PickPipelineConfig.Configuration.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_TRANSFORMATION_PASS, std::vector<const char*> { typeid(Razor::RSTransformationsPass).name() } });
	PickPipelineConfig.Configuration.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_CAMERA_PASS, std::vector<const char*> { typeid(Razor::RSCameraPass).name() } });
	PickPipelineConfig.Configuration.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_RENDER, std::vector<const char*> { typeid(Razor::RSPickBufferRenderPass).name() } });

	Engine.InitSystems();

	SceneBuffer = Engine.Renderer->CreateFrameBuffer(300, 200);
	PickBuffer = Engine.Renderer->CreateFrameBuffer(300, 200);

	// TEMPORARY for testing
	/*Razor::Entity entity = Engine.CreateEntity();
	Razor::Transform EntityTransform;
	EntityTransform.Position = glm::vec3(0.0f, 0.0f, -20.0f);
	EntityTransform.Rotate(glm::vec3(0.0f, 10.0f, 0.0f));
	EntityTransform.Scale = glm::vec3(0.05f, 0.05f, 0.05f);
	Razor::ModelInfo WormModel = Engine.ProcessModel("../Sandbox/resources/models/Articulated_Worm.obj");
	Razor::Mesh TestMesh = WormModel.ModelMesh;
	Razor::Material WormMat = WormModel.ModelMaterial;
	Razor::Collider WormCollider;
	WormCollider.Radius = 10.0f;
	WormMat.ShaderID = Engine.GetShaderForType(typeid(Razor::DefaultMeshShader).name())->ID;
	Engine.AddComponentToEntity<Razor::Mesh>(entity, TestMesh);
	Engine.AddComponentToEntity<Razor::Transform>(entity, EntityTransform);
	Engine.AddComponentToEntity<Razor::Material>(entity, WormMat);
	Engine.AddComponentToEntity<Razor::Collider>(entity, WormCollider);*/
	Razor::SceneSerializer::Deserialize(Engine.CurrentScene);
	while (!Engine.ShouldEngineClose())
	{
		Engine.Step();

		PickBuffer->Refresh(ViewportSize.x, ViewportSize.y);
		SceneBuffer->Refresh(ViewportSize.x, ViewportSize.y);
		glViewport(0, 0, ViewportSize.x, ViewportSize.y);

		Engine.ProcessInput();

		Engine.RunSystems();

		Engine.Renderer->BindFrameBuffer(PickBuffer->GetID());
		Engine.Renderer->ClearBuffer();
		Engine.RunRenderSystems(PickPipelineConfig);
		Engine.PickObject(PickBuffer->GetID());

		Engine.Renderer->BindFrameBuffer(SceneBuffer->GetID());
		Engine.Renderer->ClearBuffer();
		Engine.RunRenderSystems(PipelineConfig);

		Engine.Renderer->BindFrameBuffer();
		Engine.Renderer->ClearBuffer();

		Engine.Renderer->PollForEvents();
		bool bIsOpen;
		Engine.GetGUI().BeginNewFrame();
		Engine.GetGUI().CreateDockspace();
		RenderInspector();
		RenderSceneViewport(SceneBuffer, ViewportSize);
		ImGui::ShowMetricsWindow();
		ImGui::End();
		Engine.GetGUI().EndFrame(Razor::Engine::Get().GetWindow(), Razor::Engine::Get().Renderer);
		Engine.Renderer->SwapBuffer(Razor::Engine::Get().GetWindow());
	}
	Razor::SceneSerializer::Serialize(Engine.CurrentScene);
	Razor::Engine::Get().Renderer->TerminateRendererAPI();
}

void Edge::RenderInspector()
{
	bool bIsOpen;
	ImGui::Begin("Inspector", &bIsOpen, ImGuiWindowFlags_MenuBar);
	ImGui::SetWindowSize(ImVec2(200.0f, 200.0f));
	ImGui::Text("Hello Inspector, %d", 123);
	ImGui::Button("Button", ImVec2(50.f, 25.f));
	ImGui::End();
}

void Edge::RenderSceneViewport(Razor::Ref<Razor::Framebuffer> SceneBuffer, ImVec2 ViewportSize)
{
	bool bIsOpen;
	ImGui::Begin("Scene", &bIsOpen, ImGuiWindowFlags_MenuBar || ImGuiWindowFlags_NoScrollbar);
	ViewportSize = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
	ImGui::Image(reinterpret_cast<void*>(SceneBuffer->GetTexture()), ImVec2(ViewportSize.x, ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}