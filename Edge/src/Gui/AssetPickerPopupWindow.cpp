#include "AssetPickerPopupWindow.h"
#include <filesystem>



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
			Razor::RazorImGui::BeginTable("FileTable", 4);
			std::vector<FileEntry> fileNames = GrabFiles(_mSearchStack.top());
			for (const FileEntry& file : fileNames)
			{
				Razor::RazorImGui::TableNextColumn();
				if (DrawFileGui(file))
				{
					Close();
					bIsOpen = false;
				}
			}
			Razor::RazorImGui::EndTable();
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

	std::vector<AssetPickerPopupWindow::FileEntry> AssetPickerPopupWindow::GrabFiles(const std::string& Path)
	{
		std::vector<FileEntry> fileNames;

		struct stat sb;

		for (const std::filesystem::directory_entry& DirectoryEntry : std::filesystem::directory_iterator(Path))
		{
			std::filesystem::path FilePath = DirectoryEntry.path();
			std::string fileName = FilePath.string();
			bool bIsDirectory = false;
			if (stat(fileName.c_str(), &sb) == 0 && (sb.st_mode & S_IFDIR))
			{
				bIsDirectory = true;
			}
			fileNames.push_back({ fileName, bIsDirectory });
		}

		return fileNames;
	}

	bool AssetPickerPopupWindow::DrawFileGui(const FileEntry& file)
	{
		void* ButtonImage = 0;
		if (Razor::RazorImGui::ImageButton(ButtonImage, Razor::Vector2(100.0f, 100.0f)))
		{
			if (!file.mbIsDirectory)
			{
				_mSelectedAssetPath = file.mName;
				return true;
			}
			else
			{
				_mSearchStack.push(file.mName);
			}
		}
		Razor::RazorImGui::Text(file.mName.c_str());
		return false;
	}

	std::string AssetPickerPopupWindow::GetSelectedAssetPath() const
	{
		return _mSelectedAssetPath;
	}
}