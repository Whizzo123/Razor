#include "OpenProjectPopupWindow.h"
#include "../FileIO/ProjectSerializer.h"
#include "../Project.h"
#include "../EditorStorage.h"


namespace EdgeEditor
{
	bool OpenProjectPopupWindow::Draw()
	{
		if (bIsOpen == false)
		{
			Open();
		}
		if (Razor::RazorImGui::BeginPopupModal(WindowName.c_str(), nullptr))
		{
			// TODO will create big old file explorer for now we just wanna have a button we click which directs us to the current Sandbox
			if (Razor::RazorImGui::Button("Create"))
			{
				Razor::Ref<Project> LoadedProject = Razor::CreateRef<Project>();
				ProjectSerializer::Deserialize("../Sandbox/Sandbox", LoadedProject);
				Storage->SetProject(*LoadedProject);
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
	void OpenProjectPopupWindow::Open()
	{
		bIsOpen = true;
		Razor::RazorImGui::OpenPopup(WindowName.c_str());
		if (Storage == nullptr)
		{
			RZ_ERROR("OpenProjectPopupWindow Error: Editor Storage ref not provided closing popup window");
			Close();
		}
	}
	void OpenProjectPopupWindow::Close()
	{
		bIsOpen = false;
		Razor::RazorImGui::CloseCurrentPopup();
	}
}