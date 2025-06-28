#pragma once

namespace Coral
{
	class ManagedAssembly;
}

namespace Razor
{

	class ScriptGlue
	{
	public:
		static void RegisterComponents();
		static void RegisterFunctions(Coral::ManagedAssembly& Assembly);


	};

}

