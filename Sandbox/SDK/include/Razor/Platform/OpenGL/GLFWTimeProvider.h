#pragma once

#include "../Generic/ITimeProvider.h"

namespace Razor
{
	/**
	* Provides access to specific GLFW time functionality
	*/
	class GLFWTimeProvider : public ITimeProvider
	{
	public:
		~GLFWTimeProvider() override = default;
		/**
		* Function to get current GLFW time
		* 
		* @return float - current GLFW time
		*/
		virtual float GetTime() override;
	};
}

