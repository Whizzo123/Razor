#pragma once

#include <Razor.h>
#include "Project.h"

namespace EdgeEditor
{
	using OnProjectSetDelegate = MulticastDelegate<>;


	struct EditorStorage
	{
		EditorStorage() : SelectedEntity(nullptr) {}
		Razor::Ref<Razor::Entity> SelectedEntity;
		Razor::Model DefaultModel;
	private:
		Project CurrentProject;
		OnProjectSetDelegate ProjectSetDelegate;
	public:
		OnProjectSetDelegate& OnProjectSet()
		{
			return ProjectSetDelegate;
		}
		Project& GetProject()
		{
			return CurrentProject;
		}
		void SetProject(Project& Proj)
		{
			CurrentProject = Proj;
			ProjectSetDelegate.Broadcast();
		}
	};
}

