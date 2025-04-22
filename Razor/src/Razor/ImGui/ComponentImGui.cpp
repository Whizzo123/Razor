#include "ComponentImGui.h"
#include "../Component.h"
#include <imgui.h>

namespace Razor
{
	std::unordered_map<std::string, ComponentDrawer> ComponentImGui::_ComponentDrawers = RegisterDrawers();

	std::unordered_map<std::string, ComponentDrawer> ComponentImGui::RegisterDrawers()
	{
		std::unordered_map<std::string, ComponentDrawer> map = std::unordered_map<std::string, ComponentDrawer>();
		map["Transform"] = DrawTransform;
		return map;
	}

	void ComponentImGui::DrawComponents(Ref<Entity> InEntity)
	{
		for (auto& [name, drawer] : _ComponentDrawers)
		{
			if (ImGui::CollapsingHeader(name.c_str()))
			{
				drawer(InEntity);
			}
		}
	}

	void ComponentImGui::DrawTransform(Ref<Entity> InEntity)
	{
		if (InEntity->HasComponent<Transform>())
		{
			Transform& transform = InEntity->GetComponent<Transform>();
			if (ImGui::CollapsingHeader("Position"))
			{
				ImGui::InputFloat("X", &transform.Position.x);
				ImGui::InputFloat("Y", &transform.Position.y);
				ImGui::InputFloat("Z", &transform.Position.z);
			}
		}
	}
}