#pragma once
#include <memory>

#ifdef RZ_PLATFORM_WINDOWS

extern Razor::Application* Razor::CreateApplication();

int main(int argc, char** argv)
{
	printf("Razor Engine\n");
	auto App = Razor::CreateApplication();
	App->Run();
	delete App;
}
#else
	#error Razor only supports Windows!
#endif