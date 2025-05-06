#pragma once
#include <functional>
#include <entt/entt.hpp>
#include "Razor.h"

namespace EdgeEditor
{
	using ComponentDrawer = std::function<void(Razor::Ref<Razor::Entity> InEntity)>;

	class ComponentImGui
	{
	public:
		/*
		* Function for drawing all components on an entity
		*/
		static void DrawComponents(Razor::Ref<Razor::Entity> InEntity);
	private:
		/*
		* Function for registering all drawers must be called first before calling DrawComponents
		*/
		static std::unordered_map<std::string, ComponentDrawer> RegisterDrawers();
		static void DrawTransform(Razor::Ref<Razor::Entity> InEntity);
		static void DrawMesh(Razor::Ref<Razor::Entity> InEntity);
		static void DrawDirectionalLight(Razor::Ref<Razor::Entity> InEntity);
	private:
		static std::unordered_map<std::string, ComponentDrawer> _ComponentDrawers;
	};
}

