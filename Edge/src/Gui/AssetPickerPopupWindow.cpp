#include "AssetPickerPopupWindow.h"
#include <filesystem>
#include "Razor.h"


namespace EdgeEditor
{
	AssetPickerPopupWindow::AssetPickerPopupWindow(std::string rootFolder)
	{
		_mSearchStack.push(rootFolder);
	}

	bool AssetPickerPopupWindow::Draw()
	{
		bool bIsOpen = true;

		Open();

		if (Razor::RazorImGui::BeginPopupModal("Asset Picker Window", nullptr))
		{
			if (Razor::RazorImGui::BeginTable("FileTable", 4))
			{
				std::vector<Razor::FilePath> fileNames = GrabFiles(_mSearchStack.top());
				for (const Razor::FilePath& file : fileNames)
				{
					Razor::RazorImGui::TableNextColumn();
					if (DrawFileGui(file))
					{
						Close();
						bIsOpen = false;
					}
				}
				Razor::RazorImGui::EndTable();
			}
			Razor::RazorImGui::SameLine();
			if (Razor::RazorImGui::Button("Back"))
			{
				_mSearchStack.pop();
			}
			if (Razor::RazorImGui::Button("Cancel"))
			{
				Close();
				bIsOpen = false;
			}
			Razor::RazorImGui::EndPopup();
		}
		return bIsOpen;
	}
	void AssetPickerPopupWindow::Open()
	{
		Razor::RazorImGui::OpenPopup("Asset Picker Window");
	}
	void AssetPickerPopupWindow::Close()
	{
		Razor::RazorImGui::CloseCurrentPopup();
	}

	std::vector<Razor::FilePath> AssetPickerPopupWindow::GrabFiles(const std::string& Path)
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
			fileNames.push_back({ path, bIsDirectory });
		}

		return fileNames;
	}

	bool AssetPickerPopupWindow::DrawFileGui(const Razor::FilePath& file)
	{
		void* ButtonImage = 0;
		if (Razor::RazorImGui::ImageButton(ButtonImage, Razor::Vector2(100.0f, 100.0f)))
		{
			if (!file.IsDir())
			{		
				while (_mSearchStack.size() > 1)
				{
					_mSearchStack.pop();
				}
				Razor::FilePath path = file.RemoveFromPath(Razor::FilePath(_mSearchStack.top()));
				_mSelectedAssetPath = path;
				return true;
			}
			else
			{
				_mSearchStack.push(file);
			}
		}
		Razor::FilePath path = file.RemoveFromPath(Razor::FilePath(_mSearchStack.top()));
		Razor::RazorImGui::Text(static_cast<std::string>(path).c_str());
		return false;
	}

	std::string AssetPickerPopupWindow::GetSelectedAssetPath() const
	{
		return _mSelectedAssetPath;
	}
}