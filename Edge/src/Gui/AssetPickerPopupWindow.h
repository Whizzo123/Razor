#pragma once
#include "PopupWindow.h"
#include <stack>
#include <string>
#include <vector>

namespace EdgeEditor
{
	/**
	* Class for popup window to create new project inherits from base PopupWindow class
	*/
    class AssetPickerPopupWindow : public PopupWindow
    {
		struct FileEntry
		{
			std::string mName;
			bool mbIsDirectory;
		};

	public:
		AssetPickerPopupWindow(std::string rootFolder);
		/**
		* Function to draw popup window function to be called when wishing to draw popup
		*/
		bool Draw() override;

		std::string GetSelectedAssetPath() const;

	private:
		/**
		* Function to handle opening logic for popup such as ImGui calls and initialising NewProject
		*/
		void Open() override;
		/**
		* Function to handle closing logic for popup such as ImGui calls
		*/
		void Close() override;

		std::vector<FileEntry> GrabFiles(const std::string& Path);
		bool DrawFileGui(const FileEntry& FileName);

	private:
		std::string _mSelectedAssetPath = ""; /** String to hold selected asset path */
		std::stack<std::string> _mSearchStack; /** Root folder to start browsing from */
    };

}


