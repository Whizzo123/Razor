#pragma once

#include "Razor.h"
#include "PopupWindow.h"

namespace EdgeEditor
{
	struct Project;

	/**
	* Class for popup window to create new project inherits from base PopupWindow class
	*/
	class NewProjectPopupWindow : public PopupWindow
	{
	public:
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

		Razor::Ref<Project> NewProject; /** New Project to potentially be created */
	};
}

