#pragma once

#include "PopupWindow.h"
#include <string>
#include "Razor.h"


namespace EdgeEditor
{
	struct EditorStorage;

	class OpenProjectPopupWindow : public PopupWindow
	{
	public:
		OpenProjectPopupWindow() : bIsOpen(false), WindowName("Open Project Window"), Storage(nullptr) {}
		OpenProjectPopupWindow(Razor::Ref<EditorStorage> Storage) : bIsOpen(false), WindowName("Open Project Window"), Storage(Storage) {}
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

		bool bIsOpen;
		//TODO push this up to parent class
		std::string WindowName;
		Razor::Ref<EditorStorage> Storage;
	};
}

