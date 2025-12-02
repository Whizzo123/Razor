#pragma once

#include <Razor.h>

namespace EdgeEditor
{
	using OnProjectSetDelegate = MulticastDelegate<>;


	struct EditorStorage
	{
		EditorStorage() : SelectedEntity(nullptr), _mCurrentProjectPath("") {}
		Razor::Ref<Razor::Entity> SelectedEntity;
		Razor::Model DefaultModel;
	private:
		std::string _mCurrentProjectPath;
		OnProjectSetDelegate ProjectSetDelegate;
	public:
		OnProjectSetDelegate& OnProjectSet()
		{
			return ProjectSetDelegate;
		}
		const std::string& GetProjectPath() const
		{
			return _mCurrentProjectPath;
		}
		void SetProjectPath(const std::string& Proj)
		{
			_mCurrentProjectPath = Proj;
			ProjectSetDelegate.Broadcast();
		}
	};
}

