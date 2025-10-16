#pragma once
#include "../Core.h"

namespace Razor
{
	class IYamlWrapper;
	struct YamlEmitter;
	class Scene;
	class Entity;

	class RAZOR_API SceneSerializer
	{
	public:
		static void Serialize(Ref<Scene> OutScene);
		static bool Deserialize(Ref<Scene> OutScene);

	private:
		static void SerializeEntity(YamlEmitter* Out, Entity InEntity);
	};
}

