#include "ComponentImGui.h"

namespace EdgeEditor
{
	std::unordered_map<std::string, ComponentDrawer> ComponentImGui::_ComponentDrawers = RegisterDrawers();

	std::unordered_map<std::string, ComponentDrawer> ComponentImGui::RegisterDrawers()
	{
		std::unordered_map<std::string, ComponentDrawer> map = std::unordered_map<std::string, ComponentDrawer>();
		map["Transform"] = DrawTransform;
		map["Mesh"] = DrawMesh;
		map["DirectionalLight"] = DrawDirectionalLight;
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
			if (Razor::RazorImGui::CollapsingHeader("Transform"))
			{
				if (Razor::RazorImGui::CollapsingHeader("Position"))
				{
					Razor::RazorImGui::InputFloat("X", &transform.Position.x);
					Razor::RazorImGui::InputFloat("Y", &transform.Position.y);
					Razor::RazorImGui::InputFloat("Z", &transform.Position.z);
				}
			}
		}
	}

	void ComponentImGui::DrawMesh(Razor::Ref<Razor::Entity> InEntity)
	{
		if (InEntity->HasComponent<Razor::Mesh>())
		{
			Razor::Mesh& mesh = InEntity->GetComponent<Razor::Mesh>();
			if (Razor::RazorImGui::CollapsingHeader("Mesh"))
			{
				// Nothing really here to show just now maybe in the future but at least we know it's there
			}
		}
	}

	void ComponentImGui::DrawDirectionalLight(Razor::Ref<Razor::Entity> InEntity)
	{
		if (InEntity->HasComponent<Razor::DirectionalLight>())
		{
			Razor::DirectionalLight& mesh = InEntity->GetComponent<Razor::DirectionalLight>();
			if (Razor::RazorImGui::CollapsingHeader("Directional Light"))
			{
				// Nothing really here to show just now maybe in the future but at least we know it's there
			}
		}
	}
}