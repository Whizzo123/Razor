#pragma once

namespace Razor
{
	/**
	* Interface to provide generic access to platform/api specific functionality for time
	*/
	class ITimeProvider
	{
	public:
		/**
		* Returns the current time
		* 
		* @return float - current time
		*/
		virtual float GetTime() = 0;
	};
}

