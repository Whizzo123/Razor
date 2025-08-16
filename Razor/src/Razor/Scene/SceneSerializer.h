#pragma once

#include "../Core/Entity.h"
#include "Scene.h"

namespace Razor
{
	class IYamlWrapper;

	class RAZOR_API SceneSerializer
	{
	public:
		static void Serialize(Ref<Scene> OutScene);
		static bool Deserialize(Ref<Scene> OutScene);

	private:
		static void SerializeEntity(YAML::Emitter& Out, Entity InEntity);
	};
}

