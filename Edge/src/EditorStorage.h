#pragma once

#include <Razor.h>

namespace EdgeEditor
{
	using OnProjectSetDelegate = MulticastDelegate<>;


	struct EditorStorage
	{
		EditorStorage() : SelectedEntity(nullptr) {}
		Razor::Ref<Razor::Entity> SelectedEntity;
		Razor::Model DefaultModel;
	private:
		std::string CurrentProjectPath;
		OnProjectSetDelegate ProjectSetDelegate;
	public:
		OnProjectSetDelegate& OnProjectSet()
		{
			return ProjectSetDelegate;
		}
		std::string GetProject()
		{
			return CurrentProjectPath;
		}
		void SetProject(const std::string& Proj)
		{
			CurrentProjectPath = Proj;
			ProjectSetDelegate.Broadcast();
		}
	};
}

