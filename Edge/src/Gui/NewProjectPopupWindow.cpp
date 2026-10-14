#include "NewProjectPopupWindow.h"
#include "../Project/ProjectCreator.h"
#include "../EditorStorage.h"

namespace EdgeEditor
{
	static constexpr const char* POPUP_NAME = "New Project Window";

	NewProjectPopupWindow::NewProjectPopupWindow(Razor::Ref<EditorStorage> storage) : _mFileSelector([this](Razor::FilePath path) { location = path; }), _mStorage(storage) {

	}

	bool NewProjectPopupWindow::Draw()
	{
		if (!bIsOpen)
		{
			Open();
		}

		if (Razor::RazorImGui::BeginPopupModal(POPUP_NAME, nullptr))
		{

			Razor::RazorImGui::Text(static_cast<std::string>(location).c_str());
			Razor::RazorImGui::SameLine();
			if (Razor::RazorImGui::Button("Select folder")) {
				_mbIsFileSelectorOpen = true;
			}

			if (_mbIsFileSelectorOpen) {
				_mbIsFileSelectorOpen = _mFileSelector.Draw();
			}

			Razor::RazorImGui::InputText("Project Name", name, sizeof(name));
			if (Razor::RazorImGui::Button("Create"))
			{
				Razor::FilePath projectPath = location + Razor::FilePath(name);
				if(ProjectCreator::Create(projectPath)) {
					_mStorage->SetProjectPath(projectPath + Razor::FilePath(name));
				}
				Razor::RazorImGui::CloseCurrentPopup();
			}
			Razor::RazorImGui::SameLine();
			if (Razor::RazorImGui::Button("Cancel"))
			{ 
				Close();
				bIsOpen = false;
			}
			Razor::RazorImGui::EndPopup();
		}
		return bIsOpen;
	}
	void NewProjectPopupWindow::Open()
	{
		bIsOpen = true;
		Razor::RazorImGui::OpenPopup(POPUP_NAME);
	}
	void NewProjectPopupWindow::Close()
	{
		Razor::RazorImGui::CloseCurrentPopup();
	}
}