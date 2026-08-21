#pragma once

#include "PopupWindow.h"
#include <stack>
#include <string>
#include <vector>
#include "Razor.h"


namespace EdgeEditor
{
	struct EditorStorage;

	class OpenProjectPopupWindow : public PopupWindow
	{
	public:
		OpenProjectPopupWindow();
		OpenProjectPopupWindow(Razor::Ref<EditorStorage> Storage);
		/**
		* Function to draw popup window function to be called when wishing to draw popup
		*/
		bool Draw() override;

	private:
		/**
		* Function to handle opening logic for popup such as ImGui calls and initialising NewProject
		*/
		void Open() override;
		/**
		* Function to handle closing logic for popup such as ImGui calls
		*/
		void Close() override;

		std::vector<Razor::FilePath> GrabFiles(const std::string& Path);
		bool DrawFileGui(const Razor::FilePath& file);

		bool bIsOpen;
		Razor::Ref<EditorStorage> Storage;
		std::stack<std::string> _mSearchStack;
	};
}
