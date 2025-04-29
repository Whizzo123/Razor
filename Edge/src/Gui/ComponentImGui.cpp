#include "ComponentImGui.h"
#include <imgui.h>

namespace EdgeEditor
{
	std::unordered_map<std::string, ComponentDrawer> ComponentImGui::_ComponentDrawers = RegisterDrawers();

	std::unordered_map<std::string, ComponentDrawer> ComponentImGui::RegisterDrawers()
	{
		std::unordered_map<std::string, ComponentDrawer> map = std::unordered_map<std::string, ComponentDrawer>();
		map["Transform"] = DrawTransform;
		map["Mesh"] = DrawMesh;
		return map;
	}

	void ComponentImGui::DrawComponents(Razor::Ref<Razor::Entity> InEntity)
	{
		for (auto& [name, drawer] : _ComponentDrawers)
		{
			drawer(InEntity);
		}
	}

	void ComponentImGui::DrawTransform(Razor::Ref<Razor::Entity> InEntity)
	{
		if (InEntity->HasComponent<Razor::Transform>())
		{
			Razor::Transform& transform = InEntity->GetComponent<Razor::Transform>();
			if (ImGui::CollapsingHeader("Transform"))
			{
				if (ImGui::CollapsingHeader("Position"))
				{
					ImGui::InputFloat("X", &transform.Position.x);
					ImGui::InputFloat("Y", &transform.Position.y);
					ImGui::InputFloat("Z", &transform.Position.z);
				}
			}
		}
	}

	void ComponentImGui::DrawMesh(Razor::Ref<Razor::Entity> InEntity)
	{
		if (InEntity->HasComponent<Razor::Mesh>())
		{
			Razor::Mesh& mesh = InEntity->GetComponent<Razor::Mesh>();
			if (ImGui::CollapsingHeader("Mesh"))
			{
				// Nothing really here to show just now maybe in the future but at least we know it's there
			}
		}
	}
}