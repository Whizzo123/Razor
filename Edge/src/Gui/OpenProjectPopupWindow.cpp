#include "OpenProjectPopupWindow.h"
#include "../EditorStorage.h"
#include <filesystem>


namespace EdgeEditor
{
	static constexpr const char* POPUP_NAME = "Open Project Window";

	static std::string NormalizePath(const std::string& path)
	{
		return static_cast<std::string>(Razor::FilePath(path));
	}

	OpenProjectPopupWindow::OpenProjectPopupWindow()
		: bIsOpen(false), Storage(nullptr)
	{
		_mSearchStack.push(NormalizePath(std::filesystem::current_path().string()));
	}

	OpenProjectPopupWindow::OpenProjectPopupWindow(Razor::Ref<EditorStorage> Storage)
		: bIsOpen(false), Storage(Storage)
	{
		_mSearchStack.push(NormalizePath(std::filesystem::current_path().string()));
	}

	bool OpenProjectPopupWindow::Draw()
	{
		if (!bIsOpen)
		{
			Open();
		}
		if (Razor::RazorImGui::BeginPopupModal(POPUP_NAME, nullptr))
		{
			bool bSelected = false;
			if (Razor::RazorImGui::BeginTable("FileTable", 4))
			{
				std::vector<Razor::FilePath> files = GrabFiles(_mSearchStack.top());
				for (const Razor::FilePath& file : files)
				{
					Razor::RazorImGui::TableNextColumn();
					if (DrawFileGui(file))
					{
						bSelected = true;
						break;
					}
				}
				Razor::RazorImGui::EndTable();
			}

			if (bSelected)
			{
				Close();
			}
			else
			{
				if (Razor::RazorImGui::Button("Back"))
				{
					if (_mSearchStack.size() > 1)
					{
						_mSearchStack.pop();
					}
				}
				Razor::RazorImGui::SameLine();
				if (Razor::RazorImGui::Button("Cancel"))
				{
					Close();
				}
			}

			Razor::RazorImGui::EndPopup();
		}
		return bIsOpen;
	}

	void OpenProjectPopupWindow::Open()
	{
		if (Storage == nullptr)
		{
			RZ_ERROR("OpenProjectPopupWindow Error: Editor Storage ref not provided, cannot open popup");
			return;
		}
		bIsOpen = true;
		Razor::RazorImGui::OpenPopup(POPUP_NAME);
	}

	void OpenProjectPopupWindow::Close()
	{
		bIsOpen = false;
		Razor::RazorImGui::CloseCurrentPopup();
	}

	std::vector<Razor::FilePath> OpenProjectPopupWindow::GrabFiles(const std::string& Path)
	{
		std::vector<Razor::FilePath> fileNames;

		try
		{
			for (const std::filesystem::directory_entry& DirectoryEntry : std::filesystem::directory_iterator(Path))
			{
				const std::filesystem::path& filePath = DirectoryEntry.path();
				bool bIsDirectory = DirectoryEntry.is_directory();

				if (bIsDirectory || (filePath.extension() == ".proj"))
				{
					fileNames.push_back({ Razor::FilePath(filePath.string()), bIsDirectory });
				}
			}
		}
		catch (const std::filesystem::filesystem_error& e)
		{
			RZ_ERROR("OpenProjectPopupWindow: Failed to iterate directory '{0}': {1}", Path, e.what());
		}

		return fileNames;
	}

	bool OpenProjectPopupWindow::DrawFileGui(const Razor::FilePath& file)
	{
		Razor::FilePath labelPath = file.RemoveFromPath(Razor::FilePath(_mSearchStack.top()));
		std::string label = static_cast<std::string>(labelPath);

		if (label.empty())
			return false;

		// Use Button (not ImageButton) so each entry gets a unique ImGui ID derived from its name.
		// ImageButton uses the texture pointer as the ID; with nullptr all buttons share ID 0.
		if (Razor::RazorImGui::Button(label.c_str()))
		{
			if (!file.IsDir())
			{
				std::filesystem::path fsPath(static_cast<std::string>(file));
				// strip ".proj" — ProjectSerializer::Deserialize appends it
				if (fsPath.extension() == ".proj")
					fsPath.replace_extension("");
				Storage->SetProjectPath(fsPath.string());
				return true;
			}
			else
			{
				_mSearchStack.push(static_cast<std::string>(file));
			}
		}
		return false;
	}
}
