#pragma once
#include "../Core.h"


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
		static void RegisterFunctions(Razor::Ref<Coral::ManagedAssembly> Assembly);


	};

}

