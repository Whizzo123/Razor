#include "NewProjectPopupWindow.h"
#include "../Project/ProjectCreator.h"

namespace EdgeEditor
{
	static constexpr const char* POPUP_NAME = "New Project Window";
	bool NewProjectPopupWindow::Draw()
	{
		const std::string MagicalPathToFixWithActualSelectedPathSoon = "../../Sandboxes/";

		if (!bIsOpen)
		{
			Open();
		}

		if (Razor::RazorImGui::BeginPopupModal(POPUP_NAME, nullptr))
		{
			//ImGui::InputText("Project Name", &NewProject->ProjectName);
			if (Razor::RazorImGui::Button("Create"))
			{
				Razor::FilePath projectPath(std::filesystem::current_path().string() + "/Test", true);
				ProjectCreator::Create(projectPath);
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