#pragma once
#include <string>
#include <yaml-cpp/yaml.h>
#include "../EntityManager.h"
#include "../ComponentManager.h"
#include "../Systems/System.h"

namespace Razor
{
	class Scene
	{
	public:
		Scene(const std::string& Path);
		~Scene();
		std::string& GetPath() { return FilePath; }
	private:
		std::string FilePath;
	};
}

