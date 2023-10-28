#pragma once

#include "Systems/System.h"
#include "Coordinator.h"
#include "Component.h"
#include "ShaderReader.h"
#include "Renderer/Shader.h"
#include "Engine.h"

class MeshRenderer : public System
{
public:
	MeshRenderer(std::shared_ptr<IRenderer> Renderer) 
	{
		Coordinator = Razor::Coordinator::GetInstance(); 
		this->Renderer = Renderer;
	}
	void Run(float dt) override;
	void Init() override;
	
private:
	void InitMesh(Mesh& MeshToInit);

private:
	std::shared_ptr<Razor::Coordinator> Coordinator;
	std::shared_ptr<IRenderer> Renderer;
};

