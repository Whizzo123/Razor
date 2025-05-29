#include "Application.h"
#include "Scripting/ScriptEngine.h"

namespace Razor
{
	Application::Application()
	{
		ScriptEngine::Init();
	}

	Application::~Application()
	{
		ScriptEngine::Shutdown();
	}

	void Application::Run()
	{
		while (true)
		{
			
		}
	}
}