#pragma once

#include "../Core/Entity.h"
#include "Scene.h"

namespace Razor
{
	class IYamlWrapper;
	struct YamlEmitter;

	class RAZOR_API SceneSerializer
	{
	public:
		static void Serialize(Ref<Scene> OutScene);
		static bool Deserialize(Ref<Scene> OutScene);

	private:
		static void SerializeEntity(YamlEmitter* Out, Entity InEntity);
	};
}

