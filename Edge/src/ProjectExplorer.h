#pragma once

#include "Gui/IEditorWindow.h"
#include "Razor.h"
#include "EditorStorage.h"
#include <stack>

namespace EdgeEditor
{


	class ProjectExplorer : public IEditorWindow
	{
	public:
		ProjectExplorer() 
		{
			
		}
		ProjectExplorer(Razor::Ref<EditorStorage> Storage) : Storage(Storage), _mRootPath("")
		{
			
		}

		virtual ~ProjectExplorer() override = default;

		EditorWindowName GetName() override { return EditorWindowName::PROJECTEXPLORER; }
		void Render() override;
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

