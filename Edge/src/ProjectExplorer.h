#pragma once

#include "Razor.h"
#include "EditorStorage.h"
#include <stack>

namespace EdgeEditor
{


	class ProjectExplorer
	{
	public:
		ProjectExplorer() 
		{
			
		}
		ProjectExplorer(Razor::Ref<EditorStorage> Storage) : Storage(Storage), _mRootPath("")
		{
			
		}

		void Render();
		void Refresh(const std::string& path);
	private:
		std::vector<Razor::FilePath> GrabFiles(const std::string& Path);
		void DrawFileGui(const Razor::FilePath& FileName);
		void OpenFile();
		bool SaveModelToProject(const std::string& Name);
	private:
		Razor::Ref<EditorStorage> Storage;
		std::stack<std::string> _mSearchStack;
		std::string _mRootPath;
	};
}

