#pragma once

namespace EdgeEditor
{
	/**
	* Base class for popup windows
	*/
	class PopupWindow
	{
	public:
		/**
		* Function to draw popup window function to be called when wishing to draw popup
		*/
		virtual bool Draw() = 0;

	private:
		/**
		* Function to handle opening logic for popup such as ImGui calls
		*/
		virtual void Open() = 0;
		/**
		* Function to handle closing logic for popup such as ImGui calls
		*/
		virtual void Close() = 0;

	};

}

