#include "FileSelectorPopupWindow.h"
#include <filesystem>

namespace EdgeEditor {
    static constexpr const char* POPUP_NAME = "File Selector";

    FileSelectorPopupWindow::FileSelectorPopupWindow(CallbackFunc callback) {
        _mSearchStack.push(Razor::FilePath(std::filesystem::current_path()));
		_mCallback = callback;
    }
    // TODO:
    /*
    * We need an outside bool to represent when this should close
    * We need a way of getting the chosen folder/file back out to our caller
    * We need a way to differentiate between whether we want a folder or a file
    * and if a file what extensions do we allow and possibly hide all other files
    * and in the case of folder only hide all files
    */
    bool FileSelectorPopupWindow::Draw() {
        if (!_mbIsOpen)
		{
			Open();
		}
		if (Razor::RazorImGui::BeginPopupModal(POPUP_NAME, nullptr))
		{
			if (Razor::RazorImGui::BeginTable("FileTable", 4))
			{
				if (Razor::RazorImGui::IsWindowHovered() && Razor::RazorImGui::IsMouseClicked(Razor::MouseButton_Left) && !Razor::RazorImGui::IsAnyItemHovered()) {
					_mSelectedButtonLabel = "";
					// TODO: This will prove a bit difficult for files as this only works with the folders
					_mSelectedFilePath = _mSearchStack.top();
				}
				std::vector<Razor::FilePath> files = GrabFiles(_mSearchStack.top());
				for (const Razor::FilePath& file : files)
				{
					Razor::RazorImGui::TableNextColumn();
					DrawFileGui(file);
				}
				Razor::RazorImGui::EndTable();
			}

			Razor::RazorImGui::Text(static_cast<std::string>(_mSelectedFilePath).c_str());

			if (Razor::RazorImGui::Button("Select")) {
				CallbackAndClose();
			}
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
		
		return _mbIsOpen;
    }

    void FileSelectorPopupWindow::Open()
	{
		_mbIsOpen = true;
		_mSelectedFilePath = _mSearchStack.top();
		Razor::RazorImGui::OpenPopup(POPUP_NAME);
	}

	void FileSelectorPopupWindow::Close()
	{
		_mbIsOpen = false;
		Razor::RazorImGui::CloseCurrentPopup();
	}

    std::vector<Razor::FilePath> FileSelectorPopupWindow::GrabFiles(const Razor::FilePath& path)
	{
		std::vector<Razor::FilePath> fileNames;

		try
		{
			for (const std::filesystem::directory_entry& DirectoryEntry : std::filesystem::directory_iterator(path))
			{
				const std::filesystem::path& filePath = DirectoryEntry.path();
				bool bIsDirectory = DirectoryEntry.is_directory();

				if (bIsDirectory)
				{
					fileNames.push_back({ Razor::FilePath(filePath.string()), bIsDirectory });
				}
			}
		}
		catch (const std::filesystem::filesystem_error& e)
		{
			RZ_ERROR("FileSelectorPopupWindow::GrabFiles -> Failed to iterate directory '{0}': {1}", static_cast<std::string>(path), e.what());
		}

		return fileNames;
	}

	bool FileSelectorPopupWindow::DrawFileGui(const Razor::FilePath& file)
	{
		Razor::FilePath labelPath = file.RemoveFromPath(Razor::FilePath(_mSearchStack.top()));
		std::string label = static_cast<std::string>(labelPath);

		if (label.empty())
			return false;

		if (Razor::RazorImGui::Button(label.c_str()))
		{
			if (_mSelectedButtonLabel == label) {
				if (file.IsDir()) {
					_mSearchStack.push(file);
				} else {
					CallbackAndClose();
				}
				return false;
			}

			_mSelectedButtonLabel = label;
			_mSelectedFilePath = file;
		}
		return false;
	}

	void FileSelectorPopupWindow::CallbackAndClose() {
		_mCallback(_mSelectedFilePath);
		Close();
	}
}