#pragma once
#include <functional>
#include <entt/entt.hpp>
#include "../Core/Entity.h"

namespace Razor
{
	using ComponentDrawer = std::function<void(Ref<Entity> InEntity)>;

	class ComponentImGui
	{
	public:
		/*
		* Function for registering all drawers must be called first before calling DrawComponents
		*/
		
		/*
		* Function for drawing all components on an entity
		*/
		static void DrawComponents(Ref<Entity> InEntity);
	private:
		static std::unordered_map<std::string, ComponentDrawer> RegisterDrawers();
		static void DrawTransform(Ref<Entity> InEntity);
	private:
		static std::unordered_map<std::string, ComponentDrawer> _ComponentDrawers;
	};
}

