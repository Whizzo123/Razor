#pragma once
#include <yaml-cpp/yaml.h>
#include "../EntityManager.h"
#include "Scene.h"

namespace Razor
{
	class SceneSerializer
	{
	public:
		static void Serialize(Ref<Scene> OutScene);
		static void Deserialize(Ref<Scene> OutScene);

	private:
		static void SerializeEntity(YAML::Emitter& Out, Entity InEntity);
	};
}

