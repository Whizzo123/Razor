#pragma once
#include <memory>

#ifdef RZ_PLATFORM_WINDOWS

extern Razor::Application* Razor::CreateApplication();

int main(int argc, char** argv)
{
	Razor::Log::Init();
	RZ_CORE_ERROR("Initalising Engine!");
	RZ_INFO("Hello App!");
	Razor::Engine& GEngine = Razor::Engine::Get();
	GEngine.Init();

	auto App = Razor::CreateApplication();
	App->Run();
	delete App;
}
#else
	#error Razor only supports Windows!
#endif