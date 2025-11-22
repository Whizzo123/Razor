#include "Entity.h"
#include "../Component.h"
#include "../Scripting/ScriptInterface.h"
#include "../Engine.h"

namespace Razor
{
	void Entity::AddScriptComponent(std::string className)
	{
		ScriptComponent& comp = GetComponent<ScriptComponent>();
		ScriptInterface& interface = Engine::Get().GetScriptInterface();
		int instanceId = interface.CreateScriptInstance(interface.GetType(className));
		if (instanceId != -1) {
			comp.mScriptInstances.push_back(instanceId);
		}
	}
}