#include "NewProjectPopupWindow.h"
#include "../Project/ProjectCreator.h"

namespace EdgeEditor
{
	static constexpr const char* POPUP_NAME = "New Project Window";

	NewProjectPopupWindow::NewProjectPopupWindow() : fileSelector([this](Razor::FilePath path) { location = path; }){

	}

	bool NewProjectPopupWindow::Draw()
	{
		if (!bIsOpen)
		{
			Open();
		}

		if (Razor::RazorImGui::BeginPopupModal(POPUP_NAME, nullptr))
		{
			// Replace with folder selector
			Razor::RazorImGui::Text(static_cast<std::string>(location).c_str());
			if (Razor::RazorImGui::Button("Select folder")) {
				_mbIsFileSelectorOpen = true;
			}

			if (_mbIsFileSelectorOpen) {
				_mbIsFileSelectorOpen = fileSelector.Draw();
			}

			Razor::RazorImGui::InputText("Project Name", name, sizeof(name));
			if (Razor::RazorImGui::Button("Create"))
			{
				Razor::FilePath projectPath(name, true);
				ProjectCreator::Create(location + Razor::FilePath(std::string(name)));
				/*TODO actually load project up*/
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