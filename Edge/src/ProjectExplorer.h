#pragma once

#include "Razor.h"
#include "../EditorStorage.h"

namespace EdgeEditor
{


	class ProjectExplorer
	{
	public:
		ProjectExplorer() 
		{
			FileNames = GrabFiles(ProjectDir);
		}
		ProjectExplorer(Razor::Ref<EditorStorage> Storage) : Storage(Storage) 
		{
			FileNames = GrabFiles(ProjectDir);
		}

		void Render();
	private:
		std::vector<std::string> GrabFiles(const std::string& Path);
		void DrawFileGui(const std::string& FileName);
		std::string OpenFile();
	private:
		Razor::Ref<EditorStorage> Storage;
		const std::string ProjectDir = "project";
		std::vector<std::string> FileNames;
	};
}

