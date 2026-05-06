#include "OpenProjectPopupWindow.h"
#include "../EditorStorage.h"
#include <filesystem>
#include <sys/stat.h>


namespace EdgeEditor
{
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
		if (Razor::RazorImGui::BeginPopupModal("Open Project Window", nullptr))
		{
			Razor::RazorImGui::BeginTable("FileTable", 4);
			std::vector<Razor::FilePath> files = GrabFiles(_mSearchStack.top());
			bool bSelected = false;
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

			if (bSelected)
			{
				Close();
				bIsOpen = false;
			}
			else
			{
				if (Razor::RazorImGui::Button("Back"))
				{
					if (_mSearchStack.size() > 1)
					{
						_mSearchStack.pop();
					}
					else
					{
						// Navigate above the starting root — compute parent directory
						std::string parent = NormalizePath(
							std::filesystem::path(_mSearchStack.top()).parent_path().string());
						if (!parent.empty() && parent != _mSearchStack.top())
							_mSearchStack.top() = parent;
					}
				}
				Razor::RazorImGui::SameLine();
				if (Razor::RazorImGui::Button("Cancel"))
				{
					Close();
					bIsOpen = false;
				}
			}

			Razor::RazorImGui::EndPopup();
		}
		return bIsOpen;
	}

	void OpenProjectPopupWindow::Open()
	{
		bIsOpen = true;
		Razor::RazorImGui::OpenPopup("Open Project Window");
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

	std::vector<Razor::FilePath> OpenProjectPopupWindow::GrabFiles(const std::string& Path)
	{
		std::vector<Razor::FilePath> fileNames;

		struct stat sb;

		for (const std::filesystem::directory_entry& DirectoryEntry : std::filesystem::directory_iterator(Path))
		{
			std::filesystem::path filePath = DirectoryEntry.path();
			std::string fileName = filePath.string();
			Razor::FilePath path(fileName);

			bool bIsDirectory = false;
			if (stat(static_cast<std::string>(path).c_str(), &sb) == 0 && (sb.st_mode & S_IFDIR))
			{
				bIsDirectory = true;
			}

			if (bIsDirectory || fileName.size() >= 5 && fileName.substr(fileName.size() - 5) == ".proj")
			{
				fileNames.push_back({ path, bIsDirectory });
			}
		}

		return fileNames;
	}

	bool OpenProjectPopupWindow::DrawFileGui(const Razor::FilePath& file)
	{
		Razor::FilePath labelPath = file.RemoveFromPath(Razor::FilePath(_mSearchStack.top()));
		std::string label = static_cast<std::string>(labelPath);

		// Use Button (not ImageButton) so each entry gets a unique ImGui ID derived from its name.
		// ImageButton uses the texture pointer as the ID; with nullptr all buttons share ID 0.
		if (Razor::RazorImGui::Button(label.c_str()))
		{
			if (!file.IsDir())
			{
				std::string fullPath = static_cast<std::string>(file);
				// strip ".proj" — ProjectSerializer::Deserialize appends it
				if (fullPath.size() >= 5)
					fullPath = fullPath.substr(0, fullPath.size() - 5);
				Storage->SetProjectPath(fullPath);
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
