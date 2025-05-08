#include <Razor.h>
#include <Razor/EntryPoint.h>

#include "Inspector.h"
#include "SceneView.h"
#include "../EditorStorage.h"
#include "ProjectExplorer.h"
#include "Systems/RSEditorCamera.h"
#include "EditorCamera.h"


class Edge : public Razor::Application
{
public:
	Edge() {};
	~Edge() {};
	void Run() override;
	/**
	* Processes input from the user for the editor as well as calls Engine::ProcessInput()
	*/
	void ProcessInput();
	void RenderSceneViewport(Razor::Ref<Razor::Framebuffer> SceneBuffer, ImVec2& ViewportSize);

private:
	EdgeEditor::EditorCamera EditorCamera; /** Object to house and control the camera we will use for rendering the scene to the viewport */
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
	PipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_MATERIAL_PASS, std::vector<const char*> { typeid(Razor::RSMaterialPass).name() } });
	PipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_LIGHTING_PASS, std::vector<const char*>
	{
		typeid(Razor::RSDirectionalLightingPass).name(),
		typeid(Razor::RSPointLightingPass).name(),
		typeid(Razor::RSSpotLightingPass).name()
	} });
	PipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_TRANSFORMATION_PASS, std::vector<const char*> { typeid(Razor::RSTransformationsPass).name() } });
	PipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_CAMERA_PASS, std::vector<const char*> { typeid(Razor::RSCameraPass).name() } });
	PipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_RENDER, std::vector<const char*> { typeid(Razor::RSRenderPass).name() } });

	Razor::RenderPipelineConfig EditorPipelineConfig;
	EditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_MATERIAL_PASS, std::vector<const char*> { typeid(Razor::RSMaterialPass).name() } });
	EditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_LIGHTING_PASS, std::vector<const char*>
	{
		typeid(Razor::RSDirectionalLightingPass).name(),
		typeid(Razor::RSPointLightingPass).name(),
		typeid(Razor::RSSpotLightingPass).name()
	} });
	EditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_TRANSFORMATION_PASS, std::vector<const char*> { typeid(Razor::RSTransformationsPass).name() } });
	EditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_CAMERA_PASS, std::vector<const char*> { typeid(EdgeEditor::RSEditorCamera).name() } });
	EditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_RENDER, std::vector<const char*> { typeid(Razor::RSRenderPass).name() } });

	Razor::RenderPipelineConfig PickPipelineConfig;
	PickPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_MATERIAL_PASS, std::vector<const char*> { typeid(Razor::RSPickBufferMaterialPass).name() } });
	PickPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_LIGHTING_PASS, std::vector<const char*> { typeid(Razor::RSDirectionalLightingPass).name() } });
	PickPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_TRANSFORMATION_PASS, std::vector<const char*> { typeid(Razor::RSTransformationsPass).name() } });
	PickPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_CAMERA_PASS, std::vector<const char*> { typeid(Razor::RSCameraPass).name() } });
	PickPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_RENDER, std::vector<const char*> { typeid(Razor::RSPickBufferRenderPass).name() } });

	Engine.GetCoordinator()->RegisterSystem<EdgeEditor::RSEditorCamera>(EdgeEditor::RSEditorCamera(Engine.CurrentScene, Engine.GetRenderer(), EditorCamera.GetCamera()));

	Engine.InitSystems();

	SceneBuffer = Engine.Renderer->CreateFrameBuffer(300, 200);
	PickBuffer = Engine.Renderer->CreateFrameBuffer(300, 200);

	Razor::Model DefaultModel = Engine.ProcessModel("resources/models/Cube.obj");
	DefaultModel.SetModelShader(Engine.GetShaderForType(typeid(Razor::DefaultMeshShader).name())->ID);

	Razor::Ref<EdgeEditor::EditorStorage> Storage = std::make_shared<EdgeEditor::EditorStorage>();
	Storage->DefaultModel = DefaultModel;

	EdgeEditor::Inspector InspectorWindow(Storage);
	EdgeEditor::SceneView SceneViewWindow(Storage);
	EdgeEditor::ProjectExplorer ProjectExplorerWindow(Storage);

	Razor::SceneSerializer::Deserialize(Engine.CurrentScene);
	while (!Engine.ShouldEngineClose())
	{
		Engine.Step();

		const uint32_t SizeX = (uint32_t)ViewportSize.x;
		const uint32_t SizeY = (uint32_t)ViewportSize.y;

		PickBuffer->Refresh(SizeX, SizeY);
		SceneBuffer->Refresh(SizeX, SizeY);
		glViewport(0, 0, SizeX, SizeY);

		ProcessInput();

		Engine.RunSystems();

		std::shared_ptr<Razor::IRenderer>& Renderer = Engine.Renderer;

		Renderer->BindFrameBuffer(PickBuffer->GetID());
		Renderer->ClearBuffer();
		Engine.RunRenderSystems(PickPipelineConfig);
		Engine.PickObject(PickBuffer->GetID());

		Renderer->BindFrameBuffer(SceneBuffer->GetID());
		Renderer->ClearBuffer();
		Engine.RunRenderSystems(EditorPipelineConfig);

		Renderer->BindFrameBuffer();
		Renderer->ClearBuffer();

		Renderer->PollForEvents();
		Engine.GetGUI().BeginNewFrame();
		Engine.GetGUI().CreateDockspace();
		InspectorWindow.Render();
		SceneViewWindow.Render();
		ProjectExplorerWindow.Render();
		RenderSceneViewport(SceneBuffer, ViewportSize);
		ImGui::ShowMetricsWindow();
		ImGui::End();
		Engine.GetGUI().EndFrame(Razor::Engine::Get().GetWindow(), Razor::Engine::Get().Renderer);
		Engine.Renderer->SwapBuffer(Razor::Engine::Get().GetWindow());
	}
	Razor::SceneSerializer::Serialize(Engine.CurrentScene);
	Razor::Engine::Get().Renderer->TerminateRendererAPI();
}

void Edge::RenderSceneViewport(Razor::Ref<Razor::Framebuffer> SceneBuffer, ImVec2& ViewportSize)
{
	bool bIsOpen;
	ImGui::Begin("Scene", &bIsOpen, ImGuiWindowFlags_MenuBar || ImGuiWindowFlags_NoScrollbar);
	ViewportSize = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
	ImGui::Image(reinterpret_cast<void*>(SceneBuffer->GetTexture()), ImVec2(ViewportSize.x, ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}

void Edge::ProcessInput()
{
	Razor::Engine::Get().ProcessInput();

	EditorCamera.ProcessInput(Razor::Engine::Get().GetDeltaTime());
}