#pragma once

#include <string>

namespace EdgeEditor
{
	/**
	* Struct to hold data about this game project
	*/
	struct Project
	{
		std::string ProjectName; /** Name of the project */
		std::string AssetDirectory; /** Directory containing all the asset files */
		std::string DllDirectory; /** Directory containing the scripting DLL */
		std::string MainScenePath; /** Path to the main scene (we will open this on load up)*/

		/**
		* Default constructor for Project class
		*/
		Project() : ProjectName(""), AssetDirectory(""), DllDirectory("")
		{

		}

		/**
		* Constructor for Project class
		* 
		* @param ProjectName - name for the project
		* @param AssetDirectory - directory containing all the asset files
		* @param DllDirectory - directory containing the scripting DLL
		*/
		Project(std::string ProjectName, std::string AssetDirectory, std::string DllDirectory)
			: ProjectName(ProjectName), AssetDirectory(AssetDirectory), DllDirectory(DllDirectory)
		{

		}
	};
}